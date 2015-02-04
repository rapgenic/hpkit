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

#include "hpib.h"

#ifndef ADAPTERS_COMMON_H
#define	ADAPTERS_COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PROLOGIX_COMMAND_PREFIX "++"
#define GALVANT_COMMAND_PREFIX  "++"

    typedef enum {
        MD_BOTH = -1,
        MD_DEVICE = 0,
        MD_CONTROLLER,
    } ad_mode_t;

    typedef enum {
        EO_CRLF = 0,
        EO_CR,
        EO_LF,
        EO_NONE
    } ad_eos_t;

    typedef struct {
        char command_prefix[STR_MAXLEN];
    } ad_flags_t;

    ad_flags_t ad_flags;

    int _limit_check(int x, int y, int z);
    int _command(char *command_t, char *returnbuf, int onerowanswer, ad_mode_t mode, char* params);

#ifdef	__cplusplus
}
#endif

#endif	/* ADAPTERS_COMMON_H */

