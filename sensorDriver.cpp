#include "sensorDriver.h"

sensorDriver::sensorDriver()
    : currentDepth(0.0f) {}

void sensorDriver::init() {}

float sensorDriver::getDepth() const {
  return currentDepth;
}

void sensorDriver::updateDepth() {
  // Implementation for updating depth
}