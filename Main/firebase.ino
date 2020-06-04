//To fetch data from firebase, User2 should be your patners user name
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


//To send data to firebase, User1 should be where you send the data
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
