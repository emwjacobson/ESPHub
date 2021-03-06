#ifndef HUB_H
#define HUB_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <vector>
#include <cstring>
#include "config.h"
#include "Controller.h"
#include "sensors/ManagedSensor.h"

class Node {
public:
  Node(const char* name) {
    strncpy(this->name, name, 32);
    this->name[32] = 0;
  }
  const char* getName() const { return this->name; }
  std::vector<ManagedSensor>& getSensors() { return this->sensors; }
  ManagedSensor& sensorAt(const int& i) { return this->sensors.at(i); }
  void addSensor(const char* type) { this->sensors.push_back(std::move(ManagedSensor(type))); }
private:
  char name[33];
  std::vector<ManagedSensor> sensors;
};



class Hub : public Controller {
public:
  Hub();
  void loop() override;
  std::vector<Node>& getNodes() { return this->nodes; }
  Node& nodeAt(const int& i) { return this->nodes.at(i); }
  void addNode(const char* name) { this->nodes.push_back(std::move(Node(name))); }
private:
  ESP8266WebServer http_server;
  std::vector<Node> nodes;
};

#endif