#ifndef FOLLOWER_H
#define FOLLOWER_D

#include <vector>
#include "controller.h"
#include "sensors/sensor.h"
#include "config.h"

class Follower : public Controller {
public:
  Follower();
  void loop() override;
  void addSensor(Sensor* sensor);
private:
  std::vector<Sensor*> sensors;
};

#endif