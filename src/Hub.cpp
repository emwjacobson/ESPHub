#ifndef HUB_CPP
#define HUB_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "Hub.h"
#include "config.h"
#include "Wire.h"
#include "sensors/MultiSensor.h"

Hub::Hub() : http_server(80) {
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
  Serial.print("\nConnected! IP: ");
  Serial.println(WiFi.localIP());

  Wire.begin();

  this->registerSensors();

  // Used to push data to the Hub node
  this->http_server.on("/data", [this]() {
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

    if (this->http_server.method() != HTTPMethod::HTTP_POST) {
      this->http_server.send(405, "text/plain");
      return;
    }

    // Devices reporting over 8 sensors will start to run into problems
    const int doc_size = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(8) + JSON_OBJECT_SIZE(8) + 256;
    DynamicJsonDocument doc(doc_size);

    DeserializationError err = deserializeJson(doc, this->http_server.arg("plain"));

    if (err) {
      Serial.print("Error: ");
      Serial.println(err.f_str());
      this->http_server.send(400, "text/plain");
      doc.garbageCollect();
      doc.clear();
      return;
    }

    if (doc["name"] == nullptr) {
      this->http_server.send(400, "text/plain");
      doc.garbageCollect();
      doc.clear();
      return;
    }

    if (doc["sensors"].size() <= 0) {
      Serial.println("Sensors is empty, don't do anything");
      this->http_server.send(304, "text/plain");
      doc.garbageCollect();
      doc.clear();
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
        this->http_server.send(400, "text/plain");
        doc.garbageCollect();
        doc.clear();
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

    this->http_server.send(200, "text/plain");

    doc.garbageCollect();
    doc.clear();
  });

  // Used to collect data from the Hub node
  this->http_server.on("/collect", [this]() {
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

    if (this->http_server.method() != HTTPMethod::HTTP_GET) {
      this->http_server.send(405, "text/plain");
      return;
    }

    // Calculate how much space is needed for the JSON Document
    int num_nodes = this->getNodes().size();
    int num_sensors = 0;
    for (Node& n : this->getNodes()) {
      num_sensors += n.getSensors().size();
    }

    // Count self
    num_nodes++;
    num_sensors += this->sensors.size();

    const size_t doc_size = JSON_OBJECT_SIZE(num_nodes + num_sensors + 1) + 128;
    DynamicJsonDocument doc(doc_size);
    JsonObject nodes = doc.createNestedObject("nodes");

    bool error_encountered = false;

    // Add data from sensors connected to the Hub
    JsonObject node = nodes.createNestedObject(NODE_NAME);
    for (Sensor* s : this->sensors) {
      if (strcmp(s->getValue(), "multi") == 0) {
        MultiSensor* ms = (MultiSensor*)s;
        for (int i = 0; i < ms->getNumSensors(); i++) {
          char type[33];
          strncpy(type, ms->getType(i), 32); type[32] = 0;
          char val[33];
          strncpy(val, ms->getValue(i), 32); val[32] = 0;
          Serial.print(type);
          Serial.print("=");
          Serial.println(val);
          node[type] = val;
          if (strncmp(val, "error", 32) == 0) {
            error_encountered = true;
          }
        }
      } else {
        char type[33];
        strncpy(type, s->getType(), 32); type[32] = 0;
        char val[33];
        strncpy(val, s->getValue(), 32); val[32] = 0;
        Serial.print(type);
        Serial.print("=");
        Serial.println(val);
        node[type] = val;
        if (strncmp(val, "error", 32) == 0) {
          error_encountered = true;
        }
      }
    }

    Serial.println();

    // Add data for Follower nodes
    for (Node& n : this->getNodes()) {
      node = nodes.createNestedObject(n.getName());
      for (ManagedSensor& s : n.getSensors()) {
        node[s.getType()] = s.getValue();
      }
    }

    size_t json_size = measureJson(doc);
    char buffer[json_size];
    serializeJson(doc, buffer, json_size);

    this->http_server.send(200, "application/json", buffer, json_size);
    doc.garbageCollect();
    doc.clear();

    if (error_encountered) { // If were getting errors, attempt to restart the device.
      Serial.println("Errors encountered, restarting device...");
      ESP.restart();
    }
  });

  // Used to get information on the hub node itself
  this->http_server.on("/info", [this]() {
    // Response will be in this form:
    // {
    //   "Num Nodes": ##,
    //   "Free Heap": ##,
    //   "Heap Fragmentation": ##
    // }

    if (this->http_server.method() != HTTPMethod::HTTP_GET) {
      this->http_server.send(405, "text/plain");
      return;
    }

    const size_t doc_size = JSON_OBJECT_SIZE(4); // The main element + 2 elements
    DynamicJsonDocument doc(doc_size);

    doc["Num Nodes"] = this->getNodes().size();
    doc["Free Heap"] = ESP.getFreeHeap();
    doc["Heap Fragmentation"] = ESP.getHeapFragmentation();

    size_t json_size = measureJson(doc);
    char buffer[json_size];
    serializeJson(doc, buffer, json_size);

    this->http_server.send(200, "application/json", buffer, json_size);
    doc.garbageCollect();
    doc.clear();
  });

  this->http_server.begin();
  Serial.println("HTTP Server Started!");
}

void Hub::loop() {
  // Handle clients
  this->http_server.handleClient();
  delayMicroseconds(500);
}

#endif