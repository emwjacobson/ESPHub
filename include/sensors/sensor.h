#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include "config.h"

class Sensor {
public:
  Sensor();
  const char* getType() const;
  virtual const char* getValue() = 0;
protected:
  Sensor(const char* type);
  char type[TYPE_BUFFER + 1];
private:
};

#endif