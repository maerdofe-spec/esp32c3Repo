#include "pidController.h"

int32_t PIDController::pidControl(float currentDepth, float error) {
  float p = kp * error;
  integral += error;
  float i = ki * integral;
  float d = kd * (currentDepth - lastDepth);
  lastDepth = currentDepth;
  float raw = p + i + d + feedforward;
  // clamp to reasonable actuator range
  const float MAX_OUT = 1e6f;
  if (raw > MAX_OUT) raw = MAX_OUT;
  if (raw < -MAX_OUT) raw = -MAX_OUT;
  return (int32_t)raw;
}

void PIDController::setGains(float newKp, float newKi, float newKd) {
      kp = newKp;
      ki = newKi;
      kd = newKd;
    }