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
prologix_set_address (int pad, int sad)
{
  char param[STR_MAXLEN];

  if (sad != PROLOGIX_NONE)
    {
      if (!_prologix_limit_check (96, sad, 126))
        {
          printf ("prologix addr: non valid option -- \"%d\"\n", sad);
          return 0;
        }
    }

  if (pad != PROLOGIX_NONE)
    {
      if (!_prologix_limit_check (0, pad, 30))
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

  if (!_prologix_command ("addr", NULL, 0, MD_BOTH, param))
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

  if (!_prologix_command ("addr", buf, 1, MD_BOTH, NULL))
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

  if (!_prologix_limit_check (0, val, 1))
    {
      printf ("prologix auto: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_prologix_command ("auto", NULL, 0, MD_CONTROLLER, param))
    return 0;

  return 1;
}

int
prologix_get_auto ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("auto", buf, 1, MD_CONTROLLER, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_clr ()
{
  if (!_prologix_command ("clr", NULL, 0, MD_CONTROLLER, NULL))
    return 0;

  return 1;
}

int
prologix_set_eoi (int val)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (0, val, 1))
    {
      printf ("prologix eoi: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_prologix_command ("eoi", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
prologix_get_eoi ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("eoi", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_eos (ad_eos_t mode)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (0, mode, 3))
    {
      printf ("prologix eos: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", mode);

  if (!_prologix_command ("eos", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

ad_eos_t
prologix_get_eos ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("eos", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_eot_enable (int val)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (0, val, 1))
    {
      printf ("prologix eot_enable: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_prologix_command ("eot_enable", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
prologix_get_eot_enable ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("eot_enable", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_eot_char (int c)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (0, c, 255))
    {
      printf ("prologix eot_char: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", c);

  if (!_prologix_command ("eot_char", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
prologix_get_eot_char ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("eot_char", buf, 1, MD_BOTH, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_ifc ()
{
  if (!_prologix_command ("ifc", NULL, 0, MD_CONTROLLER, NULL));
  return 0;

  return 1;
}

int
prologix_llo ()
{
  if (!_prologix_command ("llo", NULL, 0, MD_CONTROLLER, NULL))
    return 0;

  return 1;
}

int
prologix_loc ()
{
  if (!_prologix_command ("loc", NULL, 0, MD_CONTROLLER, NULL))
    return 0;

  return 1;
}

int
prologix_set_lon (int val)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (0, val, 1))
    {
      printf ("prologix lon: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_prologix_command ("lon", NULL, 0, MD_DEVICE, param))
    return 0;

  return 1;
}

int
prologix_get_lon ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("lon", buf, 1, MD_DEVICE, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_mode (ad_mode_t mode)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (0, mode, 1))
    {
      printf ("prologix mode: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", mode);

  if (!_prologix_command ("mode", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

ad_mode_t
prologix_get_mode ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("mode", buf, 1, MD_BOTH, NULL))
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

  if (!_prologix_command ("read", buf, 0, MD_CONTROLLER, param))
    return 0;

  return 1;
}

int
prologix_read_tmo_ms (int time)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (1, time, 3000))
    {
      printf ("prologix read_tmo_ms: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", time);

  if (!_prologix_command ("read_tmo_ms", NULL, 0, MD_CONTROLLER, param))
    return 0;

  return 1;
}

int
prologix_rst ()
{
  if (!_prologix_command ("rst", NULL, 0, MD_BOTH, NULL))
    return 0;

  return 1;
}

int
prologix_set_savecfg (int val)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (0, val, 1))
    {
      printf ("prologix savecfg: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", val);

  if (!_prologix_command ("savecfg", NULL, 0, MD_BOTH, param))
    return 0;

  return 1;
}

int
prologix_get_savecfg ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("savecfg", buf, 1, MD_BOTH, NULL))
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
      if (!_prologix_limit_check (96, sad, 126))
        {
          printf ("prologix spoll: wrong parameter interval\n");
          return 0;
        }
    }

  if (pad != PROLOGIX_NONE)
    {
      if (!_prologix_limit_check (0, pad, 30))
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
  if (!_prologix_command ("spoll", buf, 0, MD_CONTROLLER, (strcmp (param, "") ? param : NULL)))
    return -1;

  puts (buf);

  return atoi (buf);
}

int
prologix_srq ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("srq", buf, 1, MD_CONTROLLER, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_set_status (int dsb)
{
  char param[STR_MAXLEN];

  if (!_prologix_limit_check (0, dsb, 255))
    {
      printf ("prologix status: wrong parameter interval\n");
      return 0;
    }

  sprintf (param, "%d", dsb);

  if (!_prologix_command ("status", NULL, 0, MD_DEVICE, param))
    return 0;

  return 1;
}

int
prologix_status ()
{
  char buf[BUF_MAXLEN];

  if (!_prologix_command ("status", buf, 1, MD_DEVICE, NULL))
    return -1;

  return atoi (buf);
}

int
prologix_trg (char *buf, char *list)
{
  if (!_prologix_command ("trg", buf, 0, MD_CONTROLLER, list))
    return 0;

  return 1;
}

int
prologix_ver (char *buf)
{
  if (!_prologix_command ("ver", buf, 1, MD_BOTH, NULL))
    return 0;

  return 1;
}

int
_prologix_limit_check (int x, int y, int z)
{
  if (x > y || y > z)
    return 0;

  return 1;
}

int
_prologix_command (char *command_t, char *returnbuf, int onerowanswer, ad_mode_t mode, char* params)
{
  char command[STR_MAXLEN] = "++";
  int i;

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