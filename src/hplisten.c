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

#include "hpib.h"
#include "prologix_proto.h"
#include "adapters.h"
#include "common.h"

#define PROGRAM_NAME "hplisten"

int
check_adapter_options ()
{
  if (ad_get_mode () == MD_CONTROLLER)
    ad_set_mode (MD_DEVICE);

  if (!ad_get_lon ())
    ad_set_lon (TRUE);
}

int
main (int argc, char **argv)
{
  // temp vars
  char buf;
  int c, d;
  char *endptr, *endptr_s;
  char *pad_temp, *sad_temp;

  // default values
  char tty[STR_MAXLEN] = "/dev/ttyUSB0";
  FILE *f_output = NULL;
  int using_output_file = 0;
  int config_adapter = 1;
  char output_path[STR_MAXLEN] = "";
  int timeout = 20;
  int ad_address = 5;
  int ad_saddress = PROLOGIX_NONE;

  // set the default adapter
  set_adapter (AD_PROLOGIX);

  // arguments parsing
  while (1)
    {
      static struct option long_options[] = {
        {"device", required_argument, 0, 'd'},
        {"adapter", required_argument, 0, 'a'},
        {"address", required_argument, 0, 'r'},
        {"output", required_argument, 0, 'o'},
        {"timeout", required_argument, 0, 't'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
      };

      int option_index = 0;

      c = getopt_long (argc, argv, "d:a:r:o:t:hv", long_options, &option_index);

      if (c == -1)
        break;

      switch (c)
        {
        case 'd':
          memset (tty, 0, STR_MAXLEN);
          memcpy (tty, optarg, strlen (optarg));
          break;

        case 'a':
          if (strcmp (optarg, "prologix") == 0)
            set_adapter (AD_PROLOGIX);
          else if (strcmp (optarg, "galvant") == 0)
            set_adapter (AD_GALVANT);
          else if (strcmp (optarg, "none") == 0)
            config_adapter = 0;
          else
            {
              printf ("%s: non valid option -- \"%s\"\nChoosing 'prologix'\n", PROGRAM_NAME, optarg);
              // no need to reconfigure the adapter: already did it
            }
          break;

        case 'r':
          pad_temp = strtok (optarg, ",");
          sad_temp = strtok (NULL, ",");

          ad_address = strtol (pad_temp, &endptr, 10);

          if (*endptr != '\0')
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, pad_temp);
              return (EXIT_FAILURE);
            }

          if (sad_temp != NULL)
            {
              ad_saddress = strtol (sad_temp, &endptr_s, 10);

              if (*endptr_s != '\0')
                {
                  printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, sad_temp);
                  return (EXIT_FAILURE);
                }
            }
          break;

        case 'o':
          memcpy (output_path, optarg, strlen (optarg));
          using_output_file = 1;
          break;

        case 't':
          d = atoi (optarg) / 100;

          if (d == 0)
            {
              printf ("%s: non valid option -- \"%s\"\n", PROGRAM_NAME, optarg);
              return (EXIT_FAILURE);
            }

          timeout = d;
          break;

        case 'h':
          puts ("\
Usage: hplisten [OPTIONS]...\n\
Dump or save data sent by HP instrument's GPIB interface\n");
          puts ("\
  -d, --device=STRING       communicate with serial interface defined by STRING;\n\
                            default: '/dev/ttyUSB0'\n\
  -a, --adapter=ADAPTER     set the adapter used to communicate with the\n\
                            instrument; default: 'prologix'\n\
  -r, --address=PAD,[SAD]   choose the address used to listen to the instrument;\n\
                            (PAD is the primary address, SAD the secondary which\n\
                            isn't necessary); default: 5,0\n\
  -o, --output=OUTFILE      send data to FILE instead of stdout\n\
  -t, --timeout=TIMEOUT     set TIMEOUT in millisecs for serial port; default:\n\
                            2000, min: 100\n\
  -h, --help                show this help and exit\n\
  -v, --version             show information about program version and exit\n");
          help_adapters ();
          help ();
          return (EXIT_SUCCESS);
          break;

        case 'v':
          version (PROGRAM_NAME);
          return (EXIT_SUCCESS);
          break;

        case '?':
          puts ("\
Usage: hplisten [OPTIONS]...\n\
Dump or save data sent by HP instrument's GPIB interface\n");
          return (EXIT_FAILURE);

        default:
          abort ();
        }
    }

  // check whether the serial port is available and configure it
  if (!HPIB_serial_config (tty, timeout))
    {
      HPIB_serial_close ();
      return (EXIT_FAILURE);
    }

  // check and config adapter's settings
  if (config_adapter)
    {
      check_adapter_options ();
      ad_set_address (ad_address, ad_saddress);
    }

  // open the output file
  if (using_output_file)
    {
      f_output = fopen (output_path, "w");

      if (f_output == NULL)
        {
          perror (output_path);
          HPIB_serial_close ();
          return (EXIT_FAILURE);
        }
    }

  // main loop
  while (1)
    {
      // read the serial
      if (!HPIB_serial_read_char (&buf))
        break;

      // print the output
      if (using_output_file)
        fprintf (f_output, "%c", buf);
      else
        printf ("%c", buf);
    }

  // close the devices
  HPIB_serial_close ();

  if (using_output_file)
    fclose (f_output);

  // end
  return (EXIT_SUCCESS);
}
