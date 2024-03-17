#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"
#define ON 0
#define OFF 1
DateTime now;
 
RTC_DS3231 rtc;
LiquidCrystal lcd(8,7, 6, 5, 4, 3 ); // (rs, e, d4, d5, d6, d7)
 
const int led=11;
int nob = A1;
int val = 0;
int check=1;

 
int on_hour=2;
int on_minute=01;
int on_second=00;
 
int off_hour=1;
int off_minute=59;
int off_second=10;
 
int c_hour=00;
int c_minute=00;
int c_second=00;
 
int onOrOffFlag = ON;
 

void showTime(void);
void loadHandler(int, int , int , int , int , int , int , int , int );

typedef struct userTime
{
int temp_hour;
int temp_minute;
int temp_second;
}userTime_t;
unsigned char checkLessThanOrEqual(userTime_t , userTime_t);
 
void setup ()
{
Serial.begin(9600);
lcd.begin(16,2);
 

pinMode(led,OUTPUT);

 
if (! rtc.begin())
{
Serial.println("Couldn't find RTC Module");
while (1);
}
 
if (rtc.lostPower())
{
Serial.println("RTC lost power, lets set the time!");
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

lcd.setCursor(0,0);
lcd.print(" RTC ");
lcd.setCursor(0,1);
lcd.print(" MODE ON ");
delay(500);


lcd.setCursor(0,0);
lcd.print(" LDR ");
lcd.setCursor(0,1);
lcd.print(" MODE ON");
delay(500);

now = rtc.now();
showTime();
c_hour=now.hour();
c_minute=now.minute();
c_second=now.second();
loadHandler( on_hour, on_minute, on_second, off_hour, off_minute, off_second, c_hour, c_minute, c_second);
}
 
void loop ()
{

if (check==0)
{
  lcd.setCursor(0,0);
  lcd.print("    ");  
  now = rtc.now();
  showTime();
  c_hour=now.hour();
  c_minute=now.minute();
  c_second=now.second();
  loadHandler( on_hour, on_minute, on_second, off_hour, off_minute, off_second, c_hour, c_minute, c_second);
  analogWrite(led,0);
}
else
{
 val = analogRead(nob);
if(val>300 && val<450)
{
lcd.setCursor(0,0);
lcd.print("25% ");
analogWrite(led, 400);
}
else if(val>450 && val<600)
{
lcd.setCursor(0,0);
lcd.print("50% ");
analogWrite(led, 600);
}
else if(val>600 && val<750 )
{
lcd.setCursor(0,0);
lcd.print("75% ");;
analogWrite(led, 800);
}
else if(val < 150&& val<300)
{
lcd.setCursor(0,0);
lcd.print("0% ");
analogWrite(led, 1020);
}
else if(val>800)
{
lcd.setCursor(0,0);
lcd.print("100% ");
analogWrite(led, 0);
}
}
now = rtc.now();
showTime();
c_hour=now.hour();
c_minute=now.minute();
c_second=now.second();
loadHandler( on_hour, on_minute, on_second, off_hour, off_minute, off_second, c_hour, c_minute, c_second);
}

 
void showTime()
{
lcd.setCursor(4,0);
lcd.print("Time:");
lcd.print(now.hour());
lcd.print(':');
lcd.print(now.minute());
lcd.print(':');
lcd.print(now.second());
lcd.print(" ");
}
 

unsigned char checkLessThanOrEqual(userTime_t a, userTime_t b)
{
  if(a.temp_hour < b.temp_hour)
    return true;
  else if ((a.temp_hour == b.temp_hour) && (a.temp_minute < b.temp_minute))
    return true;
  else if(a.temp_hour > b.temp_hour)
    return false;
  else if((a.temp_minute == b.temp_minute) && (a.temp_second < b.temp_second))
    return true;
  else if(a.temp_minute > b.temp_minute)
    return false;
  else  if(a.temp_second == b.temp_second)
  return true;
  else
  return false;
}
 
void loadHandler(int onTimeHr, int onTimeMin, int onTimeSec, int offTimeHr, int offTimeMin, int offTimeSec, int rtcTimeHr, int rtcTimeMin, int rtcTimeSec)
{
 
userTime_t in1 = {onTimeHr, onTimeMin, onTimeSec}, in2 = {offTimeHr, offTimeMin, offTimeSec}, rtc_hr = {rtcTimeHr, rtcTimeMin, rtcTimeSec}, a = {}, b = {};
 
if(checkLessThanOrEqual(in1, in2))
{
onOrOffFlag = ON;
memcpy(&a, &in1, sizeof(userTime_t));
memcpy(&b, &in2, sizeof(userTime_t));
}
else
{
onOrOffFlag = OFF;
memcpy(&a, &in2, sizeof(userTime_t));
memcpy(&b, &in1, sizeof(userTime_t));
 
}

 
if((checkLessThanOrEqual(a, rtc_hr)) && (checkLessThanOrEqual(rtc_hr, b)))
{
if(onOrOffFlag == ON)
{
// Switch on the load

lcd.setCursor(1,2);
lcd.print("OffTime:");
lcd.print(off_hour);
lcd.print(':');
lcd.print(off_minute);
lcd.print(':');
lcd.print(off_second);
check=1;
 
 
}
else
{
// Switch off the load

lcd.setCursor(1,2);
lcd.print(" OnTime:");
lcd.print(on_hour);
lcd.print(':');
lcd.print(on_minute);
lcd.print(':');
lcd.print(on_second);
check=0;
 
}
}
else
{
if(onOrOffFlag == ON)
{
// Switch off the load

lcd.setCursor(1,2);
lcd.print(" OnTime:");
lcd.print(on_hour);
lcd.print(':');
lcd.print(on_minute);
lcd.print(':');
lcd.print(on_second);
check=0;
}
else
{
// Switch on the load

lcd.setCursor(1,2);
lcd.print("OffTime:");
lcd.print(off_hour);
lcd.print(':');
lcd.print(off_minute);
lcd.print(':');
lcd.print(off_second);
check=1;
}
}
}












