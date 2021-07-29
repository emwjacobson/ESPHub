#ifndef MASTER_H
#define MASTER_H

#include <ESPAsyncWebServer.h>
#include "controller.h"

class Master : public Controller {
public:
  Master();
  void loop() override;
private:
  AsyncWebServer server;
  String data;

  // Register web server endpoints
  void registerEndpoints();
};

#endif