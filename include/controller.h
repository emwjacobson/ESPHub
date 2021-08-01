#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
  virtual void loop() = 0;
protected:
  int8_t connectToAP(const String& SSID, const String& passwd);
  bool setupSoftAP(const String& SSID, const String& passwd);
};

#endif