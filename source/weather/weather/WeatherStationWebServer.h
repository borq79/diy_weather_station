// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _WEATHER_STATION_WEB_SERVER_H_
#define _WEATHER_STATION_WEB_SERVER_H_

#include "WeatherStationCommon.h"

#define WS_LISTEN_PORT 80

class WeatherStationWebServer {

private:
    WiFiServer server;

public:
    WeatherStationWebServer();
    void start();
    void applicationLoop();
};

#endif
