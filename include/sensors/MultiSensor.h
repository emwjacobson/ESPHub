#ifndef MULTISENSOR_H
#define MULTISENSOR_H

#include <cstring>
#include "Sensor.h"

class MultiSensor : public Sensor {
protected:
  MultiSensor(int num_sensors) : Sensor("Unknown"), num_sensors(num_sensors) {
    memset(this->type, 0, 33);
    memset(this->type, 0, 33);
  }

  int num_sensors;
public:
  virtual char* getType(const int& id) = 0;
  virtual char* getValue(const int& id) = 0;

  const int& getNumSensors() const { return this->num_sensors; }
};

#endif