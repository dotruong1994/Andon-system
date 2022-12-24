#include "FastLED.h"   // khai báo thư viện led andonsign

#include <MFRC522.h>
#include <SPI.h>      
#define SS_PIN  21                           // khối sử dụng cho module RFID
#define RST_PIN  22
MFRC522 mfrc522(SS_PIN, RST_PIN);


unsigned long dem =0;
unsigned long dem2 =0;    
unsigned long dem3 =0;          // các biếm tạm để gán cho việc sử dụng đèn
unsigned long dem4 =0;
unsigned long count_1 = 1000;
long lastMsg = 0;        
bool tam=0;
bool tam2=0;
bool tam3=0;
bool checkLEDVal;
const uint32_t connectTimeoutMs = 10000;
int x=0;
#define NUM_LEDS 51
#define DATA_PIN 5
CRGB leds[NUM_LEDS];
#define LED_PIN GPIO_NUM_27

#include <WiFi.h>
#include <WiFiMulti.h>    // khai báo khối wifi và http service
WiFiMulti wifiMulti;
#include <HTTPClient.h>

#include <EasyButton.h>
#define BUTTON_PIN1 32
#define BUTTON_PIN2 33
#define BUTTON_PIN3 25
#define BUTTON_PIN4 26                  // khối khai báo nút nhấn
EasyButton button1(BUTTON_PIN1); 
EasyButton button2(BUTTON_PIN2); 
EasyButton button3(BUTTON_PIN3); 
EasyButton button4(BUTTON_PIN4); 

String ID = "11223344" ; // thay đổi ID vị trí ở chỗ này   
String rfid;

// khối chương trình con chức năng cho các nút nhấn
void call_tpm();
void receive_order();
void leader();
void material();
void dead_alive();
void monitorWiFi(void * parameter);
void andonLoop(void * parameter);
void send_rfid();
void check_RFID();



void dencanbo()
{
    for(int i=0;i<51;i++)
      {
        leds[i] = CRGB(90,32,167);
        FastLED.show();
      }
}
void denbaotri()
{
    for(int i=0;i<51;i++)
      {
        leds[i] = CRGB(0,255,20);
        FastLED.show();
      } 
}
void denhoatdong()
{
    for(int i=0;i<51;i++)
      {
        leds[i] = CRGB(236,236,236);
        FastLED.show();
      } 
}
void den_nhan_don()
{
    for(int i=0;i<51;i++)
      {
        leds[i] = CRGB(255, 0, 0);
        FastLED.show();
      } 
}
void denhetlieu()
{
    for(int i=0;i<51;i++)
      {
        leds[i] = CRGB(249,244,0);
        FastLED.show();
      } 
}

void den_trieu_hoi_thanh_dong()
{
    for(int i=0;i<51;i++)
      {
        leds[i] = CRGB(0,75,130);
        FastLED.show();
      } 
}


// chương trình con giám sát wifi

void monitorWiFi(void * parameter){
  for(;;){ // infinite loop
    if(millis()-count_1>=10000)
    {
      if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) 
      {
        Serial.print("WiFi connected: ");
        Serial.print(WiFi.SSID());
        Serial.print(" ");
        Serial.println(WiFi.RSSI());
      }
      else {
        Serial.println("WiFi not connected!");
        den_trieu_hoi_thanh_dong();
        long now = millis();
        if (now - lastMsg > 20000) 
          {
            lastMsg = now;
            ESP.restart();
          }
      }
    }
    count_1=millis();
  }
}
void andonLoop(void * parameter){
  for(;;){ // infinite loop
    button1.read();
    button2.read();
    button3.read();
    button4.read();

    check_RFID();
    if(checkLEDVal==1)
      {
        digitalWrite(LED_PIN,HIGH);
      }
    if(checkLEDVal==0)
      {
        digitalWrite(LED_PIN,LOW);
      }
      
    if(millis()-dem2>=1000)
    { 
      if(tam3==1)
      {
        den_nhan_don();
      } 
      if(tam3==1&&x==2)
      {
        tam3=0;
        x=0;
      }
      if(tam2==1)
      {
          denbaotri();
      }
      else if(tam2==0&&tam3==0)
      {
          if(tam==1)
          {
            if(millis()-dem>=60000)
            {
              tam=0;      
            }
          }
          else if(tam==0)
          {
            denhoatdong();
          }
      }
      dem2=millis();
    }
    if(millis()-dem3>=1800000)
    {
      dead_alive();
      dem3=millis();
    }
    vTaskDelay(10);
  }
//  Serial.println(uxTaskGetStackHighWaterMark());
}


