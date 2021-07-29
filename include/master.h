#ifndef MASTER_H
#define MASTER_H

#include <ESPAsyncWebServer.h>
#include "controller.h"
#include "Pair.h"

class Master : public Controller {
public:
  Master();
  void loop() override;
private:
  typedef Pair<String, String> ItemType;

  AsyncWebServer server;
  LinkedList<ItemType> data;

  // Register web server endpoints
  void registerEndpoints();
};

#endif