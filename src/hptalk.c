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

#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "adapters.h"
#include "common.h"

#define PROGRAM_NAME "hptalk"

int
main(int argc, char **argv) {
    // temp vars
    char buf[BUF_MAXLEN];
    int c, d;
    char command[STR_MAXLEN];
    char temp[STR_MAXLEN];
    char *endptr, *endptr_s;
    char *pad_temp, *sad_temp;

    // default values
    char config_file[STR_MAXLEN] = "prologix";
    char tty[STR_MAXLEN] = "/dev/ttyUSB0";
    FILE *f_output = NULL;
    int using_output_file = 0;
    int config_adapter = 1;
    int read_answer = 1;
    char output_path[STR_MAXLEN] = "";
    int timeout = 20;
    int ad_address = 20;
    int ad_saddress = -1;
    int multi_row_answer = 0;

    // the adapter
    adapter_t ad;

    // arguments parsing
    while (1) {
        static struct option long_options[] = {
            {"device", required_argument, 0, 'd'},
            {"adapter", required_argument, 0, 'a'},
            {"address", required_argument, 0, 'r'},
            {"output", required_argument, 0, 'o'},
            {"timeout", required_argument, 0, 't'},
            {"no-answer", no_argument, 0, 'n'},
            {"no-config", no_argument, 0, 'c'},
            {"multi-row-answer", no_argument, 0, 'm'},
            {"help", no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "d:a:r:o:t:ncmhv", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 'd':
                strcpy(tty, optarg);
                break;

            case 'a':
                if (optarg != "")
                    strcpy(config_file, optarg);
                break;

            case 'r':
                pad_temp = strtok(optarg, ",");
                sad_temp = strtok(NULL, ",");

                ad_address = strtol(pad_temp, &endptr, 10);
                if (*endptr != '\0') {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -r|--address: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }

                if (sad_temp != NULL) {
                    ad_saddress = strtol(sad_temp, &endptr_s, 10);
                    if (*endptr_s != '\0') {
                        AD_SAVE_ERROR_INFO(ad);
                        fprintf(stderr, "%s\n\tWhile processing -r|--address: Non valid option\n", ad.aderror_info);
                        return EXIT_FAILURE;
                    }
                }
                break;

            case 'o':
                strcpy(output_path, optarg);
                using_output_file = 1;
                break;

            case 't':
                d = strtol(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -t|--timeout: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }

                if (d < 100) {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -t|--timeout: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }

                timeout = d / 100;
                break;

            case 'n':
                read_answer = 0;
                break;

            case 'c':
                config_adapter = 0;
                break;

            case 'm':
                multi_row_answer = 1;
                break;

            case 'h':
                puts("\
Usage: hptalk [OPTIONS]... [COMMAND]\n\
Send a command to an instrument using GPIB interface and read the answer\n");
                puts("\
  -d, --device=STRING       communicate with serial interface defined by STRING;\n\
                            default: '/dev/ttyUSB0'\n\
  -a, --adapter=ADAPTER     set the adapter used to communicate with the\n\
                            instrument; default: 'prologix'\n\
  -r, --address=PAD,[SAD]   choose the address used by the instrument;\n\
                            (PAD is the primary address, SAD the secondary which\n\
                            isn't necessary); default: 20,0\n\
  -o, --output=OUTFILE      send data to FILE instead of stdout\n\
  -t, --timeout=TIMEOUT     set TIMEOUT in millisecs for serial port; default:\n\
                            2000, min: 100\n\
  -n, --no-answer           don't wait for the instrument's answer\n\
  -c, --no-config           don't configure the adapter (it will also disable\n\
                            the -r|--address option)\n\
  -m, --multi-row-answer    read more than one row in the answer\n\
  -h, --help                show this help and exit\n\
  -v, --version             show information about program version and exit\n");
                adapters_list();
                version(PROGRAM_NAME);
                help();
                return (EXIT_SUCCESS);
                break;

            case 'v':
                version(PROGRAM_NAME);
                return (EXIT_SUCCESS);
                break;

            case '?':
                puts("\
Usage: hptalk [OPTIONS]... [COMMAND]\n\
Send a command to an instrument using GPIB interface and read the answer\n");
                return (EXIT_FAILURE);

            default:
                abort();
        }
    }

    if (optind >= argc) {
        AD_SAVE_ERROR_INFO(ad);
        fprintf(stderr, "%s\n\tWhile processing arguments: Expected command\n", ad.aderror_info);
        return EXIT_FAILURE;
    }

    strcpy(temp, DATA_PATH);
    strcat(temp, config_file);
    strcat(temp, ".conf.xml");

    if (!ad_config(&ad, temp, tty, timeout)) {
        AD_SAVE_ERROR_INFO(ad);
        paderror(ad.aderror, "While configuring the adapter", ad.aderror_info);
        ad_close(&ad);
        return EXIT_FAILURE;
    }

    // check and config adapter's settings
    if (config_adapter) {
        int mode = -1;

        mode = ad_get_mode(&ad);

        if (mode != ad_get_curr_errcode(&ad))
            if (mode == ad_get_const(&ad, "MD_DEVICE"))
                if (ad_set_mode(&ad, ad_get_const(&ad, "MD_CONTROLLER")) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While configuring the adapter", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }

        if (read_answer) {
            if (ad_set_auto(&ad, ad_get_const(&ad, "ENABLED")) == ad_get_curr_errcode(&ad)) {
                AD_SAVE_ERROR_INFO(ad);
                paderror(ad.aderror, "While configuring the adapter", ad.aderror_info);
                ad_close(&ad);
                return EXIT_FAILURE;
            }
        } else {
            if (ad_set_auto(&ad, ad_get_const(&ad, "DISABLED")) == ad_get_curr_errcode(&ad)) {
                AD_SAVE_ERROR_INFO(ad);
                paderror(ad.aderror, "While configuring the adapter", ad.aderror_info);
                ad_close(&ad);
                return EXIT_FAILURE;
            }
        }

        if (ad_saddress != -1) {
            if (ad_set_address(&ad, ad_address, ad_saddress) == ad_get_curr_errcode(&ad)) {
                AD_SAVE_ERROR_INFO(ad);
                paderror(ad.aderror, "While configuring the adapter", ad.aderror_info);
                ad_close(&ad);
                return EXIT_FAILURE;
            }
        } else {
            if (ad_set_address(&ad, ad_address) == ad_get_curr_errcode(&ad)) {
                AD_SAVE_ERROR_INFO(ad);
                paderror(ad.aderror, "While configuring the adapter", ad.aderror_info);
                ad_close(&ad);
                return EXIT_FAILURE;
            }
        }
    }

    // open the output file
    if (using_output_file) {
        f_output = fopen(output_path, "w");

        if (f_output == NULL) {
            AD_SAVE_ERROR_INFO(ad);
            paderror(ad.aderror, "While opening the output file", ad.aderror_info);
            ad_close(&ad);
            return EXIT_FAILURE;
        }
    }

    strcpy(command, argv[(optind++)]);
    strcat(command, "\r\n");

    if (!ad_serial_write(&ad.ad_serial, command)) {
        AD_SAVE_ERROR_INFO(ad);
        paderror(ad.ad_serial.aderror, "While writing the serial interface", ad.aderror_info);
        ad_close(&ad);

        if (using_output_file)
            fclose(f_output);

        return EXIT_FAILURE;
    }

    if (read_answer) {
        if (multi_row_answer) {
            while (ad_serial_read_until(&ad.ad_serial, buf, BUF_MAXLEN, '\n')) {
                if (using_output_file) {
                    fprintf(f_output, "%s", buf);
                } else {
                    printf("%s", buf);
                }
            }
        } else {
            if (!ad_serial_read_until(&ad.ad_serial, buf, BUF_MAXLEN, '\n')) {
                ad_close(&ad);

                if (using_output_file) {
                    fclose(f_output);
                }

                return EXIT_SUCCESS;
            }

            // print the output
            if (using_output_file) {
                fprintf(f_output, "%s", buf);
            } else {
                printf("%s", buf);
            }
        }
    }

    // close the devicse
    ad_close(&ad);

    if (using_output_file)
        fclose(f_output);

    // end
    return EXIT_SUCCESS;
}