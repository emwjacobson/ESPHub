#ifndef RESISTIVESOIL_H
#define RESISTIVESOIL_H

#include "Sensor.h"

class ResistiveSoil : public Sensor {
public:
  ResistiveSoil() : Sensor("SoilMoisture") {}

  const char* getValue() override {
    int value = analogRead(A0);
    int mapped_val = map(value, 1024, 0, 0, 100);
    snprintf(this->value, 32, "%i", mapped_val);
    return this->value;
  }
private:
};

#endif