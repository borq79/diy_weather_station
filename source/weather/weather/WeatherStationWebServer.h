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
#include <ArduinoJson.h>

#define WS_LISTEN_PORT 80
#define MAX_SESSION_LENGTH 120000

enum HTTPSTATE {
  READ_REQUEST_LINE,
  READ_HEADERS,
  READ_BODY
};

class WeatherStationWebServer {

private:
    WiFiServer            server;
    WeatherDebug          *debugger;
    WeatherStationI       *weatherStation;
    bool                  isAPModeEnabled;

    bool updateConfiguration(String &jsonConfig);
    void sendErrorResponse(WiFiClient &client, int status, String statusPhrase);
    void sendAPIAPStatus(WiFiClient &client);
    void sendAPIJsonResponse(WiFiClient &client, String &response);
    void sendAPIWeatherData(WiFiClient &client);
    void sendAPIConfiguration(WiFiClient &client);
    void sendFile(WiFiClient &client, String requestPath);

public:
    WeatherStationWebServer();
    void init(bool isAPModeEnabled, WeatherStationI *weatherStation);
    void applicationLoop();
};

#endif
