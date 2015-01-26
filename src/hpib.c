/*
    Copyright (C) 2015 Giulio Girardi.

    This file is part of HPKit.

    HPKit is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HPKit is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with HPKit.  If not, see <http://www.gnu.org/licenses/>.
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
HPIB_serial_read_until (char *buf, int len, char until)
{
  char c;
  int count = 0;
//  int rr;

  memset (buf, 0, len);

  do
    {
      /*rr = read (f_serial, &c, 1);

      if (rr == -1)
        {
          perror ("HPIB_serial_read_until");
          return 0;
        }
      else if (rr == 0)
        return 0;*/
      
      if (!HPIB_serial_read_char (&c))
        return 0;

      if (count >= len)
        {
          puts ("buffer is too small");
          return 0;
        }

      buf[count] = c;
      count++;
    }
  while (c != until);
  
  buf[count] = '\0';
  
  return 1;
}

int
HPIB_serial_read_char (char *c)
{
  int rr;
//  char d;

  rr = read (f_serial, c, 1);

  if (rr == -1)
    {
      perror ("HPIB_serial_read_char");
      return 0;
    }

  if (rr == 0)
    return 0;

  return 1;
}

int
HPIB_serial_write (char *string)
{
  if (write (f_serial, string, strlen (string)) == -1)
    {
      perror ("HPIB_serial_write");
      return 0;
    }

  return 1;
}

int
HPIB_serial_close ()
{
  close (f_serial);
}
