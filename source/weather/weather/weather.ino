// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <SimpleTimer.h>
#include "WeatherStation.h"

#define COMM_BAUD_RATE 115200
#define CONFIG_FILE_PATH "/weather.ini"

WeatherStation weatherStation(CONFIG_FILE_PATH);
SimpleTimer timer;

void timerEvent() {
  weatherStation.timerEvent();
}

void setup() {
  Serial.begin(COMM_BAUD_RATE);
  delay(2000); // Allow for the USB to connect

  weatherStation.begin();

  timer.setInterval(SENSOR_SAMPLE_RATE, timerEvent);
}


// Main Application Loop
void loop() {
  weatherStation.applicationLoop();
  timer.run();
}
