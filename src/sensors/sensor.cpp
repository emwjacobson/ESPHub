#ifndef SENSOR_CPP
#define SENSOR_CPP

#include "Arduino.h"
#include "sensors/sensor.h"

Sensor::Sensor() {
  type[0] = 0;
  value[0] = 0;
}

Sensor::Sensor(const char* type, const char* value) {
  int len = strlen(type);
  len = len > TYPE_BUFFER ? TYPE_BUFFER : len;
  memcpy(this->type, type, len);
  this->type[len] = 0;

  len = strlen(value);
  len = len > VALUE_BUFFER ? VALUE_BUFFER : len;
  memcpy(this->value, value, len);
  this->value[len] = 0;
}

const char* Sensor::getType() const {
  return *type;
}

const char* Sensor::getValue() const {
  return *value;
}

void Sensor::setValue(const char* value) {
  int len = strlen(value);
  len = len > VALUE_BUFFER ? VALUE_BUFFER : len;
  memcpy(this->value, value, len);
  this->value[len] = 0;
}

#endif