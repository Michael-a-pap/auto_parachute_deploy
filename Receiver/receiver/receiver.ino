
#include <SPI.h>
#include <LoRa.h>
#include<TinyGPS.h>
#include <Servo.h>
int addr=0x55;
byte data=0;
boolean flag=0;
//variables for gps
long lat =12345678,lon =12345678;
unsigned long pmillis;
TinyGPS gps;
Servo myservo;


void setup() 
{
  Serial.begin(9600);
  LoRa.begin(433E6);
  LoRa.setTxPower(20);
  myservo.attach(3);
  analogReference(INTERNAL);
 
}

void loop()
{
  if(millis()-pmillis>=10000)
  {
    pmillis=millis();
    flag=1;
    LoRa.beginPacket();
    LoRa.write(addr);
    LoRa.write(0x02);
    LoRa.print(analogRead(A7));//batt reading
    LoRa.endPacket(); 
  }
    while (Serial.available())
    {
      if(gps.encode(Serial.read())&&flag==1 )
      {
        gps.get_position(&lat,&lon);
        LoRa.beginPacket();
        LoRa.write(addr);
        LoRa.write(0x01);
        LoRa.print("lon");
        LoRa.print(lon);
        LoRa.print("lat");
        LoRa.print(lat);
        LoRa.endPacket();
        flag=0;
      }
    }


 // myservo.write(90);
  onReceive(LoRa.parsePacket());  
}

void onReceive(int packetsize)
{
  if (packetsize==0) return;
  byte b_addr=LoRa.read();
  byte b_command=LoRa.read();
  
    while (LoRa.available()) 
    {
     data=LoRa.read();
    }
    
    //if(b_addr!=addr) return;
    if(b_command==0x00) // servo release
    {
        myservo.write(90);
    }
    else if (b_command==0x04) //servo retract 
    {
      myservo.write(0);
    }
    else if (b_command==0x03)
    {
      //enable beep in the future 
    }
    

}
  
