#ifndef MASTER_H
#define MASTER_H

#include <Arduino.h>
#include "controller.h"
#include "ESPAsyncWebServer.h"

class Master : public Controller {
public:
  Master();
  void setup();
  void loop();
private:
  AsyncWebServer server;

  // Creates a soft AP
  void setupSoftAP();
  // Connects to an AP
  void connectToAP();
  // Register web server endpoints
  void registerEndpoints();
};

#endif