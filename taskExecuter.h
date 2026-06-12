#ifndef EXECUTER_H
#define EXECUTER_H

#include "sensorDriver.h"
#include "stepperDriver.h"
#include "config.h"
#include "timeManager.h"

class taskExecuter {
  public:
    taskExecuter(sensorDriver &sen, stepperDriver &stp):sensor(sen), stepper(stp) {
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
    bool toDepth();
    bool hover();
    bool recovery();
    // pid控制函数，接受当前深度和误差作为输入，输出控制信号
    void pidControl(float currentDepth, float error);

  private:
    sensorDriver sensor;
    stepperDriver stepper;
    timeManager depthTimer;
    timeManager hoverTimer;
    uint32_t integral, feedforward;
    float kp, ki, kd, lastDepth, goalDepth;
};

#endif