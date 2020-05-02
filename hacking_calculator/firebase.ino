String fetchStringData()
{
   if (Firebase.getString(firebaseData,"/User2/chat"))
     {
        if(firebaseData.stringData())
        {
          return firebaseData.stringData();
        }
        else
        {
          return "No Text!";
        }
     }
     else
     {
      Serial.println(firebaseData.errorReason());
     }
}

  void insertData(String value)
  {
    if (Firebase.set(firebaseData, "/User1/chat", value))
    {
      return;
    }
    else
    {
      Serial.println(firebaseData.errorReason());
    }
  }
