#ifndef BH1750_H
#define BH1750_H

#include <BH1750.h>
#include "config.h"
#include "Device.h"

class BH1750Device : public Device {
public:
  BH1750Device() : Device() {
    if (!this->lightMeter.begin()) {
      PRINTLN("Couldn't find BH1750");
    }
  }

  virtual device_data getDeviceData() {
    device_data data;

    float light_level = this->lightMeter.readLightLevel();

    if (light_level > 40000) { // Very bright
      this->lightMeter.setMTreg(32);
    } else if (light_level > 10) { // Default
      this->lightMeter.setMTreg(69);
    } else { // Very dark
      this->lightMeter.setMTreg(138);
    }

    light_level = this->lightMeter.readLightLevel();

    data.push_back(sensor_data { "brightness", light_level });

    return data;
  }
private:
  BH1750 lightMeter;
};

#endif