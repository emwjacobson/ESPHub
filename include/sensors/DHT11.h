#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include <cstring>
#include "MultiSensor.h"

class DHT11Sensor : public MultiSensor {
public:
  DHT11Sensor(int pin) : MultiSensor(2), dht(pin, DHT11) {
    this->dht.begin();
  }

  const char* getValue() override {
    strncpy(this->value, "multi", 32);

    return this->value;
  }

  char* getType(const int& id) override {
    switch (id) {
      case 0:
        strncpy(this->type, "Temperature", 32);
        break;
      case 1:
        strncpy(this->type, "Humidity", 32);
        break;
      default:
        strncpy(this->type, "UNKNOWN", 32);
    }
    return this->type;
  }
  char* getValue(const int& id) override {
    if (id == 0) {
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
    } else if (id == 1) {
        float humidity = this->dht.readHumidity();
        int tries = 10;
        while (humidity == NAN && tries >= 0) {
          humidity = this->dht.readHumidity();
          tries--;
          delay(100);
        }

        if (tries == 0 && humidity == NAN) {
          snprintf(this->value, 32, "%.2f", humidity);
        } else {
          snprintf(this->value, 32, "%.2f", humidity);
        }
    } else {
        strncpy(this->value, "UNKNOWN", 32);
    }

    return this->value;
  }
private:
  DHT dht;

};

#endif