// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _IOT_DESTINATION_H_
#define _IOT_DESTINATION_H_

#include "WeatherStationCommon.h"
#include "WeatherConfig.h"

class IOTDestination {
  public:
    virtual void init(WeatherConfig &config) = 0;
    virtual bool send(float tempF, float humidity, float pressure, int brightness) = 0;
};

#endif
