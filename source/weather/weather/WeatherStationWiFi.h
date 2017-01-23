// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _WEATHER_STATION_WIFI_H_
#define _WEATHER_STATION_WIFI_H_

#include "WeatherStationCommon.h"

class WeatherStationWiFi {

private:
  String wifiPassword;
  String wifiSSID;

public:
  WeatherStationWiFi();
  void connect();
  String getWifiStatus(wl_status_t status);
  void setSSID(String ssid);
  void setPassword(String password);
};

#endif
