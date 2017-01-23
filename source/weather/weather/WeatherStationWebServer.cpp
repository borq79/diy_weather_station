// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "WeatherStationWebServer.h"

WeatherStationWebServer::WeatherStationWebServer() : server(WS_LISTEN_PORT){

}

void WeatherStationWebServer::start() {
  server.begin();
}

void WeatherStationWebServer::applicationLoop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client Connected to Web Server");

    // give the web browser time to receive the data
    delay(1);


    client.stop();
    Serial.println("Client Disonnected");
  }
}
