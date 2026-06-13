#include "sensorDriver.h"

sensorDriver::sensorDriver()
    : currentDepth(0.0f) {}

void sensorDriver::init() {}

float sensorDriver::getDepth() const {
  updateDepth();
  //还需加入深度误差检测
  return currentDepth;
}

void sensorDriver::updateDepth() {
  // Implementation for updating depth
}