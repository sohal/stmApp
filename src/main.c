/******************************************************************************/
/**
* @file main.c
* @brief Main calling sequence for bootloader.
* Copyright Kodezine UG 2018
******************************************************************************/
/* ***************** Header / include files ( #include ) **********************/
#include "BSP.h"
#include "Protocol.h"
/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
/* *************** Modul global constants ( static const ) ********************/
/* **************** Local func/proc prototypes ( static ) *********************/
/******************************************************************************/

int main (void)
{
    tBSPStruct* pBSP = BSP_Init();

    for (;;)
    {
        ProtocolSM_Run(pBSP);
    }
}
