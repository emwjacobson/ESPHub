#ifndef SENSOR_CPP
#define SENSOR_CPP

#include "Arduino.h"
#include "sensors/sensor.h"

Sensor::Sensor() {
  memset(this->type, 0, (TYPE_BUFFER + 1) * sizeof(char*));
  memset(this->value, 0, (TYPE_BUFFER + 1) * sizeof(char*));
}

Sensor::Sensor(const char* type) {
  int len = strlen(type);
  len = len > TYPE_BUFFER ? TYPE_BUFFER : len;
  memcpy(this->type, type, len);
  this->type[len] = 0;

  memset(this->value, 0, (TYPE_BUFFER + 1) * sizeof(char*));
}

const char* Sensor::getType() const {
  return type;
}

#endif