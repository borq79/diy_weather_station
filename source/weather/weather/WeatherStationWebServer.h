// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _WEATHER_STATION_WEB_SERVER_H_
#define _WEATHER_STATION_WEB_SERVER_H_

#include "WeatherStationCommon.h"
#include "IOTDestinationI.h"
#include "WeatherDebug.h"
#include "WeatherConfig.h"
#include "WeatherStationI.h"

#define WS_LISTEN_PORT 80

class WeatherStationWebServer {

private:
    WiFiServer      server;
    WeatherDebug    *debugger;
    WeatherStationI *weatherStation;
    bool            isAPModeEnabled;
    
    void updateConfiguration(String jsonConfig);

public:
    WeatherStationWebServer();
    void init(bool isAPModeEnabled, WeatherStationI *weatherStation);
    void applicationLoop();
};

#endif
