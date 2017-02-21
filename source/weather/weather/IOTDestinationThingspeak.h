// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _IOT_DESTINATION_THINGSPEAK_H_
#define _IOT_DESTINATION_THINGSPEAK_H_

#include <ThingSpeak.h>
#include "IOTDestinationI.h"
#include "WeatherDebug.h"

#define TS_FIELD_TEMP            1
#define TS_FIELD_HUMIDITY        2
#define TS_FIELD_PRESSURE        3
#define TS_FIELD_BRIGHTNESS      4
#define TS_SEND_RATE             60000

class IOTDestinationThingspeak : public IOTDestinationI {

private:
  WiFiClient    client;
  bool          enabled;
  String        apiKey;
  long          channelId;
  unsigned long lastSendTimeInMs;
  WeatherDebug *debugger;

  void setAPIKey(String apiKey);
  void setChannelID(String id);

public:
  IOTDestinationThingspeak();
  void init(WeatherConfig &config);
  bool send(WeatherData &weatherData);
  void applicationLoop() {}
};

#endif
