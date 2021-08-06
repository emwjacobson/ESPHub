#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include "sensor.h"

class DHT11Sensor : public Sensor {
public:
  DHT11Sensor(const int& pin) : Sensor("temperature"), dht(pin, DHT11) {
    this->dht.begin();
  }
  const char* getValue() override {
    snprintf(this->value, VALUE_BUFFER, "%4.2f", this->dht.readTemperature(true, true));
    return this->value;
  }
private:
  DHT dht;
};

#endif