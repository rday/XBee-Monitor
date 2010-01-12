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

#include "serport.h"

SerialPort::SerialPort()
{
    rts = dtr = le = 0;
}

int SerialPort::utrl()
{
    int bytes;

    ioctl(port, FIONREAD, &bytes);

    printf("%d bytes available\n", bytes);
    return bytes;
/*
    wxFileOffset cur = port.Tell();
    wxFileOffset tmp = cur;
    port.SeekEnd();
    tmp = port.Tell() - cur;

    port.Seek(cur);
    return tmp;
*/
}

int SerialPort::uts(char* buf, int size)
{
    return write(port, buf, size);
}

int SerialPort::utr(char* buf, int size)
{
    printf("Trying to read %d\n", size);
    return read(port, buf, size);
/*
    int ret = port.Read(buf, size);
    buf[ret] = 0x0;
    return ret;
*/


}

int SerialPort::Open(const wxString& name)
{
/*
    cts = port.Open(name);
    return cts;
*/
    int status;

    port = open(name.mb_str(), O_RDWR | O_NONBLOCK);
    if( port == -1 )
        perror("can't open port");
    else
    {
        tcgetattr(port, &options);
        bzero(&options, sizeof(options));
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;

        tcsetattr(port, TCSANOW, &options);
    }
    if( status & TIOCM_DTR ) { printf("DTR Detect\n"); dtr = 1; }
    if( status & TIOCM_RTS ) { printf("RTS Detect\n"); rts = 1; }
    if( status & TIOCM_LE ) { printf("DSR Detect\n"); le = 1; }

    return port;
}

int SerialPort::Close() { rts = dtr = le = 0; return close(port); }
int SerialPort::GetRts()
{
   int status;

   ioctl(port, TIOCMGET, &status);
   return status & TIOCM_RTS;
}
int SerialPort::GetDtr()
{
   int status;
   ioctl(port, TIOCMGET, &status);
   return status & TIOCM_DTR;
}
int SerialPort::GetLe()
{
   int status;
   ioctl(port, TIOCMGET, &status);
   return status & TIOCM_LE;
}
