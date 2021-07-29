#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
  virtual void loop() = 0;
protected:
  int8_t connectToAP(String SSID, String passwd);
  bool setupSoftAP(String SSID, String passwd);
};

#endif