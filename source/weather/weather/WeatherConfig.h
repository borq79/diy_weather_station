// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _WEATHER_CONFIG_H_
#define _WEATHER_CONFIG_H_

#include "WeatherStationCommon.h"
#include "WeatherDebug.h"

#define CONFIG_FILE_PATH "/weather.ini"

class WeatherConfig {
  public:
    WeatherConfig() {}

    void loadConfigurationFile();

    String getAPName() { return this->apName; }
    String getAPPassword() { return this->apPassword; }
    String getSSID() { return this->ssid; }
    String getWifiPassword() { return this->wifiPassword; }
    String getThingSpeakAPIKey() { return this->thingSpeakAPIKey; }
    unsigned long getThingSpeakChannelID() { return this->thingSpeakChannelID; }
    String getBlynkAPIKey() { return this->blynkAPIKey; }
    DEBUG_LEVEL getDebugLevel() { return this->debugLevel; }
    String getDebugLevelName() { return WeatherDebug::getDebugLevelName(this->debugLevel); }

    void setAPName(String apName){ this->apName = apName; }
    void setAPPassword(String apPassword){ this->apPassword = apPassword; }
    void setSSID(String ssid){ this->ssid = ssid; }
    void setWifiPassword(String wifiPassword){ this->wifiPassword = wifiPassword; }
    void setThingSpeakAPIKey(String thingSpeakAPIKey){ this->thingSpeakAPIKey = thingSpeakAPIKey; }
    void setThingSpeakChannelID(unsigned long thingSpeakChannelID){ this->thingSpeakChannelID = thingSpeakChannelID; }
    void setBlynkAPIKey(String blynkAPIKey){ this->blynkAPIKey = blynkAPIKey; }
    void setDebugLevel(DEBUG_LEVEL debugLevel){ this->debugLevel = debugLevel; }

  private:
    String apName;
    String apPassword;
    String ssid;
    String wifiPassword;
    String thingSpeakAPIKey;
    unsigned long thingSpeakChannelID;
    String blynkAPIKey;
    DEBUG_LEVEL debugLevel;
    WeatherDebug *debugger;
};


#endif
