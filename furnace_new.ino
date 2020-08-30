#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Wire.h>
#include "RTClib.h"

#define right_forward 14
#define right_reverse 13
#define left_forward 15
#define left_reverse 12
#define testbutton 2


void valve_off();

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Time t;
int counter = 0;
int psec =0;
int csec =0;
bool taskComp  = 0;

enum
{
 start,
 stop
}mtorstatus;

void setup() {
// put your setup code here, to run once:
Serial.begin(115200);
WiFiManager wifiManager;
Serial.println("Conecting.....");
wifiManager.autoConnect("SIGNTEK","12345678");
Serial.println("connected");

  delay(3000); // wait for console opening

//  if (! rtc.begin()) {
//    Serial.println("Couldn't find RTC");
   // while (1);
 // }

  while(! rtc.begin()){
    Serial.println("Couldn't find RTC");
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }

  pinMode(testbutton, INPUT_PULLUP);
  pinMode(right_forward, OUTPUT);
  pinMode(right_reverse, OUTPUT); 
  pinMode(left_forward, OUTPUT); 
  pinMode(left_reverse, OUTPUT);
  // turn all the motors off
  digitalWrite(right_forward, LOW);
  digitalWrite(right_reverse, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_reverse, LOW);


}


void loop() {  

DateTime now = rtc.now();

if(digitalRead(testbutton)){
  motorStop();
}
if(!digitalRead(testbutton)){
  motorStart();
}

//if(digitalRead(testbutton2)){
//  motorRun_SixSEC();
//}


//t = rtc.getTime();
//if(digitalRead(testbutton)){valve_on();}
//if(!digitalRead(testbutton)){}


//Serial.println(t.min);
}




void valve_off(){
digitalWrite(right_forward, LOW);
digitalWrite(right_reverse, LOW);
digitalWrite(left_forward, LOW);
digitalWrite(left_reverse, HIGH);
 delay(20);
 
digitalWrite(right_forward, LOW);
digitalWrite(right_reverse, LOW);
digitalWrite(left_forward, HIGH);
digitalWrite(left_reverse, LOW);
 delay(20);
 
digitalWrite(right_forward, LOW);
digitalWrite(right_reverse, HIGH);
digitalWrite(left_forward, LOW);
digitalWrite(left_reverse, LOW);
 delay(20);
 
digitalWrite(right_forward, HIGH);
digitalWrite(right_reverse, LOW);
digitalWrite(left_forward, LOW);
digitalWrite(left_reverse, LOW);
 delay(20);


}



void valve_on(){

  
digitalWrite(right_forward, HIGH);
digitalWrite(right_reverse, LOW);
digitalWrite(left_forward, LOW);
digitalWrite(left_reverse, LOW);
 delay(20);
 
digitalWrite(right_forward, LOW);
digitalWrite(right_reverse, HIGH);
digitalWrite(left_forward, LOW);
digitalWrite(left_reverse, LOW);
 delay(20);
 
digitalWrite(right_forward, LOW);
digitalWrite(right_reverse, LOW);
digitalWrite(left_forward, HIGH);
digitalWrite(left_reverse, LOW);
 delay(20);
 
digitalWrite(right_forward, LOW);
digitalWrite(right_reverse, LOW);
digitalWrite(left_forward, LOW);
digitalWrite(left_reverse, HIGH);
 delay(20);


}


void motorStart(){
  DateTime now = rtc.now();
csec=now.second();
if(psec != csec && mtorstatus== start)
{
psec =csec;
 counter++;
  Serial.println(counter, DEC);
 if(counter == 6)
 {
  mtorstatus=stop;
 counter = 0;
 psec=0;
 }
}



if(mtorstatus == stop && taskComp == 0)
{
//valve_off();
 taskComp=1;
 //Serial.println(csec);
// Serial.println(now.second(), DEC);
 Serial.println("Stop Motor");
 counter=0;
 psec=0;
}else if(mtorstatus == start && taskComp == 0) 
{
  valve_on();
  }
}



void motorStop(){
  DateTime now = rtc.now();
csec=now.second();
Serial.println(counter, DEC);
if(psec != csec && mtorstatus== start)
{
psec =csec;
 counter++;
 
  
 if(counter == 6)
 {
  mtorstatus=stop;
 counter = 0;
 }
}


if(mtorstatus == stop && taskComp == 0)
{
//valve_off();
 taskComp=1;
 //Serial.println(csec);
// Serial.println(now.second(), DEC);
 Serial.println("Stop Motor");
 counter=0;
 psec=0;
}
else if(mtorstatus == start && taskComp == 0) 
{
  valve_off();
  }
}

