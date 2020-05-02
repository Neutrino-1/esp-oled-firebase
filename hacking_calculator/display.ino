//Drawing text on OLED
void text(String s,int size,int cX,int cY,bool clear,bool inverse)
{
  if(clear)
  {
    display.clearDisplay();
  }
  if(inverse)
  {
    display.setTextColor(BLACK,WHITE);
  }
  else
  {
    display.setTextColor(WHITE);
  }
    display.setTextSize(size);
    display.setCursor(cX,cY); 
    display.println(s);
    display.display();    
}

void menu()
{
    text("Download",2,0,0,true,!selection);
    text("Ask Help",2,0,17,false,selection);
}

void dowloadsScreen()
{
    text(textFile,1,0,scrollyDir,true,false);
    delay(250);
}

void HelpScreen()
{
    text(fetchStringData(),2,0,0,true,false);
    text(sentText,1,128-(sentText.length()*6),0,false,false);
    text(dictionary[wordCount],2,0,17,false,true);
    delay(250);
}

void selectedScreen()
{
  switch(currentScreen)
  {
    case MENU:
      menu()
      break;
    case DOWNLOADS:
      dowloadsScreen();
      break;
    case ASK_HELP:
      HelpScreen();
      break;
    default:
      text("something we wrong!",1,0,0,true,false);   
  }  
}
