#ifndef EXECUTER_H
#define EXECUTER_H

#include "sensorDriver.h"
#include "stepperDriver.h"

class taskExecuter {
  public:
    taskExecuter(sensorDriver &sen, stepperDriver &stp):sensor(sen), stepper(stp) {};
    void init();
    void idle();
    void toDepth(float goal);
    void hover(float goal);
    void recovery();
    uint32_t pidOutput();

  private:
    sensorDriver sensor;
    stepperDriver stepper;
};

#endif