#include <ICONS.h>

ILI9341_due tft = ILI9341_due(TFT_CS, TFT_DC, TFT_RST);

/////////////////////////////////////////////////////////////////////////////////////////
// METODS: //////////////////////////////////////////////////////////////////////////////
float mapLog(float value, float start1, float stop1, float start2, float stop2) 
{
    start2 = log(start2);
    stop2 = log(stop2);
    float outgoing = exp(start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1)));
    return outgoing;
}
float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve)
{

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println();
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  /*
  Serial.print(OriginalRange, DEC);  
   Serial.print("   ");  
   Serial.print(NewRange, DEC);  
   Serial.print("   ");  
   Serial.println(zeroRefCurVal, DEC);  
   Serial.println();  
   */

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {  
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}
void setTextSizeAndColor(uint8_t size, uint16_t colour)
{
  tft.setTextScale(size);
  tft.setTextColor(colour);
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void transportIcons() ///////////////////////////// TRANSPORT UPDATE
{
  for (int i = 0; i<6; i++)
    {
      if (ccmuxTransport[i].getValue() !=oldccmuxTransport[i])
          {         
            if (ccmuxTransport[i].getValue() == 127) transportToggle[i] = 1;
            if (ccmuxTransport[i].getValue() == 0)   transportToggle[i] = 0;
          }

          oldccmuxTransport[i] = ccmuxTransport[i].getValue(); 
          
          if (transportToggle[i] != oldtransportToggle[i])
          {
            tft.fillRect(288,218,24,24,BLACK); // Eraser
            if (transportToggle[0] == 1) TRANSPORT_COLOUR = timeCodeColour[2];
            if (transportToggle[0] == 0) TRANSPORT_COLOUR = timeCodeColour[0];

            if (transportToggle[4] == 1 && transportToggle[1] == 1 && transportToggle[0] == 0) tft.drawBitmap(stopLoopBitmaps,288,218,22,22,TRANSPORT_COLOUR);
            if (transportToggle[4] == 1 && transportToggle[1] == 0 && transportToggle[0] == 0) tft.drawBitmap(stopBitmaps,288,218,22,22,TRANSPORT_COLOUR);
            if (transportToggle[1] == 1 && transportToggle[1] == 1 && transportToggle[0] == 0) tft.drawBitmap(playLoopBitmaps,288,218,22,22,TRANSPORT_COLOUR);
            if (transportToggle[5] == 1 && transportToggle[1] == 0 && transportToggle[0] == 0) tft.drawBitmap(playBitmaps,288,218,22,22,TRANSPORT_COLOUR); 
            if (transportToggle[0] == 1 && transportToggle[1] == 1)                            tft.drawBitmap(recLoopBitmaps,288,218,22,22,TRANSPORT_COLOUR);
            if (transportToggle[0] == 1 && transportToggle[1] == 0)                            tft.drawBitmap(recBitmaps,288,218,22,22,TRANSPORT_COLOUR);
          }

          oldtransportToggle[i] = transportToggle[i];      
    }
}

void TimeDisplay() //////////////////////////////// TIMECODE + LCD display
{    
    
    timedisplay.getBars(barStr);
    timedisplay.getBeats(beatStr);
    timedisplay.getFrames(frameStr);
    timerToDisplay = "0" + (String)barStr[0] + ":" + 
                     (String)barStr[1] + (String)barStr[2] + ":" + 
                     (String)barStr[3] + (String)barStr[4] + ":" + 
                     (String)beatStr;

    if (isDigit(barStr[0])) 
    {
      setTextSizeAndColor(2,TRANSPORT_COLOUR);
      tft.printAlignedOffseted("TC ", gTextAlignBottomLeft, 0, -2); 
       
      if (isTimecode == true) 
      {
        setTextSizeAndColor(3,TRANSPORT_COLOUR);
        tft.printAlignedOffseted(timerToDisplay, gTextAlignBottomCenter, 0, 0);

        if (toggleDisplay==0)
        {
          const char *text3 = parametersNameValue.getText();
          char buffer3[56];
          strncpy(buffer3, text3, 55);
          StoredTrackName = buffer3; 
        } 
        
        setTextSizeAndColor(2,WHITE);
        tft.printAlignedOffseted(StoredTrackName, gTextAlignTopLeft, 0, 0); // MCU DISPLAY Track Name
      } 
      
      else 
      {
        setTextSizeAndColor(3,TRANSPORT_COLOUR);
        tft.printAlignedOffseted("             ", gTextAlignTopLeft, 0, 0); 
        isTimecode = true;
      }
      
    }
    else 
    {
      if (isTimecode == false) 
      {
        setTextSizeAndColor(2,timeCodeColour[1]);
        tft.printAlignedOffseted("OFF", gTextAlignBottomLeft, 0, -2);
        tft.printAlignedOffseted(parametersNameValue.getText(), gTextAlignTopLeft, 0, 0); // MCU DISPLAY Normal
        tft.setTextScale(3);
        tft.printAlignedOffseted((String)barStr + (String)beatStr + (String)frameStr, gTextAlignBottomCenter, 0, 0);

         
      }

      else 
      {
        tft.printAlignedOffseted("             ", gTextAlignBottomCenter, 0, 0); 
        isTimecode = false;
      }
    }   
} 

void firstCallEqView() //////////////////////////// EQ VIEW build
{
    for (int j=0; j<8; j++)
    {            
      bank[j].select(3);
      tft.fillArc(xPos[j],yPos[j], circleSize-10, 5, 0, cc[j].getValue() , YELLOW);
      if (cc[j].getValue() == 127) EqIsOn[j] = true; 
      if (cc[j].getValue() == 0)   EqIsOn[j] = false;
 
      bank[j].select(2);
      tft.fillArc(xPos[j],yPos[j], circleSize-5, 5, 0, cc[j].getValue(), BLUE);
      
      bank[j].select(1);
      tft.fillArc(xPos[j],yPos[j], circleSize, 5, 0, cc[j].getValue(), RED);

      bank[j].select(0);
      tft.fillArc(xPos[j],yPos[j], circleSize+5, 5, 0, cc[j].getValue(), GREEN);     
    } 
}

void firstCallSmartView() ///////////////////////// SMART VIEW build
{
    tft.setArcParams(11);
    for (int j=0; j<8; j++)
    {   
      if (j >= 4) {smartColour = PINK;}
      else        {smartColour = CYAN;} 
      tft.fillCircle(xxPos[j],yyPos[j], circleSize - 15, smartColour);
      tft.fillArc(xxPos[j],yyPos[j], circleSize - 5, 6, 0, cccc[j].getPosition(), smartColour);     
    }  
     tft.setArcParams(127);    
}  

void topParametersUpdate() //////////////////////// UPDATE PARAMETERS AT THE TOP
{  
 tft.setTextScale(2);

 for (int i=0;i<8;i++)
 {
  if (modechange==1) // UPDATE VALUES FOR GENERAL VIEW
  {   
    // todo
  }
  if (modechange==0) // UPDATE VALUES FOR EQ VIEW
  {
    if (cc[i].getValue() != oldcc[i])
    {
      if (EqIsOn[i] == true)  tft.setTextColor(GREEN);
      if (EqIsOn[i] == false) tft.setTextColor(RED);
      if (ccMode[i] == 0) GeneralPrinter = EQText2[0] + ": " + EqFreqToPrint[i];
      if (ccMode[i] == 1) GeneralPrinter = EQText2[1] + ": " +  EqGainToPrint[i];
      if (ccMode[i] == 2) GeneralPrinter = EQText2[2] + ": " +  EqQToPrint[i];
      if (ccMode[i] == 3) GeneralPrinter = EQText2[3] + ": " +  EqONOFFToPrint[i];
      tft.printAlignedOffseted(EQText1[i] + " " + GeneralPrinter, gTextAlignTopLeft, 40, 26, gTextEraseFullLine);
    }
    oldcc[i] = cc[i].getValue();
  }
  if (modechange==2) // UPDATE VALUES FOR SMART VIEW
  {
    if (i >= 4) smartColour = PINK;
    else smartColour = CYAN;
    tft.setTextColor(smartColour);

    if (Storedbuffer2[i] != oldStoredbuffer2[i])
    {
    tft.printAlignedOffseted(Storedbuffer1[i] + ": " + Storedbuffer2[i], gTextAlignTopLeft, 0, 26, gTextEraseFullLine);
    }
    oldStoredbuffer2[i] = Storedbuffer2[i];
  } 
 } 
}

void VolumeView() ///////////////////////////////// VOLUME MODE UPDATE
{     
    for (int i=0; i<6; i++)
    {
      if (ccmuxChannel[i].getValue() == 127) setTextSizeAndColor(2,RED);
      else                                   setTextSizeAndColor(2,WHITE); 

      tft.printAt(vuNames[i], vunameXpos[i], 192); 
          
      
      if (vuValue[i] != vu[i].getValue())
      {               
          setTextSizeAndColor(1,WHITE);
          
          peakValue[i] = map(vu[i].getValue(),0.0,12.0,-100.0,12.0);

          if (peakValue[i] >= 0) 
          {
            tft.setTextColor(RED);
            vuColor[i] = RED;
          }
          else 
          {
            tft.setTextColor(WHITE);
            vuColor[i] = GREEN;
          }
          if (vu[i].getValue() >= vuValue[i]) 
          {
            tft.printAt("    ", vuXpos[i], 47);
            
            if (peakValue[i] >= 0) tft.printAt("Peak", vuXpos[i], 47);
            else tft.printAt((String)peakValue[i], vuXpos[i], 47);
          }
      }
     
      vuValue[i] = vu[i].getValue();  

        for (uint8_t j=0; j<12; j++)
        {
          tft.fillRect(vuXpos[i],y - j*(blockheight+spacing),vuWidth,blockheight,DARKGREEN); 
        }

        for (uint8_t j=0; j<vuValue[i]; j++)
        {
        tft.fillRect(vuXpos[i],
                     y - j * (blockheight + spacing), 
                     vuWidth,
                     blockheight, 
                     vuColor[i]);
        } 
     }
   
}

void SmartView() ////////////////////////////////// SMART MODE UPDATE
{
  tft.setArcParams(11);

  for (int j=0; j<8; j++)
    {   
      if (j >= 4) smartColour = PINK;
      else smartColour = CYAN;
      setTextSizeAndColor(1,smartColour);

        const char *text1 = parametersNameValue.getText() + (7 * j);
        char buffer1[7];
        strncpy(buffer1, text1, 6);
        buffer1[6] = '\0';
        const char *text2 = parametersNameValue.getText() + (7 * j) + 56;
        char buffer2[7];
        strncpy(buffer2, text2, 6);
        buffer2[6] = '\0';
        
        Storedbuffer1[j] = buffer1;
        Storedbuffer2[j] = buffer2;

      if (Storedbuffer2[j] != oldStoredbuffer2[j])
      {   
        tft.printAt(Storedbuffer1[j], xxPos[j]-15, yyPos[j]-28);
        tft.printAt(Storedbuffer2[j], xxPos[j]-15, yyPos[j]+25);

        #if debugging==true
        Serial.println("V_POT_" + (String)j + ": " + (String)cccc[j].getPosition());
        #endif

        if (cccc[j].getPosition() < oldcccc[j]) tft.fillArc(xxPos[j],yyPos[j], circleSize - 5, 6, cccc[j].getPosition(), 0, BLACK);
        if (cccc[j].getPosition() > oldcccc[j]) tft.fillArc(xxPos[j],yyPos[j], circleSize - 5, 6, 0, cccc[j].getPosition(), smartColour);      

      }  
      oldcccc[j] = cccc[j].getPosition();
    }
    tft.setArcParams(127); 
}

void EqView() ///////////////////////////////////// EQ MODE UPDATE
{  
 
 setTextSizeAndColor(1,WHITE);
  
 for (int j=0; j<8; j++)
    {   
     if (cc[j].getValue() != oldcc[j])
     { 
        timer[j] = millis();
        if (ccMode[j] == 0) 
        {
          if (cc[j].getValue() < oldcc[j]) tft.fillArc(xPos[j],yPos[j], circleSize+5, 5,cc[j].getValue(), 0, BLACK);
          if (cc[j].getValue() > oldcc[j]) tft.fillArc(xPos[j],yPos[j], circleSize+5, 5, 0, cc[j].getValue(), GREEN);      
          tft.printAt(blankTxt, xPos[j]-15,yPos[j]-35);
          EqFreqToPrint[j] = (String)(int)mapLog(cc[j].getValue(),0,127,20,20000) + "Hz";
          tft.printAt(EqFreqToPrint[j], xPos[j]-15,yPos[j]-35);
          
          
        }
        if (ccMode[j] == 1) 
        {
          if (cc[j].getValue() < oldcc[j]) tft.fillArc(xPos[j],yPos[j], circleSize, 5, cc[j].getValue(), 0, BLACK);
          if (cc[j].getValue() > oldcc[j]) tft.fillArc(xPos[j],yPos[j], circleSize, 5, 0, cc[j].getValue(), RED);
          tft.printAt(blankTxt, xPos[j]-15,yPos[j]-35);
          EqGainToPrint[j] = (String)map(cc[j].getValue(),0,127,-24,24) + "dB";
          tft.printAt(EqGainToPrint[j], xPos[j]-15,yPos[j]-35);
        }
        if (ccMode[j] == 2) 
        {
          if (cc[j].getValue() < oldcc[j]) tft.fillArc(xPos[j],yPos[j], circleSize-5, 5, cc[j].getValue(), 0, BLACK);
          if (cc[j].getValue() > oldcc[j]) tft.fillArc(xPos[j],yPos[j], circleSize-5, 5, 0, cc[j].getValue(), BLUE);
          tft.printAt(blankTxt, xPos[j]-15,yPos[j]-35);
          EqQToPrint[j] = (String)mapLog(cc[j].getValue(),0.0,127.0,.1,100.0);
          tft.printAt(EqQToPrint[j], xPos[j]-15,yPos[j]-35);
        }
        if (ccMode[j] == 3) 
        {
          if (cc[j].getValue()==127) 
          {
            tft.fillArc(xPos[j],yPos[j], circleSize-10, 5, 0, 127, YELLOW);
            EqIsOn[j] = true;
            tft.printAt(blankTxt, xPos[j]-15,yPos[j]-35);
            EqONOFFToPrint[j] = " ON";
            tft.printAt(EqONOFFToPrint[j], xPos[j]-14,yPos[j]-35);
          }
          if (cc[j].getValue()==0) 
          {
            tft.fillArc(xPos[j],yPos[j], circleSize-10, 5, 0, 127, BLACK);
            EqIsOn[j] = false;
            tft.printAt(blankTxt, xPos[j]-15,yPos[j]-35);
            EqONOFFToPrint[j] = "OFF";
            tft.printAt(EqONOFFToPrint[j], xPos[j]-14,yPos[j]-35);
          }
         }      
      }    
    }
}

void SurroundView() /////////////////////////////// SURROUND MODE UPDATE
{

  if (joystickSurround.changed)
  {    
     tft.fillCircle( 120, 130, 70, PINK);
     
     tft.fillCircle( 79, 60, 3, CYAN); // L
     tft.fillCircle(119, 49, 3, CYAN); // C
     tft.fillCircle(159, 60, 3, CYAN); // R
     tft.fillCircle( 45,158, 3, CYAN); // Ls
     tft.fillCircle(196,158, 3, CYAN); // Rs
     
     tft.fillCircle(joystickSurround.yy/8 + 120, -joystickSurround.xx/8 + 130,5, GREYGREEN); 
   
    for (int j=0;j<2;j++)
    {

      setTextSizeAndColor(1,GREYGREEN);
      tft.printAt("Diversity: ", 220, 140, gTextEraseToEOL);
      tft.setTextScale(2);
      tft.printAt(String(joystickSurround.diversity),220,150,gTextEraseToEOL);
      setTextSizeAndColor(1,PINK);
      tft.printAt("Angle: ", 220, 100, gTextEraseToEOL);
      tft.setTextScale(2);
      tft.printAt(String((joystickSurround.angleDeg*4068)/71), 220, 110, gTextEraseToEOL); 
      
    }
  } 
}

void tftStart() /////////////////////////////////// SCREEN START AND RESET
{ 

     tft.fillRect(0,40,320,167,BLACK); // reset middle
    
    if (modechange==0) 
      {
        if (toggleDisplay == 1) 
        {
          Control_Surface.MIDI().sendCC({3, CHANNEL_6}, 127);
          toggleDisplay = !toggleDisplay;
        }
        else {}
        firstCallEqView();
      }
      
    if (modechange==1) 
      {
        if (toggleDisplay == 1) 
        {
          Control_Surface.MIDI().sendCC({3, CHANNEL_6}, 127);
          toggleDisplay = !toggleDisplay;
        }
        else {}
        VolumeView();
      }

    if (modechange==2) 
      {
        if (toggleDisplay == 0)
        {
         Control_Surface.MIDI().sendCC({3, CHANNEL_6}, 127);
         toggleDisplay = !toggleDisplay;
        }
        else {}
        firstCallSmartView();
      }  
    
    transportIcons();  
}

void CheckForJoystick() /////////////////////////// LOOK FOR JOYSTICK
{
   if (digitalRead(joystickSurround.buttonPin) == LOW && surroundToggle[1] == 0) 
  {
    surroundToggle[0] = 1;
    tft.fillRect(0,40,320,167,BLACK); // reset middle screen
    surroundToggle[1] = !surroundToggle[1];
  }
  
  if (digitalRead(joystickSurround.buttonPin) == HIGH && surroundToggle[0] == 1) 
  {
    surroundToggle[0] = 0;
    surroundToggle[1] = 0;
    tftStart();
  }
}

void ScreenUpdate() /////////////////////////////// MAIN UPDATE
{ 
   if (surroundToggle[0] == 1 && surroundToggle[1] == 1) 
    {
      frameMultiplier = 6;
      SurroundView();
    }
   
   else 
   {
      frameMultiplier = 1;
      
      TimeDisplay();
      transportIcons();
   
      if (modechange==0) EqView();
      if (modechange==1) VolumeView(); 
      if (modechange==2) SmartView(); 

      topParametersUpdate();
   }
  CheckForJoystick();
  fpsTimer = millis();

}  
