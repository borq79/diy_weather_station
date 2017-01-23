// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "WeatherStationWiFi.h"

WeatherStationWiFi::WeatherStationWiFi() {

}

void WeatherStationWiFi::connect() {
  Serial.println("Current Status of WiFi Connection: " + getWifiStatus(WiFi.status()));

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
  Serial.print("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}

String WeatherStationWiFi::getWifiStatus(wl_status_t status) {
  String statusString = "Disconnected";

  switch (status) {
    case WL_NO_SHIELD: statusString = "No Sheild"; break;
    case WL_IDLE_STATUS: statusString = "Idle"; break;
    case WL_NO_SSID_AVAIL: statusString = "No SSID Available"; break;
    case WL_SCAN_COMPLETED: statusString = "Scan Completed"; break;
    case WL_CONNECT_FAILED: statusString = "Connect Failed"; break;
    case WL_CONNECTED: statusString = "Connected"; break;
    case WL_CONNECTION_LOST: statusString = "Connection Lost"; break;
    case WL_DISCONNECTED: statusString = "Disconnected"; break;
    default: statusString = "Unknown"; break;
  }

  return statusString;
}

void WeatherStationWiFi::setSSID(String ssid) {
  this->wifiSSID = ssid;
}

void WeatherStationWiFi::setPassword(String password) {
  this->wifiPassword = password;
}
