#include "config.h"
#include "floatManager.h"
#include "taskExecuter.h"
#include "mqttLink.h"
#include "stepperDriver.h"
#include "sensorDriver.h"
#include "dataRecorder.h"

unsigned long lastMillis;
unsigned long currentMillis;
mqttLink mqtt;
sensorDriver sensor;
stepperDriver stepper;
dataRecorder recorder;
taskExecuter executer(sensor, stepper,recorder);
floatManager manager(mqtt, executer,recorder);

void setup() {
  sensor.init();
  mqtt.init();
  manager.init();
  recorder.init();
  lastMillis = millis();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - lastMillis > 500) {
    lastMillis = currentMillis;
    manager.run(); 
  }
  // delay(500);
}
