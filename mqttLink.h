#ifndef MQTTLINK_H
#define MQTTLINK_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

/*
    mqtt负责浮标和上位机之间的通信，
    需要回传的数据有：深度，脉冲数（浮力），当前所处任务周期
    需要传递到浮标的数据有：三个pid参数，目标深度，初始化信号
*/
/*  
    mosquitto启动失败时，终止当前占用进程
    netstat -ano | findstr :1883
    taskkill /PID 12345 /F
*/


class mqttLink {
  public:
    // 初始化方法
    mqttLink();
    void init();
    void connectWifi();
    void connectMqtt();
    // 发布数据
    bool publish(char* topic, const char* payload);
    // 进loop，有新的命令就回调，更新commandSlot
    void update();
    // 读取数据
    bool hasNewCmd();
    const String &newCmd();
    void clearCmd();
    // 状态读取
    bool mqttConnected() {return mqttClient.connected();}

  private:
    struct commandSlot {
      const char *topic;
      String newCmd;
      bool hasNewCmd;
    };
    // 保留了原来的结构体形式，但只保留了一个话题
    static mqttLink *instance;
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    void handleMessage(char* topic, byte* payload, unsigned int length);
    String emptyCmd;
    commandSlot cmdSlot;
    WiFiClient wifiClient;
    PubSubClient mqttClient;
};

#endif