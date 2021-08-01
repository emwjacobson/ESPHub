#ifndef SENSOR_CPP
#define SENSOR_CPP

#include "Arduino.h"
#include "sensors/sensor.h"

Sensor::Sensor()
  : type(""), value("")
{}

Sensor::Sensor(const String& type, const String& value)
  : type(type), value(value)
{}

Sensor::Sensor(String&& type, String&& value)
  : type(std::move(type)), value(std::move(value))
{}

const String& Sensor::getType() const {
  return this->type;
}

const String& Sensor::getValue() const {
  return this->value;
}

void Sensor::setValue(const String& value) {
  this->value = value;
}

#endif