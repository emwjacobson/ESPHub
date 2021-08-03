#ifndef MPU6050_H
#define MPU6050_H

#include "sensor.h"

class MPU6050 : public Sensor {
public:
  MPU6050() : Sensor("MPU6050") {

  }
};

#endif