/*
  The program has 4 errors, which are very easy to fix, either compile and figure out
  or goodluck if you are good with GitHub XD
*/

//1. Include Firebase ESP8266 library (this library)
#include "FirebaseESP8266.h"
#include <Wire.h>
//2. Include ESP8266WiFi.h and must be included after FirebaseESP8266.h
#include <ESP8266WiFi.h>
//webserver 
#include<ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneButton.h>
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>
#include <FS.h>

#define FIREBASE_HOST "***"
#define FIREBASE_AUTH "***"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

typedef enum {
  MENU,        //Menu screen.
  DOWNLOADS,   //Downloaded files screen
  ASK_HELP,    //Ask help screen
}
Screen;

Screen currentScreen = MENU;

const char* password = "***";
const char* ssid = "***";

bool selection = false;
bool updateScreen = true;

String textFilePath = "/data.txt";
String textFileUrlPath = "/config.json";

String textFileUrl = "";
String textFile = "";

String dictionary[] = {"Hey?","Look here","Hide","Careful!","Need help","Not funny","Look out","Behind You","Almost done","Times up"};
int wordCount = 0;

int scrollyDir = 0;

String sentText = "";

OneButton upButton(12, true);
OneButton downButton(13, true);

FirebaseData firebaseData;

//creating a webserver
ESP8266WebServer server(80);

//configuring the Hotsopt 
IPAddress local_ip(192,168,11,4);
IPAddress gateway(192,168,11,1);
IPAddress netmask(255,255,255,0);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

  Serial.begin(115200);
  Serial.println("Started!");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS initialization failed.");
    Serial.println("For Arduino IDE, please select flash size from menu Tools > Flash size");
  }

  upButton.attachClick(singleTapUp);
  downButton.attachClick(singleTapDown);

  upButton.attachDoubleClick(doubleTapUp);
  downButton.attachDoubleClick(doubleTapDown);

  upButton.setDebounceTicks(80);
  downButton.setDebounceTicks(80);

  upButton.attachDuringLongPress(longPressUp);
  downButton.attachDuringLongPress(longPressDown);

  upButton.attachLongPressStop(longPressStopUp);
  downButton.attachLongPressStop(longPressStopDown);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address ***** for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  text("You Cheat!",2,0,0,true,false); 
  delay(2000);
  readFlashJson();
  bootServer();
  readDownloads();
}

void loop() {
  server.handleClient();
  // keep watching the push button:
  upButton.tick();
  downButton.tick();
  unsigned long now = millis(); // current (relative) time in msecs.
  if (updateScreen)
  {
    selectedScreen();
    updateScreen = false;
  }
}

void wifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}
