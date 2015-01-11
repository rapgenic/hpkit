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

#include "prologix_proto.h"

int
prologix_set_address (int address)
{
  char command[STR_MAXLEN];

  if (!(0 <= address <= 30))
    {
      printf ("set_address: non valid option -- \"%d\"\n", address);
      return 0;
    }

  sprintf (command, "++addr %d\r\n", address);

  if (!HPIB_serial_write (command))
    return 0;

  return 1;
}

int
prologix_get_address ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_read (buf, "++addr\r\n"))

    return atoi (buf);
}

int
prologix_set_mode (ad_mode_t mode)
{
  char command[STR_MAXLEN];

  sprintf (command, "++mode %d\r\n", mode);

  if (!HPIB_serial_write (command))
    return 0;

  return 1;
}

ad_mode_t
prologix_get_mode ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_read (buf, "++mode\r\n"))
    return -1;

  return atoi (buf);
}

int
prologix_set_lon (int val)
{
  char command[STR_MAXLEN];

  if (prologix_get_mode () == MD_CONTROLLER)
    return 0;

  sprintf (command, "++lon %d\r\n", val);

  if (!HPIB_serial_write (command))
    return 0;

  return 1;
}

int
prologix_get_lon ()
{
  char buf[BUF_MAXLEN];

  if (prologix_get_mode () == MD_CONTROLLER)
    return 0;

  if (!_prologix_read (buf, "++lon\r\n"))
    return -1;

  return atoi (buf);
}

int
prologix_set_auto (int val)
{
  char command[STR_MAXLEN];

  if (prologix_get_mode () == MD_DEVICE)
    return 0;

  sprintf (command, "++auto %d\r\n", val);

  if (!HPIB_serial_write (command))
    return 0;

  return 1;
}

int
prologix_get_auto ()
{
  char buf[BUF_MAXLEN];

  if (prologix_get_mode () == MD_DEVICE)
    return 0;

  if (!_prologix_read (buf, "++auto\r\n"))
    return -1;

  return atoi (buf);
}

int
_prologix_read (char *buf, char *prop)
{
  if (!HPIB_serial_write (prop))
    return 0;

  if (!HPIB_serial_read_until (buf, '\n'))
    return 0;

  if (strcmp (buf, "") == 0)
    return 0;

  return 1;
}
