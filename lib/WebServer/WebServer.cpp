#ifndef WEBSERVER_CPP
#define WEBSERVER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "WebServer.h"

WebServer::WebServer() : server(80), num_endpoints(0) {}

void WebServer::begin() {
  this->server.begin();
}

void WebServer::handleClients() {
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

bool WebServer::registerEndpoint(const char* endpoint, const HTTP_METHOD& method, CallbackFunction&& onCalled) {
  if (this->num_endpoints == NUM_CALLBACKS) {
    return false;
  }

  strncpy(this->endpoints_callbacks[num_endpoints].first, endpoint, CALLBACK_BUFFER);
  this->endpoints_callbacks[num_endpoints].second = std::move(std::pair<HTTP_METHOD, CallbackFunction>{method, std::move(onCalled)});

  this->num_endpoints++;
  return true;
}

void WebServer::handleClient(WiFiClient& client) {
  // while (client.available()) {
    // Serial.print((char)client.read());
  // }

  // POST /endpoint HTTP/1.1\r\n
  // User-Agent: PostmanRuntime/7.28.2\r\n
  // Accept: */*\r\n
  // Host: 192.168.1.23\r\n
  // Accept-Encoding: gzip, deflate, br\r\n
  // Connection: keep-alive\r\n
  // Content-Type: application/x-www-form-urlencoded\r\n
  // Content-Length: 21\r\n
  // \r\n
  // key=value&key2=value2

  int n;
  char* body_buffer = nullptr;
  int body_size = 0;
  HTTP_METHOD method;

  // Read the HTTP method. GET, POST, etc.
  char m_buffer[9];
  n = client.readBytesUntil(' ', m_buffer, 8);
  m_buffer[n] = 0;
  Serial.println(m_buffer);

  if (strcmp(m_buffer, "GET") == 0) {
    method = GET;
  } else if (strcmp(m_buffer, "POST") == 0) {
    method = POST;
  } else {
    method = UNIMPLEMENTED;
  }

  // Read endpoint
  char endpoint[33]; endpoint[32] = 0;
  n = client.readBytesUntil(' ', endpoint, 32);
  endpoint[n] = 0;
  Serial.print("Endpoint: ");
  Serial.println(endpoint);

  while ((char)client.peek() != '\n') Serial.print((char)client.read());
  // while ((char)client.read() != '\n') delay(1); // Discard rest of line

  // Header Parsing
  char header_key[65]; header_key[64] = 0;
  char header_value[65]; header_value[64] = 0;
  while (true) {
    n = client.readBytesUntil(':', header_key, 64);
    while ((char)client.peek() == ' ') client.read(); // Take care of whitespace after colon if it exists
    header_key[n] = 0;
    n = client.readBytesUntil('\n', header_value, 64);
    if (header_value[n-1] == '\r') { // Handle \r\n
      header_value[n-1] = 0;
    } else {
      header_value[n] = 0;
    }

    if (strcmp(header_key, "Content-Length") == 0 && method == POST) {
      if (body_buffer != nullptr) delete[] body_buffer; // Handle multiple Content-Length headers
      int length = atoi(header_value);
      body_buffer = new char[length + 1];
      body_buffer[length] = 0;
      body_size = atoi(header_value);
    }

    Serial.print(header_key);
    Serial.print(" = ");
    Serial.println(header_value);

    if (client.peekAvailable()) {
      if ((char)client.peek() == '\r') { // Handle \r\n
        client.peekConsume(2);
        break;
      } else if ((char)client.peek() == '\n') { // Handle \n
        client.peekConsume(1);
        break;
      }
    }
  }

  // Body Parsing
  if (method == POST) {
    client.readBytes(body_buffer, body_size);
    Serial.println(body_buffer);
  }

  this->handleEndpoints(endpoint, method, body_buffer, body_size, client);

  delete[] body_buffer;
  client.stop();
}

void WebServer::handleEndpoints(const char* endpoint, const HTTP_METHOD& method, const char* body_buffer, const int& body_size, WiFiClient& client) {
  for (int i = 0; i<this->num_endpoints; i++) {
    if (strncmp(this->endpoints_callbacks[i].first, endpoint, CALLBACK_BUFFER) == 0) {
      if (this->endpoints_callbacks[i].second.first == method) {
        (this->endpoints_callbacks[i].second.second)(std::move(Request(client)));
        return;
      }
    }
  }
  Serial.println("No endpoint matched, fallback to 404?");
  Request(client).send(404, nullptr);
}




const char* Request::getResponseCode(const int& code) {
  switch (code) {
    case 100: return "Continue";
    case 101: return "Switching Protocols";
    case 200: return "OK";
    case 201: return "Created";
    case 202: return "Accepted";
    case 203: return "Non-Authoritative Information";
    case 204: return "No Content";
    case 205: return "Reset Content";
    case 206: return "Partial Content";
    case 300: return "Multiple Choices";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 303: return "See Other";
    case 304: return "Not Modified";
    case 305: return "Use Proxy";
    case 307: return "Temporary Redirect";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 402: return "Payment Required";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 406: return "Not Acceptable";
    case 407: return "Proxy Authentication Required";
    case 408: return "Request Time-out";
    case 409: return "Conflict";
    case 410: return "Gone";
    case 411: return "Length Required";
    case 412: return "Precondition Failed";
    case 413: return "Request Entity Too Large";
    case 414: return "Request-URI Too Large";
    case 415: return "Unsupported Media Type";
    case 416: return "Requested range not satisfiable";
    case 417: return "Expectation Failed";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 502: return "Bad Gateway";
    case 503: return "Service Unavailable";
    case 504: return "Gateway Time-out";
    case 505: return "HTTP Version not supported";
    default:  return "";
  }
}

#endif