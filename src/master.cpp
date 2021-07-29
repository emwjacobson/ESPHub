#ifndef MASTER_CPP
#define MASTER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "master.h"
#include "config.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "Pair.h"

Master::Master():
  server(AsyncWebServer(80)),
  data([](Pair<String, String> pair){
    delete pair.first;
    delete pair.second;
  })
  {
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  Serial.println("ID: " + String(ESP.getChipId()));
  this->setupSoftAP(HUB_SSID, HUB_PASSWD);
  this->connectToAP(EXTERNAL_SSID, EXTERNAL_PASSWD);
  this->registerEndpoints();
}

void Master::loop() {
  Serial.print("Num connected: "); Serial.println(WiFi.softAPgetStationNum());
  Serial.print("WIFI Status: ");
  switch(WiFi.status()) {
    case WL_CONNECTED:
      Serial.println("Connected");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("SSID Not Found");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Connection Failed");
      break;
    case WL_WRONG_PASSWORD:
      Serial.println("Wrong Password");
      break;
    case WL_DISCONNECTED:
      Serial.println("Disconnected");
      break;
    default:
      Serial.println(WiFi.status());
  }
  Serial.println("");

  delay(5000);
}

void Master::registerEndpoints() {
  this->server.on("/data", HTTP_POST, [this](AsyncWebServerRequest* request) {
    // https://github.com/me-no-dev/ESPAsyncWebServer#get-post-and-file-parameters
    if (request->hasParam("data", true)) {
      AsyncWebParameter* p = request->getParam("data", true);

      Serial.println("Got post value: " + p->value());

      this->data.add(ItemType{ new String(ESP.getChipId()), new String(p->value()) });

      request->send(200, "text/plain", "OK");
      return;
    }
    request->send(400, "text/plain", "This is an error!");
  });

  this->server.on("/collect", HTTP_GET, [this](AsyncWebServerRequest* request) {
    String out;

    for (auto temp = this->data.begin(); temp != nullptr; ++temp) {
      out.concat(*(temp->first) + "=" + *(temp->second) + "\n");
    }

    request->send(200, "text/plain", out);
    this->data.free();
  });

  this->server.begin();
  Serial.println("API Endpoints Registered.");
}

#endif