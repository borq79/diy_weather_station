// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include <BlynkSimpleEsp8266.h>
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
  this->connectToWifi();
  this->initializeSensors();
  this->initializeThingSpeak();
  this->initializeBlynk();
}

void WeatherStation::applicationLoop() {
  Blynk.run();
}

WeatherStation::WeatherStation(String configurationFilePath) {
  this->configurationFilePath = configurationFilePath;
}

void WeatherStation::connectToWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    // if (WiFi.setAutoConnect(false))
    if (this->wifiSSID.equals(WiFi.SSID())) {
      Serial.println("The WiFi is already connected to " + this->wifiSSID);
    } else {
      Serial.println("Disconnecting current WiFi connection ...");
      WiFi.disconnect(false);
      while (WiFi.status() == WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print(String("Connecting to " + this->wifiSSID + " "));


    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.begin(this->wifiSSID.c_str(), this->wifiPassword.c_str());
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println();
  Serial.println(String("WiFi connected. IP: " + WiFi.localIP()));
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

void WeatherStation::initializeThingSpeak() {
  ThingSpeak.begin(this->client);
}

void WeatherStation::initializeBlynk() {
 Blynk.config(this->blynkAPIKey.c_str());
}

void WeatherStation::loadConfigurationFile() {
  Serial.println(String("Loading Configuration File " + this->configurationFilePath + " ..."));

  SPIFFS.begin();
  File configurationFile = SPIFFS.open(this->configurationFilePath, "r");
  if (!configurationFile) {
    Serial.println(String("Failed to open file " + this->configurationFilePath));
  } else {
    while (configurationFile.available()) {
      String configLine = configurationFile.readStringUntil('\n');
      //Serial.println(String("Parsing Configuration Line [" + configLine + "]"));

      int splitPoint = configLine.indexOf(":");
      String key = configLine.substring(0, splitPoint); key.trim();
      String val = configLine.substring(splitPoint + 1); val.trim();

      //  Serial.println(String("Key = " + key));
      //  Serial.println(String("Val = " + val));

      // Skip comments
      if (key.startsWith("#")) {
        continue;
      }

      if (key.equals("ssid")) { this->wifiSSID = val; }
      else if (key.equals("wifipassword")) { this->wifiPassword = val; }
      else if (key.equals("thingspeakapikey")) { this->thingSpeakAPIKey = val; }
      else if (key.equals("thingspeakchannelid")) { this->thingSpeakChannelId = val.toInt(); }
      else if (key.equals("blynkapikey")) { this->blynkAPIKey = val; }
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

  sendSamplesToThingSpeak(tempF, humidity, pressure, brightness);
  sendSamplesToBlynk(tempF, humidity, pressure, brightness);
}

void WeatherStation::sendSamplesToBlynk(float tempF, float humidity, float pressure, int brightness) {
  if (this->blynkAPIKey.length() > 0) {
    Blynk.virtualWrite(BLYNK_VF_TEMP, tempF);
    Blynk.virtualWrite(BLYNK_VF_HUMIDITY, humidity);
    Blynk.virtualWrite(BLYNK_VF_PRESSURE, pressure);
    Blynk.virtualWrite(BLYNK_VF_BRIGHTNESS_ONE, brightness);
    Blynk.virtualWrite(BLYNK_VF_BRIGHTNESS_TWO, brightness);
  }
}

void WeatherStation::sendSamplesToThingSpeak(float tempF, float humidity, float pressure, int brightness) {
  unsigned long timeSinceLastSample = millis() - this->thingSpeakLastSendTimeInMs;
  if (this->thingSpeakAPIKey.length() > 0 && timeSinceLastSample >= TS_SEND_RATE) {
    ThingSpeak.setField(TS_FIELD_TEMP, tempF);
    ThingSpeak.setField(TS_FIELD_HUMIDITY, humidity);
    ThingSpeak.setField(TS_FIELD_PRESSURE, pressure);
    ThingSpeak.setField(TS_FIELD_BRIGHTNESS, brightness);
    Serial.println("Sending samples to ThingSpeak ...");
    //  ThingSpeak.writeFields(this->thingSpeakChannelId, this->thingSpeakAPIKey.c_str());
    this->thingSpeakLastSendTimeInMs = millis();
  }


}
