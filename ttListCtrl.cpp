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

#include "ttListCtrl.h"

int ttListCtrl::SetTooltip(int row, int col, wxString& tip)
{
   int i, j;

   if( (row >= rows) || (col >= cols) )
   {
	   wxString* tmp = grid;
	   grid = new wxString[(row+5) * (col+5)];

       for(i=0;i<rows;i++)
          for(j=0;j<cols;j++)
             grid[(i*cols)+j].Printf(wxT("%s"), tmp[(i*cols)+j]);
       delete[] tmp;
       rows = row+5;
       cols = col+5;
   }
   grid[(row*cols)+col].Printf(wxT("%s"),tip);
}

int ttListCtrl::GetTooltip(int row, int col, wxString& tip)
{
   if( &grid[(row*cols)+col] != NULL )
      tip.Printf(wxT("%s"), grid[(row*cols)+col]);
}

void ttListCtrl::OnMouseMotion(wxMouseEvent& event)
{
   ttListCtrl* o = (ttListCtrl*)event.GetEventObject();
   if( o == NULL ) return;
   int id = event.GetEventType();
   int count = o->GetColumnCount();
   long row=-1, col=-1;
   int flags=0, i=0, totalWidth=0, tmpWidth=0;
   wxPoint pt;
   wxTipWindow* tipWin;
   wxTimer* killTip;
   wxString toolTip;

   if( id != wxEVT_MOTION )
	   return;

   pt = event.GetPosition();
   row = o->HitTest(pt, flags);
   for(i=0;i<count;i++)
   {
       tmpWidth = o->GetColumnWidth(i);
       totalWidth += tmpWidth;
       if( pt.x < totalWidth )
       {
    	   col = i;
    	   break;
       }
   }

   if( row > -1 && col > -1)
	   o->GetTooltip(row, col, toolTip);

   if( toolTip.Length() > 0 )
   {
      tipWin = new wxTipWindow(o,toolTip);
      //Bind(wxTimerEvent, Monitor::destroyTip, wxID_ANY, wxID_ANY, q);
      SetClientData(tipWin);
      Connect(wxEVT_TIMER, wxTimerEventHandler(ttListCtrl::destroyTip), NULL, this);
      killTip = new wxTimer(this, wxID_ANY);
      killTip->Start(1000, true);
   }
}

void ttListCtrl::destroyTip(wxTimerEvent& event)
{
   wxTipWindow *obj = (wxTipWindow*)GetClientData();
   if( obj ) obj->Destroy();
}

