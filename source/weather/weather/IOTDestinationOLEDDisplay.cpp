// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "IOTDestinationOLEDDisplay.h"

IOTDestinationOLEDDisplay::IOTDestinationOLEDDisplay() {
}

void IOTDestinationOLEDDisplay::init(WeatherConfig &config) {
  this->debugger = WeatherDebug::getWeatherDebugger();
  this->ownerName = config.getOwnerName();

  this->enabled = true; // TODO Detect

  if (this->enabled) {
    this->initDisplay();
  } else {
    this->debugger->logln(DEBUG_LEVEL_INFO, "OLED Display is DISABLED");
  }
}

bool IOTDestinationOLEDDisplay::send(WeatherData &weatherData) {
  if (this->enabled) {
    this->display->clearDisplay();

    this->display->setTextSize(1);
    this->display->setTextColor(WHITE);
    this->display->setCursor(0,0);
    this->display->print(this->ownerName.c_str()); this->display->println("'s");
    this->display->println("Weather Station");

    this->display->setCursor(0, 25);
    this->display->print("Temp       : "); this->display->println(weatherData.tempF);
    this->display->print("Humidity   : "); this->display->print(weatherData.humidity); this->display->println("%");
    this->display->print("Pressure   : "); this->display->println(weatherData.pressure);
    this->display->print("Brightness : "); this->display->println(weatherData.brightness);

    this->display->display();
  }

  return true;
}

void IOTDestinationOLEDDisplay::initDisplay() {
  delay(200);

  this->display = new Adafruit_SSD1306(4);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  this->display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  this->display->display(); // show splashscreen
  delay(1500);
  this->display->clearDisplay();   // clears the screen and buffer
}
