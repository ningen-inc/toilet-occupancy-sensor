// for M5Stack Basic
// select board to M5Stack-Core-ESP32

#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "common.h"

enum ToiletState{
  ToiletState_Occupied,
  ToiletState_Vacant,
  ToiletState_NoSignal,
  ToiletState_Unknown,
};

enum ToiletState toiletState=ToiletState_NoSignal;
int sameStateTimeMS=0;

int watchDogTimerMS=0;
int previousTimeMS=0;

WiFiUDP udpServer;

void setup() {
  M5.begin();
  //text scale
  M5.Lcd.setTextSize(4);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.print("Connecting to: ");
  M5.Lcd.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  if(!WiFi.config(SERVER_IP,SERVER_GATEWAY,SERVER_SUBNET)){
    M5.Lcd.println("WiFi.config() failed.");
  }
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("Connected.");
  udpServer.begin(SERVER_PORT);
  previousTimeMS=millis();
}

void loop() {
  int timeMS=millis();
  int deltaTimeMS=timeMS-previousTimeMS;
  {
    int packetLength=udpServer.parsePacket();
  
    char packetBuffer[PACKET_LENGTH+1];
    udpServer.read(packetBuffer,PACKET_LENGTH);
    enum ToiletState nextToiletState=ToiletState_Unknown;
    if(packetBuffer[0]==PACKET_OCCUPIED[0]){
      nextToiletState=ToiletState_Occupied;
    }
    if(packetBuffer[0]==PACKET_VACANT[0]){
      nextToiletState=ToiletState_Vacant;
    }

    if(packetLength==0){
      watchDogTimerMS-=deltaTimeMS;
      if(watchDogTimerMS<=0){
        watchDogTimerMS=0;
        nextToiletState=ToiletState_NoSignal;
      }
    }else{
      watchDogTimerMS=WATCH_DOG_TIMER_TTL_MS;
    }

    if(nextToiletState!=ToiletState_Unknown && toiletState!=nextToiletState){
      toiletState=nextToiletState;
      sameStateTimeMS=0;
    }else{
      sameStateTimeMS+=deltaTimeMS;
    }
    
  }
  
  switch(toiletState){
    case ToiletState_Vacant:
    M5.Lcd.clear(TFT_BLUE);
    break;
    case ToiletState_Occupied:
    M5.Lcd.clear(TFT_RED);
    break;
    case ToiletState_NoSignal:
    M5.Lcd.clear(TFT_YELLOW);
    break;
  }
  M5.Lcd.setCursor(0,0);
  M5.Lcd.printf("%8d[s]",sameStateTimeMS/1000);
  // M5.Lcd.setCursor(0,50);
  // M5.Lcd.printf("%8d[ms]",watchDogTimerMS);
  // M5.Lcd.setCursor(0,100);
  // M5.Lcd.printf("%8d[ms]",deltaTimeMS);
  
  previousTimeMS=timeMS;
  
  delay(100);

}
