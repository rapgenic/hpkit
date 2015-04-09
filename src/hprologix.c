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

#define PROGRAM_NAME "hprologix"

#define PROLOGIX_D_MODE "MD_CONTROLLER"
#define PROLOGIX_D_ADDR 20
#define PROLOGIX_D_SADD -1
#define PROLOGIX_D_AUTO "ENABLED"
#define PROLOGIX_D_CEOI "DISABLED"
#define PROLOGIX_D_CEOT "DISABLED"
#define PROLOGIX_D_EOTC '\0'
#define PROLOGIX_D_CEOS "EO_CRLF"

#define PROLOGIX_VERSION_STR "Prologix GPIB-USB Controller version 6.107\r\n" 

typedef enum {
    SIG_IFC = 0,
    SIG_CLR,
    SIG_LOC,
    SIG_SRQ,
    SIG_SPL,
    SIG_RST
} prologix_signal_t;

int
main(int argc, char **argv) {
    // temp vars
    int c;
    char *endptr, *endptr_s;
    char *pad_temp, *sad_temp;

    // default values
    char tty[STR_MAXLEN] = "/dev/ttyUSB0";
    char config_file[STR_MAXLEN];
    int get = 0;

    // adapter
    adapter_t ad;

    // adapter default options
    int p_mode = -1;
    int p_padd = -1;
    int p_sadd = -1;
    int p_auto = -1;
    int p_ceoi = -1;
    int p_ceot = -1;
    int p_ceos = -1;
    int p_sign = -1;

    // arguments parsing
    while (1) {
        static struct option long_options[] = {
            {"device", required_argument, 0, 'd'},
            {"get-config", no_argument, 0, 'g'},
            {"mode", required_argument, 0, 'm'},
            {"address", required_argument, 0, 'r'},
            {"autoread", required_argument, 0, 'a'},
            {"eoi", required_argument, 0, 'e'},
            {"eot", required_argument, 0, 't'},
            {"eos", required_argument, 0, 's'},
            {"signal", required_argument, 0, 'n'},
            {"help", no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "d:gm:r:a:e:t:s:n:hv", long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c) {
            case 'd':
                strcpy(tty, optarg);
                break;

            case 'g':
                get = 1;
                break;

            case 'm':
                p_mode = strtol(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -m|--mode: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }
                break;

            case 'r':
                pad_temp = strtok(optarg, ",");
                sad_temp = strtok(NULL, ",");

                p_padd = strtol(pad_temp, &endptr, 10);
                if (*endptr != '\0') {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -r|--address: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }

                if (sad_temp != NULL) {
                    p_sadd = strtol(sad_temp, &endptr_s, 10);
                    if (*endptr_s != '\0') {
                        AD_SAVE_ERROR_INFO(ad);
                        fprintf(stderr, "%s\n\tWhile processing -r|--address: Non valid option\n", ad.aderror_info);
                        return EXIT_FAILURE;
                    }
                }

                break;

            case 'a':
                p_auto = strtol(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -a|--autoread: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }
                break;

            case 'e':
                p_ceoi = strtol(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -e|--eoi: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }
                break;

            case 't':
                p_ceot = strtol(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -t|--eot: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }
                break;

            case 's':
                p_ceos = strtol(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -s|--eos: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }
                break;

            case 'n':
                printf("Sending signal %s\n", optarg);

                if (!strcmp(optarg, "IFC"))
                    p_sign = SIG_IFC;
                else if (!strcmp(optarg, "CLR"))
                    p_sign = SIG_CLR;
                else if (!strcmp(optarg, "LOC"))
                    p_sign = SIG_LOC;
                else if (!strcmp(optarg, "SRQ"))
                    p_sign = SIG_SRQ;
                else if (!strcmp(optarg, "SPOLL"))
                    p_sign = SIG_SPL;
                else if (!strcmp(optarg, "RST"))
                    p_sign = SIG_RST;
                else {
                    AD_SAVE_ERROR_INFO(ad);
                    fprintf(stderr, "%s\n\tWhile processing -n|--signal: Non valid option\n", ad.aderror_info);
                    return EXIT_FAILURE;
                }

                break;

            case 'h':
                puts("\
Usage: hprologix [OPTIONS]...\n\
Configure the prologix adapter\n");
                puts("\
  -d, --device=STRING       communicate with serial interface defined by STRING;\n\
                            default: '/dev/ttyUSB0'\n\
  -g, --get-config          get current cofiguration\n\
  -m, --mode=MODE           set adapter mode (0: DEVICE, 1: CONTROLLER);\n\
                            default: 1\n\
  -r, --address=PAD,[SAD]   set the address used by the instrument (PAD is the\n\
                            primary address, SAD the secondary which isn't\n\
                            necessary); default: 20,0\n\
  -a, --autoread=BOOL       set whether the adapter should read the instrument\n\
                            answer (disabled if mode is DEVICE); default: 1;\n\
  -e, --eoi=BOOL            enable or disable EOI signal with last transmitted\n\
                            byte\n\
  -t, --eot=CHAR            enable EOT and set the EOT character (-1: DISABLED);\n\
                            default: -1\n\
  -s, --eos=EOS             set GPIB temination characters (0: CR+LF, 1: CR,\n\
                            2: LF, 3: NONE); default: 0\n\
  -n, --signal=SIG          send to the adapter the signal SIG (disables any\n\
                            other option, except for -d, -h and -v)\n\
  -h, --help                show this help and exit\n\
  -v, --version             show information about program version and exit\n");
                puts("\
SIG stands for one of the following signals:\n\
  IFC                       send the IFC signal\n\
  CLR                       send the SDC (Selected Device Clear) to the current\n\
                            address\n\
  LOC                       enable front panel operation of the currently\n\
                            addressed instrument\n\
  SRQ                       return the current state of the SRQ signal (0: NOT\n\
                            ASSERTED, 1: ASSERTED\n\
  SPOLL                     perform a serial poll of the instrument at the\n\
                            specified address\n\
  RST                       perform a controller reset (it takes about 5 secs)\n");
                version(PROGRAM_NAME);
                help();
                return EXIT_SUCCESS;
                break;

            case 'v':
                version(PROGRAM_NAME);
                return EXIT_SUCCESS;
                break;

            case '?':
                puts("\
Usage: hprologix [OPTIONS]... [COMMAND]\n\
Configure the prologix adapter\n");
                return EXIT_FAILURE;

            default:
                abort();
        }
    }

    strcpy(config_file, DATA_PATH);
    strcat(config_file, "prologix.conf.xml");

    if (!ad_config(&ad, config_file, tty, 20)) {
        AD_SAVE_ERROR_INFO(ad);
        paderror(ad.aderror, "While configuring the adapter", ad.aderror_info);
        ad_close(&ad);
        return EXIT_FAILURE;
    }

    if (ad_ver(&ad) == ad_get_curr_errcode(&ad)) {
        AD_SAVE_ERROR_INFO(ad);
        paderror(ad.aderror, "While checking the adapter", ad.aderror_info);
        ad_close(&ad);
        return EXIT_FAILURE;
    }

    if (strcmp(PROLOGIX_VERSION_STR, ad.__ad_temp_vars.__answer) != 0) {
        AD_SAVE_ERROR_INFO(ad);
        fprintf(stderr, "%s\n\tWarning - Adapter version line doesn't match\n", ad.aderror_info);
        fprintf(stderr, "%s\n", ad.__ad_temp_vars.__answer);
    }

    if (p_sign != -1) {
        int val = 0;
        int srq;

        switch (p_sign) {
            case SIG_IFC:
                if (ad_ifc(&ad) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While sending IFC signal", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
                break;

            case SIG_CLR:
                if (ad_clr(&ad) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While sending CLR signal", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
                break;

            case SIG_LOC:
                if (ad_loc(&ad) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While sending LOC signal", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
                break;

            case SIG_SRQ:
                srq = ad_srq(&ad);
                if (srq != ad_get_curr_errcode(&ad))
                    printf("Signal returned: %d\n", ad_srq(&ad));
                else {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While sending SRQ signal", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }

                break;

            case SIG_SPL:
                // TODO: check this
                val = ad_spoll(&ad);
                printf("Signal returned: ");
                if (val != ad_get_curr_errcode(&ad))
                    printf("%d\n", val);
                else
                    printf("no answer\n");
                break;

            case SIG_RST:
                if (ad_rst(&ad) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While sending RST signal", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
                break;
        }
        return EXIT_SUCCESS;
    }


    if (get) {
        int mod;
        int pad;
        int sad;
        int eos;
        int eoi;
        int eot_e;

        mod = ad_get_mode(&ad);

        if (mod != ad_get_curr_errcode(&ad))
            printf("Mode: %s\n", (mod == ad_get_const(&ad, "MD_CONTROLLER") ? "Controller" : "Device"));
        else {
            AD_SAVE_ERROR_INFO(ad);
            paderror(ad.aderror, "While executing ad_get_mode", ad.aderror_info);
            ad_close(&ad);
            return EXIT_FAILURE;
        }

        pad = ad_get_address(&ad);

        if (pad != ad_get_curr_errcode(&ad))
            printf("Address: %d (PAD)", pad);
        else {
            AD_SAVE_ERROR_INFO(ad);
            paderror(ad.aderror, "While executing ad_get_address", ad.aderror_info);
            ad_close(&ad);
            return EXIT_FAILURE;
        }

        sad = ad_get_next_answer(&ad);

        if (sad != ad_get_curr_errcode(&ad))
            printf(" %d (SAD)\n", sad);
        else
            printf("\n");

        eoi = ad_get_eoi(&ad);

        if (eoi != ad_get_curr_errcode(&ad))
            printf("EOI: %s\n", (eoi == ad_get_const(&ad, "ENABLED") ? "Enabled" : "Disabled"));
        else {
            AD_SAVE_ERROR_INFO(ad);
            paderror(ad.aderror, "While executing ad_get_eoi", ad.aderror_info);
            ad_close(&ad);
            return EXIT_FAILURE;
        }

        eot_e = ad_get_eot_enable(&ad);

        if (eot_e != ad_get_curr_errcode(&ad)) {
            if (eot_e == ad_get_const(&ad, "ENABLED")) {
                char c = ad_get_eot_char(&ad);
                if (c != ad_get_curr_errcode(&ad))
                    printf("EOT: Character %d ('%c')\n", c, c);
                else {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While executing ad_get_eot_char", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
            } else {
                printf("EOT: Disabled\n");
            }
        } else {
            AD_SAVE_ERROR_INFO(ad);
            paderror(ad.aderror, "While executing ad_get_eot_enable", ad.aderror_info);
            ad_close(&ad);
            return EXIT_FAILURE;
        }

        printf("EOS: ");

        eos = ad_get_eos(&ad);

        if (eos == ad_get_const(&ad, "EO_CRLF")) {
            printf("CR+LF");
        } else if (eos == ad_get_const(&ad, "EO_CR")) {
            printf("CR");
        } else if (eos == ad_get_const(&ad, "EO_LF")) {
            printf("LF");
        } else if (eos == ad_get_const(&ad, "EO_NONE")) {
            printf("NONE");
        } else if (eos == ad_get_curr_errcode(&ad)) {
            AD_SAVE_ERROR_INFO(ad);
            paderror(ad.aderror, "While executing ad_get_eos", ad.aderror_info);
            ad_close(&ad);
            return EXIT_FAILURE;
        }

        printf("\n");
    } else {
        int defvals = 0;

        if (p_mode == -1 && p_padd == -1 && p_sadd == -1 && p_auto == -1 && p_ceoi == -1 && p_ceot == -1 && p_ceos == -1 && p_sign == -1) {
            puts("Configuring the Prologix adapter with default values");
            defvals = 1;
        }

        if (defvals || p_mode != -1)
            if (ad_set_mode(&ad, p_mode != -1 ? p_mode : ad_get_const(&ad, PROLOGIX_D_MODE)) == ad_get_curr_errcode(&ad)) {
                AD_SAVE_ERROR_INFO(ad);
                paderror(ad.aderror, "While executing ad_set_mode", ad.aderror_info);
                ad_close(&ad);
                return EXIT_FAILURE;
            }

        if (defvals || p_padd != -1)
            if (PROLOGIX_D_SADD != -1 || p_sadd != -1) {
                if (ad_set_address(&ad, p_padd != -1 ? p_padd : PROLOGIX_D_ADDR, p_sadd != -1 ? p_sadd : PROLOGIX_D_SADD) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While executing ad_set_address", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
            } else {
                if (ad_set_address(&ad, p_padd != -1 ? p_padd : PROLOGIX_D_ADDR) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While executing ad_set_address", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
            }

        if (defvals || p_auto != -1)
            if (ad_get_mode(&ad) == ad_get_const(&ad, "MD_CONTROLLER")) {
                if (ad_set_auto(&ad, p_auto != -1 ? p_auto : ad_get_const(&ad, PROLOGIX_D_AUTO)) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While executing ad_set_auto", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
            } else {
                AD_SAVE_ERROR_INFO(ad);
                fprintf(stderr, "%s\n\tWhile executing ad_set_auto: Wrong adapter mode\n", ad.aderror_info);
                ad_close(&ad);
                return EXIT_FAILURE;
            }

        if (defvals || p_ceoi != -1)
            if (ad_set_eoi(&ad, p_ceoi != -1 ? p_ceoi : ad_get_const(&ad, PROLOGIX_D_CEOI)) == ad_get_curr_errcode(&ad)) {
                AD_SAVE_ERROR_INFO(ad);
                paderror(ad.aderror, "While executing ad_set_eoi", ad.aderror_info);
                ad_close(&ad);
                return EXIT_FAILURE;
            }

        if (defvals || p_ceot != -1) {
            if (p_ceot != -1) {
                if (ad_set_eot_enable(&ad, ad_get_const(&ad, "ENABLED")) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While executing ad_set_eot_enable", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }

                if (ad_set_eot_char(&ad, p_ceot) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While executing ad_set_eot_char", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
            } else {
                if (ad_set_eot_enable(&ad, ad_get_const(&ad, PROLOGIX_D_CEOT)) == ad_get_curr_errcode(&ad)) {
                    AD_SAVE_ERROR_INFO(ad);
                    paderror(ad.aderror, "While executing ad_set_eot_enable", ad.aderror_info);
                    ad_close(&ad);
                    return EXIT_FAILURE;
                }
                if (ad_get_const(&ad, PROLOGIX_D_CEOT) == ad_get_const(&ad, "ENABLED"))
                    if (p_ceot == ad_get_const(&ad, "ENABLED"))
                        if (ad_set_eot_char(&ad, PROLOGIX_D_EOTC) == ad_get_curr_errcode(&ad)) {
                            AD_SAVE_ERROR_INFO(ad);
                            paderror(ad.aderror, "While executing ad_set_eot_char", ad.aderror_info);
                            ad_close(&ad);
                            return EXIT_FAILURE;
                        }
            }
        }

        if (defvals || p_ceos != -1)
            if (ad_set_eos(&ad, p_ceos != -1 ? p_ceos : ad_get_const(&ad, PROLOGIX_D_CEOS)) == ad_get_curr_errcode(&ad)) {
                AD_SAVE_ERROR_INFO(ad);
                paderror(ad.aderror, "While executing ad_set_eos", ad.aderror_info);
                ad_close(&ad);
                return EXIT_FAILURE;
            }
    }

    // close the device
    ad_close(&ad);

    // end
    return EXIT_SUCCESS;
}
