#include "FastLED.h"
#include <Tone32.h>
#define BUZZER_PIN 12
#define BUZZER_CHANNEL 0
#include <MFRC522.h>
#include <SPI.h>
#define SS_PIN  21
#define RST_PIN  22
MFRC522 mfrc522(SS_PIN, RST_PIN); 
unsigned long dem=0;
unsigned long dem2=0;
unsigned long dem3=0;
bool tam=0;
bool tam2=0;
#define NUM_LEDS 7
#include <EasyButton.h>
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#include <HTTPClient.h>
#define DATA_PIN 5
//#define BUTTON_PIN1 26
//#define BUTTON_PIN2 25
//#define BUTTON_PIN3 33
//#define BUTTON_PIN4 32

#define BUTTON_PIN1 32
#define BUTTON_PIN2 33
#define BUTTON_PIN3 25
#define BUTTON_PIN4 26
CRGB leds[NUM_LEDS];

#define LED_PIN     GPIO_NUM_27
EasyButton button1(BUTTON_PIN1); 
EasyButton button2(BUTTON_PIN2); 
EasyButton button3(BUTTON_PIN3); 
EasyButton button4(BUTTON_PIN4); 
boolean connectioWasAlive = true;
//const char* ssid = "factory-A17-1F-2";
//const char* password =  "qwerasdf";

//const char* ssid = "factory-A10-1F-3";
//const char* password =  "qwerasdf";
//const char* ssid = "Factory-A11-1F";
//const char* password =  "qwerasdf";

//const char* ssid = "Factory-K1-1F-2";
//const char* password =  "qwerasdf";


//const char* ssid = "Factory-K1-2F-2";
//const char* password =  "qwerasdf";

const char* ssid = "Factory-K1-1F-3";
const char* password =  "qwerasdf";

//const char* ssid = "factory-A15-1F";
//const char* password =  "qwerasdf";


String ID = "K11FA01";

String rfid;
void setup() 
{
  SPI.begin(); 
  mfrc522.PCD_Init(); 
  button1.begin();
  button2.begin();
  button3.begin();
  button4.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
  wifiMulti.addAP("factory-A10-1F-3", "qwerasdf");
  wifiMulti.addAP("factory-A10-1F-2", "qwerasdf");
  wifiMulti.addAP("factory-A10-1F-1", "qwerasdf");
  
  wifiMulti.addAP("factory-A17-1F-2", "qwerasdf");
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
  wifiMulti.addAP("pccguest", "Anh250482");
  
  while (wifiMulti.run() != WL_CONNECTED) 
  {
    digitalWrite(LED_PIN,HIGH);
    delay(100);
    digitalWrite(LED_PIN,LOW);
    delay(100);
    Serial.println("Connecting to WiFi..");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_PIN,LOW);
    denhoatdong();
    receive_order();
    dead_alive();
}

void loop() 
{
 monitorWiFi();
 digitalWrite(LED_PIN,LOW);
 button1.read();
 button2.read();
 button3.read();
 button4.read();
 button1.onPressed(receive_order);
 button2.onPressed(leader);
 button3.onPressed(material);
 button4.onPressed(call_tpm);
 if(millis()-dem2>=5000)
 {  
   if(tam2==1)
   {
      denbaotri();
   }
   else if(tam2==0)
   {
      if(tam==1)
      {
        if(millis()-dem>=10000)
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
  Serial.println(content);
  digitalWrite(LED_PIN,HIGH);
  delay(100);
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
  content.toUpperCase();
  Serial.println(content);
  digitalWrite(LED_PIN,HIGH);
  delay(100);
  digitalWrite(LED_PIN,LOW);
  rfid = "http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/2:"+ID+":"+content+"";            
  send_rfid();
  denhoatdong();
  tam2=0;
}

void leader()
{ beep(); 
  tam=1;
  dem=millis();
  dencanbo();
  digitalWrite(LED_PIN,HIGH);
  delay(100);
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
      delay(1000);
      }     
   }
}
void material()
{ beep(); 
  denhetlieu();
  tam=1;
  dem=millis();
  while(1)
   {
     if (wifiMulti.run() == WL_CONNECTED) 
     { //Check the current connection status
      HTTPClient http;
   
      http.begin("http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/4:"+ID+":123456"); //Specify the URL    3: het chi A11; 4:het nuoc thuoc A10
      int httpCode = http.GET();                                        //Make the request
   
      if (httpCode > 0) 
      { //Check for the returning code
          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload);
          digitalWrite(LED_PIN,HIGH);
          delay(100);
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
          delay(100);
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
{  beep(); 
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
      delay(1000);
   }
}

void beep()
{
  tone(BUZZER_PIN, NOTE_C5, 200, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
}
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
void denhetlieu()
{
    for(int i=0;i<7;i++)
      {
        leds[i] = CRGB(249,244,0);
        FastLED.show();
      } 
}

void monitorWiFi()
{
  while (wifiMulti.run() != WL_CONNECTED)
  {
        if (connectioWasAlive == true)
        {
          connectioWasAlive = false;
          digitalWrite(LED_PIN,HIGH);
          delay(100);
          digitalWrite(LED_PIN,LOW);
          delay(100);
        }
        else if (connectioWasAlive == false)
        {
          connectioWasAlive = true;
          Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
        }
  }
}
//void countdown()
//{ 
//  if(millis()-dem>10000)
//  {
//    if(tam2==1)
//    {
//      denbaotri();
//    }
//    else
//    {
//    tam=0;
//    }
//    dem = millis();
//  }
//}  
