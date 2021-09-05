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
    float light_level = this->lightMeter.readLightLevel();

    // Auto adjust MTreg for super high/low values
    // https://github.com/claws/BH1750/blob/master/examples/BH1750autoadjust/BH1750autoadjust.ino

    if (light_level > 40000) { // Very bright
      this->lightMeter.setMTreg(32);
    } else if (light_level > 10) { // Default
      this->lightMeter.setMTreg(69);
    } else { // Very Dark
      this->lightMeter.setMTreg(138);
    }

    snprintf(this->value, 32, "%f", this->lightMeter.readLightLevel());
    return this->value;
  }
private:
  BH1750 lightMeter;
};

#endif