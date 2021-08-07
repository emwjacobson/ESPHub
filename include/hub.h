#ifndef HUB_H
#define HUB_H

#include <Arduino.h>
#include <WebServer.h>
#include "config.h"
#include "controller.h"

class Hub : public Controller {
public:
  Hub();
  void loop() override;
  void handleClient(WiFiClient& client);
private:
  WebServer http;
};

#endif