#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ESP8266WiFi.h>
#include "config.h"

class Controller {
public:
  virtual void loop() = 0;
protected:
  void connectToAP();
};

void Controller::connectToAP() {
  WiFi.begin(EXTERNAL_SSID, EXTERNAL_PASSWD);
}

#endif