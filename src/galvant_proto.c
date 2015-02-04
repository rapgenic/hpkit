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

#include "galvant_proto.h"

int
galvant_set_address (int pad, int sad)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, pad, 30))
    {
      printf ("addr: non valid option -- \"%d\"\n", pad);
      return 0;
    }

  if (sad != NULL)
    {
      printf ("addr: WARNING, you set sad but using galvant adapter, it won't be set");
    }

  // We need to adjust the address, because Galvant uses 1 - 31 range,
  // but we use 0 - 30
  sprintf (param, "%d", pad + 1);

  if (!_command ("addr", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
galvant_get_address ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("addr", buf, 1, MD_BOTH, NULL))
    return -1;

  // We need to adjust the address, because Galvant uses 1 - 31 range,
  // but we use 0 - 30
  return (atoi (buf) - 1);
}

int
galvant_set_auto (int val)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, val, 1))
    {
      printf ("prologix auto: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_command ("auto", NULL, 0, MD_CONTROLLER, param))
    return 0;

  return 1;
}

int
galvant_get_auto ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("auto", buf, 1, MD_CONTROLLER, NULL))
    return -1;

  return atoi (buf);
}