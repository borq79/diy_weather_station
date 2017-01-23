// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "WeatherStation.h"

void WeatherStation::timerEvent() {
  this->readSensors();
}

void WeatherStation::begin() {
  Serial.println("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~");
  Serial.println("                                Weather Station ");
  Serial.println("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~");
  Serial.println("Starting ...");

  this->loadConfigurationFile();
  this->initializeSensors();
  this->wsWifi.connect();
  this->webServer.start();
}

void WeatherStation::applicationLoop() {
  blynk.applicationLoop();
  webServer.applicationLoop();
}

WeatherStation::WeatherStation() {
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

void WeatherStation::loadConfigurationFile() {
  String configurationFilePath = CONFIG_FILE_PATH;

  if (WS_DEBUG) { Serial.println("Loading Configuration File " + configurationFilePath + " ..."); }

  SPIFFS.begin();
  File configurationFile = SPIFFS.open(configurationFilePath, "r");
  if (!configurationFile) {
    Serial.println(String("Failed to open file " + configurationFilePath));
  } else {
    while (configurationFile.available()) {
      String configLine = configurationFile.readStringUntil('\n');
      configLine.trim();

      // Skip comments
      if (configLine.startsWith("#")) {
        continue;
      }

      if (WS_DEBUG) { Serial.println(String("Parsing Configuration Line [" + configLine + "]")); }

      int splitPoint = configLine.indexOf(":");
      String key = configLine.substring(0, splitPoint); key.trim();
      String val = configLine.substring(splitPoint + 1); val.trim();

      if (WS_DEBUG) { Serial.println(String("Key = " + key)); }
      if (WS_DEBUG) { Serial.println(String("Val = " + val)); }

      if (key.equals("ssid")) { this->wsWifi.setSSID(val); }
      else if (key.equals("wifipassword")) { this->wsWifi.setPassword(val); }
      else if (key.equals("thingspeakapikey")) { this->thingSpeak.setAPIKey(val); }
      else if (key.equals("thingspeakchannelid")) { this->thingSpeak.setChannelID(val.toInt()); }
      else if (key.equals("blynkapikey")) { this->blynk.setAPIKey(val); }
   }

   // close the file
   configurationFile.close();
  }

  SPIFFS.end();
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

  if (WS_DEBUG) {
    Serial.print("Sensor Values\nT: "); Serial.print(tempF, 2);
    Serial.print("\nH: "); Serial.print(humidity, 2);
    Serial.print("\nP: "); Serial.print(pressure, 2);
    Serial.print("\nB: "); Serial.print(brightness);
    Serial.println("\n");
  }

  thingSpeak.send(tempF, humidity, pressure, brightness);
  blynk.send(tempF, humidity, pressure, brightness);
}
