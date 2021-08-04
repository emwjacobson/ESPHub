#ifndef SENSOR_CPP
#define SENSOR_CPP

#include "Arduino.h"
#include "sensors/sensor.h"

Sensor::Sensor() {
  memset(this->type, 0, (TYPE_BUFFER + 1) * sizeof(char*));
  memset(this->value, 0, (VALUE_BUFFER + 1) * sizeof(char*));
}

Sensor::Sensor(const char* type) {
  int len = strlen(type);
  len = len > TYPE_BUFFER ? TYPE_BUFFER : len;
  memcpy(this->type, type, len);
  this->type[len] = 0;

  memset(this->value, 0, (VALUE_BUFFER + 1) * sizeof(char*));
}

const char* Sensor::getType() const {
  return type;
}

// const char* Sensor::getValue() const {
//   return *value;
// }

// void Sensor::setValue(const char* value) {
//   int len = strlen(value);
//   len = len > VALUE_BUFFER ? VALUE_BUFFER : len;
//   memcpy(this->value, value, len);
//   this->value[len] = 0;
// }

#endif