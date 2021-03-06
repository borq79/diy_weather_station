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

    long maxWaitTime = MAX_SESSION_LENGTH;
    long startTime = millis();

    while (client.connected()) {
      this->debugger->logln(DEBUG_LEVEL_TRACE, "Waiting for client to be available ...");
      while(!client.available() && ((millis() - startTime) < maxWaitTime)){
        delay(1);
      }
      this->debugger->logln(DEBUG_LEVEL_TRACE, "Client Ready.");

      String readBuffer; readBuffer.reserve(350);
      String requestBody;
      HTTPSTATE httpState = READ_REQUEST_LINE;
      int contentLength = 0;
      String requestPath;
      String httpMethod;

      while (client.connected() && ((millis() - startTime) < maxWaitTime)) {

        if (httpState == READ_BODY && requestBody.length() < contentLength && client.available() <= 0) {
          delay(1);
          continue;
        }

        this->debugger->logln(DEBUG_LEVEL_TRACE, "HTTP State: " + String(httpState == READ_REQUEST_LINE ? "Read Request Line" : (httpState == READ_HEADERS ? "Read Headers" : "Read Body")));
        if (httpState == READ_REQUEST_LINE) {
          readBuffer = client.readStringUntil('\n'); readBuffer.trim();

          this->debugger->logln(DEBUG_LEVEL_TRACE, " METHOD LINE: [" + readBuffer + "]");

          int endOfHttpMethod = readBuffer.indexOf(" ");
          httpMethod = readBuffer.substring(0, endOfHttpMethod); httpMethod.trim();

          int endOfPath = readBuffer.indexOf(" ", endOfHttpMethod + 1);
          requestPath = readBuffer.substring(endOfHttpMethod + 1, endOfPath);
          httpState = READ_HEADERS;
        } else if (httpState == READ_HEADERS) {
          readBuffer = client.readStringUntil('\n'); readBuffer.trim();

          this->debugger->logln(DEBUG_LEVEL_TRACE, " DATA: [" + readBuffer + "]");
          if (readBuffer.startsWith("Content-Length:")) {
            int endOfHeaderFieldName = readBuffer.indexOf(":");
            String contentLengthValue = readBuffer.substring(endOfHeaderFieldName + 1, readBuffer.length()); contentLengthValue.trim();
            contentLength = contentLengthValue.toInt();
            requestBody.reserve(contentLength);
            this->debugger->logln(DEBUG_LEVEL_TRACE, "HTTP Request Content Length is " + contentLengthValue + " bytes");
          } else if (readBuffer.length() == 0) {
            httpState = READ_BODY;
            this->debugger->logln(DEBUG_LEVEL_TRACE, "Starting to store the request body ...");
          } else {
            this->debugger->logln(DEBUG_LEVEL_TRACE, "HTTP Request Hdr: [" + readBuffer + "]");
          }
        } else if (httpState == READ_BODY) {
            requestBody += client.readString();
            if (requestBody.length() == contentLength) {
              break;
            }
        }
      }

      if (requestBody.length() > 0) {
        this->debugger->logln(DEBUG_LEVEL_TRACE, "HTTP Request Body [" + requestBody + "]");
      }

      this->debugger->logln(DEBUG_LEVEL_TRACE, "HTTP Request [" + httpMethod + "] for resource [" + requestPath + "]");

      if (requestPath.equals("/api/a")) {
        if (httpMethod.equals("GET")) {
          sendAPIAPStatus(client);
        } else {
          sendErrorResponse(client, 405, "Method Not Allowed");
        }
      } else if (requestPath.equals("/api/i")) {
        if (httpMethod.equals("GET") && this->isAPModeEnabled == false) {
          sendAPIWeatherData(client);
        } else {
          sendErrorResponse(client, 405, "Method Not Allowed");
        }
      } else if (requestPath.equals("/api/c")) {
        if (httpMethod.equals("GET") && this->isAPModeEnabled == true) {
          sendAPIConfiguration(client);
        } else {
          sendErrorResponse(client, 405, "Method Not Allowed");
        }
      } else if (requestPath.equals("/api/u")) {
        if (httpMethod.equals("POST")) {
          if (updateConfiguration(requestBody)) {
            String response = "{\"status\": 200 }";
            sendAPIJsonResponse(client, response);
          } else {
            sendErrorResponse(client, 500, "Internal Server Error");
          }
        } else {
          sendErrorResponse(client, 405, "Method Not Allowed");
        }
      } else if (requestPath.startsWith("/")) {
        if (requestPath.equals("/")) { requestPath = "/index.html"; }
        if (httpMethod.equals("GET")) {
          sendFile(client, requestPath);
        } else {
          sendErrorResponse(client, 500, "Internal Server Error");
        }
      }  else {
        sendErrorResponse(client, 404, "Not Found");
      }

      break; // End connection
    }

    // give the web browser time to receive the data
    delay(10);

    client.stop();
    this->debugger->logln(DEBUG_LEVEL_INFO, "Client Disonnected");
  }
}

