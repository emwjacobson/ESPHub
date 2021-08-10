#ifndef HUB_H
#define HUB_H

#include <Arduino.h>
#include <WebServer.h>
#include <vector>
#include <cstring>
#include "config.h"
#include "controller.h"

class Sensor {
public:
  Sensor(const char* type) {
    strncpy(this->type, type, 32); this->type[32] = 0;
    memset(this->value, 0, 33);
  }
  const char* getType() const { return this->type; }
  const char* getValue() const { return this->value; }
  void setValue(const char* new_value) {
    strncpy(this->value, new_value, 32);
    this->value[32] = 0;
  }
private:
  char type[33];
  char value[33];
};

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