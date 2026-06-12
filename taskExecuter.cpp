#include "taskExecuter.h"

#include <math.h>

void taskExecuter::init() {
  depthTimer.resetAndStop();
  hoverTimer.resetAndStop();
}

void taskExecuter::idle() {
  // 在待机状态下，保持机械复位状态，并重置PID控制器
  stepper.posControl(0, 0);
  integral = 0;
  lastDepth = sensor.getDepth();
}

bool taskExecuter::toDepth() {
  float currentDepth = sensor.getDepth();
  float error = goalDepth - currentDepth;
  // 如果计时器还没有初始化，则初始化计时器
  if (!depthTimer.isRunning()) {
    depthTimer.init(TRANSITION_STABLE_DURATION);
  }
  // 如果误差在容许范围内，检查计时器是否达到设定的稳定时间，如果达到则返回true
  if (fabsf(error) <= MAX_DEPTH_OFFSET) {
    if(depthTimer.isReady()) return true;
  }
  // 否则，误差超出容许范围，重置计时器
  else depthTimer.reset();
  // 进行PID控制
  pidControl(currentDepth, error);
  // 返回false表示尚未达到目标深度
  return false;
}


// 逻辑类同toDepth，但使用hoverTimer和HOVER_DURATION，且不会重置计时器，仅检查是否达到稳定时间
bool taskExecuter::hover() {
  float currentDepth = sensor.getDepth();
  float error = goalDepth - currentDepth;
  // 如果计时器还没有初始化，则初始化计时器
  if (!hoverTimer.isRunning()) {
    hoverTimer.init(HOVER_DURATION);
  }
  // 检查计时器是否达到设定的稳定时间，如果达到则返回true
  if (hoverTimer.isReady()) return true;
  // 否则，还没有悬停足够时间，继续进行PID控制
  pidControl(currentDepth, error);
  // 返回false表示尚未悬停足够时间
  return false;
}

bool taskExecuter::recovery() {
  // 待完善回收逻辑，目前先简单地上升到0.1米并认为回收完成
  setGoalDepth(0.1f);
  toDepth();
}

void taskExecuter::pidControl(float currentDepth, float error) {
  float p = kp * error;
  integral += error;
  float i = ki * integral;
  float d = kd * (currentDepth - lastDepth);
  lastDepth = currentDepth;
  uint32_t output = (uint32_t)(p + i + d + feedforward);
  /*
  脉冲数为0时理论上处于最大排水的机械复位状态
  但有可能起始状态非机械复位状态，此时脉冲数可能会是负数，则需要绝对位置反向控制
  */
  output > 0 ? stepper.posControl(0, output) : stepper.posControl(1, -output);
}
