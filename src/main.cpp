#ifndef MODE
#define MODE 2
#endif

#include <Arduino.h>
#include <LittleFS.h>
#include "config.h"

#if MODE == 1
#include "Hub.h"
#elif MODE == 2
#include "Follower.h"
#endif

Controller* controller;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial Initialized");

  if (!LittleFS.begin()) {
    Serial.println("An error has occured mounting LittleFS");
  }

  #if MODE == 1
  controller = new Hub();
  #elif MODE == 2
  controller = new Follower();
  #endif
}

void loop() {
  controller->loop();
}