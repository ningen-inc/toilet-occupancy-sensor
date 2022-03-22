// for M5Stamp C3 Mate
// select board to ESP32C3 Dev Module

#include <WiFi.h>
#include <WiFiUdp.h>
#include "common.h"

// 断線を使用中とする。
// スイッチONで断線と同じ。
// INPUT_PULLUPのため、使用中はHIGH
int sensorPin=4;

WiFiUDP udpClient;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected.");
  pinMode(sensorPin,INPUT_PULLUP);
}

void loop() {
  int sensorState=digitalRead(sensorPin);
  
  udpClient.beginPacket(SERVER_IP,SERVER_PORT);
  const char* packet=NULL;
  if(sensorState==HIGH){
    // 使用中 OR 断線
    packet=PACKET_OCCUPIED;
  }else{
    // 空き
    packet=PACKET_VACANT;
  }
  udpClient.write((const uint8_t*)packet,PACKET_LENGTH);
  udpClient.endPacket();
  Serial.println(packet);
  delay(1000);
}
