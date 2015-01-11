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

#include "proto.h"

int
_HPIB_proto_response_required (char *buf)
{
  int x = strlen (buf) - 1;

  if (buf[x] == '\n' && buf[x - 1] == '\r' && buf[x - 2] != ';')
    {
      return 1;
    }

  return 0;
}

void
_HPIB_proto_get_command (char *buf, char cmd[2])
{
  int x = strlen (buf) - 1;

  cmd[0] = buf[x - 3];
  cmd[1] = buf[x - 2];
}

int
HPIB_proto_parse (char *buf)
{
  if (_HPIB_proto_response_required (buf))
    {
      char cmd[2];
      char response[STR_MAXLEN] = "";

      _HPIB_proto_get_command (buf, cmd);

      if (strncmp (cmd, "OE", 2) == 0)
	{
	  strcpy (response, "0\r\n");
	}
      else if (strncmp (cmd, "OH", 2) == 0)
	{
	  strcpy (response, "250,279,10250,7479\r\n");
	}
      else if (strncmp (cmd, "OI", 2) == 0)
	{
	  strcpy (response, "7470A\r\n");
	}
      else if (strncmp (cmd, "OP", 2) == 0)
	{
	  strcpy (response, "250,279,10250,7479\r\n");
	}
      else if (strncmp (cmd, "OO", 2) == 0)
	{
	  strcpy (response, "0,1,0,0,0,0,0,0\r\n");
	}
      else if (strncmp (cmd, "OF", 2) == 0)
	{
	  strcpy (response, "40,40\r\n");
	}
      else if (strncmp (cmd, "OA", 2) == 0)
	{
	  strcpy (response, "250,279,0\r\n");
	}
      else if (strncmp (cmd, "OC", 2) == 0)
	{
	  strcpy (response, "250,279,0\r\n");
	}

      if (!HPIB_serial_write (response)) 
        {
          return 0;
        }
    }
  return 1;
}
