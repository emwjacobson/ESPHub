#ifndef MANAGEDSENSOR_H
#define MANAGEDSENSOR_H

#include "sensor.h"

class ManagedSensor : public Sensor {
public:
  ManagedSensor() {
    memset(this->type, 0, (TYPE_BUFFER + 1) * sizeof(char*));
    memset(this->value, 0, (VALUE_BUFFER + 1) * sizeof(char*));
  }

  ManagedSensor(const char* type, const char* value) {
    int len = strlen(type);
    len = len > TYPE_BUFFER ? TYPE_BUFFER : len;
    memcpy(this->type, type, len);
    this->type[len] = 0;

    len = strlen(value);
    len = len > VALUE_BUFFER ? VALUE_BUFFER : len;
    memcpy(this->value, value, len);
    this->value[len] = 0;
  }

  const char* getValue() const {
    return value;
  }

  void setValue(const char* value) {
    int len = strlen(value);
    len = len > VALUE_BUFFER ? VALUE_BUFFER : len;
    memcpy(this->value, value, len);
    this->value[len] = 0;
  }
};

#endif