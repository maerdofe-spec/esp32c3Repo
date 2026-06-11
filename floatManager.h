/* 应当有的状态包括: 待机，下水，悬停1，下水2，悬停2，回升，悬停3，回收
  FloatManager应当根据不同的状态发配mission
*/
#ifndef FLOAT_MANAGER_H
#define FLOAT_MANAGER_H

#include "config.h"
#include "missionList.h"
#include "sensorDriver.h"
#include "stepperDriver.h"
#include "mqttLink.h"

class floatManager {
  public:
    enum state {
      IDLE,
      DIVE,
      HOVER1,
      ASCEND,
      HOVER2,
      RECOVERY
    };

    floatManager(sensorDriver *s, stepperDriver *mt, mqttLink *mq): sensor(s), motor(mt), mqtt(mq) {}
    void init();
    void handleCurrentState();

  private:
    sensorDriver* sensor;
    stepperDriver* motor;
    mqttLink* mqtt;

    state currentState;
    unsigned long currentMillis;
};

#endif