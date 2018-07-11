/******************************************************************************/
/**
* @file BSP.c
* @brief Implement BSP (board support package) layer
* Copyright Kodezine UG 2018
*******************************************************************************/
/* ***************** Header / include files ( #include ) **********************/
#include <stddef.h>
#include "BSP.h"
#include "Can.h"
#include "Flash.h"
#include "Usart1.h"
#include "Spi1.h"
/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
static tBSPStruct gIF;
static void TorqueSensorCoreClockInit(void);
/******************************************************************************/
/**
* tBSPStruct* BSP_Init(void)
* @brief Initialize target based bsp options
* @returns pointer to structure with which protocol state machine can work
*
*******************************************************************************/
tBSPStruct* BSP_Init(void)
{
    extern uint32_t SystemCoreClock;
    gIF.BSP_Type        = BSP_Unknown;

    gIF.pInit           = NULL;
    gIF.pSend           = NULL;
    gIF.pRecv           = NULL;
    gIF.pReset          = NULL;

    gIF.BootTimeoutTicks= BootTIMEOUT;
    gIF.AppStartTicks   = BootTIMEOUT - 100000UL;
    gIF.CommDoneTicks   = 10000UL;
    gIF.TwoBytesTicks   = 300UL;

#if defined (SELECT_TORQUE)
    gIF.BSP_Type = BSP_TorqueSensor;
    #warning Torque Sensor is selected
#elif defined (SELECT_PILOT)
    gIF.BSP_Type = BSP_Pilot;
    #warning Pilot is selected
#elif defined (SELECT_CAN)
    gIF.BSP_Type = BSP_CAN;
    #warning CAN bus selected
#elif defined (SELECT_SPI)
    gIF.BSP_Type = BSP_SPI;
    #warning SPI bus selected
#else
/**
* The section below will may be used to determine the board type by dedicated GPIO  settings
* in the hardware in future. When such a functionality is available, remove the #error below and
* test the code with the correct GPIO pin/port settings. Until then, use targets in the project.
*/
    #error Select a valid board type
    uint32_t temp_u32 = DBGMCU->IDCODE;

    if ((temp_u32 & DBGMCU_IDCODE_DEV_ID) != DBGMCU_ID_F03x)
    {
        /* Make BSP config for F04x, Interface is always CAN */
        gIF.BSP_Type = BSP_CAN;
    }else
    {
        /* Interface can be SPI or USART for F03x family so more investigation necessary */

        RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
        /* Input pin 6 is made active */
        BSP_CHECK_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_CHECK_PIN_6 * 2));
        BSP_CHECK_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_CHECK_PIN_6 * 2));
        /* Input pin 6 is made active */
        BSP_CHECK_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_CHECK_PIN_7 * 2));
        BSP_CHECK_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_CHECK_PIN_7 * 2));

        BSP_CHECK_PORT->ODR   &= ~((MASK_BIT_6) | (MASK_BIT_7));

        temp_u32 = BSP_CHECK_PORT->IDR;

        if (temp_u32 & (MASK_BIT_6))
        {
            /* Pin 6 is high only for Watchdog */
            gIF.BSP_Type = BSP_ExtWatchdog;
        }else
        {
            if (temp_u32 & (MASK_BIT_7))
            {
                /* Pin 7 is high only for Pilot */
                gIF.BSP_Type = BSP_Pilot;
            }else
            {
                /* This shall be a Torque sensor */
                gIF.BSP_Type = BSP_TorqueSensor;
            }
        }
    }
#endif
    switch (gIF.BSP_Type)
    {
        case BSP_Pilot:
            gIF.pInit   = &Usart1Init;
            gIF.pSend   = &Usart1Send;
            gIF.pRecv   = &Usart1Recv;
            gIF.pReset  = &Usart1Reset;
            break;

        case BSP_TorqueSensor:
            gIF.pInit   = &Usart1Init;
            gIF.pSend   = &Usart1Send;
            gIF.pRecv   = &Usart1Recv;
            gIF.pReset  = &Usart1Reset;

            TorqueSensorCoreClockInit();
            break;

        case BSP_SPI:
            gIF.pInit   = &Spi1Init;
            gIF.pSend   = &Spi1Send;
            gIF.pRecv   = &Spi1Recv;
            gIF.pReset  = &Spi1Reset;
            break;

        case BSP_CAN:
            gIF.pInit   = &CanInit;
            gIF.pRecv   = &CanRecv;
            gIF.pSend   = &CanSend;
            gIF.pReset  = &CanReset;
            break;

        default:
            // TODO implement me
            break;
    }

    /* Let's update the global SystemCoreClock variable just in case the system
     * frequency has changed. Mandatory for calculations of delay for bootloader
     * timeouts that are solely dependent on system ticks
     */
    SystemCoreClockUpdate();
    /* Now calculate by what factor has the system changed it's core clock */
    uint32_t temp_u32 = ( SystemCoreClock / BSP_ALLBOARD_HSI_FREQUENCY );

    gIF.AppStartTicks     *= temp_u32;
    gIF.CommDoneTicks     *= temp_u32;
    gIF.BootTimeoutTicks  *= temp_u32;
    gIF.TwoBytesTicks     *= temp_u32;

    gIF.pInit(gIF.BSP_Type);

    FlashInit(gIF.BSP_Type);

    return (&gIF);
}
/******************************************************************************/
/**
void TorqueSensorCoreClockInit(void)
* @brief additional clock configuration required for matching baudrate for
* torque sensor
*
*******************************************************************************/
void TorqueSensorCoreClockInit(void)
{
    RCC->CR |= ((uint32_t)RCC_CR_HSION);                     /* Enable HSI */
    while ((RCC->CR & RCC_CR_HSIRDY) == 0)
    {
        __NOP();                                             /* Wait for HSI Ready */
    }
    RCC->CFGR = RCC_CFGR_SW_HSI;                             /* HSI is system clock */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    {
        __NOP();                                             /* Wait for HSI used as system clock */
    }

    FLASH->ACR  = FLASH_ACR_PRFTBE;                          /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_LATENCY;                         /* Flash 1 wait state */

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         /* HCLK = SYSCLK */
    RCC->CFGR |= RCC_CFGR_PPRE_DIV1;                         /* PCLK = HCLK */

    RCC->CR &= ~RCC_CR_PLLON;                                /* Disable PLL */

    /*  PLL configuration:  = HSI/2 * 8 = 32 MHz */
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMUL);
    RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL16);  /* 16 for 64Mhz */

    RCC->CR |= RCC_CR_PLLON;                                 /* Enable PLL */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
        __NOP();                                             /* Wait till PLL is ready */
    }

    RCC->CFGR &= ~RCC_CFGR_SW;                               /* Select PLL as system clock source */
    RCC->CFGR |=  RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    {
        __NOP();                                            /* Wait till PLL is system clock src */
    }
}