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

#include "hpib.h"

#ifndef PROTO_H
#define	PROTO_H

#ifdef	__cplusplus
extern "C"
{
#endif

  int _HPIB_proto_response_required (char *buf);
  void _HPIB_proto_get_command (char *buf, char cmd[2]);
  int HPIB_proto_parse (char *buf);

#ifdef	__cplusplus
}
#endif

#endif				/* PROTO_H */
