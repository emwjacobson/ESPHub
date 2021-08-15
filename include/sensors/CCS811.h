#ifndef CCS811_H
#define CCS811_H

#include <Arduino.h>
#include <cstring>
#include <Adafruit_CCS811.h>
#include "MultiSensor.h"

class CCS811 : public Sensor {
public:
  CCS811() : Sensor("CO2") {
    this->ccs.begin();

    // Wait for the sensor to become available
    while (!this->ccs.available()) delay(100);
    Serial.println("Available!");
  }

  const char* getValue() override {
    uint8_t data_read = this->ccs.readData();
    int tries = 10;
    int delay_ms = 0;
    while (data_read != 0 && tries > 0) {
      data_read = this->ccs.readData();
      delay(delay_ms += 200);
      tries--;
    }

    if (tries == 0 && data_read != 0) {
      strncpy(this->value, "error", 32);
      Serial.println("Error reading CO2.");
      return this->value;
    }

    snprintf(this->value, 32, "%i", this->ccs.geteCO2());
    return this->value;
  }
private:
  Adafruit_CCS811 ccs;
};

#endif