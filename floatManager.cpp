#include "floatManager.h"

void floatManager::init() {
  lastMillis = millis();
  currentState = IDLE;
}

void floatManager::handleCurrentState() {
  switch (currentState) {
    case IDLE: idle(); break;
    case DIVE: toDepth(TARGET_DEPTH_DEEP); break;
    case HOVER1: hover(TARGET_DEPTH_DEEP); break;
    case ASCEND: toDepth(TARGET_DEPTH_SHALLOW); break;
    case HOVER2: hover(TARGET_DEPTH_SHALLOW); break;
    case RECOVERY: recovery(); break;
  }
}

void floatManager::handleCmd() {
  if (WiFi.status() != WL_CONNECTED) {
    mqtt.connectWifi();
  }

  if (!mqtt.mqttConnected()) {
    mqtt.connectMqtt();
  }

  mqtt.update();

}