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

#include "adapters_common.h"

int
_limit_check (int x, int y, int z)
{
  if (x > y || y > z)
    return 0;

  return 1;
}

int
_command (char *command_t, char *returnbuf, int onerowanswer, ad_mode_t mode, char* params)
{
  char command[STR_MAXLEN];
  int i;
  
  strcpy (command, ad_flags.command_prefix);

  if (mode != MD_BOTH)
    if (prologix_get_mode () != mode)
      {
        printf ("prologix %s: wrong device mode\n", command_t);
        return 0;
      }

  // create the command
  strcat (command, command_t);
  if (params != NULL)
    {
      strcat (command, " ");
      strcat (command, params);
    }
  strcat (command, "\r\n");

  if (!HPIB_serial_write (command))
    return 0;

  if (returnbuf != NULL)
    {
      if (onerowanswer)
        {
          if (!HPIB_serial_read_until (returnbuf, BUF_MAXLEN, '\n'))
            return 0;
        }
      else
        {
          while (HPIB_serial_read_char (&returnbuf[i]))
            i++;

          returnbuf[i++] = '\0';
        }

      if (!strcmp (returnbuf, "") || !strcmp (returnbuf, "Unrecognized command\r\n"))
        return 0;
    }

  return 1;
}
