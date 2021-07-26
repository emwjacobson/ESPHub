#ifndef MASTER_CPP
#define MASTER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "master.h"
#include "config.h"
// #include "ESPAsyncTCP.h"
// #include "ESPAsyncWebServer.h"

Master::Master()/*: server(AsyncWebServer(80))*/ {}

void Master::setup() {
  WiFi.mode(WIFI_AP_STA);
  this->setupSoftAP();
  this->connectToAP();
  // this->registerEndpoints();
}

void Master::loop() {
  #ifdef SERIAL_ENABLED
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
  #endif

  delay(1000);
}

void Master::setupSoftAP() {
  WiFi.softAPConfig(IPAddress(10,10,0,1), IPAddress(10,10,0,1), IPAddress(255,255,255,0));
  WiFi.softAP(HUB_SSID, HUB_PASSWD);

  #ifdef SERIAL_ENABLED
  Serial.println("Ready to accept clients.");
  Serial.println(WiFi.softAPIP());
  #endif
}

void Master::connectToAP() {
  WiFi.begin(EXTERNAL_SSID, EXTERNAL_PASSWD);
}

// void Master::registerEndpoints() {
//   this->server.on("/", HTTP_POST, [](AsyncWebServerRequest* request) {
//     Serial.println("Got Request!");
//   });
// }

#endif