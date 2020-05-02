void writeDownloads()
{
  File spiffTextFile = SPIFFS.open(textFilePath, "w");
  if(spiffTextFile)
  {
    spiffTextFile.print(textFile);
    spiffTextFile.close();  
  }
}

void readDownloads()
{
  File spiffTextFile = SPIFFS.open(textFilePath, "r");
  if(spiffTextFile)
  {
    int i;
    for(i=0;i<spiffTextFile.size();i++) //Read upto complete file size
      {
        textFile += (char)spiffTextFile.read();
      }
      spiffTextFile.close();  //Close file
  }
}

void readFlashJson()
{
    if(SPIFFS.exists(textFileUrlPath))
  {
    const char* _url = "";
    File configFile = SPIFFS.open(textFileUrlPath,"r");
    if(configFile)
    {
      Serial.println("Reading to file");
      //getting the size of the file
      size_t size = configFile.size();
      //using buffer to get the data
      std::unique_ptr<char[]> buf(new char[size]);
      //store it in buffer
      configFile.readBytes(buf.get(), size);
      //closing the file
      configFile.close();
      //json buffer to store the json
      const size_t capacity = JSON_OBJECT_SIZE(1);
      //json buffer to store the json
      DynamicJsonDocument doc(capacity);
      //parsing the json object
      DeserializationError error = deserializeJson(doc,buf.get());
       if(!error)
      {
        _url = doc["link"];
        textFileUrl = _url;
        Serial.println(textFileUrl);
        Serial.println();
      }
    }
  }
}
