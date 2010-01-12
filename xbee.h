/*
 * Copyright (c) 2009, Ryan Day
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice, 
 *     this list of conditions and the following disclaimer in the documentation 
 *     and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 *   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 *   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 *   SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 *   OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 *   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 *   TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _XBEE_XBEE
#define _XBEE_XBEE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XBEE_START_CHAR 0x7E
#define XBEE_ESC_CHAR 0x7D
#define XBEE_XON_CHAR 0x11
#define XBEE_XOFF_CHAR 0x13

#define AT_COMMAND_FRAME 0x08
#define QUEUE_AT_COMMAND_FRAME 0x09
#define TRANSMIT_REQUEST 0x10
#define EXPLICIT_ZIGBEE_COMMAND_FRAME 0x11
#define CMD_TABLE_ZIGBEE_COMMAND_FRAME 0x12
#define REMOTE_AT_COMMAND_FRAME 0x12
#define REMOTE_AT_COMMAND_REQUEST 0x17
#define CREATE_SOURCE_ROUTE 0x21
#define AT_COMMAND_RESPONSE_FRAME 0x88
#define MODEM_STATUS_FRAME 0x8A
#define ZIGBEE_TRANSMIT_STATUS 0x8B
#define ADV_MODEM_STATUS_FRAME 0x8C
#define ZIGBEE_RECEIVE_PACKET 0x90
#define ZIGBEE_EXPLICIT_RX 0x91
#define ZIGBEE_IOSAMPLE_RX 0x92
#define XBEE_SENSOR_READ_INDICATOR 0x94
#define ZIGBEE_NODEID_RX 0x95
#define ZIGBEE_REMOTE_COMMAND_RESPONSE 0x97
#define OTA_FIRMWARE_UPDATE_STATUS 0xA0
#define ROUTE_RECORD_INDICATOR 0xA1

// These frame types are ours, not standard xBee frames
#define ADV_MODEM_STATUS_FRAME_0 0xA0
#define ADV_MODEM_STATUS_FRAME_1 0xA1

class XBee
{
private:
   char GetID();
   char currentID;

public:
   char Checksum(char* data, int len);
   int LocalAT(char* cmd, char* val, int len, char* buf);
};

#endif
