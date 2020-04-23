 
ILI9341_due tft = ILI9341_due(TFT_CS, TFT_DC, TFT_RST);

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

void TimeDisplay()
{    
    
    tft.setTextScale(3); 
    timedisplay.getBars(barStr);
    timedisplay.getBeats(beatStr);
    timedisplay.getFrames(frameStr);
    timerToDisplay = "0" + (String)barStr[0] + ":" + 
                     (String)barStr[1] + (String)barStr[2] + ":" + 
                     (String)barStr[3] + (String)barStr[4] + ":" + 
                     (String)beatStr;

    if (isDigit(barStr[0])) 
    {
      tft.setTextScale(2); 
      tft.setTextColor(timeCodeColour[0]);
      tft.printAlignedOffseted("TC", gTextAlignBottomLeft, 0, -2); 
       
      if (isTimecode == true) 
      {
        tft.setTextScale(3);
        tft.setTextColor(timeCodeColour[0]);
        tft.printAlignedOffseted(timerToDisplay, gTextAlignBottomCenter, 0, 0);
        tft.setTextScale(2); 
        tft.setTextColor(WHITE); 
        tft.printAlignedOffseted(trackName.getText(), gTextAlignTopLeft, -66, 0); // MCU DISPLAY
      } 
      
      else 
      {
        tft.printAlignedOffseted("             ", gTextAlignBottomCenter, 0, 0); 
        isTimecode = true;
      }
      
    }
    else 
    {
      if (isTimecode == false) 
      {
        tft.setTextScale(2); 
        tft.setTextColor(timeCodeColour[1]);
        tft.printAlignedOffseted("IN", gTextAlignBottomLeft, 0, -2);
        tft.printAlignedOffseted(trackName.getText(), gTextAlignTopLeft, 0, 0); // MCU DISPLAY 
        tft.setTextScale(3);
        tft.printAlignedOffseted((String)barStr + (String)beatStr + (String)frameStr, gTextAlignBottomCenter, 0, 0);

         
      }

      else 
      {
        tft.printAlignedOffseted("             ", gTextAlignBottomCenter, 0, 0); 
        isTimecode = false;
      }
    }

    tft.setTextScale(1);
    
} 

void firstCallEqView()
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

void firstCallSmartView()
{

    for (int j=0; j<8; j++)
    {   

      if (j >= 4) smartColour = PINK;
      else smartColour = CYAN;

      tft.fillArc(xxPos[j],yyPos[j], circleSize - 5, 6, 0, cccc[j].getValue(), smartColour);

      tft.setTextColor(smartColour);
      tft.setTextScale(1);

      const char *text1 = trackName.getText() + (7 * j);
        char buffer1[7];
        strncpy(buffer1, text1, 6);
        buffer1[6] = '\0';
        #if debugging==true
        Serial.println(String(j) + " " + (String)buffer1);
        #endif

      tft.printAt(buffer1, xxPos[j]-15,yyPos[j]-30);

        const char *text2 = trackName.getText() + (7 * j) + 56;
        char buffer2[7];
        strncpy(buffer2, text2, 6);
        buffer2[6] = '\0';
        #if debugging==true
        Serial.println(String(j) + " " + (String)buffer2);
        #endif

      tft.printAt(buffer2,xxPos[j]-15,yyPos[j]+30);
      
    }      
}  

