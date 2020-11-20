// rotary encoder

void setEncoderResolution()
{
  if (digitalRead(encoderResolutionPin1)==LOW)
  {
	encoderResolution=encoderResolution1;
	jogFeed=jogFeedRate1;
  }
  if (digitalRead(encoderResolutionPin2)==LOW)
  {
	encoderResolution=encoderResolution2;
	jogFeed=jogFeedRate2;
  }
  if (digitalRead(encoderResolutionPin3)==LOW)
  {
	encoderResolution=encoderResolution3;
	jogFeed=jogFeedRate3;
  }
}


void setEncoderAxis()
{
  int i=0;

  if (digitalRead(encoderXpin)==LOW)
  {
	encoderAxis="X";
  } else
  {
	i++;
  }

  if (digitalRead(encoderYpin)==LOW)
  {
	encoderAxis="Y";
  } else
  {
	i++;
  }
  
  if (digitalRead(encoderZpin)==LOW)
  {
	encoderAxis="Z";
  }else
  {
	i++;
  }

  if (i==3)
  {
	encoderAxis="none";
  }
}

void readEncocder()
{

  setEncoderAxis();
  setEncoderResolution();
  // cancel jogging if the encoder stopped moving
  if ((millis()- encoderPreviousMicros) > encoderCancelIntervall && grblMode == 4 && cancelSent == LOW)
  {
	Serial1.write(0x85);
	printDebugMsg("cancel");
	cancelSent = HIGH;
  }
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) 
  {
    if (grblMode!=1)
    {
      if ((millis() - encoderPreviousMicros) > encoderCmdIntervall)
	{
	  // reset the timer for sending the jog cancel cmd
	  encoderPreviousMicros=millis();
	  // reset the timer for requesting a status update from GRBL
	  lastPosChangeMillis=millis();

	  float dif = oldPosition - newPosition;
	  if ((dif/encoderResolution)<encoderMaxSteps)
	  {
	    // printDebugMsg(dif);
	    if (encoderAxis != "none")
	    {
	      Serial1.println("$J=G91 "+ encoderAxis + String(dif/encoderResolution)+" F"+jogFeed);
	      cancelSent=LOW;
	    }
	    oldPosition = newPosition;
	  }
	}
    }
  }
}

