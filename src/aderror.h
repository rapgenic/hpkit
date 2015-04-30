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
#include <string.h>
#include "colors.h"

#ifndef ADERROR_H
#define	ADERROR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define S_AD_ERR_IO             "Adapter IO error"
#define S_AD_ERR_FILE_IO        "File IO error"
#define S_AD_ERR_SYNTAX         "Wrong syntax in adapter config file"
#define S_AD_ERR_REGEX          "Could not compile regex"
#define S_AD_ERR_RANGE          "Parameter out of range"
#define S_AD_ERR_BUF_EXC        "Warning - buffer exceeded"
#define S_AD_ERR_NO_FUNCTION    "Function isn't defined"
#define S_AD_UNKNOWN            "Unknown error"

    typedef enum {
        AD_SUCCESS = 0,
        AD_ERR_IO,
        AD_ERR_FILE_IO,
        AD_ERR_SYNTAX,
        AD_ERR_REGEX,
        AD_ERR_RANGE,
        AD_ERR_BUF_EXC,
        AD_ERR_NO_FUNCTION,
        AD_UNKNOWN,
    } ad_error_t;

    typedef char ad_error_info_t[BUF_MAXLEN];

    int paderror(ad_error_t aderror, char *str, ad_error_info_t aderror_info);

#ifdef	__cplusplus
}
#endif

#endif	/* ADERROR_H */

