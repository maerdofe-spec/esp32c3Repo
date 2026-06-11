#include "config.h"
#include "floatManager.h"
#include "mqttLink.h"
#include "missionList.h"
#include "stepperDriver.h"
#include "sensorDriver.h"

unsigned long lastMillis;
mqttLink *mqtt;

void setup() {
  mqtt = new mqttLink();
  delay(500); // 等待串口稳定
  mqtt->begin();
  lastMillis = millis();
}

void loop() {
  mqtt->mqttLoop();
}
