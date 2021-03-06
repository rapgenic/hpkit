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

#include "common.h"

int
help_adapters ()
{
  puts ("\
ADAPTER stands for the adapter you are using. Supported adapters are:\n\
  prologix                  the Prologix adapter, fully supported\n\
  galvant                   the Galvant adapter, actually in development\n\
  none                      don't configure the adapter (it will also disable\n\
                            the -r|--address option)\n\
\nMore adapters may be added in the future\n");
}

int
version (char *progname)
{
  printf ("%s (%s) %s\n", progname, PACKAGE_NAME, VERSION);
  puts ("\
Copyright © 2015 Giulio Girardi.\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n");

  return 1;
}

int
help ()
{
  printf ("\
Report bugs to: %s\n\
Package home page: <%s>\n", PACKAGE_BUGREPORT, PACKAGE_URL);

  return 1;
}
