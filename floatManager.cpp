#include "floatManager.h"

void floatManager::init() {
  currentState = IDLE;
}

void floatManager::handleCurrentState() {
  switch (currentState) {
    case IDLE:
      executer.idle();
      // mqtt.publish(MQTT_TOPIC_HISTORY, "Idling...");
      break;
    case DIVE:
      // 在DIVE状态下，调用toDepth函数，如果达到目标深度则切换到HOVER1状态
      executer.setFeedforward(FEEDFORWARD_DEEP);
      if (executer.toDepth()) currentState = HOVER1;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Diving...");
      break;
    case HOVER1:
      // 在HOVER1状态下，调用hover函数，如果悬停足够时间则切换到ASCEND状态
      if (executer.hover()) currentState = ASCEND;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Hover1ng...");
      break;
    case ASCEND:
      // 在ASCEND状态下，调用toDepth函数，如果达到目标深度则切换到HOVER2状态
      executer.setGoalDepth(TARGET_DEPTH_SHALLOW);
      executer.setFeedforward(FEEDFORWARD_SHALLOW);
      if (executer.toDepth()) currentState = HOVER2;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Ascending...");
      break;
    case HOVER2:
      // 在HOVER2状态下，调用hover函数，如果悬停足够时间则切换到RECOVERY状态
      if (executer.hover()) currentState = RECOVERY;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Hover2ng...");
      break;
    case RECOVERY:
      // 在RECOVERY状态下，调用recovery函数，如果回收完成则切换到UPLOADING状态
      if (executer.recovery()) currentState = UPLOADING;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Recovering...");
      break;
    case UPLOADING:
      mqtt.publish(MQTT_TOPIC_HISTORY, "UPLOADING...");
      currentState = IDLE;
      break;
  }
}

void floatManager::handleCmd() {
  if (!mqtt.mqttConnected() || !mqtt.hasNewCmd()) return;

  String cmd = mqtt.newCmd();
  mqtt.clearCmd();

  if (cmd == "ping") {
    mqtt.publish(MQTT_TOPIC_HISTORY, "ping succeed!");
    return;
  }

  if (cmd == "start") {
    // 收到start命令后，切换到DIVE状态并设置目标深度
    currentState = DIVE;
    executer.setGoalDepth(TARGET_DEPTH_DEEP);
  }
}

void floatManager::run() {
  if (WiFi.status() != WL_CONNECTED) {
    mqtt.connectWifi();
  }

  if (!mqtt.mqttConnected()) {
    mqtt.connectMqtt();
  }

  // 处理MQTT连接和命令
  mqtt.update();
  handleCmd();

  // // updateLogging(currentDepth, nowMs);
  handleCurrentState();
}
