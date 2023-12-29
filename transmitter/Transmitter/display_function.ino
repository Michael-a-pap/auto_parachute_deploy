void show_disp_time(int timer)
{
  int x=22; //offset
  int y=25; //offset
  int hours;
  int mins;
  int secs;
  hours=timer/3600;
  mins=timer/60 -(hours*60);
  secs=timer-(timer/60)*60;
 
  if(secs<10)
  {
    display.setCursor((28+x),(0+y));
    display.println('0');
    display.setCursor((34+x),(0+y));
    display.println(secs);
  }
  else
  {
    display.setCursor(28+x,0+y);
    display.println(secs);
  }
  display.setCursor(10+x,0+y);
  display.println(':');
  if(mins<10)
  {
    display.setCursor((14+x),(0+y));
    display.println('0');
    display.setCursor((20+x),(0+y));
    display.println(mins);
  }
  else
  {
    display.setCursor(14+x,0+y);
    display.println(mins);
  }
  if(hours<10)
  {

    display.setCursor((0+x),(0+y));
    display.println('0');
    display.setCursor((6+x),(0+y));
    display.println(hours);
  }
  else
  {
    display.setCursor(0+x,0+y);
    display.println(hours);
  }
  display.setCursor(24+x,0+y);
  display.println(':');

}

void battery_indicator()
{
  
  //13x8px
  static const unsigned char PROGMEM full_battery [] = 
   {0xc0, 0x30, 0x80, 0x10, 0xb6, 0xd8, 0xb6, 0xd8, 0xb6, 0xd8, 0xb6, 0xd8, 0x80, 0x10, 0xc0, 0x30};
  static const unsigned char PROGMEM mid_battery [] = 
   {0xc0, 0x30, 0x80, 0x10, 0xb6, 0x18, 0xb6, 0x18, 0xb6, 0x18, 0xb6, 0x18, 0x80, 0x10, 0xc0, 0x30};
  static const unsigned char PROGMEM low_battery [] = 
   {0xc0, 0x30, 0x80, 0x10, 0xb0, 0x18, 0xb0, 0x18, 0xb0, 0x18, 0xb0, 0x18, 0x80, 0x10, 0xc0, 0x30};
  static const unsigned char PROGMEM empty_battery [] = 
   {0xc0, 0x30, 0x80, 0x10, 0x80, 0x18, 0x80, 0x18, 0x80, 0x18, 0x80, 0x18, 0x80, 0x10, 0xc0, 0x30};
   
  float r2=30; // divider resistor value
  float r1=100; // divider resistor value
  float bvolt;
//--------------internal_battery--------------------------
  bvolt=((1.0/1024.0)*(1.0/(r2/(r2+r1)))*analogRead(A0));// battery voltage

  if (bvolt>=4)
  {
    display.drawBitmap(71,0,full_battery,13,8,BLACK);
  }
  else if (bvolt>=3.9)
  {
    display.drawBitmap(71,0,mid_battery,13,8,BLACK);
  }
  else if(bvolt>=3.6)
  {
    display.drawBitmap(71,0,low_battery,13,8,BLACK);
  }
  else
  {
    display.drawBitmap(71,0,empty_battery,13,8,BLACK);
  }
//--------------internal_battery_end--------------------------
//--------------receiver_battery------------------------------
  bvolt=((1.0/1024.0)*(1.0/(r2/(r2+r1)))*rebatt);// battery voltage
  if (bvolt>=4)
  {
    display.drawBitmap(57,0,full_battery,13,8,BLACK);
  }
  else if (bvolt>=3.9)
  {
    display.drawBitmap(57,0,mid_battery,13,8,BLACK);
  }
  else if(bvolt>=3.6)
  {
    display.drawBitmap(57,0,low_battery,13,8,BLACK);
  }
  else if (bvolt>1)
  {
    display.drawBitmap(57,0,empty_battery,13,8,BLACK);
  }
 // Serial.println(bvolt);
//---------------receiver_battery_end------------------------
}

void signal_strength()
{
  //signal_strength 10x8px
  static const unsigned char PROGMEM full_signal [] = 
   {0x00, 0x00, 0x00, 0xc0, 0x00, 0xc0, 0x06, 0xc0, 0x06, 0xc0, 0x36, 0xc0, 0x36, 0xc0, 0xb6, 0xc0};
  static const unsigned char PROGMEM mid_signal [] = 
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x36, 0x00, 0x36, 0x00, 0xb6, 0x00};
  static const unsigned char PROGMEM low_signal [] = 
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0xb0, 0x00};
  static const unsigned char PROGMEM no_signal [] = 
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00};

  if(rssi>-80)
  {
    display.drawBitmap(0,0,full_signal,10,8,BLACK);
  }
  else if (rssi>-90)
  {
    display.drawBitmap(0,0,mid_signal,10,8,BLACK);
  }
  else if (rssi>-115)
  {
    display.drawBitmap(0,0,low_signal,10,8,BLACK);
  }
  else 
  {
    display.drawBitmap(0,0,no_signal,10,8,BLACK);
  }

  if (receiver_lost==1)  //check whether the receiver is active and display it's status
  {
    display.setCursor(0,40); 
    display.println("Lost Receiver");
  }
   
}
