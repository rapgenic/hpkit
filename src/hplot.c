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
#include "proto.h"
#include "prologix_proto.h"
#include "adapters.h"
#include "common.h"

#define PROGRAM_NAME "hplot"

int
check_adapter_options ()
{
  if (ad_get_mode () == MD_CONTROLLER)
    ad_set_mode (MD_DEVICE);

  if (ad_get_lon ())
    ad_set_lon (FALSE);
}

int
main (int argc, char **argv)
{
  // temp vars
  char buf[BUF_MAXLEN];
  int c, d;

  // default values
  char tty[STR_MAXLEN] = "/dev/ttyUSB0";
  FILE *f_output = NULL;
  int using_output_file = 0;
  char output_path[STR_MAXLEN] = "";
  int timeout = 20;
  int ad_address = 5;

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
          else
            {
              printf ("%s: non valid option -- \"%s\"\nChoosing 'prologix'\n", PROGRAM_NAME, optarg);
              set_adapter (AD_PROLOGIX);
            }
          break;

        case 'r':
          ad_address = atoi (optarg);
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
Usage: hplot [OPTIONS]...\n\
Dump or save plot data sent by HP instrument's GPIB interface\n");
          puts ("\
  -d, --device=STRING       communicate with serial interface defined by STRING;\n\
                            default: '/dev/ttyUSB0'\n\
  -a, --adapter=ADAPTER     set the adapter used to communicate with the\n\
                            instrument; default: 'prologix'\n\
  -r, --address=ADDRESS     choose the address used to listen to the\n\
                            instrument; default: 5\n\
  -o, --output=OUTFILE      send data to FILE instead of stdout\n\
  -t, --timeout=TIMEOUT     set TIMEOUT in millisecs for serial port; default: 2000\n\
                            min: 100\n\
  -h, --help                show this help and exit\n\
  -v, --version             show information about program version and exit\n");
          puts ("\
ADAPTER stands for the adapter you are using. Supported adapters are:\n\
  prologix                  the Prologix adapter, fully supported\n\
\nMore adapters may be added in the future\n");
          help ();
          return (EXIT_SUCCESS);
          break;

        case 'v':
          version (PROGRAM_NAME);
          return (EXIT_SUCCESS);
          break;

        case '?':
          puts ("\n\
Usage: hplot [OPTIONS]...\n\
Dump or save plot data sent by HP instrument's GPIB interface");
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
  check_adapter_options ();
  ad_set_address (ad_address);

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
      if (!HPIB_serial_read_until (buf, '\n'))
        break;


      // print the output
      if (using_output_file)
        fprintf (f_output, "%s", buf);
      else
        printf ("%s", buf);

      // parse the buffer read and answer to the instrument
      HPIB_proto_parse (buf);
    }

  // close the devicse
  HPIB_serial_close ();

  if (using_output_file)
    fclose (f_output);

  // end
  return (EXIT_SUCCESS);
}
