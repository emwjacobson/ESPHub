#ifndef MANAGEDSENSOR_H
#define MANAGEDSENSOR_H

#include "Sensor.h"

class ManagedSensor : public Sensor {
public:
  ManagedSensor(const char* type) : Sensor(type) {}
  const char* getValue() override { return this->value; }
  void setValue(const char* new_value) {
    strncpy(this->value, new_value, 32);
    this->value[32] = 0;
  }
};

#endif

