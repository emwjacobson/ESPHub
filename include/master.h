#ifndef MASTER_H
#define MASTER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Array.h>
#include "config.h"
#include "controller.h"
#include "sensors/ManagedSensor.h"

class Master : public Controller {
public:
  Master();
  void loop() override;

  class Node {
  public:
    Node() {
      this->node_name[0] = 0;
    }
    Node(const char* name) {
      Serial.println("Constructor start");
      int len = strlen(name);
      len = len > NAME_BUFFER ? NAME_BUFFER : len;
      memcpy(this->node_name, name, len);
      this->node_name[len] = 0;
      Serial.println(this->node_name);
      Serial.println("Constructor end");
    }
    const char* getNodeName() const { return node_name; }
    int setData(const char* key, const char* value);
    const Array<ManagedSensor, MAX_SENSORS>& getData() const;
  private:
    char node_name[NAME_BUFFER + 1];
    Array<ManagedSensor, MAX_SENSORS> data;
  };

private:
  AsyncWebServer server;
  Array<Node, MAX_NODES> nodes;

  // Register web server endpoints
  void registerEndpoints();
};

#endif