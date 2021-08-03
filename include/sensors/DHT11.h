#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include "sensor.h"

class DHT11Sensor : public Sensor {
public:
  DHT11Sensor() : Sensor("DHT11"), dht(DHT11, 2) {
    this->dht.begin();
  }
  const char* getValue() {
    sprintf(this->value, "%i", this->dht.readTemperature(true));
    return this->value;
  }
private:
  DHT dht;
};

#endif