void setup()
{
  SPI.begin(); 
  vTaskDelay(500);
  mfrc522.PCD_Init(); 
  vTaskDelay(50);
  button1.begin();
  button2.begin();
  button3.begin();
  button4.begin();
  button1.onPressed(receive_order);
  button2.onPressed(leader);
  button3.onPressed(material);
  button4.onPressed(call_tpm);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("Redmi", "thanh12345");
  wifiMulti.addAP("AE_CAM", "Aa123456");
  wifiMulti.addAP("OPPO F9", "thuat12345678!");
  wifiMulti.addAP("factory-A10-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A10-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A10-1F-1", "qwerasdf");

  wifiMulti.addAP("Factory-A1-1F-3", "qwerasdf");
  wifiMulti.addAP("Factory-A1-1F-2", "qwerasdf");
  wifiMulti.addAP("Factory-A1-1F-1", "qwerasdf");

  wifiMulti.addAP("factory-A6-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A6-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A6-1F-1", "qwerasdf");
  
  wifiMulti.addAP("factory-A7-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A7-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A7-1F-1", "qwerasdf");  
  
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
  
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(500);
  

  while (wifiMulti.run() != WL_CONNECTED) 
  {
    den_trieu_hoi_thanh_dong();
    digitalWrite(LED_PIN,HIGH);
    vTaskDelay(500);
    digitalWrite(LED_PIN,LOW);
    vTaskDelay(500);
    Serial.println("Connecting to WiFi..");
    long now = millis();
    if (now - lastMsg > 20000) {
      lastMsg = now;
      ESP.restart();
  }


  }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.localIP());
    Serial.println(ID);
    digitalWrite(LED_PIN,LOW);
    denhoatdong();
    dead_alive();

  xTaskCreatePinnedToCore(
    monitorWiFi,    // Function that should be called
    "Monitor Wifi",   // Name of the task (for debugging)
    8192,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL,             // Task handle
    CONFIG_ARDUINO_RUNNING_CORE
  );
  
  xTaskCreate(
    andonLoop,    // Function that should be called
    "andonLoop",   // Name of the task (for debugging)
    8192,            // Stack size (bytes)
    NULL,            // Parameter to pass
    2,               // Task priority
    NULL             // Task handle
  );
}

void loop() 
{
}
void check_RFID(){
  if (mfrc522.PICC_IsNewCardPresent()) 
  { 
    if (mfrc522.PICC_ReadCardSerial()) {
      checkLEDVal=1;
    }
  }
  else
  {
    checkLEDVal =0;
  }
  
}


void call_tpm()
{ 
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select a card
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  { 
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  mfrc522.PICC_HaltA(); // halt PICC
  mfrc522.PCD_StopCrypto1(); // stop encryption on PCD
  Serial.println(content);
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN,LOW);
  vTaskDelay(100);
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN,LOW);
  vTaskDelay(100);
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN,LOW);
  tam2=1;
  denbaotri();
  rfid = "http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/0:"+ID+":"+content+"";            
  send_rfid();
}
void receive_order()
{ 
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select a card
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  { 
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  mfrc522.PICC_HaltA(); // halt PICC
  mfrc522.PCD_StopCrypto1(); // stop encryption on PCD
  content.toUpperCase();
  Serial.println(content);
  rfid = "http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/2:"+ID+":"+content+"";            
  send_rfid();
  tam3=1;
  tam2=0;
  x=x+1;
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN,LOW);
  vTaskDelay(100);
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN,LOW);
  vTaskDelay(100);
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN,LOW);
}
void leader()
{
  tam=1;
  dem=millis();
  dencanbo();
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN,LOW);
    // if (wifiMulti.run() == WL_CONNECTED)
    // { //Check the current connection status
      HTTPClient http;
   
      http.begin("http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/1:"+ID+":123456"); //Specify the URL
      int httpCode = http.GET();                                        //Make the request
   
      if (httpCode > 0) 
      { //Check for the returning code
          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
      }
      else 
      {
        Serial.println("Error on HTTP request");
      }
      http.end(); //Free the resources
      //}     
   
}
void material()
{ 
  denhetlieu();
  tam=1;
  dem=millis();
     if (wifiMulti.run() == WL_CONNECTED) 
     { //Check the current connection status
      HTTPClient http;
   
      http.begin("http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/3:"+ID+":123456"); //Specify the URL    3: het chi A11; 4:het nuoc thuoc A10    5 : test tu tai
      int httpCode = http.GET();                                        //Make the request
   
      if (httpCode > 0) 
      { //Check for the returning code
          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
          digitalWrite(LED_PIN,HIGH);
          vTaskDelay(100);
          digitalWrite(LED_PIN,LOW);
      }
      else 
      {
        Serial.println("Error on HTTP request");
      }
      http.end(); //Free the resources
      }
}
void dead_alive()
{ 
     if (wifiMulti.run() == WL_CONNECTED) 
     { //Check the current connection status
      HTTPClient http;
   
      http.begin("http://172.21.143.74:8080/cmmsservice/repairCheckList/healthCheck/VY/"+ID+""); //Specify the URL    3: het chi A11; 4:het nuoc thuoc A10
      int httpCode = http.GET();                                        //Make the request
   
      if (httpCode > 0) 
      { //Check for the returning code
          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
          digitalWrite(LED_PIN,HIGH);
          vTaskDelay(100);
          digitalWrite(LED_PIN,LOW);
      }
      else 
      {
        Serial.println("Error on HTTP request");
      }
      http.end(); //Free the resources
      }
}
void send_rfid()
{ 
     if (wifiMulti.run() == WL_CONNECTED) 
     { //Check the current connection status
      HTTPClient http;
   
      http.begin(rfid); //Specify the URL
      int httpCode = http.GET();                                        //Make the request
   
      if (httpCode > 0) 
      { //Check for the returning code
          String payload = http.getString();
          Serial.println(httpCode);
          //Serial.println(payload);
      }
      else 
      {
        Serial.println("Error on HTTP request");
      }
      http.end(); //Free the resources
      }
    Serial.println(rfid);
}
