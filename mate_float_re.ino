#include "config.h"
#include "floatManager.h"
#include "mqttLink.h"
#include "missionList.h"
#include "stepperDriver.h"
#include "sensorDriver.h"

unsigned long lastMillis;
unsigned long currentMillis;
mqttLink mqtt;
sensorDriver sensor;
stepperDriver stepper;
floatManager manager(sensor, stepper, mqtt);

void setup() {
  sensor.init();
  mqtt.init();
  manager.init();
  lastMillis = millis();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - lastMillis > 20) {
    lastMillis = currentMillis;
    manager.handleCmd();
  }
}
