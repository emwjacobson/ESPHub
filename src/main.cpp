#ifndef MODE
#define MODE 2
#endif

#include <Arduino.h>
#include <LittleFS.h>
#include "config.h"

#if MODE == 1
#include "master.h"
#elif MODE == 2
#include "follower.h"
#endif

Controller* controller;

void setup() {
  Serial.begin(9600);
  while (!Serial) {delay(1);}
  Serial.println("Serial Initialized");

  if (!LittleFS.begin()) {
    Serial.println("An error has occured mounting LittleFS");
  }

  #if MODE == 1
  controller = new Master();
  #elif MODE == 2
  controller = new Follower();
  #endif
}

void loop() {
  controller->loop();
}