#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include "sensor.h"

class DHT11Sensor : public Sensor {
public:
  DHT11Sensor(const int& pin) : Sensor("DHT11"), dht(DHT11, pin) {
    this->dht.begin();
  }
  char* getValue() override {
    sprintf(this->value, "%f", this->dht.readTemperature(true));
    return this->value;
  }
private:
  DHT dht;
};

#endif