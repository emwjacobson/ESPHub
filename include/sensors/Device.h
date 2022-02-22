#ifndef DEVICE_H
#define DEVICE_H

#include <cstdint>
#include <vector>
#include <string>

struct sensor_data {
  std::string name;
  float data;
};

typedef std::vector<sensor_data> device_data;

class Device {
protected:
  Device() {}
  device_data sensors;
public:
  virtual device_data getDeviceData();
};

#endif