#ifndef FOLLOWER_H
#define FOLLOWER_D

#include <vector>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Controller.h"
#include "sensors/Sensor.h"
#include "config.h"

class Follower : public Controller {
public:
  Follower();
  void loop() override;
private:
  WiFiClient wifi_client;
  HTTPClient http;
};

#endif