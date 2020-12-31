/******************************************************************************/
/**
* @file Spi1.c
* @brief Implement SPI1
* Copyright Kodezine UG 2018
*******************************************************************************/

/* ***************** Header / include files ( #include ) **********************/
#include "Spi1.h"
#if defined (SELECT_SPI)
/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
static uint16_t index = 0U;
// static uint32_t TxPin = 0UL;
// static uint32_t RxPin = 0UL;
// static uint32_t ClkPin = 0UL;
// static uint32_t NssPin = 0UL;
// static GPIO_TypeDef *pGPIO_SPI1 = NULL;
/* *************** Modul global constants ( static const ) ********************/
/* **************** Local func/proc prototypes ( static ) *********************/
/******************************************************************************/
/**
* void Spi1Init(void)
* @brief Configure SPI1 (STM32F031:PA4(NSS),PA7(MOSI),PA6(MISO),PA5(SCK)) and
*        initialze variables.
*
*******************************************************************************/
void Spi1Init(tBSPType BSPType)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->AFR[GPIO_PIN_5 >> 3] &= ~((uint32_t)0xF <<
                                    ((uint32_t)(((uint32_t)GPIO_PIN_5) &
                                    (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_5 >> 3] |= ((uint32_t)(GPIO_AF_0) <<
                                    ((uint32_t)(((uint32_t)GPIO_PIN_5) &
                                    (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_6 >> 3] &= ~((uint32_t)0xF <<
                                    ((uint32_t)(((uint32_t)GPIO_PIN_6) &
                                    (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_6 >> 3] |= ((uint32_t)(GPIO_AF_0) <<
                                    ((uint32_t)(((uint32_t)GPIO_PIN_6) &
                                    (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_7 >> 3] &= ~((uint32_t)0xF <<
                                    ((uint32_t)(((uint32_t)GPIO_PIN_7) &
                                    (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_7 >> 3] |= ((uint32_t)(GPIO_AF_0) <<
                                    ((uint32_t)(((uint32_t)GPIO_PIN_7) &
                                    (uint32_t)0x07) * 4));

    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (GPIO_PIN_5 * 2));
    GPIOA->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (GPIO_PIN_5 * 2));
    GPIOA->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)GPIO_PIN_5));
    GPIOA->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)GPIO_PIN_5));
    GPIOA->MODER &= ~(GPIO_MODER_MODER0 << (GPIO_PIN_5 * 2));
    GPIOA->MODER |= ((uint32_t)GPIO_Mode_AF << (GPIO_PIN_5 * 2));
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (GPIO_PIN_5 * 2));
    GPIOA->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (GPIO_PIN_5 * 2));

    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (GPIO_PIN_6 * 2));
    GPIOA->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_1 << (GPIO_PIN_6 * 2));
    GPIOA->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)GPIO_PIN_6));
    GPIOA->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)GPIO_PIN_6));
    //GPIOA->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_OD) << ((uint16_t)GPIO_PIN_6));
    GPIOA->MODER &= ~(GPIO_MODER_MODER0 << (GPIO_PIN_6 * 2));
    GPIOA->MODER |= ((uint32_t)GPIO_Mode_AF << (GPIO_PIN_6 * 2));
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (GPIO_PIN_6 * 2));
    GPIOA->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (GPIO_PIN_6 * 2));
    //GPIOA->PUPDR |= ((uint32_t)GPIO_PuPd_UP << (GPIO_PIN_6 * 2));

    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (GPIO_PIN_7 * 2));
    GPIOA->OSPEEDR |= ((uint32_t)GPIO_Speed_Level_3 << (GPIO_PIN_7 * 2));
    GPIOA->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)GPIO_PIN_7));
    GPIOA->OTYPER |= (uint16_t)(((uint16_t)GPIO_OType_PP) << ((uint16_t)GPIO_PIN_7));
    GPIOA->MODER &= ~(GPIO_MODER_MODER0 << (GPIO_PIN_7 * 2));
    GPIOA->MODER |= ((uint32_t)GPIO_Mode_AF << (GPIO_PIN_7 * 2));
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (GPIO_PIN_7 * 2));
    GPIOA->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << (GPIO_PIN_7 * 2));

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI1->CR2 = SPI_CR2_FRXTH | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; // read 8 bit working
    SPI1->CR1 |= SPI_CR1_SPE;
    // SPI1->CR1 |= SPI_CR1_CPHA;
    // SPI1->CR1 |= SPI_CR1_CPOL;
}

/******************************************************************************/
/**
* void Spi1Send(uint8_t *pTxData, uint16_t size);
* @brief Implement SPI1 send. 1s timeout if nothing is received for transmitting.
*
* @param[in] pTxData pointer to the data to be transmitted
* @param[in] size number of bytes
* @returns   eFunction_Ok if successful
*            or
*            eFunction_Timeout if an timeout error occurs.
*
*******************************************************************************/
void Spi1Send(uint8_t *pTxData, uint16_t size)
{
    uint16_t i = 0U;

    while (i < size)
    {
        while ((SPI1->SR & SPI_SR_TXE) == 0)
        {
            __NOP();
        }
        SPI1->DR = pTxData[i++];
    }
}

/******************************************************************************/
/**
* eFUNCTION_RETURN Spi1Receive(uint8_t *pRxData, uint16_t size)
*
* @brief Read from SPI1.
*
* @param[out] pRxData pointer to 68 bytes data
* @param[in]  size number of bytes
* @returns    eFunction_Ok if successful
*             or
*             eFunction_Error if timeout error occurs.
*
*******************************************************************************/
eFUNCTION_RETURN Spi1Recv(uint8_t *pRxData, uint16_t size)
{
    eFUNCTION_RETURN retVal = eFunction_Timeout;
    uint8_t tmp = 0;

    if (SPI1->SR & SPI_SR_RXNE)
    {
        if ((SPI1->SR & SPI_SR_RXNE) != 0)
        {
            pRxData[index] = SPI1->DR;
            index++;
        }else
        {
            tmp = SPI1->DR; // clear data flag
            tmp = SPI1->SR; // clear overrun flag
        }
    }
    (void)tmp;
    if (index >= size)
    {
        index = 0;
        retVal = eFunction_Ok;
    }
    return retVal;
}

/******************************************************************************/
/**
* void Spi1Reset(void);
*
* @brief Function to delay about 160 us.
*
*******************************************************************************/
void Spi1Reset(void)
{
    index = 0;
}

#endif
