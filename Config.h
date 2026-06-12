#ifndef CONFIG_H
#define CONFIG_H

// 硬件引脚定义
#define PIN_SDA 6
#define PIN_SCL 7
#define PIN_BATTERY_ADC 4

// 步进电机参数
#define VELOCITY 200

// 任务参数
#define TARGET_DEPTH_DEEP 2.5f    // 目标深度1: 2.5米
#define TARGET_DEPTH_SHALLOW 0.4f // 目标深度2: 0.4米
#define TRANSITION_STABLE_DURATION 2000 // 转换稳定时间: 2秒 (毫秒)
#define HOVER_DURATION 30000      // 悬停时间: 30秒 (毫秒)
#define LOG_INTERVAL 1000         // 数据记录间隔: 1秒
#define MAX_DEPTH_OFFSET 0.05     // 最大任务深度偏移: 0.05米
#define FEEDFORWARD_SHALLOW 3600
#define FEEDFORWARD_DEEP 7200
#define CTRL_KP_DEFAULT 0.90f
#define CTRL_KI_DEFAULT 0.005f
#define CTRL_KD_DEFAULT 0.35f

// 身份信息
#define COMPANY_ID "Blue Fins"

// WiFi 参数
#define WIFI_SSID           "OPPOA5"    //"FINS"  //esp32_sjtu
#define WIFI_PASSWORD       "77777777"   //"fins1896"  //esp32_sjtu
// #define WIFI_SSID           "FINS"  //esp32_sjtu
// #define WIFI_PASSWORD       "fins1896"  //esp32_sjtu
// #define WIFI_SSID           "esp32_sjtu"
// #define WIFI_PASSWORD       "esp32_sjtu"

// MQTT 参数
#define MQTT_HOST "192.168.45.152" //"192.168.8.86"
#define MQTT_PORT 1883
#define MQTT_TOPIC_COUNTER "/counter" // MQTT_TEST
#define MQTT_TOPIC_DEBUG "/debug"     // DEBUG
#define MQTT_TOPIC_PREFIX   "/float_sjtu"
#define MQTT_TOPIC_CMD MQTT_TOPIC_PREFIX "/cmd"
#define MQTT_TOPIC_HISTORY MQTT_TOPIC_PREFIX "/history"
// #define MQTT_TOPIC_STATUS   MQTT_TOPIC_PREFIX "/status"

#define MQTT_CLIENT_ID      "float-sjtu-mqtt"

#endif