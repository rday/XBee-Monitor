#include <stdio.h>
#include <wx/wx.h>
//#include <wxListCtrl.h>
#include "ttListCtrl.h"
#include "xbee.h"
#include "main.h"

int testAT()
{
   XBee x;
   int i, j, assert=1;
   char cmd[8];
   char *known = "\x7e\x00\x04\x08\x41\x53\x48\x1b";

   i = x.LocalAT((char*)"SH", (char*)NULL, 2, cmd);

   for(j=0;j<i;j++)
      if( cmd[j] != known[j] ) assert = 0;

   return assert;
}

int testToolTip()
{
   wxListItem item;
   wxString tip;
   wxFrame *f = new wxFrame();//NULL, -1, _("XBEE Network Monitor"), wxPoint(50, 50),
                                //  wxSize(450,400));

   wxListCtrl *list = new wxListCtrl(f, wxID_ANY, wxDefaultPosition, wxSize(350,250), wxLC_REPORT | wxLC_HRULES | wxLC_SINGLE_SEL);

   if( list == NULL ) return 0;
printf("test");
/*
   item.SetText(_("Item 1"));
   list->InsertColumn(0, item);
   list->SetTooltip(0,0,wxT("0x0"));
   list->SetTooltip(0,1,wxT("0x1"));
   list->SetTooltip(1,0,wxT("1x0"));
   list->SetTooltip(1,1,wxT("1x1"));

   list->GetTooltip(0,0,tip);
   if( tip != wxT("0x0") ) return 0;
   list->GetTooltip(1,0,tip);
   if( tip != wxT("1x0") ) return 0;
*/
   return 1;
}

int testArray()
{
   wxString *ttMatrix = new wxString[10 * 10];
   wxString str;
   char buf[64];

   str.Printf("2x8");
   ttMatrix[(2*10)+8].Printf(wxT("%s"), str);
   str.Printf("3x8");
   ttMatrix[(3*10)+8].Printf(wxT("%s"), str);
   str.Printf("9x4");
   ttMatrix[(9*10)+4].Printf(wxT("%s"), str);
   str.Printf("5x1");
   ttMatrix[(5*10)+1].Printf(wxT("%s"), str);

   str.Printf(wxT("%s"), ttMatrix[(3*10)+8]);
   strcpy(buf, str.c_str());
   printf("Buf: %s\n", buf);
}

int main()
{
   if( testAT() ) printf("LocalAT works\n");
   if( testArray() ) printf("Array works\n");
}

