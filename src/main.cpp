#include <Arduino.h>
#include <LittleFS.h>
#include "master.h"
#include "follower.h"
#include "config.h"

#ifndef MODE
#define MODE 2
#endif

Controller* controller;

void setup() {
  Serial.begin(9600);
  while (!Serial) {ESP.wdtFeed();}
  delay(500);
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