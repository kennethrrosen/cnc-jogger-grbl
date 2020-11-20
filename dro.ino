// DRO



void readSerialGRBBL()              // GRBL serial
{
  while (Serial1.available())
   {
    char MinChar = (char)Serial1.read();                    // Char received from GRBL
    Serial.write(MinChar);   // read it and send it out Serial1 (pins 0 & 1)

    if (MinChar == '\n')
    {
      grblStringAvailable = true;
    }
    else
    {
      if (MinChar >= ' ') {grblStringRx += MinChar;}      // >= ' ' to avoid not wanted ctrl char.
    }
  }
}


void grblStringParse(String grblStat)
{
  
  //<Idle|MPos:-8.234,11.438,0.463|FS:0,0>
  //<Idle|MPos:-8.234,11.438,0.463|FS:0,0|WCO:-11.000,9.000,0.000>
  //<Idle|MPos:-8.234,11.438,0.463|FS:0,0|Ov:100,100,100>

  grblStat.toUpperCase();

  int startStatPos = grblStat.indexOf("<");
  int endStatPos = grblStat.indexOf(">");
  // Clean received data
  grblStat = grblStat.substring(startStatPos+1, endStatPos-0);

  grblStat.replace("MPOS:",",");
  if (grblStat.indexOf("|OV:"))
  {
      grblStat.replace("|OV:",",");
      SplittingArraySizeAdd = 3;
      updateOverride = HIGH; 
	
  }
  if (grblStat.indexOf("|WCO:"))
  {
      grblStat.replace("|WCO:",",x,x,x,");
      SplittingArraySizeAdd = 4;
  }



  grblStat.replace("|FS:",",");
  //IDLE|,0.000,0.000,0.000,0,0,99,100,100                - size9
  //IDLE|,0.000,0.000,0.000,0,0,x,x,x,x,0.000,0.000,0.000       - size13
  //IDLE|,0.000,0.000,0.000,0,0                           - size6

  // check the state
  if(grblStat[0]=='R')
  {
    grblMode=1;
  }
  else if(grblStat[0]=='J')
  {
    grblMode=4;
    printDebugMsg("jog");
  }
  else
  {
   grblMode=0;
  }

  
  splitIt(grblStat);
}


void splitIt(String msg)
{
  uint8_t i=0, j=0;
  String temp;
 
  while ( j<msg.length()) 
  {
    char t = msg.charAt(j);
    if (t==',') 
    {
      temp="";
      i ++;
    }
    else
    {
      temp=temp+t;   
    }
  j++;
  outputGRBL[i]=temp;   
  }
}


// see if the string is of any use for us
void checkGrblString(String grblRx)
{
  if (grblRx.startsWith("<"))                                 // Status GRBL
  {
    grblStringParse(grblRx);
  }
}


// create floates to print on led
void createFloat()
{
  for (int i=1; i<12; i++)
  {
    if (outputGRBL[i]!="x")
    {
      
      disp[i]=outputGRBL[i].toFloat();
    
    }
  }
}
void dro() 
{
  readSerialGRBBL(); 
  if (grblStringAvailable)
  {
    grblStringAvailable = false;
    checkGrblString(grblStringRx); // check if the string is of any use

    createFloat(); // convert everything to an float array 

    // first calcualte offsets for x y z
    calculateOffsets();
    /* 
    content of the array:
    1.00      1   x
    2.00      2   y
    3.00      3   z
    99.00     6   feed
    100.00    7   rapid
    101.00    8   spindle
    12.00     9   offset x
    13.00     10  offset y
    14.00     11  offsetz
    */
    
    // update the feed and speed overrides
    if (updateOverride == HIGH && feedSpindleOvEnable == 1)
    {
      feedOverride =  int(disp[6]);
      spindleOverride = int(disp[8]);
      overrides();
      updateOverride=LOW;
    } 
    // send to the LED display 
    printOnLed(String(disp[1]), 0);
    printOnLed(String(disp[2]), 1);
    printOnLed(String(disp[3]), 2);
    // feed and speed
    if (disp[8] < 100)
    {
      printOnLed( "F"+(String(int(disp[6]))+"  "+String(int(disp[8]))), 3);
    } else
    {
      printOnLed( "F"+(String(int(disp[6]))+" "+String(int(disp[8]))), 3);
    }
    grblStringRx = "";
  }
}



void calculateOffsets()
{
  // x
  disp[1] = disp[1] - disp[9];
  disp[2] = disp[2] - disp[10];
  disp[3] = disp[3] - disp[11];
}

void printOnLed(String content, int axis)
{
  
  // first we got to deal with the dot
  // duplicate the string to modify it later and keep the original
  String printString = content;

  // get the length of the string
  int strLength = content.length();
  // printDebugMsg("length_");
  // printDebugMsg(strLength);

  
  // check if there is a dot in the string
  bool dotTrue=false; // dot or no dot
  int dotPos=-1; //  position of the dot

  for (int i=0;i<strLength;i++)
  {
    if (content[i]=='.')
    {
      dotTrue=true;
      dotPos=i;
    } 
    
  } // end for 


  // if there is a dot we remove it from the duplicated string
  if (dotPos>=0)
  {
    printString.remove(dotPos, 1);
  }
  

  // now we need to invert dotPos in order to have the dot at the right position
  dotPos=(printString.length()-dotPos);

  
  // and writing it out on the led display 
  for (int i=0;i<printString.length();i++)
  {    
    bool setDot = false;
   
    /*  i is the position of the digit in the row of the 8 digid display
     *  i=0 means it't the far right digit
     *  So we start writing form the right to the left
     *  
     *  n is the position in the string printString
     *  it's basically inverted i (if this makes any sense)
     *  
     *  i=0 - far right position - n got to be the last character in the string
     *  
     */
    
    int n = (printString.length()-1-i); 
    if (i==dotPos)
    {
     setDot=true;
    
    } 

    // we're dealing with three displays 
    // 0=x, 1=y; 2=z
    switch(axis)
    {
      case 0:
        lcX.setChar(0,i,printString[n],setDot);
        break;
        
      case 1:
        lcY.setChar(0,i,printString[n],setDot);
        break;
        
      case 2:
        lcZ.setChar(0,i,printString[n],setDot);
        break;
      case 3:
        lcI.setChar(0,i,printString[n],setDot);
        break;
        
    } // end switch
    
  }// end for

  /*  Clear empty digits
   *  if the written number is shorter then the previous number 
   *  the unused digits need to be cleared
   *  I'm filling them here with white space
   */
  for (int i=7 ;i>=printString.length();i--)
  {
    switch(axis)
    {
      case 0:
        lcX.setChar(0,i,' ',false);
        break;
        
      case 1:
        lcY.setChar(0,i,' ',false);
        break;
        
      case 2:
        lcZ.setChar(0,i,' ',false);
        break;
     case 3:
      lcI.setChar(0,i,' ',false);
      break;
     
    } // end switch
  } // end for clear digits
  
} // end function
