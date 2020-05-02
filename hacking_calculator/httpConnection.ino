void fetchTextFile()
{
  text("Fetching..", 1, 0, 3, true, true);

  if (WiFi.status() == WL_CONNECTED)
  {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient http;
    http.begin(*client,textFileUrl);  //Enter your API
    // Specify content-type header
    delay(100);
    int httpResponseCode = http.GET(); //pass a get request
    if (httpResponseCode > 0)
    {
      textFile = http.getString();
      Serial.println(textFile);
    }
    else
    {
      text("failed getting",1,0,0,true,true); 
      delay(1000);
    }
    http.end();   //Close http connection
  }
  else
  {
    textFile = "No link";
  }
}
