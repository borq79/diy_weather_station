// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _IOT_DESTINATION_OLED_DISPLAY_H_
#define _IOT_DESTINATION_OLED_DISPLAY_H_

#include <ThingSpeak.h>
#include "IOTDestinationI.h"
#include "WeatherDebug.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

class IOTDestinationOLEDDisplay : public IOTDestinationI {

private:
  String        ownerName;
  bool          enabled;
  WeatherDebug *debugger;
  Adafruit_SSD1306 *display;

  void initDisplay();


public:
  IOTDestinationOLEDDisplay();
  void init(WeatherConfig &config);
  bool send(WeatherData &weatherData);
  void applicationLoop() {}
};

#endif
