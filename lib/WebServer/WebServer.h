#ifndef WEBSERVER_H
#define WEBSERVER_H

#define CALLBACK_BUFFER 16
#define NUM_CALLBACKS 10

enum HTTP_METHOD {
  GET = 0,
  POST,
  UNIMPLEMENTED
};

class Request {
public:
  Request(WiFiClient client): client(client) {}
  void send() {
    Serial.println("204ing...");
    this->client.write("HTTP/1.1 204 No Content\n\n");
  }
private:
  WiFiClient client;
};

class WebServer {
public:
  typedef std::function<void(const Request&)> CallbackFunction;

  WebServer();
  void begin();
  void handleClients();
  bool registerEndpoint(const char* endpoint, const HTTP_METHOD& method, CallbackFunction&& onCalled);
private:
  WiFiServer server;
  std::pair<char[CALLBACK_BUFFER], std::pair<HTTP_METHOD, CallbackFunction>> endpoints_callbacks[NUM_CALLBACKS];

  int num_endpoints;

  void handleClient(WiFiClient& client);
  void handleEndpoints(const char* endpoint, const HTTP_METHOD& method, const char* body_buffer, const int& body_size, WiFiClient& client);
};

#endif