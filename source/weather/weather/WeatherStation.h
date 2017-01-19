// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================

#ifndef _WEATHERSTATION_H_
#define _WEATHERSTATION_H_

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <SparkFunBME280.h>
#include <ThingSpeak.h>

#define BME_COMM_MODE            I2C_MODE
#define BME_ADDRESS              0x76
#define BME_SPI_CHIP_SELECT_PIN  10
#define LIGHT_INTENSITY_PIN      A0
#define SENSOR_SAMPLE_RATE       (1000)
#define PRESSURE_CONVERSION      0.000295299830714 /* merc: 0.000295299830714 / psi: 0.000145037738*/

#define TS_FIELD_TEMP            1
#define TS_FIELD_HUMIDITY        2
#define TS_FIELD_PRESSURE        3
#define TS_FIELD_BRIGHTNESS      4
#define TS_SEND_RATE             60000

#define BLYNK_VF_TEMP            1
#define BLYNK_VF_HUMIDITY        2
#define BLYNK_VF_PRESSURE        3
#define BLYNK_VF_BRIGHTNESS_ONE  4
#define BLYNK_VF_BRIGHTNESS_TWO  5
#define BLYNK_SEND_RATE          1000

#define WS_DEBUG                 true
#define BLYNK_PRINT              Serial    // Comment this out to disable prints and save space

class WeatherStation
{
  public:
    WeatherStation(String configurationFilePath);
    void begin();
    void applicationLoop();
    void timerEvent();

  private:
    String        configurationFilePath;
    String        wifiPassword;
    String        wifiSSID;
    String        thingSpeakAPIKey;
    long          thingSpeakChannelId;
    unsigned long thingSpeakLastSendTimeInMs;
    String        blynkAPIKey;

    WiFiClient  client;
    BME280      bme280;

    void loadConfigurationFile();
    void connectToWifi();
    void initializeSensors();
    void initializeThingSpeak();
    void initializeBlynk();
    int  getLightIntensity();
    void readSensors();
    void sendSamplesToThingSpeak(float tempF, float humidity, float pressure, int brightness);
    void sendSamplesToBlynk(float tempF, float humidity, float pressure, int brightness);
    // void checkForClientConnections();
    // void updateConfigurationFile();
};


#endif
