#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <cstdint>
#include "config.h"

class PIDController {
  public:
    PIDController(float kp, float ki, float kd)
      : kp(kp), ki(ki), kd(kd), lastDepth(0.0f), integral(0.0f), feedforward(0) {}

    int32_t pidControl(float currentDepth, float error);
    // 现可用MQTT动态调整PID参数
    void setGains(float newKp, float newKi, float newKd);
    void resetIntegral() { integral = 0.0f; }
    void setFeedforward(float ff) { feedforward = ff; }
    void setLastDepth(float depth) { lastDepth = depth; }
    
  private:
    float kp, ki, kd;
    float lastDepth;
    float integral;
    float feedforward;
};

#endif // PID_CONTROLLER_H