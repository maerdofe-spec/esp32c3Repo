#include "mqttLink.h"
mqttLink *mqttLink::instance = nullptr;

mqttLink::mqttLink()
    : mqttClient(),
      cmdSlot(MQTT_TOPIC_CMD, "", false),
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
  
  mqttClient.setClient(wifiClient);
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

  if (!mqttConnected()) {
    //设置心跳频率
    mqttClient.setKeepAlive(MQTT_KEEPALIVE);
    bool ifConnected = mqttClient.connect(MQTT_CLIENT_ID);
    if (ifConnected) {
      mqttClient.publish(MQTT_TOPIC_CMD, "A new connection is established.");
      mqttClient.subscribe(MQTT_TOPIC_CMD);
      mqttClient.subscribe(MQTT_TOPIC_HISTORY);
    }
  }
}

// mqtt publish回传原始数据
bool mqttLink::publish(const char* topic, const char* payload) {
  if (!mqttConnected()) {
    return false;
  }
  return mqttClient.publish(topic, payload);
}

// mqtt.loop()的回调函数
void mqttLink::mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (instance != nullptr) {
    instance->handleMessage(topic, payload, length);
  }
}

// 用实例执行回调
void mqttLink::handleMessage(char* topic, byte* payload, unsigned int length) {
  String msg;
  msg.reserve(length);

  for (unsigned int i = 0; i < length; ++i) {
    msg += (char)payload[i];
  }

  cmdSlot.newCmd = msg;
  cmdSlot.hasNewCmd = true;
}

// 尝试状态更新
void mqttLink::update() {
  mqttClient.loop();
}

bool mqttLink::hasNewCmd() {
  return cmdSlot.hasNewCmd;
}
const String &mqttLink::newCmd() {
  return cmdSlot.newCmd;
}
void mqttLink::clearCmd() {
  cmdSlot.newCmd = emptyCmd;
  cmdSlot.hasNewCmd = false;
}