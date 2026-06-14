#ifndef EXECUTER_H
#define EXECUTER_H

#include "sensorDriver.h"
#include "stepperDriver.h"
#include "config.h"
#include "timeManager.h"
#include "dataRecorder.h"
#include "pidController.h"

class taskExecuter {
  public:
    taskExecuter(sensorDriver &sen, stepperDriver &stp, dataRecorder &rec, PIDController &pidCtrl)
      :sensor(sen), stepper(stp), recorder(rec), pid(pidCtrl) {};
    void init();
    void idle();
    // 设置变量
    void setGoalDepth(float depth) {goalDepth = depth;}
    // 任务函数，返回值表示是否完成任务
    bool toDepth(timeManager &recorderTimer);
    bool hover(timeManager &recorderTimer);
    bool recovery(timeManager &recorderTimer);
    // pid控制函数，接受当前深度和误差作为输入，输出控制信号
    void Control(float currentDepth, float error);

  private:
    sensorDriver &sensor;
    stepperDriver &stepper;
    timeManager hardResetter;
    timeManager depthTimer;
    timeManager hoverTimer;
    timeManager recorderTimer;
    dataRecorder &recorder;
    PIDController &pid;

    float goalDepth;
    uint32_t startTime = 0;
};

#endif