#ifndef MASTER_H
#define MASTER_H

#include <ESPAsyncWebServer.h>
#include <Array.h>
#include "config.h"
#include "controller.h"
#include "Pair.h"

class Master : public Controller {
public:
  Master();
  void loop() override;

  class DataElement {
  public:
    DataElement(): node_name("") {}
    DataElement(String name):
      node_name(name) {}
    String getNodeName() const { return node_name; }
    int setData(String key, String value);
    Array<Pair<String, String>, MAX_SENSORS>::iterator begin() { return this->data.begin(); }
    Array<Pair<String, String>, MAX_SENSORS>::iterator end() { return this->data.end(); }
  private:
    String node_name;
    Array<Pair<String, String>, MAX_SENSORS> data;
  };

private:
  AsyncWebServer server;
  Array<DataElement, MAX_NODES> nodes;

  // Register web server endpoints
  void registerEndpoints();
};

#endif