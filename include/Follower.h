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
  void addSensor(Sensor* sensor);
private:
  std::vector<Sensor*> sensors;
  WiFiClient wifi_client;
  HTTPClient http;
};

#endif