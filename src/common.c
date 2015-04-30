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
#include <dirent.h>

#include "adapters.h"
#include "common.h"
#include "colors.h"

int version(char *progname) {
    printf("%s (%s) %s\n", progname, PACKAGE_NAME, VERSION);
    puts("\
Copyright © 2015 Giulio Girardi.\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n");

    return 1;
}

int help() {
    printf("\
Report bugs to: %s\n\
Package home page: <%s>\n", PACKAGE_BUGREPORT, PACKAGE_URL);

    return 1;
}

int adapters_list() {
    DIR *d;
    struct dirent *dir;
    d = opendir(DATA_PATH);

    puts("ADAPTER stands for the adapter you are using. Available adapters are:");

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") && dir->d_type == DT_REG) {
                // Get the extension
                char d[10];
                int i;
                memset(d, 0, 10);
                
                for (i = 0; i < 10; i++) {
                    d[i] = dir->d_name[strlen(dir->d_name) - 9 + i];
                }
                
                // Check the extension
                if (!strcmp(d, ".conf.xml")) {
                    char *c = dir->d_name;
                    c[strlen(c) - 9] = 0;
                    printf("  %s\n", dir->d_name);
                }
            }
        }
        closedir(d);
    } else {
        char *str = strerror(errno);
        fprintf (stderr, KRED "In %s: function %s (near line %d)\n\tWhile opening DATA_PATH: %s\n" KRST, __FILE__, __func__, __LINE__, str);
        return 0;
    }

    puts("\nMore adapters may be added in the future\n");
    printf("Adapters config files are in:\n\
  %s\n\n", DATA_PATH);

    return 1;
}