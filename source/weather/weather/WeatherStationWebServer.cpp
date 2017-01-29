// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "WeatherStationWebServer.h"

WeatherStationWebServer::WeatherStationWebServer() : server(WS_LISTEN_PORT) {
}

void WeatherStationWebServer::init(bool isAPModeEnabled, WeatherStationI *weatherStation) {
  this->debugger         = WeatherDebug::getWeatherDebugger();
  this->isAPModeEnabled  = isAPModeEnabled;
  this->weatherStation   = weatherStation;
  server.begin();
}

void WeatherStationWebServer::applicationLoop() {

  WiFiClient client = server.available();
  if (client) {
    this->debugger->logln(DEBUG_LEVEL_INFO, "Client Connected to Web Server");

    while (client.connected()) {
      int status = 200;
      String statusPhrase = "OK";
      String response;
      String contentType = "text/html";
      response.reserve(1024);

      if (client.available()) {
        String readBuffer;
        readBuffer.reserve(128);
        readBuffer = client.readString();
        this->debugger->logln(DEBUG_LEVEL_TRACE, " DATA: [" + readBuffer + "]");

        int endOfHttpMethod = readBuffer.indexOf(" ");
        String httpMethod = readBuffer.substring(0, endOfHttpMethod); httpMethod.trim();

        int endOfPath = readBuffer.indexOf(" ", endOfHttpMethod + 1);
        String requestPath = readBuffer.substring(endOfHttpMethod + 1, endOfPath);

        // Read the rest and toss
        this->debugger->logln(DEBUG_LEVEL_TRACE, "Toss the rest of the HTTP request");
        while (client.available() && client.readString().length() > 0) { this->debugger->logln(DEBUG_LEVEL_TRACE, "."); }

        this->debugger->logln(DEBUG_LEVEL_TRACE, "HTTP Request [" + httpMethod + "] for resource [" + requestPath + "]");

        if (requestPath.equals("/api/i")) {
          if (httpMethod.equals("GET") && this->isAPModeEnabled == false) {
            contentType = "application/json";
            WeatherData d = weatherStation->getWeatherData();
            response = "{\"t\": " + String(d.lastRead) + ",\"t\": " + String(d.tempF) + ",\"p\": " + String(d.pressure) + ",\"h\": " + String(d.humidity) + ",\"i\": " + String(d.brightness) + "}";
          } else {
            status = 405;
            statusPhrase = "Method Not Allowed";
          }
        } if (requestPath.equals("/api/c")) {
          if (httpMethod.equals("GET")) {
            contentType = "application/json";
            WeatherConfig c = weatherStation->getWeatherConfig();
            response = "{\"apn\": \"" + c.getAPName() + "\" ,\"ssid\": \"" + c.getSSID() + "\" ,\"d\": " + c.getDebugLevelName() + "\"}";
          } else {
            status = 405;
            statusPhrase = "Method Not Allowed";
          }
        } else if (requestPath.startsWith("/")) {
          if (requestPath.equals("/")) { requestPath = "/index.html"; }

          if (httpMethod.equals("GET")) {
            SPIFFS.begin();
            File serverSideFile = SPIFFS.open(requestPath, "r");
            if (!serverSideFile) {
              this->debugger->logln(DEBUG_LEVEL_ERROR, String("Failed to open file " + serverSideFile));
              status = 404;
              statusPhrase = "Not Found";
            } else {
              while (serverSideFile.available()) {
                response += serverSideFile.readString();
              }
              // close the file
              serverSideFile.close();
            }
            SPIFFS.end();

            if (requestPath.endsWith(".css")) { contentType = "text/css"; }
            else if (requestPath.endsWith(".html")) { contentType = "text/html"; }
            else if (requestPath.endsWith(".js")) { contentType = "application/javascript"; }
          //  response = "<html><head><title>Weather Station</title></head><body>Hello World</body></html>";
          } else {
            status = 405;
            statusPhrase = "Method Not Allowed";
          }
        }  else {
          status = 404;
          statusPhrase = "Not Found";
        }

        client.print("HTTP/1.1 " + String(status) + " " + statusPhrase + "\r\n");
        client.print("Content-Type: " + contentType + "\r\n");
        client.print("Connection: close\r\n");  // the connection will be closed after completion of the response
        client.print("Content-Length: " + String(response.length()) + "\r\n");
        client.print("\r\n");
        client.print(response);
        client.print("\r\n\r\n");

        break; // End connection
      }
    }

    // give the web browser time to receive the data
    delay(1);

    client.stop();
    this->debugger->logln(DEBUG_LEVEL_INFO, "Client Disonnected");
  }
}
