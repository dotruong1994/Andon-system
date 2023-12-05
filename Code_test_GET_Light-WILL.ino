#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#include "FastLED.h"
// #include "FastLED.h"
// #define NUM_LEDS 1
// #define DATA_PIN 5
// CRGB leds[NUM_LEDS];

#define NUM_LEDS 7
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

#include <HTTPClient.h>
#include <EasyButton.h>

#include <MFRC522.h>
#include <SPI.h>
#define SS_PIN 21  // khối dùng cho module RFID
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

bool checkLEDVal;  // biến điều khiển chớp tắt khi đưa thẻ vào RFID
int updateDeadAlive_time = 600000;
unsigned long countingTimeDeadAlive = 0;
unsigned long countingTimeWIFIcheck = 0;
unsigned long espRestartTimeOut = 0;

unsigned long callLeaderCountDown = 0;
unsigned long callMaterialCountDown = 0;

const uint32_t connectTimeoutMs = 10000;
int state = 0;
int countState = 0;

#define LED_PIN GPIO_NUM_27  //LED đơn theo dõi

#define BUTTON_PIN1 32
#define BUTTON_PIN2 33
#define BUTTON_PIN3 25
#define BUTTON_PIN4 26

EasyButton button1(BUTTON_PIN1);
EasyButton button2(BUTTON_PIN2);
EasyButton button3(BUTTON_PIN3);
EasyButton button4(BUTTON_PIN4);


String ID = "A1-CB-22";
String rfid;
//Your Domain name with URL path or IP address with path

String callLeader = "http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/1:" + ID + ":123456";
String callMaterial = "http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/3:" + ID + ":123456";
String callalive = "http://172.21.143.74:8080/cmmsservice/repairCheckList/healthCheck/VY/" + ID + "";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);
  delay(10);
  SPI.begin();
  delay(100);
  mfrc522.PCD_Init();
  delay(50);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("SteveiPhone", "11223344");
   wifiMulti.addAP("factory-A10-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A10-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A10-1F-1", "qwerasdf");

  wifiMulti.addAP("Factory-A1-1F-3", "qwerasdf");
  wifiMulti.addAP("Factory-A1-1F-2", "qwerasdf");
  wifiMulti.addAP("Factory-A1-1F-1", "qwerasdf");

  wifiMulti.addAP("Factory-A2-1F-3", "qwerasdf");
  wifiMulti.addAP("Factory-A2-1F-2", "qwerasdf");
  wifiMulti.addAP("Factory-A2-1F-1", "qwerasdf");

  wifiMulti.addAP("Factory-A3-1F-3", "qwerasdf");
  wifiMulti.addAP("Factory-A3-1F-2", "qwerasdf");
  wifiMulti.addAP("Factory-A3-1F-1", "qwerasdf");

  wifiMulti.addAP("factory-A6-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A6-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A6-1F-1", "qwerasdf");
  
  wifiMulti.addAP("factory-A7-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A7-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A7-1F-1", "qwerasdf");  

  wifiMulti.addAP("factory-A8-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A8-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A8-1F-1", "qwerasdf");
  
  wifiMulti.addAP("factory-A17-1F-1", "qwerasdf");
  wifiMulti.addAP("factory-A17-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A17-1F-3", "qwerasdf");
  
  wifiMulti.addAP("factory-A10-1F-3", "qwerasdf");
  wifiMulti.addAP("Factory-A11-1F", "qwerasdf");

  wifiMulti.addAP("Factory-A11-1F-1", "qwerasdf");
  wifiMulti.addAP("factory-A15-1F", "qwerasdf");
  wifiMulti.addAP("factory-A18-1F-3", "qwerasdf");
  
  wifiMulti.addAP("Factory-K1-1F-1", "qwerasdf");
  wifiMulti.addAP("Factory-K1-1F-2", "qwerasdf");
  wifiMulti.addAP("Factory-K1-1F-3", "qwerasdf"); 
  wifiMulti.addAP("Factory-K1-2F-1", "qwerasdf");
  wifiMulti.addAP("Factory-K1-2F-2", "qwerasdf");
  wifiMulti.addAP("Factory-K1-2F-3", "qwerasdf");

  wifiMulti.addAP("factory-j1-7", "qwerasdf");
  wifiMulti.addAP("factory-j1-8", "qwerasdf");
  wifiMulti.addAP("factory-j1-9", "qwerasdf");
  wifiMulti.addAP("factory-j1-10", "qwerasdf");
  wifiMulti.addAP("factory-J1-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-J1-1F-3", "qwerasdf");

  wifiMulti.addAP("factory-J2-1F-1", "qwerasdf");
  wifiMulti.addAP("factory-J2-1F3", "qwerasdf");
  wifiMulti.addAP("factory-J2-2F-1", "qwerasdf");

  wifiMulti.addAP("Factory-j3-2f-2", "qwerasdf");
  wifiMulti.addAP("factory-J3-2F", "qwerasdf");
  wifiMulti.addAP("factory-J3-1F", "qwerasdf");

  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  button1.begin();
  button2.begin();
  button3.begin();
  button4.begin();

  button1.onPressed(receive_order);
  button2.onPressed(leader);
  button3.onPressed(material);
  button4.onPressed(call_tpm);


  Serial.println("Connecting");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.RSSI());
  }
  while (1) {
    check_WIFI();
    if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
      break;
    }
  }
  dead_alive();
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  //Check WiFi connection status
  //check_WIFI();
  check_RFID();
  if (checkLEDVal == 1) {
    digitalWrite(LED_PIN, HIGH);
  }
  if (checkLEDVal == 0) {
    digitalWrite(LED_PIN, LOW);
  }
  button1.read();
  button2.read();
  button3.read();
  button4.read();
  if (millis() - countingTimeDeadAlive >= updateDeadAlive_time) {
    dead_alive();
    countingTimeDeadAlive = millis();
  }
  if (millis() - countingTimeWIFIcheck >= connectTimeoutMs) {
    check_WIFI();
    countingTimeWIFIcheck = millis();
  }

  switch (state) {
    case 0:
      denhoatdong();
      break;
    case 1:
      // if ((state == 3)||(state == 4)) {
      //   state =3;
      //   break;
      // }
      dencanbo();
      if (millis() - callLeaderCountDown >= 60000) {
        state = 0;
        callLeaderCountDown = millis();
      }
      break;
    case 2:
      // if ((state == 3)||(state == 4)) {
      //   state =3;
      //   break;
      // }
      denhetlieu();
      if (millis() - callMaterialCountDown >= 60000) {
        state = 0;
        callMaterialCountDown = millis();
      }
      break;
    case 3:
      denbaotri();
      break;
    case 4:
      den_nhan_don();
      if(countState==2){
        state =0;
        countState =0;
      }
      break;
  }
}
