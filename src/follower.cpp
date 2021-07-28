#ifndef FOLLOWER_CPP
#define FOLLOWER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "follower.h"

Follower::Follower() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  this->connectToAP();
}

void Follower::loop() {
  delay(1);
}

#endif