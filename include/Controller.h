#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
  virtual void loop() = 0;
protected:
  void connectToAP(const char* SSID, const char* passwd);
  bool setupSoftAP(const char* SSID, const char* passwd);
};

#endif