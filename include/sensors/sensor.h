#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include "config.h"

class Sensor {
public:
  Sensor(const char* type) {
    strncpy(this->type, type, 32); this->type[32] = 0;
    memset(this->value, 0, 33);
  }

protected:
  char type[33];
  char value[33];
};

#endif