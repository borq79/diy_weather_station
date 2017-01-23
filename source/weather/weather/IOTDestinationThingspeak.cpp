// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "IOTDestinationThingspeak.h"

IOTDestinationThingspeak::IOTDestinationThingspeak() {
  ThingSpeak.begin(client);
}

boolean IOTDestinationThingspeak::send(float tempF, float humidity, float pressure, int brightness) {
  unsigned long timeSinceLastSample = millis() - this->lastSendTimeInMs;
  if (this->enabled > 0 && timeSinceLastSample >= TS_SEND_RATE) {
    ThingSpeak.setField(TS_FIELD_TEMP, tempF);
    ThingSpeak.setField(TS_FIELD_HUMIDITY, humidity);
    ThingSpeak.setField(TS_FIELD_PRESSURE, pressure);
    ThingSpeak.setField(TS_FIELD_BRIGHTNESS, brightness);
    if (WS_DEBUG){ Serial.println("Sending samples to ThingSpeak ..."); }
    //  ThingSpeak.writeFields(this->thingSpeakChannelId, this->thingSpeakAPIKey.c_str());
    this->lastSendTimeInMs = millis();
  }

  return true;
}

void IOTDestinationThingspeak::setAPIKey(String apiKey) {
  this->apiKey = apiKey;
  this->enabled = true;
}

void IOTDestinationThingspeak::setChannelID(long id) {
  this->channelId = id;
  this->enabled = true;
}
