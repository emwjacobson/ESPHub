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
private:
  WebServer http;
};

#endif