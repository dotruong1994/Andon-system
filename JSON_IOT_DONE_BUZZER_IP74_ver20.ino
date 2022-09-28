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
bool tam=0;
bool tam2=0;
bool tam3=0;
bool checkLEDVal;
int x=0;
#define NUM_LEDS 7
#define DATA_PIN 5
CRGB leds[NUM_LEDS];
#define LED_PIN     GPIO_NUM_27


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

String ID = "A6-MK-23" ; // thay đổi ID vị trí ở chỗ này   
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
    for(int i=0;i<7;i++)
      {
        leds[i] = CRGB(90,32,167);
        FastLED.show();
      }
}
void denbaotri()
{
    for(int i=0;i<7;i++)
      {
        leds[i] = CRGB(0,255,20);
        FastLED.show();
      } 
}
void denhoatdong()
{
    for(int i=0;i<7;i++)
      {
        leds[i] = CRGB(236,236,236);
        FastLED.show();
      } 
}
void den_nhan_don()
{
    for(int i=0;i<7;i++)
      {
        leds[i] = CRGB(255, 0, 0);
        FastLED.show();
      } 
}
void denhetlieu()
{
    for(int i=0;i<7;i++)
      {
        leds[i] = CRGB(249,244,0);
        FastLED.show();
      } 
}

// chương trình còn giám sát wifi

void monitorWiFi(void * parameter){
  for(;;){ // infinite loop
    if(millis()-dem4>=10000)
     {
        if((wifiMulti.run() != WL_CONNECTED)){
            Serial.println("Reset ESP");
            ESP.restart();
            }
          dem4=millis();
     }
    vTaskDelay(10);
  }
}
void andonLoop(void * parameter){
  for(;;){ // infinite loop
    button1.read();
    button2.read();
    button3.read();
    button4.read();
    button1.onPressed(receive_order);
    button2.onPressed(leader);
    button3.onPressed(material);
    button4.onPressed(call_tpm);
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
}


void setup()
{
  SPI.begin(); 
  //WiFi.mode(WIFI_STA);
  vTaskDelay(500);
  mfrc522.PCD_Init(); 
  vTaskDelay(50);
  button1.begin();
  button2.begin();
  button3.begin();
  button4.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);

  //wifiMulti.addAP("steve", "11223344");
  wifiMulti.addAP("Redmi K30 5G Speed", "88888888");
  wifiMulti.addAP("factory-A10-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A10-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A10-1F-1", "qwerasdf");

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
    digitalWrite(LED_PIN,HIGH);
    vTaskDelay(500);
    digitalWrite(LED_PIN,LOW);
    vTaskDelay(500);
    Serial.println("Connecting to WiFi..");
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
    64000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL,             // Task handle
    CONFIG_ARDUINO_RUNNING_CORE
  );
  
  xTaskCreate(
    andonLoop,    // Function that should be called
    "andonLoop",   // Name of the task (for debugging)
    64000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    2,               // Task priority
    NULL             // Task handle
  );
}

void loop() 
{
}

void check_RFID(){
 if ( ! mfrc522.PICC_IsNewCardPresent()) 
  { 
    checkLEDVal=0;
    return;
  }
  // Select a card
  checkLEDVal =1;
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
  rfid = "http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/2:"+ID+":"+content+"";            
  send_rfid();
  tam3=1;
  tam2=0;
  x=x+1;
}
void leader()
{
  tam=1;
  dem=millis();
  dencanbo();
  digitalWrite(LED_PIN,HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN,LOW);
  while(1)
   {
     if (wifiMulti.run() == WL_CONNECTED)
     { //Check the current connection status
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
      if(httpCode==200) break;
      }     
   }
}
void material()
{ 
  denhetlieu();
  tam=1;
  dem=millis();
  while(1)
   {
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
      if(httpCode==200) break;
      }
   }
}
void dead_alive()
{ 
  while(1)
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
      if(httpCode==200) break;
      }
   }
}
void send_rfid()
{ 
   while(1)
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
      if(httpCode==200) break;
      }
      Serial.println(rfid);
   }
}
