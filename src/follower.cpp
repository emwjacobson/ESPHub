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

  Serial.print("Attempting connection to '" + String(HUB_SSID) + "'");
  this->connectToAP(HUB_SSID, HUB_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");

  // TODO: Collect sensor data and send to master
  WiFiClient wifi_client;
  HTTPClient http;
  http.begin(wifi_client, "http://922c0c5a-8560-4919-995c-5367af83939a.mock.pstmn.io/data");
  // http.begin(wifi_client, IPAddress(10,1,0,1).toString() + "/data");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String data;
  data.concat("name=" + String(NODE_NAME_PREFIX) + String(ESP.getChipId()));
  data.concat("&type=temperature");
  data.concat("&value=100");
  int response_code = http.POST(data);
  Serial.println("Sent with response code: " + String(response_code));
  Serial.println(http.getString());

  Serial.println("Took " + String((millis() - start)/1000) + " seconds. Starting deep sleep.");
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