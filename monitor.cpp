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

#include "monitor.h"

Monitor::Monitor(SerialPort* p, wxTextCtrl* hex, ttListCtrl* cmd)
{
   port = p;
   hexBox = hex;
   packetList = cmd;
   packet = (char*)malloc(1024);
   state = WAIT_FOR_PACKET_START;
   packetsize = 0;
}

Monitor::~Monitor()
{
	free(packet);
}

void Monitor::Notify()
{
   char *data, *tmp, buf[2];
   int i, j, size;

   if( port->utrl() > 0 )
   {
       size = port->utrl();
       data = (char *)malloc(size + 1);
       tmp = (char *)malloc((size*4)); // AB[space]
       bzero(data, size+1);
       bzero(tmp, size*4);
       j = port->utr(data, size);
       GetPacket(data, j);

       for(i=0;i<j;i++)
       {
          if( i % 16 == 0 )
             sprintf(buf, "\n%02X", data[i] & 0x0ff);
          else
             sprintf(buf, " %02X", data[i] & 0x0ff);
          strcat(tmp, buf);
       }

       wxString d(tmp, wxConvUTF8);
       hexBox->AppendText(d);
       //free(data);
       //free(tmp);
   }
}

void Monitor::GetPacket(char* data, size_t len)
{
    int i;
    XBee x;

    for(i=0;i<len;i++)
    {
        switch( state )
        {
           case WAIT_FOR_PACKET_START: 
                if( data[i] == XBEE_START_CHAR ) 
                {
                    state = WAIT_FOR_LEN1;
                    //cmdBox->AppendText(_("Got Packet Start\n"));
                }
                break;
           case WAIT_FOR_LEN1: 
                state = WAIT_FOR_LEN2;
                packetsize = data[i];
                packetsize <<= 8;
                packetsize &= 0x0ff00;
                break;
           case WAIT_FOR_LEN2:
                state = RECEIVE_DATA;
                packetsize |= (0x0ff & data[i]);
                packetcount = 0;
                printf("Got Size: %02x\n", packetsize );
                break;
           case RECEIVE_DATA:
                if(data[i] == XBEE_ESC_CHAR)
                    state = GOT_ESCAPE;
                else
                {
                    packet[packetcount++] = data[i];
                    if( packetcount >= packetsize ) state = WAIT_FOR_CHECKSUM;
                }
                //printf("Data: %02x\n", data[i]);
                //cmd.Clear();
                //cmd << data[i];
                //cmdBox->AppendText(cmd);
                break;
           case GOT_ESCAPE:
                packet[packetcount++] = data[i] ^ 0x20;
                if( packetcount >= packetsize ) state = WAIT_FOR_CHECKSUM;
                else state = RECEIVE_DATA;
                break;
           case WAIT_FOR_CHECKSUM:
                if( x.Checksum(packet, packetcount) == data[i] & 0x0ff )
                {

                    printf("Got good checksum\n");
                    Process();
                }
                state = WAIT_FOR_PACKET_START;
                break; 
        }
    }
printf("exit getpacket\n");
}

