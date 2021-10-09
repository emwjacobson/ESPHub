#ifndef VOLTAGEDIVIDER_H
#define VOLTAGEDIVIDER_H

#include "Sensor.h"
#include "config.h"

class VoltageDivider : public Sensor {
public:
  VoltageDivider() : Sensor("VoltageDivider") {}

  const char* getValue() override {
    float v_in = (float)(analogRead(A0) + VDIV_ANALOG_OFFSET) / 1023;
    float scale = (float)(VDIV_R1 + VDIV_R2) / VDIV_R2;
    snprintf(this->value, 32, "%f", (float)(v_in * scale));
    return this->value;
  }
private:
};

#endif