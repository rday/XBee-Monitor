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

#include "main.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame( NULL, -1, title, pos, size )
{
    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *buttonRow = new wxBoxSizer( wxHORIZONTAL );
    wxButton* nodeDiscoverBtn;
    wxButton* getAddrBtn;
    int widths[4];

    wxMenu *menuFile = new wxMenu;

    menuFile->Append( ID_Config, _("&Config") );
    menuFile->Append( ID_Listen, _("&Listen") );
    menuFile->Append( ID_About, _("&About") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _("E&xit") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _("&File") );

    SetMenuBar( menuBar );

    nodeDiscoverBtn = new wxButton(this, ID_ND, _T("Node Discover"));
    nodeDiscoverBtn->SetSize(10, 10, 75,35);
    //nodeDiscoverBtn->Show(true);
    buttonRow->Add(nodeDiscoverBtn, 0, 0, 5);
    getAddrBtn = new wxButton(this, ID_GetAddr, _T("Get Address"));
    getAddrBtn->SetSize(10, 10, 75,35);
    buttonRow->Add(getAddrBtn, 0, 0, 5);

    topsizer->Add( buttonRow, 0, 0, 5);

    topsizer->Add( new wxStaticText(this, wxID_ANY, wxT("Hex Data"), wxDefaultPosition, wxDefaultSize),
                   0, wxEXPAND|wxALL|wxALIGN_RIGHT, 5 );

    txtBox = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(350,250), wxTE_MULTILINE);
    topsizer->Add(txtBox, 1, wxEXPAND|wxALL, 5);

    topsizer->Add( new wxStaticText(this, wxID_ANY, wxT("Packets"), wxDefaultPosition, wxDefaultSize),
                   0, wxEXPAND|wxALL|wxALIGN_RIGHT, 5 );

    packetList = new ttListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(350,250), wxLC_REPORT | wxLC_HRULES | wxLC_SINGLE_SEL);
    wxListItem item;

    item.SetText(_("Type"));
    packetList->InsertColumn(0, item);
    item.SetText(_("Size"));
    packetList->InsertColumn(1, item);
    item.SetText(_("Network"));
    packetList->InsertColumn(2, item);
    item.SetText(_("Source"));
    packetList->InsertColumn(3, item);
    item.SetText(_("Dest"));
    packetList->InsertColumn(4, item);
    item.SetText(_("Data"));
    packetList->InsertColumn(5, item);

    packetList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
    packetList->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
    packetList->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);
    packetList->SetColumnWidth(3, wxLIST_AUTOSIZE_USEHEADER);
    packetList->SetColumnWidth(4, wxLIST_AUTOSIZE_USEHEADER);
    packetList->SetColumnWidth(5, wxLIST_AUTOSIZE_USEHEADER);

    packetList->Connect(wxEVT_MOTION, wxMouseEventHandler(ttListCtrl::OnMouseMotion));

    topsizer->Add(packetList, 1, wxEXPAND|wxALL, 5);

    SetSizer( topsizer );      // use the sizer for layout
    topsizer->SetSizeHints( this );   // set size hints to honour minimum size

    status = CLOSED;
    CreateStatusBar(4);
    widths[0] = 200;
    widths[1] = 150;
    widths[2] = 150;
    widths[3] = 50;
    SetStatusWidths(4, widths);
    if( status == CLOSED )
       SetStatusText( _("Closed") );
    else
       SetStatusText( _("Listening") );
    LoadConfig();
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MainFrame::OnConfig(wxCommandEvent& WXUNUSED(event))
{
    ConfigFrame *frame = new ConfigFrame( this, _("Configure"), wxPoint(75,75),wxSize(450,340) );

    frame->Show(true);
}

void MainFrame::OnListen(wxCommandEvent& WXUNUSED(event))
{
    if( status == CLOSED )
    {
        if( p.Open(portname) == -1 )
        {
            wxMessageBox(_T("Could not access file"), _T("Port Communication"),
                                     wxICON_ERROR | wxOK, this);
        }
        else
        {
           status = LISTENING;
           SetStatusText( _("Listening") );
           if(p.GetRts()) SetStatusText("RTS", 2);
           if(p.GetDtr()) SetStatusText("DTR", 3);
           if(p.GetLe()) SetStatusText("LE", 4);
           m = new Monitor(&p, txtBox, packetList);
           m->Start(1000);
        }
    }
    else
    {
        delete m;
        SetStatusText( _("Closed") );
        status = CLOSED;
        printf("Close: %d\n", p.Close());
    }
}
void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("Designed to display Xbee network traffic"),
                  _("About Xbee Monitor"),
                  wxOK | wxICON_INFORMATION, this);
    XBee x;
    int i;
    char cmdbuf[32];
    i = x.LocalAT((char*)"SH", (char*)NULL, 2, cmdbuf);
    p.uts(cmdbuf, i);
    i = x.LocalAT((char*)"SL", (char*)NULL, 2, cmdbuf);
    p.uts(cmdbuf, i);
    i = x.LocalAT((char*)"ID", (char*)NULL, 2, cmdbuf);
    p.uts(cmdbuf, i);
    i = x.LocalAT((char*)"CH", (char*)NULL, 2, cmdbuf);
    p.uts(cmdbuf, i);
}

void MainFrame::OnND(wxCommandEvent& WXUNUSED(event))
{
    printf("Entering onND\n");
    XBee x;
    char cmdbuf[64];
    int i;

    i = x.LocalAT((char*)"ND", (char*)NULL, 2, cmdbuf);
    p.uts(cmdbuf, i);
}

void MainFrame::OnGetAddr(wxCommandEvent& WXUNUSED(event))
{
    printf("Entering onGetAddr\n");
    XBee x;
    char cmdbuf[64];
    int i;

    i = x.LocalAT((char*)"SH", (char*)NULL, 2, cmdbuf);
    p.uts(cmdbuf, i);
    i = x.LocalAT((char*)"SL", (char*)NULL, 2, cmdbuf);
    p.uts(cmdbuf, i);
}

void MainFrame::SetSerial(wxString port)
{
    SetStatusText(port, 1);
    portname = port;
}

void MainFrame::SetPanId(int pid)
{
    XBee x;
    char val[2];
    char cmdbuf[64];
    int i,j;
    wxString tmp;

    panid = pid;
    sprintf(val, "%c%c", (pid>>8) & 0x0ff, pid & 0x0ff);

    if( p.Open(portname) == -1 )
        perror("Could not open port");
    else
    {
       i = x.LocalAT((char*)"ID", val, 2 + strlen(val), cmdbuf);
       p.uts(cmdbuf, i);
       tmp.Printf("%02X%02X", (pid>>8) & 0x0ff, pid & 0x0ff);
       SetStatusText(tmp, 2);
    }
}

void MainFrame::SetChannel(int c)
{
    XBee x;
    char val[2];
    char cmdbuf[64];
    int i,j;

    channel = c;
    sprintf(val, "%c", c & 0x0ff);

    if( p.Open(portname) == -1 )
        perror("Could not open port");
    else
    {
       i = x.LocalAT((char*)"CH", val, 2 + strlen(val), cmdbuf);
       p.uts(cmdbuf, i);
    }
    for(j=0;j<i;j++) printf("%02x ", cmdbuf[j]);
}

void MainFrame::LoadConfig()
{
    wxConfig* cnf = new wxConfig("XBeeMonitor");
    wxString buf;

    cnf->Read("Port", &buf);
    SetSerial(buf);
    cnf->Read("PanId", &buf);
    SetPanId(ConfigFrame::hex2int(buf));
}
