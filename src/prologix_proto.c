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

#include "prologix_proto.h"

int
prologix_set_address (int pad, int sad)
{
  char param[STR_MAXLEN];

  if (sad != PROLOGIX_NONE)
    {
      if (!_limit_check (96, sad, 126))
        {
          printf ("prologix addr: non valid option -- \"%d\"\n", sad);
          return 0;
        }
    }

  if (pad != PROLOGIX_NONE)
    {
      if (!_limit_check (0, pad, 30))
        {
          printf ("prologix addr: non valid option -- \"%d\"\n", pad);
          return 0;
        }
    }

  if (pad != PROLOGIX_NONE && sad == PROLOGIX_NONE)
    sprintf (param, "%d", pad);
  else if (pad != PROLOGIX_NONE && sad != PROLOGIX_NONE)
    sprintf (param, "%d %d", pad, sad);
  else if (pad == PROLOGIX_NONE && sad == PROLOGIX_NONE)
    {
      printf ("prologix addr: you must set pad\n");
      return 0;
    }
  else
    {
      printf ("prologix addr: cannot set sad but not pad\n");
      return 0;
    }

  if (!_command ("addr", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

/*
 * TODO: ADD READING SUPPORT FOR SAD
 */
int
prologix_get_address (int *returnsad)
{
  char buf[BUF_MAXLEN];
  char *pad;
  char *sad = NULL;

  if (!_command ("addr", buf, 1, MD_BOTH, NULL))
    return -1;

  pad = strtok (buf, " ");
  sad = strtok (NULL, " ");

  if (sad != NULL && returnsad != NULL)
    *returnsad = atoi (sad);

  return atoi (pad);
}

int
prologix_set_auto (int val)
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
prologix_get_auto ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("auto", buf, 1, MD_CONTROLLER, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_clr ()
{
  if (!_command ("clr", NULL, 0, MD_CONTROLLER, NULL))
    return 0;

  return 1;
}

int
prologix_set_eoi (int val)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, val, 1))
    {
      printf ("prologix eoi: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_command ("eoi", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
prologix_get_eoi ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("eoi", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_eos (ad_eos_t mode)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, mode, 3))
    {
      printf ("prologix eos: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", mode);

  if (!_command ("eos", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

ad_eos_t
prologix_get_eos ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("eos", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_eot_enable (int val)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, val, 1))
    {
      printf ("prologix eot_enable: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_command ("eot_enable", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
prologix_get_eot_enable ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("eot_enable", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_eot_char (int c)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, c, 255))
    {
      printf ("prologix eot_char: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", c);

  if (!_command ("eot_char", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
prologix_get_eot_char ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("eot_char", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_ifc ()
{
  if (!_command ("ifc", NULL, 0, MD_CONTROLLER, NULL));
  return 0;

  return 1;
}

int
prologix_llo ()
{
  if (!_command ("llo", NULL, 0, MD_CONTROLLER, NULL))
    return 0;

  return 1;
}

int
prologix_loc ()
{
  if (!_command ("loc", NULL, 0, MD_CONTROLLER, NULL))
    return 0;

  return 1;
}

int
prologix_set_lon (int val)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, val, 1))
    {
      printf ("prologix lon: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_command ("lon", NULL, 0, MD_DEVICE, param))
    return 0;

  return 1;
}

int
prologix_get_lon ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("lon", buf, 1, MD_DEVICE, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_mode (ad_mode_t mode)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, mode, 1))
    {
      printf ("prologix mode: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", mode);

  if (!_command ("mode", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

ad_mode_t
prologix_get_mode ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("mode", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_read (char* buf, char until)
{
  char param[STR_MAXLEN];
  int count = 0;

  if (until <= 255)
    sprintf (param, "%d", until);
  else
    sprintf (param, "EOI");

  if (!_command ("read", buf, 0, MD_CONTROLLER, param))
    return 0;

  return 1;
}

int
prologix_read_tmo_ms (int time)
{
  char param[STR_MAXLEN];

  if (!_limit_check (1, time, 3000))
    {
      printf ("prologix read_tmo_ms: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", time);

  if (!_command ("read_tmo_ms", NULL, 0, MD_CONTROLLER, param))
    return 0;

  return 1;
}

int
prologix_rst ()
{
  if (!_command ("rst", NULL, 0, MD_BOTH, NULL))
    return 0;

  return 1;
}

int
prologix_set_savecfg (int val)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, val, 1))
    {
      printf ("prologix savecfg: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_command ("savecfg", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
prologix_get_savecfg ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("savecfg", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_spoll (int pad, int sad)
{
  char param[STR_MAXLEN] = "";
  char buf[BUF_MAXLEN];

  if (sad != PROLOGIX_NONE)
    {
      if (!_limit_check (96, sad, 126))
        {
          printf ("prologix spoll: wrong parameter interval\n");
          return 0;
        }
    }

  if (pad != PROLOGIX_NONE)
    {
      if (!_limit_check (0, pad, 30))
        {
          printf ("prologix spoll: wrong parameter interval\n");
          return 0;
        }
    }

  if (pad != PROLOGIX_NONE && sad == PROLOGIX_NONE)
    sprintf (param, "%d", pad);
  else if (pad != PROLOGIX_NONE && sad != PROLOGIX_NONE)
    sprintf (param, "%d %d", pad, sad);
  else if (pad == PROLOGIX_NONE && sad != PROLOGIX_NONE)
    {
      printf ("prologix spoll: cannot set sad but not pad\n");
      return 0;
    }

  // TODO: CHECK THIS
  if (!_command ("spoll", buf, 0, MD_CONTROLLER, (strcmp (param, "") ? param : NULL)))
    return -1;

  puts (buf);

  return atoi (buf);
}

int
prologix_srq ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("srq", buf, 1, MD_CONTROLLER, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_status (int dsb)
{
  char param[STR_MAXLEN];

  if (!_limit_check (0, dsb, 255))
    {
      printf ("prologix status: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", dsb);

  if (!_command ("status", NULL, 0, MD_DEVICE, param))
    return 0;

  return 1;
}

int
prologix_status ()
{
  char buf[BUF_MAXLEN];

  if (!_command ("status", buf, 1, MD_DEVICE, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_trg (char *buf, char *list)
{
  if (!_command ("trg", buf, 0, MD_CONTROLLER, list))
    return 0;

  return 1;
}

int
prologix_ver (char *buf)
{
  if (!_command ("ver", buf, 1, MD_BOTH, NULL))
    return 0;

  return 1;
}