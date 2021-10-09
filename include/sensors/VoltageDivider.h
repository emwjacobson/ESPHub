#ifndef VOLTAGEDIVIDER_H
#define VOLTAGEDIVIDER_H

#include "Sensor.h"
#include "config.h"

#ifndef VDIV_ANALOG_OFFSET
#define VDIV_ANALOG_OFFSET 0
#endif

#ifndef VDIV_R1
#define VDIV_R1 0
#endif

#ifndef VDIV_R2
#define VDIV_R2 1
#endif

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