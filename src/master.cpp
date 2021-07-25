#ifndef MASTER_CPP
#define MASTER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "master.h"
#include "config.h"

void Master::setup() {
  WiFi.mode(WIFI_AP_STA);
  this->setupSoftAP();
  this->connectToAP();
}

void Master::loop() {
  #ifdef SERIAL_ENABLED
  Serial.print("Num connected: "); Serial.println(WiFi.softAPgetStationNum());
  Serial.print("WIFI Status: "); Serial.println(WiFi.status());
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

#endif