// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _IOT_DESTINATION_I_H_
#define _IOT_DESTINATION_I_H_

#include "WeatherStationCommon.h"
#include "WeatherConfig.h"

struct WeatherData {
  unsigned long lastRead;
  float tempF;
  float humidity;
  float pressure;
  int brightness;
};

class IOTDestinationI {
  public:
    virtual void init(WeatherConfig &config) = 0;
    virtual bool send(WeatherData &weatherData) = 0;
    virtual void applicationLoop() = 0;
};

#endif
