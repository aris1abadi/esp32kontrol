
#include <Arduino.h>
// #include <ESP8266WiFi.h>
#include <WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <mqttHandle.h>
#include <eepromHandle.h>
#include <mySensorsHandle.h>
#include <timeHandle.h>
#include <kontrolHandle.h>
#include <bluethootHandle.h>

// #include <bleHandle.h>

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}

// #define MQTT_DEBUG
#define RECONECT_WIFI_TIME 10 // detik
#define RECONECT_MQTT_TIME 10

String kontrolID = "AB5578";
// extern const char *firmwareVersion;
String ssid = "Lele Bakar";
String pass = "@LesehanPundong#123";
String mqtt_host = "mqtt.eclipseprojects.io";
int mqtt_port = 1883;

char hostName[6];
const char *firmwareVersion = "1.0.1";

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
// Ticker mqttReconnectTimer;
// Ticker wifiReconnectTimer;
Ticker rebootTimer;

// variable esternal

bool wifi_status = false;
bool mqtt_status = false;

unsigned long mqtt_delay = 0;
unsigned long heartbeat_time = 10000;
unsigned int heartBeat_val = 0;

void sendHeatBeat()
{
  ++heartBeat_val;
  kirimKeRemote("kontrol", 0, "heartBeat", String(heartBeat_val));
}
void mqtt_loop()
{
  if ((millis() - mqtt_delay) > heartbeat_time)
  {
    mqtt_delay = millis();
    if (mqtt_status)
    {
      sendHeatBeat();
    }
  }
}
void rebootNow()
{
  ESP.restart();
}
void connectToMqtt()
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}
void connectToWifi()
{
  Serial.println("Connecting to Wi-Fi...");
// if (!WiFi.config(local_IP, gateway, subnet)) {
//   Serial.println("STA Failed to configure");
// }
// Configures static IP address
#ifdef USE_LOCAL_OTA
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());
  // WiFi.begin("siprosida", "balingtan123");
}
/*
void onWifiConnect(const WiFiEventStationModeGotIP &event)
{
  Serial.println("Connected to Wi-Fi.");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  connectToMqtt();
  wifi_status = true;
}
void onWifiDisconnect(const WiFiEventStationModeDisconnected &event)
{
  Serial.println("Disconnected from Wi-Fi.");
  wifi_status = false;
  mqtt_status = false;
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(RECONECT_WIFI_TIME, connectToWifi);
  // led_mode = 0;
}
*/
bool getWifi_sts()
{
  return wifi_status;
}
void onMqttConnect(bool sessionPresent)
{
  mqtt_status = true;
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  String subTxt = "abadinet-in/";
  subTxt += kontrolID;
  subTxt += "/#";
  uint16_t packetIdSub = mqttClient.subscribe(subTxt.c_str(), 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  subTxt = "abadinet-out/";
  subTxt += kontrolID;
  subTxt += "/kontrol/0/status";
  String k_id = "KontrolID ";
  k_id += kontrolID;
  k_id += " firmware Version ";
  k_id += firmwareVersion;
  mqttClient.publish(subTxt.c_str(), 0, true, k_id.c_str());
  Serial.println("Publishing at QoS 0");
  // ota online init

  ntp_init();
  ntp_sync();
}
void kirimKeRemote(String type, uint8_t nomer, String cmd, String msg)
{
  String topic_id = "abadinet-out/";
  topic_id += kontrolID;
  topic_id += "/";
  topic_id += type;
  topic_id += "/";
  topic_id += String(nomer);
  topic_id += "/";
  topic_id += cmd;
  if(mqtt_status){
    mqttClient.publish(topic_id.c_str(), 0, true, msg.c_str());
  }  
  String ble_msg = topic_id;
  ble_msg += ';';
  ble_msg += msg;
  ble_msg += ";";
  kirimBluethooth(ble_msg);
}
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  Serial.println("Disconnected from MQTT.");
  mqtt_status = false;

  if (WiFi.isConnected())
  {
    xTimerStart(mqttReconnectTimer, 0);
  }
}
void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}
void onMqttUnsubscribe(uint16_t packetId)
{
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}
void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  String mqttMsg = "";
  for (unsigned int i = 0; i < len; i++)
  {
    mqttMsg += payload[i];
  }
  String mqttTopic = String(topic);

  // mqttTopic += "/dummyTopic";
  cekMsg(mqttTopic, mqttMsg);
#ifdef MQTT_DEBUG
  Serial.print("topic: ");
  Serial.println(mqttTopic);
  Serial.print("Rcv msg: ");
  Serial.println(mqttMsg);
#endif
}
void onMqttPublish(uint16_t packetId)
{
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void WiFiEvent(WiFiEvent_t event)
{
  // Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    wifi_status = true;
    connectToMqtt();
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    Serial.println("WiFi lost connection");
    wifi_status = false;
    mqtt_status = false;
    xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    xTimerStart(wifiReconnectTimer, 0);
    break;
  }
}

void mqtt_init()
{
  Serial.println("network init");

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(10000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(5000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(mqtt_host.c_str(), mqtt_port);
  connectToWifi();
}
