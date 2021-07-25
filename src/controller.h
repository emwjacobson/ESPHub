#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
  virtual void setup() = 0;
  virtual void loop() = 0;
};

#endif