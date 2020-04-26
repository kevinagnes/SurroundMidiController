#include <Arduino.h> 
#include <GLOBAL.h> 
#include <MIDI.h> 

#if NOSCREEN==0
#include <DISPLAY.h> 
#endif

void setup() 
{
  
#if debugging==true || screenShootEnabled==true 

Serial.begin(115200);
Serial.println("Hello Bitches! I am awaken");

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

  for (int r=0;r<10;r++)
  {
    encSMART[r].disable();
  }
  
#if NOSCREEN==0
  tft.begin();
  tft.setRotation(iliRotation90);
  tft.setArcParams(127);
  tft.setAngleOffset(-180);
  tft.setFont(SystemFont5x7);
  tft.fillScreen(BLACK);
  tft.drawLine(0,20,320,20,WHITE);
  tft.drawLine(0,211,320,211,timeCodeColour[0]);
  tftStart();
#endif  
  
  Control_Surface.begin(); // Initialize tttapa's Control Surface Magic 
  timerToToogleDisplay = millis(); 

}

void programChange()
{
    modechange += 1;
    if (modechange > 2 ) modechange = 0;
    
#if NOSCREEN==0   
    if (lcdState == 1) tftStart(); // Reseting the display
#endif    
    
    for (int r=0;r<10;r++) // Changing Modes for the Borowed Encoders
    {
     if (encGENERAL[r].isEnabled() && modechange == 2)
     {
        encGENERAL[r].disable();
        encSMART[r].resetPositionOffset();
        encSMART[r].enable();
     }
     else if (encSMART[r].isEnabled() && modechange == 0)
     {
        encSMART[r].disable();
        encEQ[r].resetPositionOffset();
        encEQ[r].enable();
     }
     else if (encEQ[r].isEnabled() && modechange == 1)
     {
        encEQ[r].disable();
        encGENERAL[r].resetPositionOffset();
        encGENERAL[r].enable();
     }
    } 
}

void loop() {

  Control_Surface.loop(); // tttapa's magic sauce
  
  if (timerToToogleDisplay - millis() > 3000 && timerToToogleDisplay != 0)
  {
    Control_Surface.MIDI().sendCC({3, CHANNEL_6}, 127);
    timerToToogleDisplay = 0;
  }

  static uint32_t lastService = 0;
  for (int i=0;i<10;i++) // START BUTTONS SERVICES
  {
    if (lastService + 1000 < micros())
      {
        b[i].service(); 
      } 
  }

  for (int k=0;k<10;k++) // READ BUTTONS
  {
    bValue[k] = b[k].getButton(); 
  }

  for (int j=0;j<10;j++) // LOGIC FOR ENCODER BUTTONS
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


  if (Fn4.update() == Button::Rising) // BUTTON FOR CHANGING MODES
  {
     programChange();
  }

#if NOSCREEN==0 // SCREEN UPDATE FUNCTION
  if (lcdState == 1) if (millis() - fpsTimer > (1000/(ScreenFrameRate*frameMultiplier)) ) ScreenUpdate();
  frameCount += 1;
#endif 


if (vPOTstick.takeScreenShoot==1)
  {
    Serial.println("Get ready for the pic: ");
    tft.screenshotToConsole();
    vPOTstick.takeScreenShoot = !vPOTstick.takeScreenShoot; 
  }


}
 
 
