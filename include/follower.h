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
private:
  Array<Sensor, MAX_SENSORS> sensors;
};

#endif