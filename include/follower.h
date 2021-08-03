#ifndef FOLLOWER_H
#define FOLLOWER_D

#include <Array.h>
#include "controller.h"
#include "sensors/sensor.h"
#include "config.h"

class Follower : public Controller {
public:
  Follower();
  void loop() override;
  void addSensor(Sensor* sensor);
private:
  Sensor* sensors[MAX_SENSORS];
  int num_sensors;
};

#endif