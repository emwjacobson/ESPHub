#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
  virtual void loop() = 0;
protected:
  void connectToAP(String SSID, String passwd);
  void setupSoftAP(String SSID, String passwd);
};

#endif