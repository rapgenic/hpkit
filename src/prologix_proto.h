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

#include "hpib.h"
#include "adapters_common.h"

#ifndef PROLOGIX_PROTO_H
#define	PROLOGIX_PROTO_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    int prologix_set_address(int address);
    int prologix_get_address();
    int prologix_set_mode(ad_mode_t mode);
    ad_mode_t prologix_get_mode();
    int prologix_set_lon(int val);
    int prologix_get_lon();
    int prologix_set_auto(int val);
    int prologix_get_auto();
    
    int _prologix_read(char *buf, char *prop);

#ifdef	__cplusplus
}
#endif

#endif	/* PROLOGIX_PROTO_H */

