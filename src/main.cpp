#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>

#include <NTPClient.h>
#include <WiFiUdp.h>


//MAX7912 Definition
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   D5
#define DATA_PIN  D7
#define	CS_PIN		D8

MD_Parola maxx = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//Wifi 

const char *ssid = "AlisWifi_2G";
const char *pass = "8BDZL3GQ59";

//NTP and Time variables

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "cz.pool.ntp.org");


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

  maxx.print("|");
  delay(500);
  maxx.print("|-");
  delay(500);
  maxx.print("|-|");
  delay(500);
  maxx.print("|-|-");
  delay(500);
  maxx.print("|-|-|");
  delay(500);
  maxx.print("|-|-|-");
  delay(500);
  maxx.print("|-|-|-|");
  delay(500);
  maxx.print("|-|-|-|-");
  delay(500);
  maxx.print("|-|-|-|-|");
  delay(1000);

  maxx.displayClear();
  maxx.print("WiFi:");

  delay(2000);
  maxx.setIntensity(2);
  delay(2000);
  maxx.setIntensity(5);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    maxx.print("WiFi: |");
    delay(500);
    maxx.print("WiFi:--");
  }
  maxx.setTextAlignment(PA_CENTER);
  delay(1000);
  maxx.print("NTP");
  maxx.setIntensity(3);

  timeClient.begin();

  timeClient.setUpdateInterval(60000);
  timeClient.setTimeOffset(7200);           //3600 zimni //7200 letni

}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > loopinterval) {
    
    timeClient.update();
    print_time();
    brightness();
    previousMillis = currentMillis;

  }

}

void print_time() {
  hour = timeClient.getHours();
  minute = timeClient.getMinutes();

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