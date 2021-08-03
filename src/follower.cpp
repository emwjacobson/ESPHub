#ifndef FOLLOWER_CPP
#define FOLLOWER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "config.h"
#include "follower.h"

#include "sensors/DHT11.h"

Follower::Follower(): num_sensors(0) {
  time_t start = millis();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.print("Attempting connection to '" HUB_SSID "'");
  this->connectToAP(HUB_SSID, HUB_PASSWD);

  #ifdef DHT11_Sensor
  this->addSensor(new DHT11Sensor(DHT11_PIN));
  #endif

  // The connection is delayed so we can run sensor collection while it's connecting
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");

  WiFiClient wifi_client;
  HTTPClient http;
  // TODO: Return this to original IP address.
  http.begin(wifi_client, "https://d89369fb-8a3f-4325-95b7-8580a84fe800.mock.pstmn.io/data");
  // http.begin(wifi_client, "http://10.1.0.1/data");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  for (Sensor* s : this->sensors) {
    String data;
    data.concat("name=" NODE_NAME);
    data.concat("&type=");
    data.concat(s->getType());
    data.concat("&value=");
    data.concat(s->getValue());
    time_t start_req = micros();
    int response_code = http.POST(data);
    time_t end_req = micros();
    Serial.print("Sent with response code: ");
    Serial.println(response_code);
    Serial.println(http.getString());
  }

  Serial.print("Took ");
  Serial.print(millis() - start);
  Serial.print(" ms. Starting deep sleep.");

  // Deep sleep for 1-2 minutes
  long time = random(60e6, 120e6);
  ESP.deepSleep(time);
}

void Follower::loop() {
  // For the follower we don't really expect it to loop, as after
  // it powers on, it should send results to the master node then
  // enter deep sleep. Upon waking up it should repeat that.
  Serial.print("YOU SHOULDNT SEE ME!");
  delay(1);
}

void Follower::addSensor(Sensor* sensor) {
  if (num_sensors > MAX_SENSORS) {
    return;
  }
  this->sensors[this->num_sensors] = sensor;
  this->num_sensors++;
}

#endif