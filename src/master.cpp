#ifndef MASTER_CPP
#define MASTER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "master.h"
#include "config.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "Pair.h"

Master::Master():
  server(AsyncWebServer(80))
  {
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();

  Serial.println("ID: " + String(ESP.getChipId()));

  if(!this->setupSoftAP(HUB_SSID, HUB_PASSWD)) {
    Serial.println("Error setting up soft AP.");
  } else {
    Serial.println("Soft AP Enabled. IP: " + WiFi.softAPIP().toString());
  }

  Serial.print("Attempting connection to '" + String(EXTERNAL_SSID) + "'");
  this->connectToAP(EXTERNAL_SSID, EXTERNAL_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");

  this->registerEndpoints();
}

void Master::loop() {
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

void Master::registerEndpoints() {
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

      // Check if it exists
      auto it = this->nodes.begin();
      for(; it != this->nodes.end(); ++it) {
        if ((*it).getNodeName().equals(node_name->value())) break;
      }

      // If node is not already in the list then add it
      if (it == this->nodes.end()) {
        // If nodes are full, then error out
        if (this->nodes.full()) {
          Serial.println("Unable to add new node, array is full.");
          request->send(400, "text/plain", "Error: Maximum number of nodes added.");
          return;
        }
        this->nodes.push_back(DataElement(node_name->value()));
      }

      // At this point (*it) should be the node were looking for
      int res = (*it).setData(type->value(), value->value());
      if (res == 1) {
        request->send(400, "text/plain", "Error: Maximum number of sensors added to " + (*it).getNodeName());
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
    String out = "{\"num_devices\":\"" + String(this->nodes.size()) + "\",";
    out.concat("\"max_devices\":\"" + String(this->nodes.max_size()) + "\",");
    out.concat("\"nodes\": {");

    for (auto node_it = this->nodes.begin(); node_it != this->nodes.end(); ++node_it) {

      if (node_it != this->nodes.begin()) out.concat(",");
      out.concat("\"" + (*node_it).getNodeName() + "\":{");

      for (auto data_it = (*node_it).begin(); data_it != (*node_it).end(); ++data_it) {
        if (data_it != (*node_it).begin()) out.concat(",");
        out.concat("\"" + (*data_it).first + "\":\"" + (*data_it).second + "\"");
      }
      out.concat("}");

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

int Master::DataElement::setData(String key, String value) {
    auto it = this->data.begin();
    for(; it != this->data.end(); ++it) {
      if ((*it).first.equals(key)) break;
    }

    // If data is not already in the list then add it
    if (it == this->data.end()) {
      if (this->data.full()) {
        Serial.println("Unable to add new sensor data, array is full.");
        return 1;
      }
      this->data.push_back(Pair<String, String>{key, value});
    }

    (*it).second = value;

    return 0;
}

#endif