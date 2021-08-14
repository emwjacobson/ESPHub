#ifndef FOLLOWER_CPP
#define FOLLOWER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "Follower.h"
#include "sensors/DHT11.h"
#include "sensors/CCS811.h"

Follower::Follower() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.print("Attempting connection to '" HUB_SSID "'");
  this->connectToAP(HUB_SSID, HUB_PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");

  #ifdef DHT11_Sensor
  this->addSensor(new DHT11Sensor(DHT11_PIN));
  #endif

  #ifdef CCS811_Sensor
  this->addSensor(new CCS811());
  #endif

  this->http.begin(wifi_client, "http://10.1.0.1/data");
}

void Follower::loop() {
  this->http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(9001); // TODO: Calculate number later

  doc["name"] = NODE_NAME;
  JsonArray sensor_doc = doc.createNestedArray("sensors");
  boolean error_encountered = false;

  for (Sensor* s : this->sensors) {
    if (strcmp(s->getValue(), "multi") == 0) {
      MultiSensor* ms = (MultiSensor*)s;
      for (int i = 0; i < ms->getNumSensors(); i++) {
        JsonObject sensor = sensor_doc.createNestedObject();
        sensor["type"] = ms->getType(i);
        char* val = ms->getValue(i);
        sensor["value"] = val;
        if (strncmp(val, "error", 32) == 0) {
          error_encountered = true;
        }
      }
    } else {
      JsonObject sensor = sensor_doc.createNestedObject();
      sensor["type"] = s->getType();
      const char* val = s->getValue();
      sensor["value"] = val;
      if (strncmp(val, "error", 32) == 0) {
        error_encountered = true;
      }
    }
  }

  int json_size = measureJson(doc);
  char buffer[json_size];
  serializeJson(doc, buffer, json_size);

  int response_code = this->http.POST((uint8_t*)buffer, json_size);
  Serial.println(response_code);

  if (error_encountered) { // If were getting errors, attempt to restart the device.
    Serial.println("Errors encountered, restarting device...");
    ESP.restart();
  }

  #if defined(MODE_ACTIVE_DELAY)
  delay(random(60e3, 120e3)); // Sleep for 60-120 seconds
  #elif defined(MODE_ACTIVE)
  delay(1000); // Only sleep for 1 second.
  #else // Fallback is Deep Sleep
  ESP.deepSleep(random(60e6, 120e6)); // Deep sleep for 60-120 seconds
  #endif
}

void Follower::addSensor(Sensor* sensor) {
  this->sensors.push_back(sensor);
}

#endif