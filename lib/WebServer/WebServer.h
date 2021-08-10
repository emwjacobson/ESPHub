#ifndef WEBSERVER_H
#define WEBSERVER_H

#define CALLBACK_BUFFER 16
#define NUM_CALLBACKS 10

#define ENDPOINT_BUFFER 16
#define PARAMS_BUFFER 64

enum HTTP_METHOD {
  GET = 0,
  POST,
  UNIMPLEMENTED
};

class Request {
public:
  Request(WiFiClient client,
          const char* params,
          const int& params_size,
          const char* body,
          const int& body_size): client(client), m_isSent(false) {
    this->params = params;
    this->body = body;
  }

  void send(const int& code) {
    this->send(code, nullptr, 0);
  }

  void send(const int& code, const char* body, const int& body_size) {
    const int buffer_size = 300;
    char buffer[buffer_size + 1];
    buffer[buffer_size] = 0;

    // Start Line
    snprintf(buffer, buffer_size, "HTTP/1.1 %i %s", code, this->getResponseCode(code));
    this->client.println(buffer);

    // Headers
    snprintf(buffer, buffer_size, "Connection: close");
    this->client.println(buffer);

    if (body != nullptr) {
      snprintf(buffer, buffer_size, "Content-Length: %i", body_size);
    }

    this->client.write("\n"); // Header-to-body seperator

    // Body
    if (body != nullptr) {
      this->client.write(body, body_size); // Don't append a newline
    }

    this->m_isSent = true;
  }

  // Gets the raw parameter string, eg `key1=value1&key2=value2`
  const char* getParams() const { return this->params; }

  // Gets the raw body string, eg `somekey=somevalue&otherkey=othervalue`
  const char* getBody() const { return this->body; }

  // Determines if the request has been sent
  const bool& isSent() const { return this->m_isSent; }
private:
  WiFiClient client;
  const char* params;
  const char* body;
  bool m_isSent;


  const char* getResponseCode(const int& code);
};





class WebServer {
public:
  typedef std::function<void(Request&)> CallbackFunction;

  WebServer();
  void begin();
  void handleClients();
  bool registerEndpoint(const char* endpoint, const HTTP_METHOD& method, CallbackFunction&& onCalled);
private:
  WiFiServer server;
  std::pair<char[CALLBACK_BUFFER], std::pair<HTTP_METHOD, CallbackFunction>> endpoints_callbacks[NUM_CALLBACKS];

  int num_endpoints;

  void handleClient(WiFiClient& client);
  void handleEndpoints(const char* endpoint, const HTTP_METHOD& method, const char* params,
            const int& params_size, const char* body, const int& body_size, WiFiClient& client);
  const char* getResponseCode(const int& code);
};

#endif