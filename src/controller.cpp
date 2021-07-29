#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "controller.h"

void Controller::connectToAP(String SSID, String passwd) {
  WiFi.begin(SSID, passwd);

  Serial.print("Connecting to '" + SSID + "'");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Connected!");
}

void Controller::setupSoftAP(String SSID, String passwd) {
  WiFi.softAPConfig(IPAddress(10,1,0,1), IPAddress(10,1,0,1), IPAddress(255,255,255,0));
  WiFi.softAP(SSID, passwd);

  Serial.print("Ready to accept clients. IP: ");
  Serial.println(WiFi.softAPIP());
}

#endif