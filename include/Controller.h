#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <sensors/Sensor.h>

class Controller {
public:
  virtual void loop() = 0;
protected:
  std::vector<Sensor*> sensors;

  void connectToAP(const char* SSID, const char* passwd);
  bool setupSoftAP(const char* SSID, const char* passwd);
  void addSensor(Sensor* sensor);
};

#endif