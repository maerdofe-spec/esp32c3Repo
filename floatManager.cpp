#include "floatManager.h"

void floatManager::init() {
  currentState = IDLE;
  recorderTimer.resetAndStop();
}

void floatManager::handleCurrentState() {
  switch (currentState) {
    case IDLE:
      executer.idle();
      isFirstTask = true;
      // mqtt.publish(MQTT_TOPIC_HISTORY, "Idling...");
      break;

    case DIVE:
      // 在DIVE状态下，调用toDepth函数，如果达到目标深度则切换到HOVER1状态
      executer.setGoalDepth(TARGET_DEPTH_DEEP);
      pid.setFeedforward(FEEDFORWARD_DEEP);
      if (executer.toDepth(recorderTimer)) currentState = HOVER1;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Diving...");
      break;

    case HOVER1:
      // 在HOVER1状态下，调用hover函数，如果悬停足够时间则切换到ASCEND状态
      if (executer.hover(recorderTimer)) currentState = ASCEND;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Hover1ng...");
      break;

    case ASCEND:
      // 在ASCEND状态下，调用toDepth函数，如果达到目标深度则切换到HOVER2状态
      executer.setGoalDepth(TARGET_DEPTH_SHALLOW);
      pid.setFeedforward(FEEDFORWARD_SHALLOW);
      if (executer.toDepth(recorderTimer)) currentState = HOVER2;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Ascending...");
      break;

    case HOVER2:
      // 在HOVER2状态下，调用hover函数，如果悬停足够时间则进行第二次任务，或在第二次完成后切换到RECOVERY状态
      if (executer.hover(recorderTimer)) {
        // 测试只进行一次任务
        // if(isFirstTask)  {
        //   currentState = DIVE;
        //   isFirstTask = false;}
        //else 
        currentState = RECOVERY;
      }
      mqtt.publish(MQTT_TOPIC_HISTORY, "Hover2ng...");
      break;

    case RECOVERY:
      // 在RECOVERY状态下，调用recovery函数，如果回收完成则切换到UPLOADING状态
      if (executer.recovery(recorderTimer)) currentState = UPLOADING;
      mqtt.publish(MQTT_TOPIC_HISTORY, "Recovering...");
      break;

    case UPLOADING:
      // 停止记录计时器
      recorderTimer.resetAndStop();

      // 上传数据到MQTT服务器的data话题
      if(WiFi.status() == WL_CONNECTED && mqtt.mqttConnected()) {
        mqtt.publish(MQTT_TOPIC_HISTORY, "UPLOADING...");
        uint8_t pendingCnt = recorder.pendingCount();
        uint8_t processedCnt = 0;
        const dataRecorder::sample *data = recorder.pendingData();
        while(processedCnt < pendingCnt){
          char payload[80];
          snprintf(payload, sizeof(payload), "{\"time_ms\":%lu,\"depth_m\":%.2f}", data[processedCnt].timeMs, data[processedCnt].depth);
          mqtt.publish(MQTT_TOPIC_DATA, payload);
          processedCnt++;
        }
        // 完成上传后应当清空记录器数据并重置状态
        recorder.clear();
        mqtt.publish(MQTT_TOPIC_HISTORY, "Complete uploading");
        currentState = IDLE;
      }
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
    // 收到start命令后，切换到DIVE状态并开始数据记录计时器
    currentState = DIVE;
    if(!recorderTimer.isRunning()) {
    recorderTimer.init(LOG_INTERVAL);
    }
  }

  // 支持通过MQTT动态设置PID参数：
  // 格式: "setpid <kp> <ki> <kd>"
  if (cmd.startsWith("setpid ")) {
    float kp=0, ki=0, kd=0;
    uint8_t parsed = sscanf(cmd.c_str()+7, "%f %f %f", &kp, &ki, &kd);
    if (parsed == 3) {
      pid.setGains(kp, ki, kd);
      char buf[64];
      snprintf(buf, sizeof(buf), "PID updated kp=%.4f ki=%.6f kd=%.4f", kp, ki, kd);
      mqtt.publish(MQTT_TOPIC_HISTORY, buf);
    } else {
      mqtt.publish(MQTT_TOPIC_HISTORY, "setpid format error");
    }
    return;
  }

  // 支持设置前馈: "setff <value>"，value 可以为小数
  if (cmd.startsWith("setff ")) {
    float ff = 0.0f;
    int parsed = sscanf(cmd.c_str()+6, "%f", &ff);
    if (parsed == 1) {
      pid.setFeedforward(ff);
      char buf[48];
      snprintf(buf, sizeof(buf), "feedforward set %.4f", ff);
      mqtt.publish(MQTT_TOPIC_HISTORY, buf);
    } else {
      mqtt.publish(MQTT_TOPIC_HISTORY, "setff format error");
    }
    return;
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
