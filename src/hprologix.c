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

#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "hpib.h"
#include "prologix_proto.h"
#include "adapters.h"
#include "common.h"

#define PROGRAM_NAME "hprologix"

#define PROLOGIX_D_MODE MD_CONTROLLER
#define PROLOGIX_D_ADDR 20
#define PROLOGIX_D_AUTO 1
#define PROLOGIX_D_CEOI 0
#define PROLOGIX_D_CEOT -1
#define PROLOGIX_D_CEOS EO_CRLF

typedef enum
{
  SIG_IFC = 0,
  SIG_CLR,
  SIG_LOC,
  SIG_SRQ,
  SIG_SPL,
  SIG_RST
} prologix_signal_t;

int
main (int argc, char **argv)
{
  // temp vars
  int c;
  char *endptr, *endptr_s;
  char *pad_temp, *sad_temp;

  // default values
  char tty[STR_MAXLEN] = "/dev/ttyUSB0";
  int get = 0;
  int default_values = 1;

  // set the default adapter
  set_adapter (AD_PROLOGIX);

  // adapter default options
  int p_mode = PROLOGIX_D_MODE;
  int p_padd = PROLOGIX_D_ADDR;
  int p_sadd = PROLOGIX_NONE;
  int p_auto = PROLOGIX_D_AUTO;
  int p_ceoi = PROLOGIX_D_CEOI;
  int p_ceot = PROLOGIX_D_CEOT;
  int p_ceos = PROLOGIX_D_CEOS;
  int p_sign = PROLOGIX_NONE;

  // arguments parsing
  while (1)
    {
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

      c = getopt_long (argc, argv, "d:gm:r:a:e:t:s:n:hv", long_options, &option_index);

      if (c == -1)
        {
          break;
        }

      switch (c)
        {
        case 'd':
          memset (tty, 0, STR_MAXLEN);
          memcpy (tty, optarg, strlen (optarg));
          break;

        case 'g':
          default_values = 0;
          get = 1;
          break;

        case 'm':
          default_values = 0;
          if (!strcmp (optarg, "1") || !strcmp (optarg, "0"))
            p_mode = atoi (optarg);
          else
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, optarg);
              return (EXIT_FAILURE);
            }
          break;

        case 'r':
          default_values = 0;

          pad_temp = strtok (optarg, ",");
          sad_temp = strtok (NULL, ",");
          
          p_padd = strtol (pad_temp, &endptr, 10);

          if (*endptr != '\0')
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, pad_temp);
              return (EXIT_FAILURE);
            }

          if (sad_temp != NULL)
            {
              p_sadd = strtol (sad_temp, &endptr_s, 10);

              if (*endptr_s != '\0')
                {
                  printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, sad_temp);
                  return (EXIT_FAILURE);
                }
            }

          break;

        case 'a':
          default_values = 0;
          if (!strcmp (optarg, "1") || !strcmp (optarg, "0"))
            p_auto = atoi (optarg);
          else
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, optarg);
              return (EXIT_FAILURE);
            }
          break;

        case 'e':
          default_values = 0;
          if (!strcmp (optarg, "1") || !strcmp (optarg, "0"))
            p_ceoi = atoi (optarg);
          else
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, optarg);
              return (EXIT_FAILURE);
            }
          break;

        case 't':
          default_values = 0;
          p_ceot = strtol (optarg, &endptr, 10);
          if (*endptr != '\0')
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, optarg);
              return (EXIT_FAILURE);
            }
          break;

        case 's':
          default_values = 0;
          if (!strcmp (optarg, "3") || !strcmp (optarg, "2") || !strcmp (optarg, "1") || !strcmp (optarg, "0"))
            p_ceos = atoi (optarg);
          else
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, optarg);
              return (EXIT_FAILURE);
            }
          break;

        case 'n':
          printf ("Sending signal %s\n", optarg);

          if (!strcmp (optarg, "IFC"))
            p_sign = SIG_IFC;
          else if (!strcmp (optarg, "CLR"))
            p_sign = SIG_CLR;
          else if (!strcmp (optarg, "LOC"))
            p_sign = SIG_LOC;
          else if (!strcmp (optarg, "SRQ"))
            p_sign = SIG_SRQ;
          else if (!strcmp (optarg, "SPOLL"))
            p_sign = SIG_SPL;
          else if (!strcmp (optarg, "RST"))
            p_sign = SIG_RST;
          else
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, optarg);
              return (EXIT_FAILURE);
            }

          break;

        case 'h':
          puts ("\
Usage: hprologix [OPTIONS]...\n\
Configure the prologix adapter\n");
          puts ("\
  -d, --device=STRING       communicate with serial interface defined by STRING;\n\
                            default: '/dev/ttyUSB0'\n\
  -g, --get-config          get current cofiguration\n\
  -m, --mode=MODE           set adapter mode (0: DEVICE, 1: CONTROLLER); default: 1\n\
  -r, --address=PAD,[SAD]   set the address used by the instrument (PAD is the\n\
                            primary address, SAD the secondary which isn't necessary);\n\
                            default: 20,0\n\
  -a, --autoread=BOOL       set whether the adapter should read the instrument\n\
                            answer (disabled if mode is DEVICE); default: 1;\n\
  -e, --eoi=BOOL            enable or disable EOI signal with last transmitted byte\n\
  -t, --eot=CHAR            enable EOT and set the EOT character (-1: DISABLED);\n\
                            default: -1\n\
  -s, --eos=EOS             set GPIB temination characters (0: CR+LF, 1: CR, 2: LF,\n\
                            3: NONE); default: 0\n\
  -n, --signal=SIG          send to the adapter the signal SIG (disables any other\n\
                            option, except for -d, -h and -v)\n\
  -h, --help                show this help and exit\n\
  -v, --version             show information about program version and exit\n");
          puts ("\
SIG stands for one of the following signals:\n\
  IFC                       send the IFC signal\n\
  CLR                       send the SDC (Selected Device Clear) to the currently\n\
                            specified address\n\
  LOC                       enable front panel operation of the currently\n\
                            addressed instrument\n\
  SRQ                       return the current state of the SRQ signal (0: NOT\n\
                            ASSERTED, 1: ASSERTED\n\
  SPOLL                     perform a serial poll of the instrument at the\n\
                            specified address\n\
  RST                       perform a controller reset (it takes about 5 secs)\n");
          help ();
          return (EXIT_SUCCESS);
          break;

        case 'v':
          version (PROGRAM_NAME);
          return (EXIT_SUCCESS);
          break;

        case '?':
          puts ("\
Usage: hprologix [OPTIONS]... [COMMAND]\n\
Configure the prologix adapter\n");
          return (EXIT_FAILURE);

        default:
          abort ();
        }
    }

  // check whether the serial port is available and configure it
  if (!HPIB_serial_config (tty, 5))
    {
      HPIB_serial_close ();
      return (EXIT_FAILURE);
    }

  if (p_sign != PROLOGIX_NONE)
    {
      int val = 0;
      
      switch (p_sign)
        {
        case SIG_IFC:
          ad_ifc ();
          break;

        case SIG_CLR:
          ad_clr ();
          break;

        case SIG_LOC:
          ad_loc ();
          break;

        case SIG_SRQ:
          printf ("Signal returned: %d\n", ad_srq ());
          break;

        case SIG_SPL:
          val = ad_spoll (PROLOGIX_NONE, PROLOGIX_NONE);
          printf ("Signal returned: ");
          if (val != PROLOGIX_NONE)
            printf ("%d\n", val);
          else 
            printf ("no answer\n");
          break;

        case SIG_RST:
          ad_rst ();
          break;
        }
      return (EXIT_SUCCESS);
    }

  if (default_values)
    puts ("Configuring the Prologix adapter with default values");

  if (get)
    {
      int sad;

      printf ("Mode: %s\n", (ad_get_mode () ? "Controller" : "Device"));
      printf ("Address: %d (PAD)", ad_get_address (&sad));
      if (sad != NULL)
        printf (" %d (SAD)\n", sad);
      else
        printf ("\n");

      printf ("EOI: %s\n", (ad_get_eoi () ? "Enabled" : "Disabled"));
      if (ad_get_eot_enable)
        {
          char c = ad_get_eot_char ();
          printf ("EOT: Character %d ('%c')\n", c, c);
        }
      else
        {
          printf ("EOT: Disabled\n");
        }
      printf ("EOS: ");
      switch (ad_get_eos ())
        {
        case EO_CRLF:
          printf ("CR+LF\n");
          break;
        case EO_CR:
          printf ("CR\n");
          break;
        case EO_LF:
          printf ("LF\n");
          break;
        case EO_NONE:
          printf ("NONE\n");
          break;
        }
    }
  else
    {
      if (!ad_set_mode (p_mode))
        return (EXIT_FAILURE);

      if (!ad_set_address (p_padd, p_sadd))
        return (EXIT_FAILURE);

      if (p_mode != 0)
        if (!ad_set_auto (p_auto))
          return (EXIT_FAILURE);

      if (!ad_set_eoi (p_ceoi))
        return (EXIT_FAILURE);

      if (p_ceot != -1)
        {
          if (!ad_set_eot_enable (TRUE))
            return (EXIT_FAILURE);

          if (!ad_set_eot_char (p_ceot))
            return (EXIT_FAILURE);
        }
      else
        {
          if (!ad_set_eot_enable (FALSE))
            return (EXIT_FAILURE);
        }

      if (!ad_set_eos (p_ceos))
        return (EXIT_FAILURE);
    }

  // close the devicse
  HPIB_serial_close ();

  // end
  return (EXIT_SUCCESS);
}
