#include "sensorDriver.h"
#include "lib/MS5837.cpp"

sensorDriver::sensorDriver()
    : currentDepth(0.0f) {}

void sensorDriver::init() {
  Wire.begin(PIN_SDA, PIN_SCL);
  sensor.init(Wire);
}

float sensorDriver::getDepth() {
  updateDepth();
  return currentDepth;
  // // 测试用
  // return 0.0f;
}

void sensorDriver::updateDepth() {
  sensor.read();
  currentDepth = sensor.depth();
}