/******************************************************************************/
/**
* @file Spi1.h
* @brief Implement SPI1
* Copyright Kodezine UG 2018
*******************************************************************************/
#ifndef SPI1_H
#define SPI1_H

/* ***************** Header / include files ( #include ) **********************/
#include <stddef.h>
#include "stm32f0xx.h"
#include "Common.h"
#include "BSP.h"
#include "Gpio.h"
/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
void Spi1Init(tBSPType BSPType);
void Spi1Send(uint8_t *pTxData, uint16_t size);
void Spi1Reset(void);
eFUNCTION_RETURN Spi1Recv(uint8_t *pRxData, uint16_t size);
#endif
