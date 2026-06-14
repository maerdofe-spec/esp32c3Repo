#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include "lib/MS5837.h"
#include "config.h"

class sensorDriver {
  public:
    sensorDriver();
    void init();
    float getDepth();
    void updateDepth();

  private:
    MS5837 sensor;
    float currentDepth;
};

#endif