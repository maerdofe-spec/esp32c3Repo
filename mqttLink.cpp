#include "mqttLink.h"
mqttLink *mqttLink::instance = nullptr;

mqttLink::mqttLink()
    : mqttClient(wifiClient),
      cmdSlots{{MQTT_TOPIC_CMD, "", false},
               {MQTT_TOPIC_HISTORY, "", false}},
      emptyCmd("")
{
  instance = this;
}

// 初始化，包括wifi初始化
void mqttLink::init() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    connectWifi();
  }

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  connectMqtt();
}

// 尝试连接wifi，直到超时
void mqttLink::connectWifi() {
  unsigned long startMs = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startMs < 8000) {
    delay(500);
  }
}

// 尝试连接mqtt，并订阅（所有）给定的主题
void mqttLink::connectMqtt() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (!mqttClient.connected()) {
    bool ifConnected = mqttClient.connect(MQTT_CLIENT_ID);
    if (ifConnected) {
      mqttClient.publish(MQTT_TOPIC_CMD, "Where is my mind?");
      mqttClient.subscribe(MQTT_TOPIC_CMD);
      mqttClient.subscribe(MQTT_TOPIC_HISTORY);
    }
  }
}

// mqtt publish回传原始数据
void mqttLink::publish(char* topic, byte* payload) {
}

// mqtt.loop()的回调函数
void mqttLink::mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (instance != nullptr) {
    instance->handleMessage(topic, payload, length);
  }
}

void mqttLink::handleMessage(char* topic, byte* payload, unsigned int length) {
  // String msg;
  // msg.reserve(length);

  // for (unsigned int i = 0; i < length; ++i) {
  //   msg += (char)payload[i];
  // }

  // cmdSlot.latestCmd = msg;
  // cmdSlot.hasNewCmd = true;
}

// 尝试状态更新
void mqttLink::update() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  if (!mqttClient.connected()) {
    connectMqtt();
  }

  mqttClient.loop();
}