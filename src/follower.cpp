#ifndef FOLLOWER_CPP
#define FOLLOWER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "config.h"
#include "follower.h"

Follower::Follower() {
  time_t start = millis();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.print("Attempting connection to '" HUB_SSID "'");
  this->connectToAP(HUB_SSID, HUB_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");

  WiFiClient wifi_client;
  HTTPClient http;
  // TODO: Return this to original IP address.
  // http.begin(wifi_client, "http://922c0c5a-8560-4919-995c-5367af83939a.mock.pstmn.io/data");
  http.begin(wifi_client, "http://10.1.0.1/data");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // TODO: Implement real sensor readings
  char* type = "temperature";
  char* value = "100";

  String data;
  data.concat("name=" NODE_NAME);
  data.concat("&type=");
  data.concat(type);
  data.concat("&value=");
  data.concat(value);
  time_t start_req = micros();
  int response_code = http.POST(data);
  time_t end_req = micros();
  Serial.print("Sent with response code: ");
  Serial.println(response_code);
  Serial.println(http.getString());

  Serial.print("Took ");
  Serial.print(millis() - start);
  Serial.print(" ms. Starting deep sleep.");
  Serial.println(end_req - start_req);

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

#endif