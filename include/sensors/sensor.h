#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include "config.h"

class Sensor {
public:
  Sensor();
  const char* getType() const;
  virtual char* getValue() = 0;
protected:
  Sensor(const char* type);
  char type[TYPE_BUFFER + 1];
  char value[VALUE_BUFFER + 1];
private:
};

#endif