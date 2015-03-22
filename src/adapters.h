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

#include <stdarg.h>
#include <stdio.h>
#include <error.h>
#include <regex.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "aderror.h"
#include "adapters_lowlevel.h"

#ifndef ADAPTERS_H
#define	ADAPTERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define _AD_SAVE_ERROR_INFO sprintf(ad->aderror_info, "In %s: function %s (near line %d)", __FILE__, __func__, __LINE__)

#define AD_SAVE_ERROR_INFO(x) sprintf(x.aderror_info, "In %s: function %s (near line %d)", __FILE__, __func__, __LINE__)

#define _NOT(x, y) if(x == ad_get_curr_errcode(&)) return 0; else return x; 

    typedef struct {
        char __answer[BUF_MAXLEN];
        char *__answer_strtok_t;
        char __answer_separator;
        int __curr_err_ret_val;
    } __ad_temp_vars_t; /* Adapter temp data: you shouldn't use that */

    typedef struct {
        char prefix[STR_MAXLEN];
        char syntax[STR_MAXLEN];
        int baudrate;
    } ad_conf_t;

    typedef struct {
        __ad_temp_vars_t __ad_temp_vars;
        xmlDoc *doc;
        xmlXPathContext *xpath_context;
        ad_error_t aderror;
        ad_error_info_t aderror_info;
        ad_conf_t ad_conf;
        ad_serial_t ad_serial;
    } adapter_t;

    static xmlAttr* _ad_find_node_attr_by_name(xmlAttr *attr, char *name);
    static int _ad_command_compile(adapter_t *ad, char *command_name, size_t count, int values[], char *_command);
    static int _ad_command_read_answer(adapter_t *ad, char *command_name, char *_answer);

    int ad_config(adapter_t *ad, char *filename, char *tty, int timeout);
    int __ad_command(adapter_t *ad, char *command_name, size_t count, int values[]) __attribute__((warn_unused_result));
    int ad_get_next_answer(adapter_t *ad);
    inline int ad_get_curr_errcode(adapter_t *ad);
    int ad_get_const(adapter_t *ad, char *const_name);
    int ad_close(adapter_t *ad);

#define _ad_command(x, y, ...) __ad_command (x, y, sizeof ((int []){ __VA_ARGS__ }) / sizeof (int), (int []){ __VA_ARGS__ })

#define ad_set_address(x, ...)      _ad_command(x, "/adapter/commands/setaddress",  __VA_ARGS__)
#define ad_get_address(x, ...)      _ad_command(x, "/adapter/commands/getaddress",  __VA_ARGS__)
#define ad_set_auto(x, ...)         _ad_command(x, "/adapter/commands/setauto",     __VA_ARGS__)
#define ad_get_auto(x, ...)         _ad_command(x, "/adapter/commands/getauto",     __VA_ARGS__)
#define ad_clr(x, ...)              _ad_command(x, "/adapter/commands/clr",         __VA_ARGS__)
#define ad_set_eoi(x, ...)          _ad_command(x, "/adapter/commands/seteoi",      __VA_ARGS__)
#define ad_get_eoi(x, ...)          _ad_command(x, "/adapter/commands/geteoi",      __VA_ARGS__)
#define ad_set_eos(x, ...)          _ad_command(x, "/adapter/commands/seteos",      __VA_ARGS__)
#define ad_get_eos(x, ...)          _ad_command(x, "/adapter/commands/geteos",      __VA_ARGS__)
#define ad_set_eot_enable(x, ...)   _ad_command(x, "/adapter/commands/seteotenable",__VA_ARGS__)
#define ad_get_eot_enable(x, ...)   _ad_command(x, "/adapter/commands/geteotenable",__VA_ARGS__)
#define ad_set_eot_char(x, ...)     _ad_command(x, "/adapter/commands/seteotchar",  __VA_ARGS__)
#define ad_get_eot_char(x, ...)     _ad_command(x, "/adapter/commands/geteotchar",  __VA_ARGS__)
#define ad_ifc(x, ...)              _ad_command(x, "/adapter/commands/ifc",         __VA_ARGS__)
#define ad_llo(x, ...)              _ad_command(x, "/adapter/commands/llo",         __VA_ARGS__)
#define ad_loc(x, ...)              _ad_command(x, "/adapter/commands/loc",         __VA_ARGS__)
#define ad_set_lon(x, ...)          _ad_command(x, "/adapter/commands/setlon",      __VA_ARGS__)
#define ad_get_lon(x, ...)          _ad_command(x, "/adapter/commands/getlon",      __VA_ARGS__)
#define ad_set_mode(x, ...)         _ad_command(x, "/adapter/commands/setmode",     __VA_ARGS__)
#define ad_get_mode(x, ...)         _ad_command(x, "/adapter/commands/getmode",     __VA_ARGS__)
#define ad_read(x, ...)             _ad_command(x, "/adapter/commands/read",        __VA_ARGS__)
#define ad_set_read_tmo_ms(x, ...)  _ad_command(x, "/adapter/commands/setreadtmoms",__VA_ARGS__)
#define ad_get_read_tmo_ms(x, ...)  _ad_command(x, "/adapter/commands/getreadtmoms",__VA_ARGS__)
#define ad_rst(x, ...)              _ad_command(x, "/adapter/commands/rst",         __VA_ARGS__)
#define ad_set_savecfg(x, ...)      _ad_command(x, "/adapter/commands/setsavecfg",  __VA_ARGS__)
#define ad_get_savecfg(x, ...)      _ad_command(x, "/adapter/commands/getsavecfg",  __VA_ARGS__)
#define ad_spoll(x, ...)            _ad_command(x, "/adapter/commands/spoll",       __VA_ARGS__)
#define ad_srq(x, ...)              _ad_command(x, "/adapter/commands/srq",         __VA_ARGS__)
#define ad_set_status(x, ...)       _ad_command(x, "/adapter/commands/setstatus",   __VA_ARGS__)
#define ad_get_status(x, ...)       _ad_command(x, "/adapter/commands/getstatus",   __VA_ARGS__)
#define ad_trg(x, ...)              _ad_command(x, "/adapter/commands/trg",         __VA_ARGS__)
#define ad_ver(x, ...)              _ad_command(x, "/adapter/commands/ver",         __VA_ARGS__)

#ifdef	__cplusplus
}
#endif

#endif	/* ADAPTERS_H */

