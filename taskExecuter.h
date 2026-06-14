#ifndef EXECUTER_H
#define EXECUTER_H

#include "sensorDriver.h"
#include "stepperDriver.h"
#include "config.h"
#include "timeManager.h"
#include "dataRecorder.h"

class taskExecuter {
  public:
    taskExecuter(sensorDriver &sen, stepperDriver &stp, dataRecorder &rec):sensor(sen), stepper(stp), recorder(rec) {
      kp = CTRL_KP_DEFAULT;
      ki = CTRL_KI_DEFAULT;
      kd = CTRL_KD_DEFAULT;
    };
    void init();
    void idle();
    // 设置与重设变量
    void setFeedforward(float ff) {feedforward = ff;}
    void setGoalDepth(float depth) {goalDepth = depth;}
    void resetIntegral() {integral = 0;}
    // 任务函数，返回值表示是否完成任务
    bool toDepth(timeManager &recorderTimer);
    bool hover(timeManager &recorderTimer);
    bool recovery(timeManager &recorderTimer);
    // pid控制函数，接受当前深度和误差作为输入，输出控制信号
    void pidControl(float currentDepth, float error);

  private:
    sensorDriver &sensor;
    stepperDriver &stepper;
    timeManager hardResetter;
    timeManager depthTimer;
    timeManager hoverTimer;
    timeManager recorderTimer;
    dataRecorder &recorder;
    uint32_t integral, feedforward;
    float kp, ki, kd, lastDepth, goalDepth;
};

#endif