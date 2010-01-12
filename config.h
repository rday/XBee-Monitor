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

#ifndef _XBEE_CONFIG_H
#define _XBEE_CONFIG_H

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
//#include <wx/msw/regconf.h>
#include "xbee.h"

class ConfigFrame: public wxFrame
{
private:
    wxFrame* mainFrame;

public:
    wxComboBox* comCB;
    wxTextCtrl* panId;
    wxTextCtrl* channel;

    ConfigFrame(wxFrame* parent, const wxString& title, const wxPoint& pos, const wxSize& size);
    static int hex2int(wxString hex);
    void OnSave(wxCommandEvent& event);

};

enum
{
    ID_ConfigSaveBtn = 1,
};
#endif
