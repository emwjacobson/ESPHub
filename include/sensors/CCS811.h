#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <cstring>
#include <Adafruit_CCS811.h>
#include "MultiSensor.h"

class DHT11Sensor : public Sensor {
public:
  DHT11Sensor(int pin) : Sensor("CO2") {
    ccs.begin();
  }

  const char* getValue() override {
    if (this->ccs.available()) {
      if (!ccs.readData()) {
        snprintf(this->value, 32, "%i", this->ccs.geteCO2());
        return this->value;
      }
    }

    strncpy(this->value, "ERROR", 32);
    return this->value;
  }
private:
  Adafruit_CCS811 ccs;
};

#endif