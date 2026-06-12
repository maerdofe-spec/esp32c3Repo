#ifndef EXECUTER_H
#define EXECUTER_H

#include "sensorDriver.h"
#include "stepperDriver.h"
#include "config.h"

class taskExecuter {
  public:
    taskExecuter(sensorDriver &sen, stepperDriver &stp):sensor(sen), stepper(stp) {
      kp = CTRL_KP_DEFAULT;
      ki = CTRL_KI_DEFAULT;
      kd = CTRL_KD_DEFAULT;
    };
    void init();
    void idle();
    void toDepth(float goal);
    void hover(float goal);
    void recovery();
    uint32_t pidOutput(float goal);

  private:
    sensorDriver sensor;
    stepperDriver stepper;
    uint32_t integeral;
    float kp, ki, kd;
};

#endif