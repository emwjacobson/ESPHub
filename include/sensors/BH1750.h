#ifndef BH1750_H
#define BH1750_H

#include <BH1750.h>
#include "Sensor.h"

class BH1750Sensor : public Sensor {
public:
  BH1750Sensor() : Sensor("LightLevel"), lightMeter(0x23) {
    this->lightMeter.begin();
  }

  const char* getValue() override {
    snprintf(this->value, 32, "%f", this->lightMeter.readLightLevel());
    return this->value;
  }
private:
  BH1750 lightMeter;
};

#endif