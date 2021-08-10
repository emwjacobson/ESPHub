#ifndef HUB_CPP
#define HUB_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
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
    Serial.println(req.getParams());
    Serial.println(req.getBody());
    Serial.println("Called Data!");
    req.send(200);
  });

  this->http.registerEndpoint("/collect", HTTP_METHOD::GET, [](Request& req) {
    req.send(501);
  });

  this->http.registerEndpoint("/info", HTTP_METHOD::GET, [](Request& req) {
    req.send(501);
  });
}

void Hub::loop() {
  // Handle clients
  this->http.handleClients();
}

#endif