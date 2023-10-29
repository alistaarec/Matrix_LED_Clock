#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>

#include <NTP.h>
#include <WiFiUdp.h>

#include <ESPAsyncWiFiManager.h>
#include <ESPAsyncWebServer.h>

//MAX7912 Definition
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   D5
#define DATA_PIN  D7
#define	CS_PIN		D8

MD_Parola maxx = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//Wifi/server 
DNSServer dns;
AsyncWebServer server (80);


//NTP and Time variables

WiFiUDP ntpUDP;
NTP ntp(ntpUDP);


String minutestr;
String hourstr;

unsigned long hour;
unsigned long minute;

//looptime variables

unsigned long loopinterval = 2000;
unsigned long previousMillis = 0;

//void definitons

void print_time();
void brightness();

//auto brightness definitons

const int photo_sens = A0;
int sensData = 0;

//------------------//
void setup() {

  //Serial.begin(9600);

  maxx.begin();
  delay(200);
  maxx.displayClear();
  maxx.setTextAlignment(PA_LEFT);


  maxx.print("- ");
  delay(500);
  maxx.print("- -");
  delay(500);
  maxx.print("- - -");
  delay(500);
  maxx.print("- - - - ");
  delay(500);
  maxx.print("- - - - -");
  delay(1000);

  maxx.displayClear();
  maxx.setTextAlignment(PA_CENTER);
  maxx.print("--:--");
  maxx.displayAnimate();
  delay(2000);
  maxx.setIntensity(2);
  delay(2000);
  maxx.setIntensity(5);

  AsyncWiFiManager wifiManager(&server, &dns);

  wifiManager.autoConnect("Hodiny");



  maxx.setIntensity(3);

  ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120);
  ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60);

  ntp.begin();


}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > loopinterval) {
    
    ntp.update();
    print_time();
    brightness();
    previousMillis = currentMillis;

  }

}

void print_time() {
  hour = ntp.hours();
  minute = ntp.minutes();

  if (minute < 10) {
    minutestr = ("0" + String(minute));
  }
  else {
    minutestr = String(minute);
  }

  if (hour < 10) {
    hourstr = ("0" + String(hour));
  }
  else {
    hourstr = String(hour);
  }
  

  String str = hourstr + ":" + minutestr;

  maxx.print(str);

}

void brightness() {

  //sensData = analogRead(photo_sens);

  //sensData = map(sensData, 0, 600, 3, 12);

  //maxx.setIntensity(sensData);

  if (hour == 17) {
    maxx.setIntensity(0);
  }

  if (hour == 7) {
    maxx.setIntensity(3);
  }

}