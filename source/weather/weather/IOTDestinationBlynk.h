// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _IOT_DESTINATION_BLYNK_H_
#define _IOT_DESTINATION_BLYNK_H_

#include "IOTDestinationI.h"
#include "WeatherDebug.h"

#define BLYNK_VF_TEMP            1
#define BLYNK_VF_HUMIDITY        2
#define BLYNK_VF_PRESSURE        3
#define BLYNK_VF_BRIGHTNESS_ONE  4
#define BLYNK_VF_BRIGHTNESS_TWO  5
#define BLYNK_SEND_RATE          1000

#define BLYNK_PRINT              Serial    // Comment this out to disable prints and save space

class IOTDestinationBlynk : public IOTDestinationI {

private:
  bool enabled;
  bool debugEnabled;
  WeatherDebug *debugger;

  void setAPIKey(String apiKey);

public:
  IOTDestinationBlynk();
  void init(WeatherConfig &config);
  bool send(WeatherData &weatherData);
  void applicationLoop();
};

#endif
