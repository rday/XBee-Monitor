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

#ifndef _XBEE_MONITOR
#define _XBEE_MONITOR

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include "serport.h"
#include "xbee.h"
#include "ttListCtrl.h"

#define WAIT_FOR_PACKET_START 0
#define WAIT_FOR_LEN1 1
#define WAIT_FOR_LEN2 2
#define RECEIVE_DATA 3
#define GOT_ESCAPE 4
#define WAIT_FOR_CHECKSUM 5

class Monitor: public wxTimer
{
private:
   ttListCtrl* packetList;
   SerialPort* port;
   wxTextCtrl* hexBox;
   int state;
   int packetsize;
   int packetcount;
   char* packet;
   void GetPacket(char* data, size_t len);
   void Process();//char* data, size_t len);

public:
   Monitor(SerialPort* p, wxTextCtrl* hex, ttListCtrl* cmd);
   ~Monitor();
   virtual void Notify();
};

#endif
