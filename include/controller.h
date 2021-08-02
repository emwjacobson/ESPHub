#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
  virtual void loop() = 0;
protected:
  int8_t connectToAP(const char* SSID, const char* passwd);
  bool setupSoftAP(const char* SSID, const char* passwd);
};

#endif