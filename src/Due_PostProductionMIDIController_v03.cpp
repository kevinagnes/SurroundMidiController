#include <Arduino.h> 
#include <GLOBAL.h> 
#include <MIDI.h> 

#if NOSCREEN==0
#include <DISPLAY.h> 
#endif

void setup() 
{
  
#if debugging==true 
Serial.begin(115200);
#endif
  
  mux.begin(); // initiate Multiplexer for buttons 
              
  pinMode(TFT_LED,OUTPUT);
  digitalWrite(TFT_LED,lcdState);
  
  for (int l=0;l<10;l++)
  {
    b[l].setButtonHeldEnabled(true);
    b[l].setDoubleClickEnabled(true);
  }
  
  RelativeCCSender::setMode(relativeCCmode::MACKIE_CONTROL_RELATIVE);

  for (int r=0;r<10;r++)
  {
    encEQ[r].disable();
  }
  
#if NOSCREEN==0
  tft.begin();
  tft.setRotation(iliRotation90);
  tft.setArcParams(127);
  tft.setAngleOffset(-180);
  tft.setFont(SystemFont5x7);
  tft.fillScreen(BLACK);
  tft.drawLine(0,19,320,19,WHITE);
  tft.drawLine(0,211,320,211,timeCodeColour[0]);
  tftStart();
#endif  
  
  Control_Surface.begin(); // Initialize tttapa's Control Surface Magic 
}

void programChange()
{
    boolmode = !boolmode;
    
#if NOSCREEN==0   
    if (lcdState == 1) tftStart();
#endif    
    
    for (int r=0;r<10;r++)
    {
     if (encGENERAL[r].isEnabled())
     {
        encGENERAL[r].disable();
        encEQ[r].resetPositionOffset();
        encEQ[r].enable();
     }
     else
     {
        encEQ[r].disable();
        encGENERAL[r].resetPositionOffset();
        encGENERAL[r].enable();
     }
    } 
}

void loop() {

  Control_Surface.loop(); // tttapa magic sauce

  midi.update();
     
  static uint32_t lastService = 0;
  
  for (int i=0;i<10;i++)
  {
    if (lastService + 1000 < micros())
      {
        b[i].service(); // START BUTTONS SERVICES
      } 
  }

  for (int k=0;k<10;k++)
  {
    bValue[k] = b[k].getButton(); // READ BUTTONS
  }

  
  for (int j=0;j<10;j++) 
  {
    if (bValue[j]==(ClickEncoder::Clicked)) 
    {
      ccMode[j] = 1;
      toggle[j] = !toggle[j];
      bank[j].select(toggle[j]); 
      timer[j] = millis(); 
      timerToggle[j] = 1;
    }
    
    if (bValue[j]==(ClickEncoder::DoubleClicked))
    {
      ccMode[j] = 2;
      bank[j].select(2);
      timer[j] = millis();   
      timerToggle[j] = 1;          
    }
    
    if (bValue[j]==(ClickEncoder::Held))
    {   
      ccMode[j] = 3;
      bank[j].select(3); 
      timer[j] = millis(); 
      timerToggle[j] = 1;
    } 
  
    if ((millis()-timer[j])>timeBackToBankZero && timerToggle[j]==1) // If no activity returns to first parameter.
    {
      ccMode[j] = 0;
      toggle[j] = 0;
      bank[j].select(toggle[j]);  
      timerToggle[j] = !timerToggle[j];   
    } 
  }


  if (Fn4.update() == Button::Rising)
  {
     programChange();
  }

#if NOSCREEN==0
  if (lcdState == 1) if (millis() - fpsTimer > (1000/(ScreenFrameRate*frameMultiplier)) ) ScreenUpdate();
  frameCount += 1;
#endif 

    
/*
#if debugging==true
  if ((millis() - debugDelay - olddebugDelay) > 2)Serial.println("Accumulated delay: " + String(millis() - debugDelay));
  olddebugDelay = millis() - debugDelay;
  debugDelay = millis();
#endif   
*/ 
}
 
 
