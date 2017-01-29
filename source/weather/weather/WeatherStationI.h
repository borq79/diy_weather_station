// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _WEATHER_STATION_I_H_
#define _WEATHER_STATION_I_H_

#include "WeatherStationCommon.h"
#include "WeatherConfig.h"

class WeatherStationI {
  public:
    virtual const WeatherData& getWeatherData() = 0;
    virtual WeatherConfig& getWeatherConfig() = 0;
};

#endif
