#ifndef PLAYGROUNDSENSOR_H
#define PLAYGROUNDSENSOR_H

#include "Sensor.h"

class Photoresistor : public Sensor {
public:
  Photoresistor() : Sensor("LightLevel") {}

  const char* getValue() override {
    snprintf(this->value, 32, "%i", analogRead(A0));
    return this->value;
  }
private:
};

#endif