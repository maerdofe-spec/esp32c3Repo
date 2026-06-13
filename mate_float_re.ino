#include "config.h"
#include "floatManager.h"
#include "taskExecuter.h"
#include "mqttLink.h"
#include "stepperDriver.h"
#include "sensorDriver.h"

unsigned long lastMillis;
unsigned long currentMillis;
mqttLink mqtt;
sensorDriver sensor;
stepperDriver stepper;
taskExecuter executer(sensor, stepper);
floatManager manager(mqtt, executer);

void setup() {
  sensor.init();
  mqtt.init();
  manager.init();
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
