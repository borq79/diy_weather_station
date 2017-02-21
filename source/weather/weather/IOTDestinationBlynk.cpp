// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include <BlynkSimpleEsp8266.h>
#include "IOTDestinationBlynk.h"

IOTDestinationBlynk::IOTDestinationBlynk() {
}

void IOTDestinationBlynk::init(WeatherConfig &config) {
  this->enabled = false;
  this->debugger = WeatherDebug::getWeatherDebugger();
  this->setAPIKey(config.getBlynkAPIKey());
}

bool IOTDestinationBlynk::send(WeatherData &weatherData) {
  if (this->enabled) {
    Blynk.virtualWrite(BLYNK_VF_TEMP, weatherData.tempF);
    Blynk.virtualWrite(BLYNK_VF_HUMIDITY, weatherData.humidity);
    Blynk.virtualWrite(BLYNK_VF_PRESSURE, weatherData.pressure);
    Blynk.virtualWrite(BLYNK_VF_BRIGHTNESS_ONE, weatherData.brightness);
    Blynk.virtualWrite(BLYNK_VF_BRIGHTNESS_TWO, weatherData.brightness);
  }

  return true;
}

void IOTDestinationBlynk::setAPIKey(String apiKey) {
  if (apiKey.length() > 0) {
    Blynk.config(apiKey.c_str());
    Blynk.connect();
    this->enabled = true;
    this->debugger->logln(DEBUG_LEVEL_TRACE, "Blynk Enabled: " + String(apiKey.c_str()));
  } else {
    this->debugger->logln(DEBUG_LEVEL_INFO, "Blynk is DISABLED");
  }
}

void IOTDestinationBlynk::applicationLoop() {
  if (this->enabled) {
    if (Blynk.connected() == false) {
      this->debugger->logln(DEBUG_LEVEL_ERROR, "Blynk is enabled, but disconnected - trying to reconnect ...");
      Blynk.connect();
    } else {
      Blynk.run();
    }
  }
}
