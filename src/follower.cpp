#ifndef FOLLOWER_CPP
#define FOLLOWER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "follower.h"
#include "sensors/DHT11.h"

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

  WiFiClient wifi_client;
  HTTPClient http;

  // // TODO: Return this to original IP address.
  http.begin(wifi_client, "http://124b6aa2-1b95-4872-8f89-d23b097cee66.mock.pstmn.io/data");
  // http.begin(wifi_client, "http://10.1.0.1/data");
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(9001); // TODO: Calculate number later

  doc["name"] = NODE_NAME;
  JsonArray sensor_doc = doc.createNestedArray("sensors");

  for (Sensor* s : this->sensors) {
    JsonObject sensor = sensor_doc.createNestedObject();
    sensor["type"] = s->getType();
    sensor["value"] = s->getValue();
  }

  int json_size = measureJson(doc);
  char buffer[json_size];
  serializeJson(doc, buffer, json_size);

  int response_code = http.POST((uint8_t*)buffer, json_size);
  Serial.println(response_code);

  // // Deep sleep for 1-2 minutes
  // const long time = random(60e6, 120e6);
  const long time = random(1e6, 5e6);
  ESP.deepSleep(time);
}

void Follower::loop() {
  // For the follower we don't really expect it to loop, as after
  // it powers on, it should send results to the hub node then
  // enter deep sleep. Upon waking up it should repeat that.
  Serial.print("YOU SHOULDNT SEE ME!");
  delay(1);
}

void Follower::addSensor(Sensor* sensor) {
  this->sensors.push_back(sensor);
}

#endif