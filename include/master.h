#ifndef MASTER_H
#define MASTER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Array.h>
#include "config.h"
#include "controller.h"
#include "sensors/ManagedSensor.h"


class Node {
public:
  Node() {
    this->node_name[0] = 0;
  }
  void setNodeName(const char* name) {
    int len = strlen(name);
    len = len > NAME_BUFFER ? NAME_BUFFER : len;
    memcpy(this->node_name, name, len);
  }
  const char* getNodeName() const { return node_name; }
  int setData(const char* key, const char* value);
  const ManagedSensor* getData() const;
  const int& getNumSensors();
private:
  char node_name[NAME_BUFFER + 1];
  ManagedSensor data[MAX_SENSORS];
  int num_sensors;
};



class Master : public Controller {
public:
  Master();
  void loop() override;
private:
  AsyncWebServer server;
  Node nodes[MAX_NODES];
  int num_nodes;

  // Register web server endpoints
  void registerEndpoints();
};

#endif