#include "pidController.h"

int32_t PIDController::pidControl(float currentDepth, float error) {
  float p = kp * error;
  integral += error;
  float i = ki * integral;
  float d = kd * (currentDepth - lastDepth);
  lastDepth = currentDepth;
  float raw = p + i + d + feedforward;
  // 最大值限位
  const float MAX_OUT = CLK_MAX; // 根据步进电机的最大脉冲数设置
  if (raw > MAX_OUT) raw = MAX_OUT;
  if (raw < -MAX_OUT) raw = -MAX_OUT;
  return (int32_t)raw;
}

void PIDController::setGains(float newKp, float newKi, float newKd) {
      kp = newKp;
      ki = newKi;
      kd = newKd;
    }