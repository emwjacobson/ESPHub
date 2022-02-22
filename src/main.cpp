#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <vector>
#include "config.h"
#include "sensors/Device.h"
  #include "sensors/AHT10.h"
  #include "sensors/BH1750.h"

WiFiClient wifiClient;
PubSubClient mqtt(MQTT_HOST, MQTT_PORT, wifiClient);
std::vector<Device*> devices;

void setup() {
  Serial.begin(74880);
  Serial.println("Serial Initialized");

  // Connect to wifi
  PRINT("Connecting to " WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    PRINT(".");
  }
  PRINTLN(" Connected!");

  // Register devices
  #ifdef AHT10_Sensor
  devices.push_back(new AHT10Device());
  #endif

  #ifdef BH1750_Sensor
  devices.push_back(new BH1750Device());
  #endif

  mqtt.connect(NODE_NAME, MQTT_USERNAME, MQTT_PASSWD);

  for (Device* device : devices) {
    std::vector<sensor_data> data = device->getDeviceData();
    for (sensor_data s_data : data) {
      mqtt.publish(std::string("home/" NODE_NAME "/" + s_data.name).c_str(), std::to_string(s_data.data).c_str());
    }
  }

  ESP.deepSleep(SLEEP_TIME, WAKE_RF_DEFAULT);
}

void loop() {}