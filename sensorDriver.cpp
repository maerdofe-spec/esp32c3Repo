#include "sensorDriver.h"

sensorDriver::sensorDriver()
    : currentDepth(0.0f) {}

void sensorDriver::init() {}

float sensorDriver::getDepth() const {
  //return currentDepth;
  //测试用
  return 0.0f;
}

void sensorDriver::updateDepth() {
  // Implementation for updating depth
}