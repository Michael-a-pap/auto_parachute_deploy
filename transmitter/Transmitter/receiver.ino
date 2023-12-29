//PACKET CONSTRUCTION START,ADDR,COMMAND,DATA,END
/*
const byte addr=0x55; // address
const byte servo_release=0x00; // servo release command
const byte servo_retract=0x04; //servo retract command
const byte gps_cm=0x01; // gps data command
const byte batt_cm=0x02; // battery level comand
const byte beepen=0x03;// beeper enable
*/

void onReceive(int packetsize)
{
  if (packetsize==0) return;
  byte b_addr=LoRa.read(); //read address and put it in a buffer
  byte b_command=LoRa.read();
  String data="";
  boolean offset;

  while (LoRa.available())
  {
    data+=(char)LoRa.read();
  }
  if (b_addr!=addr) return;
  receiver_lost=0; // receiver is active
  receiver_lost_timer=millis(); //timer reset for the receiver, indicated that heartbeat is active
  if (b_command==batt_cm)
  {
    rebatt=data.toFloat();
  }
  else if (b_command==gps_cm)
  {
    if (data.substring(0,3)=="lon")
    {
      if (data.substring(3,4)=="-")
      {
        String sub_data="";
        sub_data=data.substring(3,12);
        longitude=sub_data.toFloat()/1000000.0;
        offset=1;       
      }
      else
      {
        String sub_data="";
        sub_data=data.substring(3,11);
        longitude=sub_data.toFloat()/1000000.0;
        offset=0;
      }
    }
    if (offset==1)
    {
      if (data.substring(12,15)=="lat")
      {
        if(data.substring(15,16)=="-")
        {
          String sub_data="";
          sub_data=data.substring(15,24);
          latitude=sub_data.toFloat()/1000000.0;          
        }
        else
        {
          String sub_data="";
          sub_data=data.substring(15,23);
          latitude=sub_data.toFloat()/1000000.0;
        }
      }
    }
    else
    {
      if (data.substring(11,14)=="lat")
      {
        if(data.substring(14,15)=="-")
        {
          String sub_data="";
          sub_data=data.substring(14,23);
          latitude=sub_data.toFloat()/1000000.0;          
        }
        else
        {
          String sub_data="";
          sub_data=data.substring(14,22);
          latitude=sub_data.toFloat()/1000000.0;
        }
      }
    }
  }
  
  rssi=LoRa.packetRssi();
  Serial.println("longitude");
  Serial.println(longitude,6);
  Serial.println("latitude");
  Serial.println(latitude,6);
  Serial.println(rebatt);
}
