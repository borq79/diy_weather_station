// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "WeatherStation.h"

WeatherStation::WeatherStation() {
  this->IOTDestinations[IOT_DEST_THING_SPEAK] = new IOTDestinationThingspeak();
  this->IOTDestinations[IOT_DEST_BLYNK]       = new IOTDestinationBlynk();
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
  apEnabled = this->wsWifi.connect(config, apEnabled);

  if (apEnabled == false) {
    this->initializeSensors();

    for(int i = 0; i < NUM_OF_IOT_DESTINATIONS; i++) {
      if (this->IOTDestinations[i] != NULL) {
        this->IOTDestinations[i]->init(config);
      }
    }
  }

  this->webServer.init(this->wsWifi.isAPModeEnabled(), this);
}

void WeatherStation::applicationLoop() {
  if (this->wsWifi.isAPModeEnabled() == false) {
    for(int i = 0; i < NUM_OF_IOT_DESTINATIONS; i++) {
      if (this->IOTDestinations[i] != NULL) {
        this->IOTDestinations[i]->applicationLoop();
      }
    }
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
  dataSample.lastRead   = millis();
  dataSample.tempF      = bme280.readTempF();
  dataSample.humidity   = bme280.readFloatHumidity();
  dataSample.pressure   = bme280.readFloatPressure() * PRESSURE_CONVERSION;
  dataSample.brightness = 1024 - this->getLightIntensity();

  this->debugger->log(DEBUG_LEVEL_INFO, "Sensor Values\nT: "); this->debugger->log(DEBUG_LEVEL_INFO, String(dataSample.tempF, 2));
  this->debugger->log(DEBUG_LEVEL_INFO, "\tH: "); this->debugger->log(DEBUG_LEVEL_INFO, String(dataSample.humidity, 2));
  this->debugger->log(DEBUG_LEVEL_INFO, "\tP: "); this->debugger->log(DEBUG_LEVEL_INFO, String(dataSample.pressure, 2));
  this->debugger->log(DEBUG_LEVEL_INFO, "\tB: "); this->debugger->log(DEBUG_LEVEL_INFO, String(dataSample.brightness));
  this->debugger->log(DEBUG_LEVEL_INFO, "\n");

  for(int i = 0; i < NUM_OF_IOT_DESTINATIONS; i++) {
    if (this->IOTDestinations[i] != NULL) {
      this->IOTDestinations[i]->send(dataSample);
    }
  }
}

const WeatherData& WeatherStation::getWeatherData() {
  return this->dataSample;
}

WeatherConfig& WeatherStation::getWeatherConfig() {
  return this->config;
}
