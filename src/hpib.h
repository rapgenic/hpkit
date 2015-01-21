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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#ifndef COMMON_H
#define	COMMON_H

#define BAUDRATE B115200
#define _POSIX_SOURCE 1

#ifdef	__cplusplus
extern "C"
{
#endif

  extern int f_serial;
  extern struct termios tio_serial;

  int HPIB_serial_config (char *tty, int timeout);
  int HPIB_prologix_config ();
  int HPIB_serial_close ();
  int HPIB_serial_read_until (char *buf, int len, char until);
  int HPIB_serial_write (char *string);
  int HPIB_serial_read_char (char *c);
  
#ifdef	__cplusplus
}
#endif

#endif				/* COMMON_H */
