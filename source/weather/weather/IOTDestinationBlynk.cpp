// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include <BlynkSimpleEsp8266.h>
#include "IOTDestinationBlynk.h"

IOTDestinationBlynk::IOTDestinationBlynk() {
  this->enabled = false;
}

boolean IOTDestinationBlynk::send(float tempF, float humidity, float pressure, int brightness) {
  if (this->enabled) {
    Blynk.virtualWrite(BLYNK_VF_TEMP, tempF);
    Blynk.virtualWrite(BLYNK_VF_HUMIDITY, humidity);
    Blynk.virtualWrite(BLYNK_VF_PRESSURE, pressure);
    Blynk.virtualWrite(BLYNK_VF_BRIGHTNESS_ONE, brightness);
    Blynk.virtualWrite(BLYNK_VF_BRIGHTNESS_TWO, brightness);
  }

  return true;
}

void IOTDestinationBlynk::setAPIKey(String apiKey) {
  Blynk.config(apiKey.c_str());
  this->enabled = true;
}

void IOTDestinationBlynk::applicationLoop() {
  Blynk.run();
}
