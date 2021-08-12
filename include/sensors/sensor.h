#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include "config.h"

class Sensor {
protected:
  Sensor(const char* type) {
    strncpy(this->type, type, 32); this->type[32] = 0;
    memset(this->value, 0, 33);
  }
public:
  const char* getType() const { return this->type; }
  virtual const char* getValue() = 0;

protected:
  char type[33];
  char value[33];
};

#endif