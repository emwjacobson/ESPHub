#ifndef AHT10_H
#define AHT10_H

#include <Adafruit_AHTX0.h>
#include "config.h"
#include "Device.h"

class AHT10Device : public Device {
public:
  AHT10Device() : Device() {
    if (!this->aht.begin()) {
      PRINTLN("Couldn't find AHT10");
    }
  }

  virtual device_data getDeviceData() {
    device_data data;

    // Temperature
    sensors_event_t temperature;
    this->aht.getTemperatureSensor()->getEvent(&temperature);
    #if AHT10_READ_FAHRENHEIT
    temperature.temperature = (temperature.temperature * ((float)9/5)) + 32;
    #endif
    data.push_back(sensor_data { "temperature", temperature.temperature });

    // Humidity
    sensors_event_t humidity;
    this->aht.getHumiditySensor()->getEvent(&humidity);
    data.push_back(sensor_data { "humidity", humidity.relative_humidity });

    return data;
  }
private:
  Adafruit_AHTX0 aht;
};

#endif