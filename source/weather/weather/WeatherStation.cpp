// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "WeatherStation.h"

WeatherStation::WeatherStation() {
}

void WeatherStation::timerEvent() {
  if (this->wsWifi.isAPModeEnabled() == false) {
    this->readSensors();
  }
}

void WeatherStation::begin() {
  this->debugger = WeatherDebug::getWeatherDebugger();

  this->debugger->logln(DEBUG_LEVEL_INFO, "=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~");
  this->debugger->logln(DEBUG_LEVEL_INFO, "                                Weather Station ");
  this->debugger->logln(DEBUG_LEVEL_INFO, "=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~");
  this->debugger->logln(DEBUG_LEVEL_INFO, "Starting ...");

  pinMode(AP_MODE_TOGGLE_PIN, INPUT_PULLUP);
  bool apEnabled = digitalRead(AP_MODE_TOGGLE_PIN) == LOW;
  if (apEnabled) {
    this->debugger->logln(DEBUG_LEVEL_INFO, "Starting application in AP mode since the toggle button was held down.");
  }

  config.loadConfigurationFile();
  this->initializeSensors();
  this->wsWifi.connect(config, apEnabled);
  this->webServer.start(config);
  this->thingSpeak.init(config);
  this->blynk.init(config);
}

void WeatherStation::applicationLoop() {
  if (this->wsWifi.isAPModeEnabled() == false) {
    blynk.applicationLoop();
  }
  webServer.applicationLoop();
}

void WeatherStation::initializeSensors() {
  // Setup the A0 pin (GPIO) to Input
  pinMode(LIGHT_INTENSITY_PIN, INPUT);

  // Communication Mode Intialization
  bme280.settings.commInterface = BME_COMM_MODE;

  if (BME_COMM_MODE == SPI_MODE) {
	  bme280.settings.chipSelectPin = BME_SPI_CHIP_SELECT_PIN;
  } else {
    bme280.settings.I2CAddress = BME_ADDRESS;
  }

  bme280.settings.runMode         = 3;
  bme280.settings.tStandby        = 0;
  bme280.settings.filter          = 0;
  bme280.settings.tempOverSample  = 1;
  bme280.settings.pressOverSample = 1;
  bme280.settings.humidOverSample = 1;

  // Give the BME280 enough time to start up (takes 2ms)
  delay(10);
  bme280.begin();
}

int WeatherStation::getLightIntensity() {
  int intensity = analogRead(LIGHT_INTENSITY_PIN);
  return intensity;
}

void WeatherStation::readSensors() {
  float tempF      = bme280.readTempF();
  float humidity   = bme280.readFloatHumidity();
  float pressure   = bme280.readFloatPressure() * PRESSURE_CONVERSION;
  int brightness   = 1024 - this->getLightIntensity();

  this->debugger->log(DEBUG_LEVEL_INFO, "Sensor Values\nT: "); this->debugger->log(DEBUG_LEVEL_INFO, String(tempF, 2));
  this->debugger->log(DEBUG_LEVEL_INFO, "\nH: "); this->debugger->log(DEBUG_LEVEL_INFO, String(humidity, 2));
  this->debugger->log(DEBUG_LEVEL_INFO, "\nP: "); this->debugger->log(DEBUG_LEVEL_INFO, String(pressure, 2));
  this->debugger->log(DEBUG_LEVEL_INFO, "\nB: "); this->debugger->log(DEBUG_LEVEL_INFO, String(brightness));
  this->debugger->log(DEBUG_LEVEL_INFO, "\n");

  thingSpeak.send(tempF, humidity, pressure, brightness);
  blynk.send(tempF, humidity, pressure, brightness);
}
