#ifndef AHT10_H
#define AHT10_H

#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <cstring>
#include "MultiSensor.h"

#ifndef AHT10_READ_FAHRENHEIT
#define AHT10_READ_FAHRENHEIT false
#endif

class AHT10Sensor : public MultiSensor {
public:
  AHT10Sensor() : MultiSensor(2) {
    if (!this->aht.begin()) {
      Serial.println("Couldnt find AHT10");
    }
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
    if (id == 0) { // Temperature
      sensors_event_t temperature;
      this->aht.getTemperatureSensor()->getEvent(&temperature);

      #if AHT10_READ_FAHRENHEIT
      temperature.temperature = (temperature.temperature * ((float)9/5)) + 32;
      #endif

      temperature.temperature += AHT10_TEMP_OFFSET;

      snprintf(this->value, 32, "%.2f", temperature.temperature);
      // float temp = this->dht.readTemperature(DHT11_READ_FAHRENHEIT);
      // int tries = 10;
      // int delay_ms = 0;
      // while (isnan(temp) && tries > 0) {
      //   Serial.println("Got Temp = nan");
      //   temp = this->dht.readTemperature(DHT11_READ_FAHRENHEIT, true);
      //   tries--;
      //   delay(delay_ms += 250);
      // }

      // if (tries == 0 && isnan(temp)) {
      //   Serial.println("Got Temp = nan Final");
      //   strncpy(this->value, "error", 32);
      //   Serial.println("Error reading Temperature.");
      // } else {
      //   snprintf(this->value, 32, "%.2f", temp);
      // }

    } else if (id == 1) { // Humidity
      sensors_event_t humidity;
      this->aht.getHumiditySensor()->getEvent(&humidity);
      snprintf(this->value, 32, "%.2f", humidity.relative_humidity);
      // float humidity = this->dht.readHumidity();
      // int tries = 10;
      // int delay_ms = 0;
      // while (isnan(humidity) && tries > 0) {
      //   Serial.println("Got Humidity = nan");
      //   humidity = this->dht.readHumidity(true);
      //   tries--;
      //   delay(delay_ms += 250);
      // }

      // if (tries == 0 && isnan(humidity)) {
      //   Serial.println("Got Humidity = nan Final");
      //   strncpy(this->value, "error", 32);
      //   Serial.println("Error reading Humidity.");
      // } else {
      //   snprintf(this->value, 32, "%.2f", humidity);
      // }

    } else { // Error
        strncpy(this->value, "error", 32);
    }

    return this->value;
  }
private:
  Adafruit_AHTX0 aht;

};

#endif