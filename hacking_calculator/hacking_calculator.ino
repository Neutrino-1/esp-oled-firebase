  /*
  The program has 4 errors, which are very easy to fix, either compile and figure out
  or goodluck if you are good with GitHub XD
*/

//Include Firebase ESP8266 library for firebase support
#include "FirebaseESP8266.h"\
//for I2C with OLED
#include <Wire.h>
//Include ESP8266WiFi.h and must be included after FirebaseESP8266.h
#include <ESP8266WiFi.h>
//local webserver
#include<ESP8266WebServer.h>
//connecting to http client
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//for display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//for multitap functionality
#include <OneButton.h>
//for parsing json data
#include <ArduinoJson.h>
//for https links
#include <WiFiClientSecureBearSSL.h>
//spiffs file system
#include <FS.h>

//Enter the realtime database link here
#define FIREBASE_HOST "***"
//Enter the webAPI key from setttings here
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

// Hotspot where your calculator should connect
const char* password = "***";
const char* ssid = "***";

//for selecting the menu
bool selection = false;
bool updateScreen = true;

//To store the file in file system once downloaded from firebase
String textFilePath = "/data.txt";
String textFileUrlPath = "/config.json";

//url where the file should be fetched from can be from firebase storage or any other server
String textFileUrl = "";
//Stores the content of the text file
String textFile = "";

//Preset text, change the array if you want to change the reply messages
String dictionary[] = {"Hey?","Look here","Hide","Careful!","Need help","Not funny","Look out","Behind You","Almost done","Times up"};
int wordCount = 0;

//Scroll in y direction for the text file
int scrollyDir = 0;

//Last sent text from chat
String sentText = "";

//init halleffects sensor to OneButton library
OneButton upButton(12, true);
OneButton downButton(13, true);

//firebaseData object(this contatins the table from realtime database)
FirebaseData firebaseData;

//creating a webserver
ESP8266WebServer server(80);

//configuring the Hotsopt 
IPAddress local_ip(192,168,11,4);
IPAddress gateway(192,168,11,1);
IPAddress netmask(255,255,255,0);

//init the display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

/*for debugging*/
//  Serial.begin(115200);
//  Serial.println("Started!");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  if (!SPIFFS.begin())
  {
    /*for debugging*/
//    Serial.println("SPIFFS initialization failed.");
//    Serial.println("For Arduino IDE, please select flash size from menu Tools > Flash size");
  }

  /*Configuring different types of button inputs*/
  upButton.attachClick(singleTapUp);
  downButton.attachClick(singleTapDown);

  upButton.attachDoubleClick(doubleTapUp);
  downButton.attachDoubleClick(doubleTapDown);

  upButton.attachDuringLongPress(longPressUp);
  downButton.attachDuringLongPress(longPressDown);

  upButton.attachLongPressStop(longPressStopUp);
  downButton.attachLongPressStop(longPressStopDown);

  /*software debouncing*/  
  upButton.setDebounceTicks(80);
  downButton.setDebounceTicks(80);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address ***** for 128x32
      /*for debugging*/
//    Serial.println(F("SSD1306 allocation failed"));
      for (;;); // Don't proceed, loop forever
  }
  
  text("You Cheat!",2,0,0,true,false); 
  delay(2000);
  //read if something in flash memory
  readFlashJson();
  //try to boot the local webserver
  bootServer();
  //read if something in downloads
  readDownloads();
}

void loop() {
  //handles the hosted server
  server.handleClient();
  
  // keep watching the push button:
  upButton.tick();
  downButton.tick();
  
  unsigned long now = millis(); // current (relative) time in msecs.
  
  if (updateScreen) //update screen when necessary
  {
    selectedScreen(); //update the selected screen
    updateScreen = false; //stop updating the screen
  }
}

/*
 *
 *
 *
 *
 For testing
 *
 *
*/
//
//void wifiConnect()
//{
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(500);
//  }
//}
