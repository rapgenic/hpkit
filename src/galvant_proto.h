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

#ifndef GALVANT_PROTO_H
#define	GALVANT_PROTO_H

#define PROLOGIX_EOI 256
#define PROLOGIX_NONE -1

#ifdef	__cplusplus
extern "C" {
#endif

    // SAD is not necessary
    int galvant_set_address(int pad, int sad);
    int galvant_get_address();
    int galvant_set_auto(int val);
    int galvant_get_auto();
    
#ifdef	__cplusplus
}
#endif

#endif	/* GALVANT_PROTO_H */

