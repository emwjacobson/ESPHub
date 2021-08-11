#ifndef HUB_H
#define HUB_H

#include <Arduino.h>
#include <WebServer.h>
#include <vector>
#include <cstring>
#include "config.h"
#include "controller.h"
#include "sensors/sensor.h"

class Node {
public:
  Node(const char* name) {
    strncpy(this->name, name, 32);
    this->name[32] = 0;
  }
  const char* getName() const { return this->name; }
  const std::vector<Sensor>& getSensors() const { return this->sensors; }
  Sensor& sensorAt(const int& i) { return this->sensors.at(i); }
  void addSensor(const char* type) { this->sensors.push_back(std::move(Sensor(type))); }
private:
  char name[33];
  std::vector<Sensor> sensors;
};



class Hub : public Controller {
public:
  Hub();
  void loop() override;
  const std::vector<Node>& getNodes() const { return this->nodes; }
  Node& nodeAt(const int& i) { return this->nodes.at(i); }
  void addNode(const char* name) { this->nodes.push_back(std::move(Node(name))); }
private:
  WebServer http;
  std::vector<Node> nodes;
};

#endif