#ifndef MASTER_H
#define MASTER_H

#include "controller.h"
#include "ESPAsyncWebServer.h"

class Master : public Controller {
public:
  Master();
  void loop() override;
private:
  AsyncWebServer server;

  // Register web server endpoints
  void registerEndpoints();
};

#endif