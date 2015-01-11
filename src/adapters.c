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

#include "adapters.h"

int
set_adapter(adapters_t ad) 
{
  switch (ad) 
    {
    case AD_PROLOGIX:
      ad_set_address = &prologix_set_address;
      ad_get_address = &prologix_get_address;
      ad_set_mode = &prologix_set_mode;
      ad_get_mode = &prologix_get_mode;
      ad_set_lon = &prologix_set_lon;
      ad_get_lon = &prologix_get_lon;
      ad_set_auto = &prologix_set_auto;
      ad_get_auto = &prologix_get_auto;
      break;
    default:
      return 0;
    }
}