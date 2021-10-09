#ifndef FOLLOWER_CPP
#define FOLLOWER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "Follower.h"
#include "sensors/ManagedSensor.h"
#include "sensors/CCS811.h"
#include "sensors/DHT11.h"
#include "sensors/Photoresistor.h"
#include "sensors/BH1750.h"
#include "sensors/ResistiveSoil.h"
#include "sensors/AHT10.h"
#include "sensors/VoltageDivider.h"

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



  // ******************************
  // BEGIN SENSORS
  // ******************************

  #ifdef DHT11_Sensor
  this->addSensor(new DHT11Sensor(DHT11_PIN));
  #endif

  #ifdef AHT10_Sensor
  this->addSensor(new AHT10Sensor());
  #endif

  #ifdef CCS811_Sensor
  this->addSensor(new CCS811());
  #endif

  #ifdef Photoresistor_Sensor
  this->addSensor(new Photoresistor());
  #endif

  #ifdef BH1750_Sensor
  this->addSensor(new BH1750Sensor());
  #endif

  #ifdef ResistiveSoil_Sensor
  this->addSensor(new ResistiveSoil());
  #endif

  #ifdef Voltage_Divider
  this->addSensor(new VoltageDivider());
  #endif

  // ******************************
  // END SENSORS
  // ******************************



  this->http.begin(wifi_client, "http://10.1.0.1/data");
}

void Follower::loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WiFi");
    this->connectToAP(HUB_SSID, HUB_PASSWD);

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }

    Serial.println();
  }



  DynamicJsonDocument doc(9001); // TODO: Calculate number later

  doc["name"] = NODE_NAME;
  JsonArray sensor_doc = doc.createNestedArray("sensors");
  bool error_encountered = false;

  for (Sensor* s : this->sensors) {
    if (strcmp(s->getValue(), "multi") == 0) {
      MultiSensor* ms = (MultiSensor*)s;
      for (int i = 0; i < ms->getNumSensors(); i++) {
        JsonObject sensor = sensor_doc.createNestedObject();
        sensor["type"] = ms->getType(i);
        char* val = ms->getValue(i);
        sensor["value"] = val;
        Serial.print(ms->getType(i));
        Serial.print("=");
        Serial.println(val);
        if (strncmp(val, "error", 32) == 0) {
          error_encountered = true;
        }
      }
    } else {
      JsonObject sensor = sensor_doc.createNestedObject();
      sensor["type"] = s->getType();
      const char* val = s->getValue();
      sensor["value"] = val;
      Serial.print(s->getType());
      Serial.print("=");
      Serial.println(val);
      if (strncmp(val, "error", 32) == 0) {
        error_encountered = true;
      }
    }
  }

  this->http.addHeader("Content-Type", "application/json");

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
    #ifdef ACTIVE_DELAY_INTERNET_DISCONNECT
    WiFi.disconnect();
    #endif
  delay(random(60e3, 120e3)); // Sleep for 60-120 seconds
  #elif defined(MODE_ACTIVE)
  delay(1000); // Only sleep for 1 second.
  #else // Fallback is Deep Sleep
  ESP.deepSleep(random(60e6, 120e6)); // Deep sleep for 60-120 seconds
  #endif
}

#endif