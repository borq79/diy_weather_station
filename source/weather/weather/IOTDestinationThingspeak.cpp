// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "IOTDestinationThingspeak.h"

IOTDestinationThingspeak::IOTDestinationThingspeak() {
}

void IOTDestinationThingspeak::init(WeatherConfig &config) {
  this->debugger = WeatherDebug::getWeatherDebugger();

  this->setAPIKey(config.getThingSpeakAPIKey());
  this->setChannelID(config.getThingSpeakChannelID());

  if (this->enabled) { ThingSpeak.begin(client); }
  else { this->debugger->logln(DEBUG_LEVEL_INFO, "ThingSpeak is DISABLED"); }
}

bool IOTDestinationThingspeak::send(WeatherData &weatherData) {
  unsigned long timeSinceLastSample = millis() - this->lastSendTimeInMs;
  if (this->enabled && timeSinceLastSample >= TS_SEND_RATE) {
    ThingSpeak.setField(TS_FIELD_TEMP, weatherData.tempF);
    ThingSpeak.setField(TS_FIELD_HUMIDITY, weatherData.humidity);
    ThingSpeak.setField(TS_FIELD_PRESSURE, weatherData.pressure);
    ThingSpeak.setField(TS_FIELD_BRIGHTNESS, weatherData.brightness);
    this->debugger->logln(DEBUG_LEVEL_TRACE, "Sending samples to ThingSpeak ...");
    ThingSpeak.writeFields(this->channelId, this->apiKey.c_str());
    this->lastSendTimeInMs = millis();
  }

  return true;
}

void IOTDestinationThingspeak::setAPIKey(String apiKey) {
  if (apiKey.length() > 0) {
    this->apiKey = apiKey;
    this->enabled = true;
  }
}

void IOTDestinationThingspeak::setChannelID(String id) {
  if (id.length() > 0) {
    this->channelId = id.toInt();
    this->enabled = true;
  }
}
