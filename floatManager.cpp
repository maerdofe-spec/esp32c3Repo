#include "floatManager.h"

void floatManager::init() {
  lastMillis = millis();
  currentState = IDLE;
  // kp = CTRL_KP_DEFAULT;
  // ki = CTRL_KP_DEFAULT;
  // kd = CTRL_KP_DEFAULT;
  // feedShallow = FEEDFORWARD_SHALLOW;
  // feedDeep = FEEDFORWARD_SHALLOW;
}

void floatManager::handleCurrentState() {
  switch (currentState) {
    case IDLE: executer.idle(); break;
    case DIVE: executer.toDepth(TARGET_DEPTH_DEEP); break;
    case HOVER1: executer.hover(TARGET_DEPTH_DEEP); break;
    case ASCEND: executer.toDepth(TARGET_DEPTH_SHALLOW); break;
    case HOVER2: executer.hover(TARGET_DEPTH_SHALLOW); break;
    case RECOVERY: executer.recovery(); break;
    case UPLOADING: break;
  }
}

void floatManager::run() {
  if (WiFi.status() != WL_CONNECTED) {
    mqtt.connectWifi();
  }

  if (!mqtt.mqttConnected()) {
    mqtt.connectMqtt();
  }

  // 尝试获取最新状态，给对应topic更新msg
  mqtt.update();
  // 去处理给定cmd的任务
  // handleCurrentState();
}
