#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "controller.h"

void Controller::connectToAP(String SSID, String passwd) {
  WiFi.begin(SSID, passwd);
}

void Controller::setupSoftAP(String SSID, String passwd) {
  WiFi.softAPConfig(IPAddress(10,1,0,1), IPAddress(10,1,0,1), IPAddress(255,255,255,0));
  WiFi.softAP(SSID, passwd);

  Serial.println("Ready to accept clients.");
  Serial.println(WiFi.softAPIP());
}

#endif