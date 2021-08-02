#ifndef MASTER_H
#define MASTER_H

#include <ESPAsyncWebServer.h>
#include <Array.h>
#include "config.h"
#include "controller.h"
#include "sensors/sensor.h"

class Master : public Controller {
public:
  Master();
  void loop() override;

  class DataElement {
  public:
    DataElement() {
      this->node_name[0] = 0;
    }
    DataElement(const char* name) {
      int len = strlen(name);
      len = len > NAME_BUFFER ? NAME_BUFFER : len;
      memcpy(this->node_name, name, len);
      this->node_name[len] = 0;
    }
    const char* getNodeName() const { return *node_name; }
    int setData(const char* key, const char* value);
    const Array<Sensor, MAX_SENSORS>& getData() const;
  private:
    char* node_name[NAME_BUFFER + 1];
    Array<Sensor, MAX_SENSORS> data;
  };

private:
  AsyncWebServer server;
  Array<DataElement, MAX_NODES> nodes;

  // Register web server endpoints
  void registerEndpoints();
};

#endif