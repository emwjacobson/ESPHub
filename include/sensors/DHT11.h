#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include <cstring>
#include "sensor.h"

class DHT11Sensor : public Sensor {
public:
  DHT11Sensor(int pin) : Sensor("Temperature"), dht(pin, DHT11) {
    this->dht.begin();
  }
  virtual const char* getValue() {
    float temp = this->dht.readTemperature(DHT11_READ_FAHRENHEIT);
    int tries = 10;
    while (temp == NAN && tries >= 0) {
      temp = this->dht.readTemperature(DHT11_READ_FAHRENHEIT);
      tries--;
      delay(100);
    }

    if (tries == 0 && temp == NAN) {
      snprintf(this->value, 32, "%.2f", temp);
    } else {
      snprintf(this->value, 32, "%.2f", temp);
    }

    return this->value;
  }
private:
  DHT dht;

};

#endif