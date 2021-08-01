#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "controller.h"
#include "config.h"

int8_t Controller::connectToAP(char* SSID, char* passwd) {
  WiFi.begin(SSID, passwd);

  return WiFi.waitForConnectResult(60);
}

bool Controller::setupSoftAP(char* SSID, char* passwd) {
  WiFi.softAPConfig(IPAddress(10,1,0,1), IPAddress(10,1,0,1), IPAddress(255,255,255,0));
  return WiFi.softAP(SSID, passwd, 1, HUB_SSID_HIDDEN, 8);
}

#endif