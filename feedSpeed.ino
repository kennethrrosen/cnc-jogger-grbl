// =================================================
// all functions dealing with the spindle and feed 
// overrides
// =================================================

// read the analog value of the potentiometers and map them to theyr override values
void overrides()
{
  checkPoti();
  sendOverrideCmd();
}


// smoothing the reading to avoid sending unnecessary override commands
float averageOfArray(int data[])
{
  int s = sizeof(data);
  int total=0;
  for (int i=0; i< s; i++)
  {
    total=total+data[i];
  }
  return (total/s);
}


void checkPoti()
{
  if (stepsCount > smoothSteps)
  {
	stepsCount=0;
  }
  int feedTmp=analogRead(feedPin);
  int spindleTmp=analogRead(spindlePin);
  
  // map it to some usufull values
  // min and max values are defined in config.h 
  if (feedTmp < 512)
  {
    feedInput=map(feedTmp, 0, 511, feedOvMin, 100);
  } else
  {
    feedInput=map(feedTmp, 512, 1023, 100, feedOvMax);
  }
  // feedInput =feedInput+feedOffset;

  // min and max values are defined in config.h 
  if (spindleTmp < 512)
  {
    spindleInput=map(spindleTmp, 0, 511, spindleOvMin, 100);
  } else
  {
    spindleInput=map(spindleTmp, 512, 1023, 100, spindleOvMax);
  }
  // spindleInput=spindleInput+spindleOffset;


  feedAry[stepsCount]=feedInput;
  spindleAry[stepsCount]=spindleInput;
  stepsCount ++;
  feedInput=(int)averageOfArray(feedAry);
  spindleInput=(int)averageOfArray(spindleAry);

   
  spindleDifference = spindleOverride - spindleInput;
  feedDifference = feedOverride - feedInput ;
}

// figure which command to send
int adjustOverride(int dif)
{
  // pos or negative
  if (dif > 0)
  {
    // bigger then 10?
    if (dif > 10)
    {
      return -10;
    } else
    {
      return -1;
    }
  }
  if (dif < 0)
  {
    // smaller 10
    if (dif < -10)
    {
      return 10;
    }
    else
    {
      return 1;
    }
  }
  if (dif == 0)
  {
    return 0;
  }
}

// send the commands
void sendOverrideCmd()
{
  int val;
  // feed override
  if (feedDifference > 1 || feedDifference < -1)
  lastPosChangeMillis=millis();
  printDebugMsg("millis reset feed");
  // if (feedDifference != 0)
  {
    // reset the timer for requesting a status update from GRBL

    val = adjustOverride(feedDifference);
    switch(val)
    {
      case -10:
	Serial1.write(0x92);
	feedOverride=feedOverride-10;


      break;

      case -1:
	Serial1.write(0x94);
	feedOverride=feedOverride-1;


      break;

      case 1:
	Serial1.write(0x93);
	feedOverride=feedOverride+1;


      break;

      case 10:
	
	Serial1.write(0x91);
	feedOverride=feedOverride+10;


      break;
    }
  }
  // spindle override
  if (spindleDifference > 1 || spindleDifference < -1)
  lastPosChangeMillis=millis();
  printDebugMsg("millis reset feed");

  // if (spindleDifference != 0)
  {
    val = adjustOverride(spindleDifference);
    switch(val)
    {
      case -10:
	Serial1.write(0x9B);
	spindleOverride=spindleOverride-10;
      break;

      case -1:
	 Serial1.write(0x9D);
	spindleOverride=spindleOverride-1;
      break;

      case 1:
	 Serial1.write(0x9C);
	spindleOverride=spindleOverride+1;
      break;

      case 10:
	Serial1.write(0x9A);
	spindleOverride=spindleOverride+10;
      break;
    }
  }
}
