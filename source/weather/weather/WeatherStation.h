// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _WEATHERSTATION_H_
#define _WEATHERSTATION_H_

#include "WeatherStationCommon.h"
#include "IOTDestinationBlynk.h"
#include "IOTDestinationThingspeak.h"
#include "WeatherStationWiFi.h"
#include "WeatherStationWebServer.h"
#include "WeatherDebug.h"
#include "WeatherStationI.h"

#define BME_COMM_MODE            I2C_MODE
#define BME_ADDRESS              0x76
#define BME_SPI_CHIP_SELECT_PIN  10
#define LIGHT_INTENSITY_PIN      A0
#define SENSOR_SAMPLE_RATE       (1000)
#define PRESSURE_CONVERSION      0.000295299830714 /* merc: 0.000295299830714 / psi: 0.000145037738*/

enum IOT_DESTINATIONS {
  IOT_DEST_THING_SPEAK = 0,
  IOT_DEST_BLYNK       = 1,

  NUM_OF_IOT_DESTINATIONS = 2
};

class WeatherStation : public WeatherStationI {

  public:
    WeatherStation();
    void begin();
    void applicationLoop();
    void timerEvent();
    const WeatherData& getWeatherData();
    WeatherConfig& getWeatherConfig();

  private:
    WeatherConfig            config;
    WeatherData              dataSample;
    BME280                   bme280;
    WeatherStationWiFi       wsWifi;
    IOTDestinationI          *IOTDestinations[NUM_OF_IOT_DESTINATIONS];
    WeatherStationWebServer  webServer;
    WeatherDebug             *debugger;

    void loadConfigurationFile();
    void initializeSensors();
    int  getLightIntensity();
    void readSensors();
};


#endif
