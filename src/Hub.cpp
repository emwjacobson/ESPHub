#ifndef HUB_CPP
#define HUB_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "Hub.h"
#include "config.h"
#include "sensors/ManagedSensor.h"

Hub::Hub() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();

  // Setup softAP and connect to main WiFI
  this->setupSoftAP(HUB_SSID, HUB_PASSWD);
  this->connectToAP(EXTERNAL_SSID, EXTERNAL_PASSWD);

  // Wait for the wifi to actually connect
  Serial.print("Connecting to WiFi '" EXTERNAL_SSID "'");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");

  Serial.println("Starting server...");
  this->http.begin();

  // Used to push data to the Hub node
  this->http.registerEndpoint("/data", HTTP_METHOD::POST, [this](Request& req) {
    // Input data should be JSON in this form
    // {
    //   "name": "Bedroom",
    //   "sensors": [
    //     {
    //       "type": "temperature",
    //       "value": "68"
    //     },
    //     {
    //       "type": "humidity",
    //       "value": "70"
    //     },
    //     ...
    //   ]
    // }

    // Devices reporting over 8 sensors will start to run into problems
    const int doc_size = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(8) + JSON_OBJECT_SIZE(8);
    DynamicJsonDocument doc(doc_size);

    DeserializationError err = deserializeJson(doc, req.getBody());

    if (err) {
      Serial.print("Error: ");
      Serial.println(err.f_str());
      req.send(400);
      return;
    }

    if (doc["name"] == nullptr) {
      req.send(400);
      return;
    }

    if (doc["sensors"].size() <= 0) {
      Serial.println("Sensors is empty, don't do anything");
      req.send(304);
      return;
    }

    bool node_found = false;
    size_t node_index;
    for (node_index = 0; node_index < this->getNodes().size(); node_index++) {
      if (this->nodeAt(node_index).getName() == doc["name"]){
        Serial.println("Node Matched!");
        node_found = true;
        break;
      }
    }

    if (!node_found) {
      this->addNode(doc["name"]);
      Serial.println("Added new node!");
    }

    // The node at position `node_index` is the node that we are interested in
    size_t sensor_index;
    bool sensor_found = false;
    Node& n = this->nodeAt(node_index);
    for (const JsonObject& s : doc["sensors"].as<JsonArray>()) {
      if (s["type"] == nullptr || s["value"] == nullptr) {
        req.send(400);
        return;
      }

      for (sensor_index = 0; sensor_index < n.getSensors().size(); sensor_index++) {
        if (n.sensorAt(sensor_index).getType() == s["type"]) {
          Serial.println("Sensor Matched!");
          sensor_found = true;
          break;
        }
      }

      if (!sensor_found) {
        n.addSensor(s["type"]);
        Serial.println("Added new sensor!");
      }

      n.sensorAt(sensor_index).setValue(s["value"]);
    }

    req.send(200);
  });

  // Used to collect data from the Hub node
  this->http.registerEndpoint("/collect", HTTP_METHOD::GET, [this](Request& req) {
    // Returns a JSON object in this form:
    // {
    //   "nodes": {
    //     "Bedroom": {
    //       "temperature": "82",
    //       "humidity": "68"
    //     },
    //     "Kitchen": {
    //       "temperature": "100",
    //       "humidity": "70"
    //     },
    //     "Barn": {
    //       "co2": "3000"
    //     }
    //   }
    // }

    // Calculate how much space is needed for the JSON Document

    int num_nodes = this->getNodes().size();
    int num_sensors = 0;
    for (Node& n : this->getNodes()) {
      num_sensors += n.getSensors().size();
    }

    const size_t doc_size = JSON_OBJECT_SIZE(num_nodes + num_sensors + 1);
    DynamicJsonDocument doc(doc_size);

    JsonObject nodes = doc.createNestedObject("nodes");

    for (Node& n : this->getNodes()) {
      JsonObject node = nodes.createNestedObject(n.getName());
      for (ManagedSensor& s : n.getSensors()) {
        node[s.getType()] = s.getValue();
      }
    }

    size_t json_size = measureJson(doc);
    char buffer[json_size];
    serializeJson(doc, buffer, json_size);

    req.setHeader("Content-Type", "application/json");
    req.send(200, buffer, json_size);
  });

  // Used to get information on the hub node itself
  this->http.registerEndpoint("/info", HTTP_METHOD::GET, [this](Request& req) {
    // Response will be in this form:
    // {
    //   "Num Nodes": ##,
    //   "Free Heap": ##,
    //   "Heap Fragmentation": ##
    // }

    const size_t doc_size = JSON_OBJECT_SIZE(4); // The main element + 2 elements
    DynamicJsonDocument doc(doc_size);

    doc["Num Nodes"] = this->getNodes().size();
    doc["Free Heap"] = ESP.getFreeHeap();
    doc["Heap Fragmentation"] = ESP.getHeapFragmentation();

    size_t json_size = measureJson(doc);
    char buffer[json_size];
    serializeJson(doc, buffer, json_size);

    req.setHeader("Content-Type", "application/json");

    req.send(200, buffer, json_size);
  });
}

void Hub::loop() {
  // Handle clients
  this->http.handleClients();
}

#endif