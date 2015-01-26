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
#include "adapters_common.h"

#ifndef PROLOGIX_PROTO_H
#define	PROLOGIX_PROTO_H

#define PROLOGIX_EOI 256
#define PROLOGIX_NONE -1

#ifdef	__cplusplus
extern "C" {
#endif

    int prologix_set_address(int pad, int sad);
    int prologix_get_address(int *returnsad);
    int prologix_set_auto(int val);
    int prologix_get_auto();
    int prologix_clr();
    int prologix_set_eoi(int val);
    int prologix_get_eoi();
    int prologix_set_eos(ad_eos_t mode);
    ad_eos_t prologix_get_eos();
    int prologix_set_eot_enable(int val);
    int prologix_get_eot_enable();
    int prologix_set_eot_char(int c);
    int prologix_get_eot_char();
    int prologix_ifc();
    int prologix_llo();
    int prologix_loc();
    int prologix_set_lon(int val);
    int prologix_get_lon();
    int prologix_set_mode(ad_mode_t mode);
    ad_mode_t prologix_get_mode();
    int prologix_read(char *buf, char until);
    int prologix_read_tmo_ms(int time);
    int prologix_rst();
    int prologix_set_savecfg(int val);
    int prologix_get_savecfg();
    int prologix_spoll(int pad, int sad);
    int prologix_srq();
    int prologix_set_status(int dsb);
    int prologix_status();
    int prologix_trg(char *buf, char *list);
    int prologix_ver(char *buf);

    int _prologix_limit_check(int x, int y, int z);
    int _prologix_command (char *command_t, char *returnbuf, int onerowanswer, ad_mode_t mode, char* params);

#ifdef	__cplusplus
}
#endif

#endif	/* PROLOGIX_PROTO_H */

