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
  const char* getType() const { return this->type; }
  const char* getValue() const { return this->value; }
  void setValue(const char* new_value) {
    strncpy(this->value, new_value, 32);
    this->value[32] = 0;
  }
private:
  char type[33];
  char value[33];
};

#endif