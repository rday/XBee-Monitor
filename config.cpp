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

#include "config.h"
#include "main.h"

ConfigFrame::ConfigFrame(wxFrame* parent, const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame( NULL, -1, title, pos, size )
{
    wxArrayString choices;
    wxButton* saveBtn;
    wxStatusBar* sb = parent->GetStatusBar();
    wxStaticText* label;
    mainFrame = parent;

    choices.Add(_T("COM1"));
    choices.Add(_T("COM2"));
    choices.Add(_T("COM3"));
    choices.Add(_T("/dev/ttyUSB0"));

    comCB = new wxComboBox(this, wxID_ANY, _T("Choose COM port"), 
				wxDefaultPosition, wxDefaultSize, 
				choices, wxCB_DROPDOWN | wxCB_READONLY);
    comCB->SetSize(10,10, 100,30);
    comCB->SetValue(sb->GetStatusText(1));

    label = new wxStaticText(this, wxID_ANY, wxT("PAN ID"), wxDefaultPosition, wxDefaultSize);
    label->SetSize(10, 110, 75,25);

    panId = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(350,50));
    panId->SetSize(87, 110, 75, 25);
    panId->SetValue(sb->GetStatusText(2));

    label = new wxStaticText(this, wxID_ANY, wxT("Channel"), wxDefaultPosition, wxDefaultSize);
    label->SetSize(10, 140, 75,25);

    channel = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(350,50));
    channel->SetSize(87, 140, 75, 25);

    saveBtn = new wxButton(this, ID_ConfigSaveBtn, _T("Save"));
    saveBtn->SetSize(size.GetWidth() - 85, size.GetHeight() - 65, 75,35);
    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ConfigFrame::OnSave));

    panId->Show(true);
    label->Show(true);
    comCB->Show(true);
    saveBtn->Show(true);
}

void ConfigFrame::OnSave(wxCommandEvent& event)
{
    wxConfig* cnf = new wxConfig("XBeeMonitor");
    int q;
    MainFrame* f = (MainFrame*)mainFrame;

    cnf->Write("Port", comCB->GetValue());
    cnf->Write("PanId", panId->GetValue());
    f->SetSerial(comCB->GetValue());
    f->SetPanId(hex2int(panId->GetValue()));
    f->SetChannel(hex2int(channel->GetValue()));

    delete cnf;
    delete comCB;
    delete panId;
    
    Close();
}

int ConfigFrame::hex2int(wxString hex)
{
   int i,j=0;
   int val=0;
   char buf[16];

   strcpy(buf, (const char*)hex.mb_str(wxConvUTF8));

   printf("Converting %s\n", buf);
   for(i=strlen(buf); i>0; i--) {
       if( buf[j] >= 65 && buf[j] <= 90 )
           val += (buf[j] - 55) * (int)pow(16,i-1);
       else
           val += (buf[j] -48) * (int)pow(16,i-1);
       j++;
   }

   return val;
}
