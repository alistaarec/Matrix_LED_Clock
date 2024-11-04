#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>
#include <EEPROM.h>

#include <NTPClient.h>
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
NTPClient ntp(ntpUDP, "0.cz.pool.ntp.org", 0, 600000);


String minutestr;
String hourstr;
String secostr;

unsigned long hour;
unsigned long minute;
unsigned long second;

bool isDST = false;
const int EEPROM_ADDR = 0;

//looptime variables

unsigned long loopinterval = 1000;
unsigned long previousMillis = 0;

//void definitons

void print_time();
void brightness();
void updateTimeOffset();

//auto brightness definitons

const int photo_sens = A0;
int sensData = 0;

//------------------//

void updateTimeOffset() {
  // Adjust for Standard or Daylight Saving Time
  if (isDST) {
    ntp.setTimeOffset(7200);  // DST = UTC + 1 hour
  } else {
    ntp.setTimeOffset(3600);     // Standard Time = UTC
  }
}

void handleRoot(AsyncWebServerRequest *request) {
  String html = R"=====( 
<!DOCTYPE html>
<html>
<head>
  <title>NTP Hodiny</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      background-color: #f0f0f5;
      margin: 0;
    }
    .container {
      text-align: center;
      background-color: #ffffff;
      padding: 30px;
      border-radius: 10px;
      box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);
      width: 300px;
    }
    h1 {
      color: #333;
    }
    #time {
      font-size: 1.5em;
      margin: 20px 0;
      color: #666;
    }
    .button {
      padding: 10px 20px;
      font-size: 16px;
      border: none;
      border-radius: 5px;
      background-color: #007bff;
      color: white;
      cursor: pointer;
      margin-top: 15px;
    }
    .button:hover {
      background-color: #0056b3;
    }
  </style>
  <script>
    function fetchTime() {
      fetch('/time')
        .then(response => response.text())
        .then(data => {
          document.getElementById('time').innerText = data;
        });
    }

    function toggleTimeMode() {
      fetch('/toggle')
        .then(() => setTimeout(fetchTime, 500));
    }

    setInterval(fetchTime, 1000);  // Update time every second
    window.onload = fetchTime;     // Initial fetch
  </script>
</head>
<body>
  <div class="container">
    <h1>NTP Hodiny Nastaveni</h1>
    <p id="time">Nacitam</p>
    <button class="button" onclick="toggleTimeMode()">Prepnout cas - 
      <span id="toggleMode">Standard Time</span>
    </button>
  </div>
</body>
</html>
)=====";
  request->send(200, "text/html", html);
}

void handleToggleTime(AsyncWebServerRequest *request) {
  // Toggle between DST and Standard Time
  isDST = !isDST;
  updateTimeOffset();

  // Save the new DST setting to EEPROM
  EEPROM.write(EEPROM_ADDR, isDST ? 1 : 0);
  EEPROM.commit();

  request->send(200, "text/plain", isDST ? "Daylight Saving Time" : "Standard Time");
}

void handleTime(AsyncWebServerRequest *request) {
  String currentTime = ntp.getFormattedTime();
  request->send(200, "text/plain", currentTime + " (" + (isDST ? "DST" : "Standard") + ")");
}

//--------------------------//

void setup() {

  //Serial.begin(9600);

  EEPROM.begin(512);

  isDST = EEPROM.read(EEPROM_ADDR) == 1;

  maxx.begin();
  delay(200);
  maxx.displayClear();

  maxx.displayClear();
  maxx.setTextAlignment(PA_CENTER);
  maxx.print("--:--");
  maxx.setIntensity(2);
  delay(2000);

  AsyncWiFiManager wifiManager(&server, &dns);

  wifiManager.autoConnect("Hodiny_ESP");

  maxx.setIntensity(3);

  ntp.begin();
  updateTimeOffset();

  // Configure web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_GET, handleToggleTime);
  server.on("/time", HTTP_GET, handleTime);
  server.begin();

}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > loopinterval) {
    
    
    print_time();
    brightness();
    previousMillis = currentMillis;

  }

  ntp.update();

}

void print_time() {
  hour = ntp.getHours();
  minute = ntp.getMinutes();
  
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

  if (hour >= 16) {
    maxx.setIntensity(0);
  }

  if (hour <= 7) {
    maxx.setIntensity(6);
  }

}

