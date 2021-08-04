#ifndef HUB_CPP
#define HUB_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "hub.h"
#include "config.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "sensors/ManagedSensor.h"

Hub::Hub():
  server(AsyncWebServer(80))
  {
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();

  Serial.println("ID: ");
  Serial.println(ESP.getChipId());

  if(!this->setupSoftAP(HUB_SSID, HUB_PASSWD)) {
    Serial.println("Error setting up soft AP.");
  } else {
    Serial.println("Soft AP Enabled. IP: ");
    Serial.println(WiFi.softAPIP());
  }

  Serial.print("Attempting connection to '" EXTERNAL_SSID "'");
  this->connectToAP(EXTERNAL_SSID, EXTERNAL_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");

  this->registerEndpoints();
}

void Hub::loop() {
  Serial.print("Num connected: "); Serial.println(WiFi.softAPgetStationNum());
  Serial.print("WIFI Status: ");
  switch(WiFi.status()) {
    case WL_CONNECTED:
      Serial.println("Connected");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("SSID Not Found");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Connection Failed");
      break;
    case WL_WRONG_PASSWORD:
      Serial.println("Wrong Password");
      break;
    case WL_DISCONNECTED:
      Serial.println("Disconnected");
      break;
    default:
      Serial.println(WiFi.status());
  }
  Serial.println("");

  delay(5000);
}

void Hub::registerEndpoints() {
  this->server.on("/data", HTTP_POST, [this](AsyncWebServerRequest* request) {
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){ //p->isPost() is also true
        Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }

    // https://github.com/me-no-dev/ESPAsyncWebServer#get-post-and-file-parameters
    if (request->params() == 3 &&
        request->hasParam("name", true) &&
        request->hasParam("type", true) &&
        request->hasParam("value", true)) {
      AsyncWebParameter* node_name = request->getParam("name", true);
      AsyncWebParameter* type = request->getParam("type", true);
      AsyncWebParameter* value = request->getParam("value", true);

      int i = 0;
      for(; i < this->num_nodes; i++) {
        if (strcmp(this->nodes[i].getNodeName(), node_name->value().c_str()) == 0) {
          break; // Found a match
        }
      }

      // If i == num_nodes, then the node was not found
      if (i == this->num_nodes) {
        // Check to make sure there is room for more nodes
        if (this->num_nodes == MAX_NODES) {
          const char* err = "Cannot add more nodes";
          Serial.println(err);
          request->send(200, "text/plain", err);
          return;
        }
        this->nodes[i].setNodeName(node_name->value().c_str());
        this->num_nodes++;
      }

      // At this point, i is the node were interested in

      int res = this->nodes[i].setData(type->value().c_str(), value->value().c_str());
      if (res == 1) {
        const char* err = "Cannot add more sensors to device";
        request->send(400, "text/plain", err);
        return;
      }

      request->send(200, "text/plain", "OK");
      return;
    }

    String out = "An error has occured:\n";
    if (!request->hasParam("type", true))
      out.concat("Missing 'type' parameter\n");
    if (!request->hasParam("name", true))
      out.concat("Missing 'name' parameter\n");
    request->send(400, "text/plain", out);
  });


  this->server.on("/collect", HTTP_GET, [this](AsyncWebServerRequest* request) {
    // {
    //   "num_devices": "2",
    //   "max_devices": "8",
    //   "nodes": {
    //     "NODE_1234": {
    //       "temperature": "123",
    //       "humidity": "123"
    //     },
    //     "NODE_4321": {
    //       "battery": "123"
    //     }
    //   }
    // }

    // #7 https://cpp4arduino.com/2018/11/21/eight-tips-to-use-the-string-class-efficiently.html
    String out((char*)0);
    out.reserve(128);
    out.concat("{\"num_devices\":\"");
    out.concat(this->num_nodes);
    out.concat("\",");
    out.concat("\"max_devices\":\"");
    out.concat(MAX_NODES);
    out.concat("\",");
    out.concat("\"nodes\": {");

    for(int i = 0; i < this->num_nodes; i++) {
      out.concat("\"");
      out.concat(this->nodes[i].getNodeName());
      out.concat("\":{");

      Node n = this->nodes[i];
      for(int j = 0; j < n.getNumSensors(); j++) {
        out.concat("\"");
        out.concat(n.getData()[j].getType());
        out.concat("\":\"");
        out.concat(n.getData()[j].getValue());
        out.concat("\"");
        if (j < n.getNumSensors() - 1) out.concat(",");
      }

      out.concat("}");
      if (i < this->num_nodes - 1) out.concat(",");
    }

    out.concat("}}");
    request->send(200, "application/json", out);
  });


  // Get stats on the server
  this->server.on("/stats", HTTP_GET, [](AsyncWebServerRequest* request) {
    // {
    //   "Heap Free": "123",
    //   "Heap Fragmentation": 123
    // }
    String out = "{";
    out.concat("\"Heap Free\": \"" + String(ESP.getFreeHeap()) + "\",");
    out.concat("\"Heap Fragmentation\": \"" + String(ESP.getHeapFragmentation()) + "\"");

    out.concat("}");
    request->send(200, "application/json", out);
  });

  this->server.begin();
  Serial.println("API Endpoints Registered.");
}






int Node::setData(const char* type, const char* value) {
  int i = 0;
  for(; i < this->num_sensors; i++) {
    if (strcmp(this->data[i].getType(), type) == 0) {
      break; // The type already exists
    }
  }

  // If it doesnt exist, then add it
  if (i == this->num_sensors) {
    if (i == MAX_SENSORS) {
      const char* err = "Cannot add more sensors";
      Serial.println(err);
      return 1;
    }

    this->data[i].setType(type);
    this->num_sensors++;
  }

  this->data[i].setValue(value);

  return 0;
}


ManagedSensor* Node::getData() {
  return this->data;
}

const int& Node::getNumSensors() {
  return this->num_sensors;
}

#endif