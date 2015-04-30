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

#include "adapters_lowlevel.h"
#include "aderror.h"
#include "colors.h"

/**
 * 
 * @param ser
 * @param tty
 * @param timeout
 * @param baudrate
 * @return 
 */
int ad_serial_config(ad_serial_t *ser, char *tty, int timeout, int baudrate) {
    struct termios tio_serial;

    ser->f_serial = open(tty, O_RDWR | O_NOCTTY);

    if (ser->f_serial == -1) {
        ser->aderror = AD_ERR_IO;
        return 0;
    }

    memset(&tio_serial, 0, sizeof (tio_serial));

    tio_serial.c_iflag = 0;
    tio_serial.c_oflag = 0;
    tio_serial.c_cflag = CS8 | CREAD | CLOCAL;
    tio_serial.c_lflag &= ~ICANON;
    tio_serial.c_cc[VMIN] = 0;
    tio_serial.c_cc[VTIME] = timeout;

    cfsetospeed(&tio_serial, baudrate);
    cfsetispeed(&tio_serial, baudrate);

    tcsetattr(ser->f_serial, TCSANOW, &tio_serial);

    return 1;
}

/**
 * 
 * @param ser
 * @param buf
 * @param len
 * @param until
 * @return 
 */
int ad_serial_read_until(ad_serial_t *ser, char *buf, int len, char until) {
    char c;
    int count = 0;

    memset(buf, 0, len);

    do {
        if (!ad_serial_read_char(ser, &c)) {
            ser->aderror = AD_SUCCESS;
            return 0;
        }

        if (count >= len) {
            ser->aderror = AD_ERR_BUF_EXC;
            return 0;
        }

        buf[count] = c;
        count++;
    } while (c != until);

    buf[count] = '\0';

    return 1;
}

/**
 * 
 * @param ser
 * @param c
 * @return 
 */
int ad_serial_read_char(ad_serial_t *ser, char *c) {
    int rr;

    rr = read(ser->f_serial, c, 1);

    if (rr == -1) {
        ser->aderror = AD_ERR_IO;
        return 0;
    }

    if (rr == 0) {
        ser->aderror = AD_SUCCESS;
        return 0;
    }

    return 1;
}

/**
 * 
 * @param ser
 * @param string
 * @return 
 */
int ad_serial_write(ad_serial_t *ser, char *string) {
    if (write(ser->f_serial, string, strlen(string)) == -1) {
        ser->aderror = AD_ERR_IO;
        return 0;
    }

    return 1;
}

/**
 * 
 * @param ser
 * @return 
 */
int ad_serial_close(ad_serial_t *ser) {
    close(ser->f_serial);
}

