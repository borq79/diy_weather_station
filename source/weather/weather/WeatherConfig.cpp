// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "WeatherConfig.h"

void WeatherConfig::loadConfigurationFile() {
  this->debugger = WeatherDebug::getWeatherDebugger();

  String configurationFilePath = CONFIG_FILE_PATH;

  this->debugger->logln(DEBUG_LEVEL_INFO, "Loading Configuration File " + configurationFilePath + " ...");

  SPIFFS.begin();
  File configurationFile = SPIFFS.open(configurationFilePath, "r");
  if (!configurationFile) {
    this->debugger->logln(DEBUG_LEVEL_ERROR, String("Failed to open file " + configurationFilePath));
  } else {
    while (configurationFile.available()) {
      String configLine = configurationFile.readStringUntil('\n');
      configLine.trim();

      // Skip comments
      if (configLine.startsWith("#")) {
        continue;
      }

      this->debugger->logln(DEBUG_LEVEL_TRACE, "Parsing Configuration Line [" + configLine + "]");

      int splitPoint = configLine.indexOf(":");
      String key = configLine.substring(0, splitPoint); key.trim();
      String val = configLine.substring(splitPoint + 1); val.trim();

      this->debugger->logln(DEBUG_LEVEL_TRACE, String("Key = " + key));
      this->debugger->logln(DEBUG_LEVEL_TRACE, String("Val = " + val));

      if (key.equals("wsname")) { this->setOwnerName(val); }
      else if (key.equals("ssid")) { this->setSSID(val); }
      else if (key.equals("wifipassword")) { this->setWifiPassword(val); }
      else if (key.equals("apname")) { this->setAPName(val); }
      else if (key.equals("appassword")) { this->setAPPassword(val); }
      else if (key.equals("thingspeakapikey")) { this->setThingSpeakAPIKey(val); }
      else if (key.equals("thingspeakchannelid")) { this->setThingSpeakChannelID(val.toInt()); }
      else if (key.equals("blynkapikey")) { this->setBlynkAPIKey(val); }
      else if (key.equals("debuglevel")) { this->debugger->setDebugLevel(val); }

   }

   // close the file
   configurationFile.close();
   this->debugger->logln(DEBUG_LEVEL_INFO, "Done parsing Configuration File");
  }

  SPIFFS.end();
}
