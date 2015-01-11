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

#include "adapters_common.h"
#include "prologix_proto.h"

#define TRUE 1
#define FALSE 0

#ifndef ADAPTERS_H
#define	ADAPTERS_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        AD_PROLOGIX = 0,
    } adapters_t;

    int set_adapter(adapters_t ad);

    int (*ad_set_address)(int addr);
    int (*ad_get_address)();
    int (*ad_set_mode)(ad_mode_t mode);
    ad_mode_t(*ad_get_mode)();
    int (*ad_set_lon)(int val);
    int (*ad_get_lon)();
    int (*ad_set_auto)(int val);
    int (*ad_get_auto)();

#ifdef	__cplusplus
}
#endif

#endif	/* ADAPTERS_H */

