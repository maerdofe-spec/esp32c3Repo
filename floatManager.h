/* 应当有的状态包括: 待机，下水，悬停1，下水2，悬停2，回升，悬停3，回收
  FloatManager应当根据不同的状态发配mission
*/
#ifndef FLOAT_MANAGER_H
#define FLOAT_MANAGER_H

#include "config.h"
#include "sensorDriver.h"
#include "stepperDriver.h"
#include "mqttLink.h"
#include "taskExecuter.h"
#include "dataRecorder.h"
#include "timeManager.h"
#include "pidController.h"

class floatManager {
  public:
    enum state {
      IDLE,
      DIVE,
      HOVER1,
      ASCEND,
      HOVER2,
      RECOVERY,
      UPLOADING
    };

    floatManager(mqttLink &mq, taskExecuter &exe, dataRecorder &rec, PIDController &p): mqtt(mq), executer(exe), recorder(rec), pid(p) {};
    void init();
    void handleCurrentState();
    void run();
    void handleCmd();

  private:
    bool isFirstTask = true;
    mqttLink &mqtt;
    taskExecuter &executer;
    dataRecorder &recorder;
    PIDController &pid;
    timeManager recorderTimer;
    state currentState;
};

#endif