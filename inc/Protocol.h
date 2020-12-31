/******************************************************************************/
/**
* @file Protocol.h
* @brief Header file for state machine for bootloader protocol
* Copyright Kodezine UG 2018
*******************************************************************************/
#ifndef PROTOCOL_H
#define PROTOCOL_H
/* ***************** Header / include files ( #include ) **********************/
#include <stdint.h>
#include <stddef.h>
#include "Common.h"
#include "Usart1.h"
#include "Command.h"
#include "Flash.h"
#include "Packet.h"
#include "BSP.h"
/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
union myCmd
{
    uint8_t         bufferCMD[2];
    eRESPONSE_ID    returnValue;
    eCOMMAND_ID     receivedvalue;
};
typedef union myCmd tCmdUnion;

union myPayload
{
    tDATA_PACKET    packet;
    uint8_t         bufferPLD[68];
};
typedef union myPayload tPldUnion;

union myAppData
{
    tFIRMWARE_PARAM Firmware;
    uint8_t         bufferData[4];
};
typedef union myAppData tAppDataUnion;

enum myProtoState
{
    eDefaultState = 0,
    eFlashEraseCMD,
    eWriteMemory,
    ePayloadReceive,
    ePayloadCheck,
    eWriteAppCRC,
    eFinishUpdate,
    eFlashVerifyApplication,
    eStartAppCMD
};
typedef enum myProtoState tProtoState;
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
eFUNCTION_RETURN ProtocolSM_Run(const tBSPStruct *);

#endif