void Monitor::Process()
{
    char buf[72], buf2[50];
    wxString colText;
    wxString tooltip;
    int tmp;
    wxListItem item;

printf("Enter process\n");
    colText.Printf(wxT("%02X"), packet[0] & 0x0ff);
    packetList->InsertItem(0, colText);
    packetList->GetItem(item);
    colText.Printf(wxT("%02X"), packetsize & 0x0ffff);
    packetList->SetItem(0, 1, colText);

    switch( packet[0] & 0x0ff)
    {
       case ZIGBEE_NODEID_RX:
           tmp = packet[9] << 8;
           tmp = (tmp&0x0ff00) + packet[10];
           colText.Printf(wxT("%X"), tmp);
           packetList->SetItem(0, 2, colText);
           memcpy(buf, &packet[14], 8);
           colText.Printf(wxT("%02x%02x%02x%02x%02x%02x%02x%02x"),
                    buf[0]&0x0ff,buf[1]&0x0ff,buf[2]&0x0ff,buf[3]&0x0ff,
                    buf[4]&0x0ff,buf[5]&0x0ff,buf[6]&0x0ff,buf[7]&0x0ff);
           packetList->SetItem(0, 3, colText);
           break;
       case ZIGBEE_RECEIVE_PACKET:
           tmp = packet[9] << 8;
           tmp = (tmp&0x0ff00) + packet[10];
           colText.Printf(wxT("%X"), tmp);
           packetList->SetItem(0, 2, colText);
           memcpy(buf, &packet[1], 8);
           colText.Printf(wxT("%02x%02x%02x%02x%02x%02x%02x%02x"),
                    buf[0]&0x0ff,buf[1]&0x0ff,buf[2]&0x0ff,buf[3]&0x0ff,
                    buf[4]&0x0ff,buf[5]&0x0ff,buf[6]&0x0ff,buf[7]&0x0ff);
           packetList->SetItem(0, 3, colText);
           memcpy(buf, &packet[12], packetsize-12);

           buf[packetsize-12] = 0x0;
           colText.Printf(wxT("%s"), buf);
           packetList->SetItem(0, 5, colText);

           if( packet[11] == 0x02 ) packetList->SetItem(0,4, wxT("Broadcast"));
           if( packet[11] == 0x01 ) packetList->SetItem(0,4, wxT("Ack"));
           break;
       case MODEM_STATUS_FRAME:
           tmp = packet[9] << 8;
           tmp = (tmp&0x0ff00) + packet[10];
           colText.Printf(wxT("%X"), tmp);
           packetList->SetItem(0, 2, colText);
           if( packet[1] == 0x00 ) packetList->SetItem(0,5, wxT("Hardware Reset"));
           if( packet[1] == 0x06 ) packetList->SetItem(0,5, wxT("Coordinator Started"));
           break;
       case AT_COMMAND_RESPONSE_FRAME:
           wxString data;

           colText.Clear();
           colText.Printf("AT%c%c", packet[2]&0x0ff, packet[3]&0x0ff);
           packetList->SetItem(0,3,colText);
	   tooltip.Printf("AT Response");
	   packetList->SetTooltip(0,3,tooltip);
           colText.Clear();

           if( packet[4] == 0x0 ) data.Printf("OK: ");
           if( packet[4] == 0x1 ) data.Printf("Error: ");
           if( packet[4] == 0x2 ) data.Printf("Inv. Cmd: ");
           if( packet[4] == 0x3 ) data.Printf("Inv. Param: ");
 
           if( packet[2] == 'S' && (packet[3] == 'L' || packet[3] == 'H') )
           {
              memcpy(buf, &packet[5], 4);
              colText.Printf(wxT("%02X%02X%02X%02X"),
                    buf[0]&0x0ff,buf[1]&0x0ff,buf[2]&0x0ff,buf[3]&0x0ff);
              packetList->SetItem(0, 4, colText);
	      tooltip.Printf("High/Low Address");
	      packetList->SetTooltip(0,3,tooltip);
           }

           if( packet[2] == 'N' && packet[3] == 'D' )
           {
              tmp = packet[5] << 8;
              tmp = (tmp&0x0ff00) + packet[6];
              colText.Printf(wxT("%X/%02X%02x"), tmp, packet[17] & 0x0ff, packet[18] & 0x0ff);
              packetList->SetItem(0, 2, colText);
	      tooltip.Printf("Network Address");
	      packetList->SetTooltip(0,2,tooltip);

              memcpy(buf, &packet[7], 8);
              colText.Printf(wxT("%02X%02X%02X%02X%02X%02X%02X%02X"),
                    buf[0]&0x0ff,buf[1]&0x0ff,buf[2]&0x0ff,buf[3]&0x0ff,
                    buf[4]&0x0ff,buf[5]&0x0ff,buf[6]&0x0ff,buf[7]&0x0ff);
              packetList->SetItem(0, 4, colText);
	      tooltip.Printf("Source Radio Address");
	      packetList->SetTooltip(0,4,tooltip);

              if( packet[19] == 0 )
                  colText.Printf("Type:Coordinator");
              if( packet[19] == 1 )
                  colText.Printf("Type:Router");
              if( packet[19] == 2 )
                  colText.Printf("Type:Endpoint");

              data.Append(colText);
           }

           packetList->SetItem(0,5,data);
    }

printf("exit process\n");
    bzero(packet, packetsize);
}
