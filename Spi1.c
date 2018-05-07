/******************************************************************************/
/**
* @file Spi1.c
* @brief Implement SPI1
*
*******************************************************************************/

/* ***************** Header / include files ( #include ) **********************/
#include "Spi1.h"
#include "Gpio.h"

/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
static uint8_t firstByte;
static uint8_t firstByteRead;

/* *************** Modul global constants ( static const ) ********************/
/* **************** Local func/proc prototypes ( static ) *********************/
static void Spi1Delay160us(void);

/******************************************************************************/
/**
* void Spi1Init(void)
* @brief Configure SPI1 (STM32F031:PA4(NSS),PA7(MOSI),PA6(MISO),PA5(SCK)) and 
*        initialze variables.
*
*******************************************************************************/
void Spi1Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->AFR[GPIO_PIN_5 >> 3] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)GPIO_PIN_5) & (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_5 >> 3] |= ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)GPIO_PIN_5) & (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_6 >> 3] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)GPIO_PIN_6) & (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_6 >> 3] |= ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)GPIO_PIN_6) & (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_7 >> 3] &= ~((uint32_t)0xF << ((uint32_t)(((uint32_t)GPIO_PIN_7) & (uint32_t)0x07) * 4));
    GPIOA->AFR[GPIO_PIN_7 >> 3] |= ((uint32_t)(GPIO_AF_0) << ((uint32_t)(((uint32_t)GPIO_PIN_7) & (uint32_t)0x07) * 4));

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
    SPI1->CR1 |= SPI_CR1_CPHA;
    SPI1->CR1 |= SPI_CR1_CPOL;

    firstByte = 0;
    firstByteRead = 0;
}

/******************************************************************************/
/**
* eFUNCTION_RETURN Spi1Transmit(uint8_t *pTxData, uint16_t size)
* @brief Implement SPI1 send. 1s timeout if nothing is received for transmitting.
*
* @param[in] pTxData pointer to the data to be transmitted
* @param[in] size number of bytes
* @returns   eFunction_Ok if successful
*            or
*            eFunction_Timeout if an timeout error occurs.
*
*******************************************************************************/
eFUNCTION_RETURN Spi1Transmit(uint8_t *pTxData, uint16_t size)
{
    uint8_t tmp;
    uint16_t sizeLoc = size;
    uint32_t i = 0;
    eFUNCTION_RETURN ret;
    while(size-- > 0)
    {
        while((SPI1->SR & SPI_SR_TXE) == 0);
        *(uint8_t *)&(SPI1->DR) = (uint8_t)*pTxData++;
    }
    while((i++ < TIMEOUT_1s) && ((SPI1->SR & SPI_SR_FTLVL) != 0));
    /* The  goes to 0 after the first bit of the last byte is
     * send out, so we need to wait another 7 bits plus the time
     * till CS goes high. This takes 160 us with SPI bit rate 100000.   */
    Spi1Delay160us();
    while (sizeLoc-- > 0) // flush Rx FIFO as it was filled with dummy data for Tx.
    {      
        tmp = SPI1->DR;
        tmp = SPI1->SR;
    }
    if((SPI1->SR & SPI_SR_FTLVL) == 0)
    {
        ret = eFunction_Ok;
    }
    else
    {
        ret = eFunction_Timeout;
    }
    return ret;
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
eFUNCTION_RETURN Spi1Receive(uint8_t *pRxData, uint16_t size)
{
    uint8_t tmp;
    uint16_t sizeLoc = size;
    uint8_t *pRxDataLoc = pRxData;
    uint8_t retryCnt = 0; // retry counter, if it reachs 3, return timeout
    uint8_t DataReceived = 0;
    if(firstByteRead)
    {
        *pRxDataLoc = firstByte;
        pRxDataLoc++;
        sizeLoc--;
        size--;
        pRxData++;
        firstByteRead = 0;
        DataReceived = 1;
    }
    while(sizeLoc > 0)
    {
        uint32_t i = 0;        
        while((i++ < PACKET_TIMEOUT_SPI_MS) && ((SPI1->SR & SPI_SR_RXNE) == 0));
        if ((SPI1->SR & SPI_SR_RXNE) != 0)
        {
            *pRxDataLoc = SPI1->DR;
            pRxDataLoc++;
            retryCnt = 0;
            sizeLoc--;
            DataReceived = 1;
        }
        else
        {
            sizeLoc = size;
            pRxDataLoc = pRxData;
            tmp = SPI1->DR; // clear overrun flag
            tmp = SPI1->SR; // clear overrun flag
            if(retryCnt++ >= 3)
            {
                if (DataReceived)
                {
                    // data lost
                    return eFunction_Error;
                }
                else
                {          
                    // polling timeout
                    return eFunction_Timeout;
                }
            }
        }
    }
    return eFunction_Ok;
}

/******************************************************************************/
/**
* uint8_t Spi1ByteReceived(void)
*
* @brief Check if there is any data on bus. Read the first byte out.
*
* @returns    1 if there is a byte on bus.
*             or
*             0 if there is nothing.
*
*******************************************************************************/
uint8_t Spi1ByteReceived(void)
{
    uint8_t ret;  
    if(SPI1->SR & SPI_SR_RXNE)
    {
        firstByte = SPI1->DR;
        firstByteRead = 1;
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/******************************************************************************/
/**
* static void Spi1Delay160us(void)
*
* @brief Function to delay about 160 us.
*
*******************************************************************************/
static void Spi1Delay160us(void)
{
    for(uint16_t i = 0; i < 160u; i++){}
}
