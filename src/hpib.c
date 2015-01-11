/*
    Copyright (C) 2015 Giulio Girardi.

    This file is part of HPlot.

    HPlot is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HPlot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with HPlot.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hpib.h"

int f_serial;
struct termios tio_serial;

int
HPIB_serial_config (char *tty, int timeout)
{
  f_serial = open (tty, O_RDWR | O_NOCTTY);

  if (f_serial == -1)
    {
      perror (tty);
      return 0;
    }

  memset (&tio_serial, 0, sizeof (tio_serial));

  tio_serial.c_iflag = 0;
  tio_serial.c_oflag = 0;
  tio_serial.c_cflag = CS8 | CREAD | CLOCAL;
  tio_serial.c_lflag &= ~ICANON;
  tio_serial.c_cc[VMIN] = 0;
  tio_serial.c_cc[VTIME] = timeout;

  cfsetospeed (&tio_serial, BAUDRATE);
  cfsetispeed (&tio_serial, BAUDRATE);

  tcsetattr (f_serial, TCSANOW, &tio_serial);

  return 1;
}

int
HPIB_serial_read_until (unsigned char *buf, char until)
{
  unsigned char c;
  int count = 0;

  memset (buf, 0, BUF_MAXLEN);

  do
    {
      if (read (f_serial, &c, 1) <= 0)
        {
          return 0;
        }

      if (count >= BUF_MAXLEN)
        return 0;

      buf[count] = c;
      count++;
    }
  while (c != until);

  return 1;
}

int
HPIB_serial_write (unsigned char *string)
{
  if (write (f_serial, string, strlen (string)) == -1)
    {
      perror ("writing serial failed");
      return 0;
    }
  
  return 1;
}

int
HPIB_serial_close ()
{
  close (f_serial);
}
