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


#ifndef _XBEE_MAIN
#define _XBEE_MAIN

#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
//#include <wx/msw/regconf.h>
#include "config.h"
#include "serport.h"
#include "monitor.h"
#include "xbee.h"
#include "ttListCtrl.h"

class MainFrame: public wxFrame
{
private:
    int status;
    Monitor *m;
    wxString portname;
    int panid;
    int channel;
    int operating_panid;

    void LoadConfig();
public:
    SerialPort p;
    wxTextCtrl* txtBox;
    ttListCtrl* packetList;
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnConfig(wxCommandEvent& event);
    void OnListen(wxCommandEvent& event);
    void OnND(wxCommandEvent& event);
    void OnGetAddr(wxCommandEvent& event);

    void SetSerial(wxString port);
    void SetPanId(int pid);
    void SetChannel(int c);
    
    DECLARE_EVENT_TABLE()
};

enum
{
    ID_Quit = 1,
    ID_About,
    ID_Config,
    ID_Listen,
    ID_ND,
    ID_GetAddr,
};

enum
{
   CLOSED = 0,
   LISTENING,
};

#endif
