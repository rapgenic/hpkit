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

#include "adapters.h"

int main()
{
    adapter_t ad;

    if (!ad_config(&ad, "../src/adapters/prologix.conf.xml", "/dev/ttyUSB0", 20)) {
        paderror(ad.aderror, NULL, ad.aderror_info);
        ad_close(&ad);
        return EXIT_FAILURE;
    }

    /*if (ad_set_address(&ad, 20, 100) == ad_get_curr_errcode(&ad))
        paderror(ad.aderror, NULL, ad.aderror_info);
    if (ad_get_address(&ad) == ad_get_curr_errcode(&ad))
        paderror(ad.aderror, NULL, ad.aderror_info);

    printf("%d\n", ad_get_address(&ad));

    if (ad_set_auto(&ad, 1) == ad_get_curr_errcode(&ad))
        paderror(ad.aderror, NULL, ad.aderror_info);
    */
    
    ad_set_address(&ad, 20);
    printf("Address: %d\n", ad_get_address(&ad));
    printf("Eventual second address: %d\n", ad_get_next_answer(&ad));
    
    if (ad_get_mode(&ad) == ad_get_const(&ad, "MD_DEVICE"))
        ad_set_mode(&ad, ad_get_const(&ad, "MD_CONTROLLER"));
    else
        ad_set_mode(&ad, ad_get_const(&ad, "MD_DEVICE"));
    
    printf("Mode: %s\n", ad_get_mode(&ad) ? "Controller" : "Device");
    
    ad_close(&ad);
}