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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "aderror.h"

#ifndef ADAPTERS_LOWLEVEL_H
#define	ADAPTERS_LOWLEVEL_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct {
        int f_serial;
        ad_error_t aderror;
    } ad_serial_t;

    int ad_serial_config(ad_serial_t *ser, char *tty, int timeout, int baudrate);
    int ad_serial_read_until(ad_serial_t *ser, char *buf, int len, char until);
    int ad_serial_read_char(ad_serial_t *ser, char *c);
    int ad_serial_write(ad_serial_t *ser, char *string);
    int ad_serial_close(ad_serial_t *ser);

#ifdef	__cplusplus
}
#endif

#endif	/* ADAPTERS_LOWLEVEL_H */