void WeatherStationWebServer::sendAPIJsonResponse(WiFiClient &client, String &response) {
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print("Connection: close\r\n");  // the connection will be closed after completion of the response
  client.print("Content-Length: " + String(response.length()) + "\r\n");
  client.print("\r\n");
  client.print(response);
  client.print("\r\n\r\n");
}

void WeatherStationWebServer::sendAPIAPStatus(WiFiClient &client) {
  StaticJsonBuffer<128> jsonBuffer;
  String response;

  WeatherConfig c = weatherStation->getWeatherConfig();
  JsonObject& root = jsonBuffer.createObject();
  root["oname"] = c.getOwnerName();
  root["mode"] = String(this->isAPModeEnabled ? "ap" : "station");
  root.printTo(response);

  sendAPIJsonResponse(client, response);
}

void WeatherStationWebServer::sendAPIWeatherData(WiFiClient &client) {
  StaticJsonBuffer<256> jsonBuffer;
  String response;
  WeatherData d = weatherStation->getWeatherData();
  JsonObject& root = jsonBuffer.createObject();
  root["u"] = d.lastRead;
  root["t"] = d.tempF;
  root["p"] = d.pressure;
  root["h"] = d.humidity;
  root["b"] = d.brightness;
  root.printTo(response);

  sendAPIJsonResponse(client, response);
}

void WeatherStationWebServer::sendAPIConfiguration(WiFiClient &client) {
  StaticJsonBuffer<350> jsonBuffer;
  String response;

  WeatherConfig c = weatherStation->getWeatherConfig();
  JsonObject& root = jsonBuffer.createObject();
  root["o"] = c.getOwnerName();
  root["an"] = c.getAPName();
  root["apwd"] = c.getAPPassword();
  root["ssid"] = c.getSSID();
  root["pwd"] = c.getWifiPassword();
  root["tk"] = c.getThingSpeakAPIKey();
  root["tc"] = c.getThingSpeakChannelID();
  root["bk"] = c.getBlynkAPIKey();
  root["dl"] = c.getDebugLevelName();
  root.printTo(response);

  sendAPIJsonResponse(client, response);
}

void WeatherStationWebServer::sendErrorResponse(WiFiClient &client, int status, String statusPhrase) {
  client.print("HTTP/1.1 " + String(status) + " " + statusPhrase + "\r\n");
  client.print("Connection: close\r\n");  // the connection will be closed after completion of the response
  client.print("Content-Length: 0\r\n\r\n\r\n");
}

void WeatherStationWebServer::sendFile(WiFiClient &client, String requestPath) {
  String contentType = "text/html";
  if (requestPath.endsWith(".css")) { contentType = "text/css"; }
  else if (requestPath.endsWith(".html")) { contentType = "text/html"; }
  else if (requestPath.endsWith(".js")) { contentType = "application/javascript"; }

  SPIFFS.begin();
  File serverSideFile = SPIFFS.open(requestPath, "r");
  if (!serverSideFile) {
    this->debugger->logln(DEBUG_LEVEL_ERROR, String("Failed to open file " + serverSideFile));
    sendErrorResponse(client, 404, "Not Found");
  } else {
    int fileSize = serverSideFile.size();
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: " + contentType + "\r\n");
    client.print("Connection: close\r\n");  // the connection will be closed after completion of the response
    client.print("Content-Length: " + String(fileSize) + "\r\n");
    client.print("\r\n");

    const int TXFR_BUFFER_SIZE = 128;
    char *buffer = new char[TXFR_BUFFER_SIZE];
    while (serverSideFile.available()) {
      size_t bytesRead = serverSideFile.readBytes(buffer, TXFR_BUFFER_SIZE);
      if (bytesRead > 0) {
        client.write((const char*)buffer, bytesRead);
        // give the web browser time to receive the data
        delay(5);
      }
    }
    client.print("\r\n\r\n");

    // close the file
    serverSideFile.close();
  }
  SPIFFS.end();
}

bool WeatherStationWebServer::updateConfiguration(String &jsonConfig) {
  WeatherConfig c = weatherStation->getWeatherConfig();
  return c.saveConfigurationFile(jsonConfig);
}
