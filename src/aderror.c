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

#include "aderror.h"

int paderror(ad_error_t aderror, char *str, ad_error_info_t aderror_info) {
    char msg[STR_MAXLEN];

    switch (aderror) {
        case AD_SUCCESS:
            return 0;
        case AD_ERR_IO:
            strcpy(msg, S_AD_ERR_IO);
            break;
        case AD_ERR_FILE_IO:
            strcpy(msg, S_AD_ERR_FILE_IO);
            break;
        case AD_ERR_SYNTAX:
            strcpy(msg, S_AD_ERR_SYNTAX);
            break;
        case AD_ERR_REGEX:
            strcpy(msg, S_AD_ERR_REGEX);
            break;
        case AD_ERR_RANGE:
            strcpy(msg, S_AD_ERR_RANGE);
            break;
        case AD_ERR_BUF_EXC:
            strcpy(msg, S_AD_ERR_BUF_EXC);
            break;
        case AD_ERR_NO_FUNCTION:
            strcpy(msg, S_AD_ERR_NO_FUNCTION);
            break;
        case AD_UNKNOWN:
            strcpy(msg, S_AD_UNKNOWN);
            break;
        default:
            return 0;
    }

    if (aderror_info != NULL) {
        fprintf(stderr, KRED "%s\n\t", aderror_info);
        aderror_info[0] = 0;
    }

    if (str == NULL)
        fprintf(stderr, "%s\n" KRST, msg);
    else
        fprintf(stderr, "%s: %s\n" KRST, str, msg);

    aderror = AD_SUCCESS;

    return 1;
}
