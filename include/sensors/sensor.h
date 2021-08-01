#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"

class Sensor {
public:
  Sensor();
  Sensor(const String& type, const String& value);
  Sensor(String&& type, String&& value);
  const String& getType() const;
  const String& getValue() const;
  void setValue(const String& value);
private:
  String type;
  String value;
};

#endif