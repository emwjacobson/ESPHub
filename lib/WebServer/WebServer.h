#ifndef WEBSERVER_H
#define WEBSERVER_H

enum HTTP_METHOD {
  GET = 0,
  POST,
  UNIMPLEMENTED
};

class WebServer {
public:
  WebServer();
  void begin();
  void handleClients();
private:
  WiFiServer server;

  void handleClient(WiFiClient& client);
};

#endif