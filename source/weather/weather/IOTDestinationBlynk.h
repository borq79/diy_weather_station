// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _IOT_DESTINATION_BLYNK_H_
#define _IOT_DESTINATION_BLYNK_H_

#include "IOTDestination.h"

#define BLYNK_VF_TEMP            1
#define BLYNK_VF_HUMIDITY        2
#define BLYNK_VF_PRESSURE        3
#define BLYNK_VF_BRIGHTNESS_ONE  4
#define BLYNK_VF_BRIGHTNESS_TWO  5
#define BLYNK_SEND_RATE          1000

#define BLYNK_PRINT              Serial    // Comment this out to disable prints and save space

class IOTDestinationBlynk : public IOTDestination {

private:
  boolean enabled;

public:
  IOTDestinationBlynk();
  boolean send(float tempF, float humidity, float pressure, int brightness);
  void applicationLoop();
  void setAPIKey(String apiKey);
};

#endif
