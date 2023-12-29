#include <SPI.h>               // include SPI library
#include <LoRa.h>              //lora library
#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 (Nokia 5110) library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "Jk1001"
#define APPSK  "yolo3000"
#endif

//Objects
Adafruit_PCD8544 display = Adafruit_PCD8544(2, 5, 15);// D/C,CS,RST 
ESP8266WebServer server(80);


/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

//Global variables
unsigned long pmillis=0;
unsigned int prefreshrate=0;
unsigned long timer=0; //timer 
int timerreset; // timer reset buffer var
float rebatt=0.0; // value of the adc ,receiver battery
float longitude,latitude; //gps longitude latitude 
int rssi=-200; // signal strenght
//switches
int sw1=4; //Left switch
int sw2=0; //Right switch
//flags
boolean sw1state;
boolean sw2state;
boolean psw1=0;
boolean psw2=0;
boolean psw1flag;
boolean psw2flag;
unsigned long sw1debouncereset;
unsigned long debouncereset;
boolean sw1laststate;
boolean sw2laststate;
boolean receiver_lost=1; // if the communication with the receiver is lost this is 1
boolean cm_send; //whether the command is send or not
//settings
int debouncedelay=70;
int lcdrefreshrate=100;
//Lora communication 0x01!!
const byte addr=0x55; // address
const byte servo_release=0x00; // servo release command
const byte servo_retract=0x04; //servo retract command
const byte gps_cm=0x01; // gps data command
const byte batt_cm=0x02; // battery level comand
const byte beepen=0x03;// beeper enable


int testmillis=0; //restrict the excecution in the loop
int receiver_lost_timer;

 
void setup() {
  //delay(1000);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  Serial.begin(115200);
  LoRa.setPins(9, 16, 10); //LoRa.setPins(csPin, resetPin, irqPin)
  LoRa.begin(433E6);
  LoRa.setTxPower(20);
  //wifi ap parameters
  ESP.eraseConfig();
 // WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid);
 // WiFi.begin(ssid,password);
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  //settings for display
  display.begin();
  display.setContrast(70);
  display.clearDisplay();
  display.setTextSize(1.8);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("testing");
  display.display();
  /*
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(500e3);
  LoRa.setCodingRate4(5); */
  delay(1000);
}

void loop() {
  if((millis()-testmillis)>=30) // run the code every 30ms
  {
    testmillis=millis();
  
  


  if(psw1==0&&cm_send==1) //send retrack servo command
  {
    cm_send=0;
    LoRa.beginPacket();
    LoRa.write(addr);
    LoRa.write(servo_retract);
    LoRa.endPacket();
  } 
  else if (psw1==1&& cm_send==0) //send release servo command
  {
    cm_send=1;
    LoRa.beginPacket();
    LoRa.write(addr);
    LoRa.write(servo_release);
    LoRa.endPacket();
  }
//--------toggle_sw2------------------
   if(sw2state==0 && psw2flag==1) 
   {
    psw2=!psw2;   //toggle psw2 
    psw2flag=0;
   }
   else if (sw2state==1 && psw2flag==0)
   {
    psw2flag=1;
   }
//--------toggle_end_sw2------------------
//--------toggle_sw1------------------
   if(sw1state==0 && psw1flag==1) 
   {
    psw1=!psw1;   //toggle psw1 
    psw1flag=0;
   }
   else if (sw1state==1 && psw1flag==0)
   {
    psw1flag=1;
   }
//--------toggle_end_sw1------------------

//--------timer-------------------------
    if(millis()-pmillis>=1000 && psw2==1) //timer start stop
    {
      pmillis=millis();
      timer++;
    }
  show_disp_time(timer);
   if (sw2state==1) timerreset=millis(); // enable the reset after a long press
   if (millis()-timerreset>=3000)//reset the timer after 3s
   {
     timer=0;
     psw2=0; 
   }
//----------timer_end-------------------
  battery_indicator(); //read and display the battery level
  onReceive(LoRa.parsePacket()); //receiver always listening
  signal_strength(); //parse and display the signal strength
  server.handleClient();
  //WiFi.forceSleepWake();
  
//------------------display_refresh----------------------
  if((millis()-prefreshrate)>lcdrefreshrate)
  {
    prefreshrate=millis();    
   // display.clearDisplay();
   display.display();
  }
  else
  {
    //display.display();
   display.clearDisplay();
  }
//-----------------display_refresh_end--------------------- 
  if (millis()-receiver_lost_timer>=20000)
  {
    receiver_lost=1;
  }
 }
 
 //----------------------debounce_sw1-------------------
  int sw1reading=digitalRead(sw1);
  if (sw1reading!=sw1laststate)
  {
    debouncereset=millis();
  }
  if ((millis()-sw1debouncereset)>debouncedelay)
  { 
    if (sw1reading !=sw1state)
    {
      sw1state=sw1reading;
    }  
  }
  sw1laststate=sw1reading;
//---------------------------debounce_end_sw1---------------
 
//----------------------debounce_sw2-------------------
  int sw2reading=digitalRead(sw2);
  if (sw2reading!=sw2laststate)
  {
    debouncereset=millis();
  }
  if ((millis()-debouncereset)>debouncedelay)
  { 
    if (sw2reading !=sw2state)
    {
      sw2state=sw2reading;
    }  
  }
  sw2laststate=sw2reading;
//---------------------------debounce_end_sw2---------------
 onReceive(LoRa.parsePacket());
 server.handleClient();
  
}
