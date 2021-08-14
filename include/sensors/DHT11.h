#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include <cstring>
#include "MultiSensor.h"

#ifndef DHT11_READ_FAHRENHEIT
#define DHT11_READ_FAHRENHEIT false
#endif

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
      int delay_ms = 0;
      while (isnan(temp) && tries > 0) {
        Serial.println("Got Temp = nan");
        temp = this->dht.readTemperature(DHT11_READ_FAHRENHEIT);
        tries--;
        delay(delay_ms += 100);
      }

      if (tries == 0 && isnan(temp)) {
        Serial.println("Got Temp = nan Final");
        strncpy(this->value, "nan", 32);
        Serial.println("Error reading Temperature.");
      } else {
        snprintf(this->value, 32, "%.2f", temp);
      }
    } else if (id == 1) {
      float humidity = this->dht.readHumidity();
      int tries = 10;
      int delay_ms = 0;
      while (isnan(humidity) && tries >= 0) {
        Serial.println("Got Humidity = nan");
        humidity = this->dht.readHumidity();
        tries--;
        delay(delay_ms += 100);
      }

      if (tries == 0 && humidity == isnan(humidity)) {
        Serial.println("Got Humidity = nan Final");
        strncpy(this->value, "nan", 32);
        Serial.println("Error reading Humidity.");
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