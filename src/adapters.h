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

#include "adapters_common.h"
#include "prologix_proto.h"
#include "galvant_proto.h"

#define TRUE 1
#define FALSE 0

#ifndef ADAPTERS_H
#define	ADAPTERS_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        AD_PROLOGIX = 0,
        AD_GALVANT,
    } adapters_t;

    int set_adapter(adapters_t ad);

    int (*ad_set_address)(int pad, int sad);
    int (*ad_get_address)(int *returnsad);
    int (*ad_set_auto)(int val);
    int (*ad_get_auto)();
    int (*ad_clr)();
    int (*ad_set_eoi)(int val);
    int (*ad_get_eoi)();
    int (*ad_set_eos)(ad_eos_t mode);
    ad_eos_t(*ad_get_eos)();
    int (*ad_set_eot_enable)(int val);
    int (*ad_get_eot_enable)();
    int (*ad_set_eot_char)(int c);
    int (*ad_get_eot_char)();
    int (*ad_ifc)();
    int (*ad_llo)();
    int (*ad_loc)();
    int (*ad_set_lon)(int val);
    int (*ad_get_lon)();
    int (*ad_set_mode)(ad_mode_t mode);
    ad_mode_t(*ad_get_mode)();
    int (*ad_read)(char *buf, char until);
    int (*ad_read_tmo_ms)(int time);
    int (*ad_rst)();
    int (*ad_set_savecfg)(int val);
    int (*ad_get_savecfg)();
    int (*ad_spoll)(int pad, int sad);
    int (*ad_srq)();
    int (*ad_set_status)(int dsb);
    int (*ad_status)();
    int (*ad_trg)(char *buf, char *list);
    int (*ad_ver)(char *buf);

#ifdef	__cplusplus
}
#endif

#endif	/* ADAPTERS_H */

