#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <Wire.h>
#include "lib/MS5837.h"
#include "config.h"

class sensorDriver {
  public:
    float getDepth() const{return currentDepth;};
  
  private:
    MS5837 sensor;
    float currentDepth;
};

#endif