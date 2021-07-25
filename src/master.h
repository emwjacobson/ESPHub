#ifndef MASTER_H
#define MASTER_H

#include <Arduino.h>
#include "controller.h"

class Master : public Controller {
public:
  void setup();
  void loop();
private:
  // Creates a soft AP
  void setupSoftAP();
  // Connects to an AP
  void connectToAP();
};

#endif