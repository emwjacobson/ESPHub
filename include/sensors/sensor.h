#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include "config.h"

class Sensor {
public:
  Sensor();
  Sensor(const char* type, const char* value);
  const char* getType() const;
  const char* getValue() const;
  void setValue(const char* value);
private:
  char* type[TYPE_BUFFER + 1];
  char* value[VALUE_BUFFER + 1];
};

#endif