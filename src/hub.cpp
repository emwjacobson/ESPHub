#ifndef HUB_CPP
#define HUB_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "hub.h"
#include "config.h"

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
  this->http.registerEndpoint("/data", HTTP_METHOD::POST, [](Request& req) {
    // Input data should be JSON in this form
    // {
    //   "name": "Bedroom",
    //   "num_sensors": N,
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

    int doc_size = req.getBodySize();
    DynamicJsonDocument doc(doc_size < 200 ? doc_size + 32 : doc_size);

    DeserializationError err = deserializeJson(doc, req.getBody());

    if (err) {
      Serial.print("Error: ");
      Serial.println(err.f_str());
      return;
    }

    // Serial.println(req.getParams());
    // Serial.println(req.getBody());
    req.send(200);
  });

  // Used to collect data from the Hub node
  this->http.registerEndpoint("/collect", HTTP_METHOD::GET, [](Request& req) {
    // Returns a JSON object in this form:
    // {
    //   "num_nodes": 3,
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
    req.send(501);
  });

  // Used to get information on the hub node itself
  this->http.registerEndpoint("/info", HTTP_METHOD::GET, [](Request& req) {
    req.send(501);
  });
}

void Hub::loop() {
  // Handle clients
  this->http.handleClients();
}

#endif