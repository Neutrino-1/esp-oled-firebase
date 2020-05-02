void bootServer()
{
  if(!digitalRead(12))
  {
  WiFi.softAPdisconnect(true);  
  WiFi.disconnect();
  delay(100);
    if (WiFi.status() != WL_CONNECTED)
   {
    text("WEB",2,50,0,true,false); 
    text("192.168.11.4",1,10,17,false,true);
    WiFi.softAPConfig(local_ip, gateway, netmask);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Calculator", "donotcheat"); 
    startServer();
    updateScreen = false;
    }
  }
}

bool wifiConnection()
{
    WiFi.softAPdisconnect(true);  
    WiFi.disconnect();
    delay(100);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) 
        {
          text("CONNECTING",2,0,0,true,true);  
          delay(500);
          if ((unsigned long)(millis() - startTime) >= 10000)
          {
            text("Failed!",2,0,0,true,true); 
            delay(1000);
            return false;
          }
        }
    text("Connected!",2,0,0,true,true); 
    delay(1000);
    if(currentScreen != ASK_HELP)
    {
    currentScreen = MENU;
    updateScreen = true;
    }
    return true;
}