void topParametersUpdate()
{  
  tft.setTextScale(2);
    
if (modechange==1) // UPDATE VALUES FOR GENERAL
{   
  for (int j=0; j<10; j++)
    {   
     if (cccc[j].getValue() != oldcccc[j]) 
     { 
     
     timer[j] = millis(); 
          /*    
          if (ccMode[j] == 0) 
          { 
            tft.setTextColor(CYAN);
            if (j == 2) cccConverted[j] = map(ccc[j].getValue(),0,127,-50,0);
            if (j == 3) cccConverted[j] = fscale(0,127,0,200,ccc[j].getValue(),-2.2);
            if (j == 4) cccConverted[j] = mapLog(ccc[j].getValue(),0.0,127.0,1.0,30.0);
            if (j == 5) cccConverted[j] = (int)mapLog(ccc[j].getValue(),0,127,5,5000);
            if (j == 7) cccConverted[j] = fscale(0,127,-100,6,ccc[j].getValue(),logCurveDb);
            if (j == 8) cccConverted[j] = fscale(0,127,-80,6,ccc[j].getValue(),logCurveDb);
            if (j == 9) cccConverted[j] = map(ccc[j].getValue(),0,127,-180,180);
                    
            GeneralPrinter = (String)cccConverted[j] + CompText1scale[j];
            
            tft.printAlignedOffseted(CompText1[j], gTextAlignTopLeft, 0, 24, gTextEraseFullLine);
            tft.printAlignedOffseted(GeneralPrinter, gTextAlignTopLeft, 200, 24, gTextEraseToEOL);
          }
          if (ccMode[j] == 1) 
          { 
            tft.setTextColor(MAGENTA);
            if (j == 2) 
            {
              cccConverted[2] = map(ccc[j].getValue(),0,127,1,7);
              GeneralPrinter = CompType[cccConverted[2]-1] + CompText2scale[j];  
            }
            else
            {
            if (j == 3) cccConverted[j] = fscale(0,127,-20,50,ccc[j].getValue(),-1);
            if (j == 4) cccConverted[j] = map(ccc[j].getValue(),0,127,0,10);
            if (j == 5) cccConverted[j] = map(ccc[j].getValue(),0,127,0,100);
            if (j == 7) cccConverted[j] = fscale(0,127,-100,6,ccc[j].getValue(),logCurveDb);
            if (j == 8) cccConverted[j] = fscale(0,127,-80,6,ccc[j].getValue(),logCurveDb);
            if (j == 9) cccConverted[j] = fscale(0,127,-80,6,ccc[j].getValue(),logCurveDb);
            
            GeneralPrinter = (String)cccConverted[j] + CompText2scale[j];
            }
            
            tft.printAlignedOffseted(CompText2[j], gTextAlignTopLeft, 0, 24, gTextEraseFullLine);
            tft.printAlignedOffseted(GeneralPrinter, gTextAlignTopLeft, 200, 24, gTextEraseToEOL); 
          }
       */
      }    
   oldcccc[j] = cccc[j].getValue();
  }
}
if (modechange==0) // UPDATE VALUES FOR EQUALIZER
{
  for (int i=0;i<8;i++)
  {
    if (cc[i].getValue() != oldcc[i])
    
    {
      if (EqIsOn[i] == true)  tft.setTextColor(GREEN);
      if (EqIsOn[i] == false) tft.setTextColor(RED);
        
      tft.printAlignedOffseted(EQText1[i], gTextAlignTopLeft, 0, 24, gTextEraseFullLine);
      if (ccMode[i] == 0) GeneralPrinter = EQText2[0] + " " + EqFreqToPrint[i];
      if (ccMode[i] == 1) GeneralPrinter = EQText2[1] + " " +  EqGainToPrint[i];
      if (ccMode[i] == 2) GeneralPrinter = EQText2[2] + " " +  EqQToPrint[i];
      if (ccMode[i] == 3) GeneralPrinter = EQText2[3] + " " +  EqONOFFToPrint[i];
      tft.printAlignedOffseted(GeneralPrinter, gTextAlignTopLeft, 70, 24, gTextEraseToEOL);
    }
    oldcc[i] = cc[i].getValue();
  }
}
  tft.setTextScale(1);
  tft.setTextColor(WHITE); 
}

