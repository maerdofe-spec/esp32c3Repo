#include "mqttLink.h"
mqttLink *mqttLink::instance = nullptr;

mqttLink::mqttLink()
    : mqttClient(wifiClient),
      cmdSlot(MQTT_TOPIC_CMD_BASE, "", false),
      emptyCmd("")
{
  Serial.begin(115200);
  instance = this;
}

// 初始化，包括wifi初始化
void mqttLink::begin() {
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
      mqttClient.publish(cmdSlot.topic, "Where is my mind?");
      mqttClient.subscribe(cmdSlot.topic);
    }
  }
}

// mqtt publish回传原始数据
void mqttLink::publishRaw() {
}

void mqttLink::mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (instance != nullptr) {
    instance->handleMessage(topic, payload, length);
  }
}

void mqttLink::handleMessage(char* topic, byte* payload, unsigned int length) {
  String msg;
  msg.reserve(length);

  for (unsigned int i = 0; i < length; ++i) {
    msg += (char)payload[i];
  }

  cmdSlot.latestCmd = msg;
  cmdSlot.hasNewCmd = true;
}

void mqttLink::update() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  if (!mqttClient.connected()) {
    connectMqtt();
  }

  mqttClient.loop();
}