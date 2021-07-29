#ifndef FOLLOWER_H
#define FOLLOWER_D

#include "controller.h"

class Follower : public Controller {
public:
  Follower();
  void loop() override;
private:
};

#endif