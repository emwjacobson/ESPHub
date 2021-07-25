#include <Arduino.h>
#include "master.h"
#include "config.h"

#ifndef MODE
#define MODE 1
#endif

Controller* controller;

void setup() {
  #ifdef SERIAL_ENABLED
  Serial.begin(9600);
  while (!Serial) {delay(1);}
  Serial.println("Serial Initialized");
  #endif

  #if MODE == 1
  controller = new Master();
  #elif MODE = 2
  controller = new Follower();
  #endif

  controller->setup();
}

void loop() {
  controller->loop();
}