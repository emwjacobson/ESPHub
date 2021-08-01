#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
  virtual void loop() = 0;
protected:
  int8_t connectToAP(char* SSID, char* passwd);
  bool setupSoftAP(char* SSID, char* passwd);
};

#endif