void VolumeView() 
{     
   
    for (int i=0; i<6; i++)
    {
      tft.setTextScale(2); 
      if (ccmuxChannel[i].getValue() == 127) tft.setTextColor(RED);
      else tft.setTextColor(WHITE);                        
      tft.printAt(vuNames[i], vunameXpos[i], 192); 
          
      
      if (vuValue[i] != vu[i].getValue())
      {               
          tft.setTextScale(1);
          tft.setTextColor(WHITE);
          
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
          tft.setTextColor(WHITE);
      }
     
      vuValue[i] = vu[i].getValue();
     
     /*
      newPeak[i] = vuValue[i] * (blockheight + spacing);
      
      if (newPeak[i] >= peak[i]) {
             peak[i] = newPeak[i];
             previousDecay[i] = millis();
             decaying[i] = false;
         } else if (!decaying[i] &&
                    (millis() - previousDecay[i] > VU_PEAK_HOLD_TIME)) {
             decaying[i] = true;
             previousDecay[i] += VU_PEAK_HOLD_TIME - decayTime[i];
         } else if (decaying[i] && (millis() - previousDecay[i] > decayTime[i])) {
             if (peak[i] > 0) {
                 peak[i] -= VU_PEAK_SMOOTH_DECAY ? 1 : (blockheight + spacing);
                 previousDecay[i] += decayTime[i];
             }
         }
        
        
        tft.drawFastHLine(vuXpos[i],
                          y - spacing + blockheight - peak[i], 
                          vuWidth, 
                          RED); */
         

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

void EqView()
{  
 
 tft.setTextColor(WHITE); 
  
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

/*
void CompView()
{  
  for (int j=2; j<6; j++)
    {        
      if (ccc[j].getValue() != oldccc[j])
      {  if (ccMode[j] == 0) 
        {
          if (ccc[j].getValue() < oldccc[j]) tft.fillArc(xPos[j],yPos[j], circleSize+12, 8, ccc[j].getValue(), 0, BLACK);
          if (ccc[j].getValue() > oldccc[j]) tft.fillArc(xPos[j],yPos[j], circleSize+12, 8, 0, ccc[j].getValue(), CYAN);                
        }
        if (ccMode[j] == 1) 
        {
          if (ccc[j].getValue() < oldccc[j]) tft.fillArc(xPos[j],yPos[j], circleSize, 8, ccc[j].getValue(), 0, BLACK);
          if (ccc[j].getValue() > oldccc[j]) tft.fillArc(xPos[j],yPos[j], circleSize, 8, 0, ccc[j].getValue(), MAGENTA);
        } 
      }    
    }
}*/

void SmartView()
{
  for (int j=0; j<8; j++)
    {   
     if (cccc[j].getValue() != oldcccc[j])
     { 
        timer[j] = millis();
        if (j >= 4) smartColour = PINK;
        else smartColour = CYAN;
        if (ccMode[j] == 0) 
        {
          if (cccc[j].getValue() < oldcccc[j]) tft.fillArc(xxPos[j],yyPos[j], circleSize - 5, 6, cccc[j].getValue(), 0, BLACK);
          if (cccc[j].getValue() > oldcccc[j]) tft.fillArc(xxPos[j],yyPos[j], circleSize - 5, 6, 0, cccc[j].getValue(), smartColour);      

        const char *text1 = trackName.getText() + (7 * j);
        char buffer1[7];
        strncpy(buffer1, text1, 6);
        buffer1[6] = '\0';

          tft.printAt(buffer1, xxPos[j]-15,yyPos[j]-30);

        const char *text2 = trackName.getText() + (7 * j) + 56;
        char buffer2[7];
        strncpy(buffer2, text2, 6);
        buffer2[6] = '\0';

          tft.printAt(buffer2,xxPos[j]-15,yyPos[j]+30);
        
        }    
      } 
      oldcccc[j] = cccc[j].getValue();   
    }
}

void SurroundView()
{

  if (joystickSurround.changed)
  {    
     tft.fillCircle( 120, 130, 70, PINK);
     tft.fillCircle(joystickSurround.yy/8 + 120, -joystickSurround.xx/8 + 130,5, GREYGREEN); 
   
    for (int j=0;j<2;j++)
    {
      tft.setTextScale(1);
      tft.setTextColor(GREYGREEN);
      tft.printAt("Diversity: ", 220, 140, gTextEraseToEOL);
      tft.setTextScale(2);
      tft.printAt(String(joystickSurround.diversity),220,150,gTextEraseToEOL);
      tft.setTextScale(1);
      tft.setTextColor(PINK);
      tft.printAt("Angle: ", 220, 100, gTextEraseToEOL);
      tft.setTextScale(2);
      tft.printAt(String((joystickSurround.angleDeg*4068)/71), 220, 110, gTextEraseToEOL); 
      
    }
  } 
}

void transportIcons()
{
  for (int i = 1; i<6; i++)
    {
      if (ccmuxTransport[i].getValue() !=oldccmuxTransport[i])
          {         
            if (ccmuxTransport[i].getValue() == 127) transportToggle[i] = 1;
            if (ccmuxTransport[i].getValue() == 0)   transportToggle[i] = 0;
          }

          oldccmuxTransport[i] = ccmuxTransport[i].getValue(); 
          
          if (transportToggle[i] != oldtransportToggle[i])
          {
            tft.fillRect(290,219,30,20,BLACK); // Eraser
            
            
            if (transportToggle[1] == 1) 
            {
              tft.drawCircle(310,230,2,TRANSPORT_COLOUR); // LOOP
              tft.drawCircle(314,230,2,TRANSPORT_COLOUR); // LOOP
            }
            if (transportToggle[4] == 1) tft.fillRect(288,222,16,16,TRANSPORT_COLOUR); // STOP
            if (transportToggle[5] == 1) tft.fillTriangle(288,238,288,222,304,230,TRANSPORT_COLOUR); // PLAY
            // if (transportToggle[0] == 1) tft.printAt("M",308,230);
          }

          oldtransportToggle[i] = transportToggle[i];      
    }
}

void tftStart()
{ 

     tft.fillRect(0,40,320,167,BLACK); // reset middle
    
    if (modechange==0) 
      {
        tft.setTextScale(1);
        firstCallEqView();
        EqView();
      }
      
    if (modechange==1) 
      {
        VolumeView();
      }

    if (modechange==2) 
      {
        firstCallSmartView();
        SmartView();
      }  
    
    transportIcons();  
}

void checkForJoystick()
{
   if (digitalRead(joystickSurround.buttonPin) == LOW && surroundToggle[1] == 0) 
  {
    surroundToggle[0] = 1;
    tft.fillRect(0,40,320,167,BLACK); // reset middle screen

    // tft.drawRect( 96, 66, 130, 130, BLUE);
    surroundToggle[1] = !surroundToggle[1];

  }
  
  if (digitalRead(joystickSurround.buttonPin) == HIGH && surroundToggle[0] == 1) 
  {
    surroundToggle[0] = 0;
    surroundToggle[1] = 0;
    tftStart();
  }
}

void ShowFramesPerSecond()
{
   if (millis() - frameTimer >999) 
   {
     tft.setTextScale(1);
     tft.setTextColor(WHITE);
     tft.printAt(String(frameCount),300,5, gTextEraseFullLine);
     frameCount = 0;
     frameTimer = millis();
   }
}

void ScreenUpdate()
{ 
   // ShowFramesPerSecond();
   
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
   
      if (modechange==0) 
      {
        EqView();
      }
   
      if (modechange==1) 
      {
        VolumeView(); 
      }

      if (modechange==2) 
      {
        SmartView(); 
      }

     topParametersUpdate();
   }

   checkForJoystick();
  
  fpsTimer = millis();

}  
