#ifndef HUB_CPP
#define HUB_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "hub.h"
#include "config.h"

Hub::Hub(): server(80)
{
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
  this->server.begin();
}

void Hub::loop() {
  // Handle clients
  WiFiClient client = this->server.available();
  if (client) {
    Serial.println("\n\nNew Client");
    if (client.connected()) {
      delay(20);
      this->handleClient(client);
    }
    client.stop();
  }
}

void Hub::handleClient(WiFiClient& client) {
  // while (client.available()) {
    // Serial.print((char)client.read());
  // }

  // POST / HTTP/1.1
  // User-Agent: PostmanRuntime/7.28.2
  // Accept: */*
  // Host: 192.168.1.23
  // Accept-Encoding: gzip, deflate, br
  // Connection: keep-alive
  // Content-Type: application/x-www-form-urlencoded
  // Content-Length: 21
  //
  // key=value&key2=value2

  int n;

  // Read the HTTP method. GET, POST, etc.
  char method[9];
  n = client.readBytesUntil(' ', method, 8);
  method[n] = 0;
  Serial.println(method);

  while ((char)client.read() != '\n') delay(1); // Discard rest of line

  // Header Parsing

  char header_key[65];
  char header_value[65];
  while (true) {
    n = client.readBytesUntil(':', header_key, 64);
    while ((char)client.peek() == ' ') client.read(); // Take care of whitespace after colon if it exists
    header_key[n] = 0;
    n = client.readBytesUntil('\n', header_value, 64);
    header_value[n] = 0;

    Serial.print(header_key);
    Serial.print(" = ");
    Serial.println(header_value);

    if (client.peekAvailable()) {
      if ((char)client.peek() == '\n') {
        break;
      }
    }
  }

  // Body Parsing
  Serial.println("TODO: Body parsing");

  client.stop();
}

#endif