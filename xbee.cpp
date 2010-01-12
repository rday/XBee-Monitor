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

#include "xbee.h"

char XBee::Checksum(char* data, int len)
{
    char checksum=0;
    int i;

    for(i=0;i<len;i++)
    {
        checksum+=data[i] & 0x0ff;
    }

    return 0x0ff - (checksum&0x0ff);
}

/*
   @param cmd The AT command (ND, MY, SH, etc)
   @param val The value to set to the AT register
   @param len The size of the AT command + the value 
          (some vals are 0x0 and we can't compute in this function)
   @param buf The buffer the command will be written to
*/
int XBee::LocalAT(char* cmd, char* val, int len, char* buf)
{
    int datalen = len + 2;		// api id + frame id + AT cmd + val
    int packetsize = 3 + datalen + 1;	// start char + size + datalen + checksum

    buf[0] = XBEE_START_CHAR;
    buf[1] = (datalen>>8) & 0x0ff;
    buf[2] = datalen & 0x0ff;
    buf[3] = 0x08;	// AT Command API ID
    buf[4] = GetID() & 0x0ff;
    memcpy(&buf[5], cmd, strlen(cmd));
    if( val )
       memcpy(&buf[5+strlen(cmd)], val, len - strlen(cmd));
    buf[packetsize-1] = Checksum(&buf[3], datalen) & 0x0ff;

    return packetsize;
}

char XBee::GetID()
{
    if( currentID < 'A' || currentID >= 'Z' )
       currentID = 'A';
    else
       currentID++;

    return currentID;
}
