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
#include "adapters.h"

/**
 * 
 * @param ad
 * @param filename
 * @param tty
 * @param timeout
 * @return 
 */
int ad_config(adapter_t *ad, char *filename, char *tty, int timeout) {
    // resetting adapter struct
    // needed for ad_close
    *ad = (const adapter_t){0};

    xmlXPathObject *xpathObj;

    LIBXML_TEST_VERSION

    ad->doc = xmlReadFile(filename, NULL, 0);

    if (ad->doc == NULL) {
        ad->aderror = AD_ERR_FILE_IO;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    ad->xpath_context = xmlXPathNewContext(ad->doc);

    if (ad->xpath_context == NULL) {
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    xpathObj = xmlXPathEvalExpression("/adapter/commands/@syntax", ad->xpath_context);

    if (xpathObj == NULL) {
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    if (xpathObj->nodesetval->nodeNr != 1) {
        ad->aderror = AD_ERR_SYNTAX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    strcpy(ad->ad_conf.syntax, xpathObj->nodesetval->nodeTab[0]->children->content);

    xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression("/adapter/commands/@prefix", ad->xpath_context);

    if (xpathObj == NULL) {
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    if (xpathObj->nodesetval->nodeNr != 1) {
        ad->aderror = AD_ERR_SYNTAX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    strcpy(ad->ad_conf.prefix, xpathObj->nodesetval->nodeTab[0]->children->content);

    xmlXPathFreeObject(xpathObj);

    xpathObj = xmlXPathEvalExpression("/adapter/@baudrate", ad->xpath_context);

    if (xpathObj == NULL) {
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    if (xpathObj->nodesetval->nodeNr != 1) {
        ad->aderror = AD_ERR_SYNTAX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    // TODO: add baudrate configuration
    ad->ad_conf.baudrate = B115200;

    xmlXPathFreeObject(xpathObj);

    if (!ad_serial_config(&ad->ad_serial, tty, timeout, ad->ad_conf.baudrate)) {
        ad->aderror = ad->ad_serial.aderror;
        ad->ad_serial.aderror = AD_SUCCESS;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    return 1;
}

/**
 * 
 * @param attr
 * @param name
 * @return 
 */
static xmlAttr* _ad_find_node_attr_by_name(xmlAttr *attr, char *name) {
    int x;

    // TODO: check this!!! possible segfault
    do {
        if (strcmp(attr[x].name, name) == 0)
            return &attr[x];
        x++;
    } while (attr[x - 1].next);

    return NULL;
}

/**
 * 
 * @param ad
 * @param command_name
 * @param count
 * @param values
 * @param _command
 * @return 
 */
static int _ad_command_compile(adapter_t *ad, char *command_name, size_t count, int values[], char *_command) {
    int x, y;
    char temp[BUF_MAXLEN];
    char command[STR_MAXLEN] = "";
    xmlXPathObject *current_regex_result;

    // checks mode
    sprintf(temp, "%s/@mode", command_name);

    current_regex_result = xmlXPathEvalExpression(temp, ad->xpath_context);

    if (current_regex_result == NULL) {
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    if (current_regex_result->nodesetval->nodeNr != 1) {
        ad->aderror = AD_ERR_SYNTAX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    if (strcmp(current_regex_result->nodesetval->nodeTab[0]->children->content, "device") == 0) {
        if (ad_get_mode(ad) != ad_get_const(ad, "MD_DEVICE"))
            return 0;
    } else if (strcmp(current_regex_result->nodesetval->nodeTab[0]->children->content, "controller") == 0) {
        if (ad_get_mode(ad) != ad_get_const(ad, "MD_CONTROLLER"))
            return 0;
    }

    xmlXPathFreeObject(current_regex_result);

    for (x = 0; ad->ad_conf.syntax[x] != '\0'; x++) {
        char separator = NULL;
        int size;
        int min = 0, max = 0;

        switch (ad->ad_conf.syntax[x]) {
            case 'p':
                strcat(command, ad->ad_conf.prefix);
                break;
            case 'c':
                sprintf(temp, "%s/@cmd", command_name);

                current_regex_result = xmlXPathEvalExpression(temp, ad->xpath_context);

                if (current_regex_result == NULL) {
                    ad->aderror = AD_ERR_REGEX;
                    _AD_SAVE_ERROR_INFO;
                    return 0;
                }

                if (current_regex_result->nodesetval->nodeNr != 1) {
                    ad->aderror = AD_ERR_SYNTAX;
                    _AD_SAVE_ERROR_INFO;
                    return 0;
                }

                strcat(command, current_regex_result->nodesetval->nodeTab[0]->children->content);

                xmlXPathFreeObject(current_regex_result);
                break;
            case 's':
                strcat(command, " ");
                break;
            case 'a':
                if (ad->ad_conf.syntax[x + 1] != '(') {
                    ad->aderror = AD_ERR_SYNTAX;
                    _AD_SAVE_ERROR_INFO;
                    return 0;
                } else {
                    if (ad->ad_conf.syntax[x + 2] == ')') {
                        ad->aderror = AD_ERR_SYNTAX;
                        _AD_SAVE_ERROR_INFO;
                        return 0;
                    } else {
                        switch (ad->ad_conf.syntax[x + 2]) {
                            case 's':
                                separator = ' ';
                                break;
                            default:
                                separator = ad->ad_conf.syntax[x + 2];
                                break;
                        }
                    }
                }

                if (ad->ad_conf.syntax[x + 3] != ')') {
                    ad->aderror = AD_ERR_SYNTAX;
                    _AD_SAVE_ERROR_INFO;
                    return 0;
                }

                x += 3;

                strcpy(temp, command_name);
                strcat(temp, "/params/param");

                current_regex_result = xmlXPathEvalExpression(temp, ad->xpath_context);

                if (current_regex_result == NULL) {
                    ad->aderror = AD_ERR_REGEX;
                    _AD_SAVE_ERROR_INFO;
                    return 0;
                }

                size = (current_regex_result->nodesetval) ? current_regex_result->nodesetval->nodeNr : 0;

                // Maybe not really necessary...
                //if (size != count) {
                //    fprintf(stderr, "Warning - %s: different number of arguments\n", command_name);
                //}

                // Adds the arguments to the command
                for (y = 0; y < size; y++) {
                    if (y >= count)
                        break;

                    // Determines param type
                    if (strcmp(_ad_find_node_attr_by_name(current_regex_result->nodesetval->nodeTab[y]->properties, "type")->children->content, "limit") == 0) {
                        char temp_xpath_expr[STR_MAXLEN];
                        xmlChar *str;

                        xmlXPathObject *from;
                        xmlXPathObject *to;

                        sprintf(temp_xpath_expr, "%s/params/param[%d]/from", command_name, (y + 1));

                        from = xmlXPathEvalExpression(temp_xpath_expr, ad->xpath_context);

                        if (from == NULL) {
                            ad->aderror = AD_ERR_REGEX;
                            _AD_SAVE_ERROR_INFO;
                            return 0;
                        }

                        if (from->nodesetval->nodeNr != 1) {
                            ad->aderror = AD_ERR_SYNTAX;
                            _AD_SAVE_ERROR_INFO;
                            return 0;
                        }

                        sprintf(temp_xpath_expr, "%s/params/param[%d]/upto", command_name, (y + 1));
                        to = xmlXPathEvalExpression(temp_xpath_expr, ad->xpath_context);

                        if (to == NULL) {
                            ad->aderror = AD_ERR_REGEX;
                            _AD_SAVE_ERROR_INFO;
                            return 0;
                        }

                        if (to->nodesetval->nodeNr != 1) {
                            ad->aderror = AD_ERR_SYNTAX;
                            _AD_SAVE_ERROR_INFO;
                            return 0;
                        }

                        min = atoi(from->nodesetval->nodeTab[0]->children->content);
                        max = atoi(to->nodesetval->nodeTab[0]->children->content);

                        xmlXPathFreeObject(from);
                        xmlXPathFreeObject(to);
                    } else if (strcmp(_ad_find_node_attr_by_name(current_regex_result->nodesetval->nodeTab[y]->properties, "type")->children->content, "bit") == 0) {
                        min = 0;
                        max = 1;
                    }

                    // Checks value
                    if (min == max) {
                        ad->aderror = AD_ERR_SYNTAX;
                        _AD_SAVE_ERROR_INFO;
                        return 0;
                    }

                    if (values[y] < min || values[y] > max) {
                        ad->aderror = AD_ERR_RANGE;
                        _AD_SAVE_ERROR_INFO;
                        return 0;
                    } else {
                        char add[STR_MAXLEN];

                        strncat(command, &separator, 1);
                        sprintf(add, "%d", values[y]);
                        strcat(command, add);
                    }
                }

                xmlXPathFreeObject(current_regex_result);

                break;
        }
    }

    strcat(command, "\r\n");

    strcpy(_command, command);

    return 1;
}

/**
 * 
 * @param ad
 * @param command_name
 * @param _answer
 * @return 
 */
static int _ad_command_read_answer(adapter_t *ad, char *command_name, char *_answer) {
    char temp[BUF_MAXLEN];
    char returnbuf[BUF_MAXLEN];
    int i = 0;
    xmlXPathObject *current_regex_result;
    xmlXPathObject *second_regex_result;

    sprintf(temp, "%s/answer", command_name);

    current_regex_result = xmlXPathEvalExpression(temp, ad->xpath_context);

    if (current_regex_result == NULL) {
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    if (current_regex_result->nodesetval->nodeNr != 1) {
        ad->aderror = AD_ERR_SYNTAX;
        _AD_SAVE_ERROR_INFO;
        return 0;
    }

    if (current_regex_result->nodesetval->nodeTab[0]->children) {
        if (strcmp(current_regex_result->nodesetval->nodeTab[0]->children->next->name, "separator") == 0) {
            sprintf(temp, "%s/answer/separator", command_name);

            second_regex_result = xmlXPathEvalExpression(temp, ad->xpath_context);

            if (second_regex_result == NULL) {
                ad->aderror = AD_ERR_REGEX;
                _AD_SAVE_ERROR_INFO;
                return 0;
            }

            if (second_regex_result->nodesetval->nodeNr != 1) {
                ad->aderror = AD_ERR_SYNTAX;
                _AD_SAVE_ERROR_INFO;
                return 0;
            }

            if (second_regex_result->nodesetval->nodeTab[0]->children) {
                if (second_regex_result->nodesetval->nodeTab[0]->children->type == XML_TEXT_NODE) {
                    xmlChar *str = second_regex_result->nodesetval->nodeTab[0]->children->content;

                    if (strlen(str) != 1) {
                        ad->aderror = AD_ERR_SYNTAX;
                        _AD_SAVE_ERROR_INFO;
                        return 0;
                    }

                    switch (str[0]) {
                        case 's':
                            ad->__ad_temp_vars.__answer_separator = ' ';
                            break;
                        default:
                            ad->__ad_temp_vars.__answer_separator = str[0];
                    }
                }
            }

            xmlXPathFreeObject(second_regex_result);
        } else if (strcmp(current_regex_result->nodesetval->nodeTab[0]->children->next->name, "str") == 0) {
            ad->__ad_temp_vars.__answer_separator = 0;
        }

        if (1) {
            if (!ad_serial_read_until(&ad->ad_serial, returnbuf, BUF_MAXLEN, '\n')) {
                ad->aderror = AD_ERR_IO;
                _AD_SAVE_ERROR_INFO;
                return 0;
            }
        } else {
            while (ad_serial_read_char(&ad->ad_serial, &returnbuf[i]))
                i++;

            returnbuf[i++] = '\0';
        }

        if (!strcmp(returnbuf, "") || !strcmp(returnbuf, "Unrecognized command\r\n")) {
            ad->aderror = AD_ERR_IO;
            _AD_SAVE_ERROR_INFO;
            return 0;
        }

        strcpy(_answer, returnbuf);
    } else {
        _answer[0] = '\0';
    }

    xmlXPathFreeObject(current_regex_result);

    return 1;
}

/**
 * 
 * @param ad
 * @param command_name
 * @param count
 * @param values
 * @return 
 */
int __ad_command(adapter_t *ad, char *command_name, size_t count, int values[]) {
    char command[STR_MAXLEN];
    char temp[BUF_MAXLEN];
    char answer[BUF_MAXLEN];
    char *answer_str;
    char *answer_str_strtol_endptr;
    int converted_answer;
    xmlXPathObject *current_regex_result;

    ad->__ad_temp_vars.__curr_err_ret_val = -1;

    // Check whether function exists
    current_regex_result = xmlXPathEvalExpression(command_name, ad->xpath_context);

    if (current_regex_result == NULL) {
        fprintf(stderr, "Warning - __ad_command() error return value could be mismatched\n");
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return ad->__ad_temp_vars.__curr_err_ret_val;
    }

    if (!current_regex_result->nodesetval->nodeNr) {
        fprintf(stderr, "Warning - __ad_command() error return value could be mismatched\n");
        ad->aderror = AD_ERR_NO_FUNCTION;
        _AD_SAVE_ERROR_INFO;
        return ad->__ad_temp_vars.__curr_err_ret_val;
    }

    xmlXPathFreeObject(current_regex_result);

    // Read error code
    sprintf(temp, "%s/return/error", command_name);

    current_regex_result = xmlXPathEvalExpression(temp, ad->xpath_context);

    if (current_regex_result == NULL) {
        fprintf(stderr, "Warning - __ad_command() error return value could be mismatched\n");
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return ad->__ad_temp_vars.__curr_err_ret_val;
    }

    if (current_regex_result->nodesetval->nodeNr != 1) {
        fprintf(stderr, "Warning - __ad_command() error return value could be mismatched\n");
        ad->aderror = AD_ERR_SYNTAX;
        _AD_SAVE_ERROR_INFO;
        return ad->__ad_temp_vars.__curr_err_ret_val;
    }

    if (current_regex_result->nodesetval->nodeTab[0]->children) {
        if (current_regex_result->nodesetval->nodeTab[0]->children->type == XML_TEXT_NODE) {
            ad->__ad_temp_vars.__curr_err_ret_val = atoi(current_regex_result->nodesetval->nodeTab[0]->children->content);
        }
    }

    xmlXPathFreeObject(current_regex_result);

    // Create command string
    if (!_ad_command_compile(ad, command_name, count, values, command)) {
        // aderror is already set
        return ad->__ad_temp_vars.__curr_err_ret_val;
    }

    // Send the command to the adapter
    if (!ad_serial_write(&ad->ad_serial, command)) {
        ad->aderror = ad->ad_serial.aderror;
        ad->ad_serial.aderror = AD_SUCCESS;
        _AD_SAVE_ERROR_INFO;
        return ad->__ad_temp_vars.__curr_err_ret_val;
    }

    // Read the adapter's answer
    if (!_ad_command_read_answer(ad, command_name, ad->__ad_temp_vars.__answer)) {
        // aderror is already set
        return ad->__ad_temp_vars.__curr_err_ret_val;
    }

    // If there is an answer AND if it isn't a <str/> answer, process it
    if (ad->__ad_temp_vars.__answer[0] != '\0' && ad->__ad_temp_vars.__answer_separator != 0) {
        answer_str = strtok_r(ad->__ad_temp_vars.__answer, &ad->__ad_temp_vars.__answer_separator, &ad->__ad_temp_vars.__answer_strtok_t);

        // To distinguish failure after call
        errno = 0;
        converted_answer = strtol(answer_str, &answer_str_strtol_endptr, 10);

        // Check for errors: every error is considered a ADAPTER IO ERROR
        if ((errno == ERANGE && (converted_answer == LONG_MAX || converted_answer == LONG_MIN)) || (errno != 0 && converted_answer == 0)) {
            _AD_SAVE_ERROR_INFO;
            ad->aderror = AD_ERR_IO;
            return ad->__ad_temp_vars.__curr_err_ret_val;
        }

        if (answer_str_strtol_endptr == answer_str) {
            _AD_SAVE_ERROR_INFO;
            ad->aderror = AD_ERR_IO;
            return ad->__ad_temp_vars.__curr_err_ret_val;
        }

        return converted_answer;
    } else {
        // success is the error value - 1
        return (ad->__ad_temp_vars.__curr_err_ret_val - 1);
    }
}

/**
 * 
 * @param ad
 * @return 
 */
int ad_get_next_answer(adapter_t *ad) {
    char *answer_str;
    char *endptr;
    int converted_answer;

    if (ad->__ad_temp_vars.__answer[0] != '\0') {
        answer_str = strtok_r(NULL, &ad->__ad_temp_vars.__answer_separator, &ad->__ad_temp_vars.__answer_strtok_t);

        if (answer_str == NULL)
            return ad->__ad_temp_vars.__curr_err_ret_val;

        // To distinguish failure after call
        errno = 0;
        converted_answer = strtol(answer_str, &endptr, 10);

        // Check for errors: every error is considered a ADAPTER IO ERROR
        if ((errno == ERANGE && (converted_answer == LONG_MAX || converted_answer == LONG_MIN))
                || (errno != 0 && converted_answer == 0)) {
            _AD_SAVE_ERROR_INFO;
            ad->aderror = AD_ERR_IO;
            return ad->__ad_temp_vars.__curr_err_ret_val;
        }

        if (endptr == answer_str) {
            _AD_SAVE_ERROR_INFO;
            ad->aderror = AD_ERR_IO;
            return ad->__ad_temp_vars.__curr_err_ret_val;
        }

        return converted_answer;
    } else {
        return ad->__ad_temp_vars.__curr_err_ret_val;
    }
}

/**
 * 
 * @param ad
 * @return 
 */
inline int ad_get_curr_errcode(adapter_t *ad) {
    return ad->__ad_temp_vars.__curr_err_ret_val;
}

/**
 * 
 * @param ad
 * @param const_name
 * @return 
 */
int ad_get_const(adapter_t *ad, char *const_name) {
    xmlXPathObject *current_regex_result;
    char temp[STR_MAXLEN];

    sprintf(temp, "/adapter/const[@name=\"%s\"]/text()", const_name);

    current_regex_result = xmlXPathEvalExpression(temp, ad->xpath_context);

    if (current_regex_result == NULL) {
        ad->aderror = AD_ERR_REGEX;
        _AD_SAVE_ERROR_INFO;
        return -1;
    }

    if (current_regex_result->nodesetval->nodeNr != 1) {
        ad->aderror = AD_ERR_NO_FUNCTION;
        _AD_SAVE_ERROR_INFO;
        return -1;
    }

    return atoi(current_regex_result->nodesetval->nodeTab[0]->content);

    return -1;
}

/**
 * 
 * @param ad
 * @return 
 */
int ad_close(adapter_t *ad) {
    if (ad->ad_serial.f_serial) {
        ad_serial_close(&ad->ad_serial);
    }
    if (ad->xpath_context) {
        xmlXPathFreeContext(ad->xpath_context);
    }
    if (ad->doc) {
        xmlFreeDoc(ad->doc);
    }

    xmlCleanupParser();

    return 1;
}