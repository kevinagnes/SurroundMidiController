#include <Arduino.h> 
#include <GLOBAL.h> 
#include <MIDI.h> 
#if NOSCREEN==0
#include <DISPLAY.h>
#endif

void setup() 
{
  RelativeCCSender::setMode(relativeCCmode::MACKIE_CONTROL_RELATIVE);

  #if debugging==true 
    Serial.begin(115200);
    Serial.println("Hello Bitches! I am awaken");
  #endif
  
  mux.begin(); // initiate Multiplexer for buttons 
  #if NOSCREEN==0          
    pinMode(TFT_LED,OUTPUT);
    digitalWrite(TFT_LED,lcdState);
  #endif  

  for (int r=0;r<10;r++) encEQ[r].disable();
  for (int r=0;r<10;r++) encSMART[r].disable();
  
  #if NOSCREEN==0
  tft.begin();
  tft.setRotation(iliRotation90);
  tft.setArcParams(127);
  tft.setAngleOffset(-180);
  tft.setFont(SystemFont5x7);
  tft.fillScreen(BLACK);
  tft.drawBitmap(hub_vetor_pbBitmaps,90,54,141,141,RED);
  delay(8000);
  tft.fillScreen(BLACK);
  tft.drawLine(0,20,320,20,WHITE);
  tft.drawLine(0,211,320,211,timeCodeColour[0]);
  tftStart();
  #endif  
  
  Control_Surface.begin(); // Initialize tttapa's Control Surface Magic 
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
void lcdControl()
{
  if (lcdLvl  < 5) lcdLvl += 1;
  if (lcdLvl  > 4) lcdLvl  = 0;
  if (lcdLvl == 1) lcdBrightness = 255;
  if (lcdLvl == 2) lcdBrightness = 127;
  if (lcdLvl == 3) lcdBrightness = 64;
  if (lcdLvl == 4) lcdBrightness = 0;
  if (lcdBrightness == 0) lcdState = 0;
  if (lcdBrightness  > 1) lcdState = 1;
  analogWrite(TFT_LED,lcdBrightness);
}
int changeBank(int j , int bankNumber)
{
  ccMode[j] = bankNumber;
  bank[j].select(bankNumber);
}

void loop() {

  Control_Surface.loop(); // tttapa's magic sauce
  
  if (timerToToogleDisplay == 1)
  {
      Control_Surface.MIDI().sendCC({3, CHANNEL_6}, 127);
      timerToToogleDisplay = 0;
  }
  

  // MAIN LOGIC ////////////////////////////////////////
  for (int j=0; j<10; j++) 
  {
    if (encoderfn1.update() == Button::Pressed) changeBank(j,1);
    if (encoderfn2.update() == Button::Pressed) changeBank(j,2);
    if (!encoderB[j].getButtonState()) changeBank(j,3);
    if (encoderfn1.update() == Button::Released && encoderfn2.update() == Button::Released) changeBank(j,0);
  }
  if (joybutton.update() == Button::Rising) lcdControl();
  if (Fn4.update() == Button::Rising) programChange();
  ////////////////////////////////////////
  #if NOSCREEN==0 // SCREEN UPDATE FUNCTION
  if (lcdState == 1) if (millis() - fpsTimer > (1000/(ScreenFrameRate*frameMultiplier)) ) ScreenUpdate();
  /*
  if (bValue[8]==(ClickEncoder::Held) && toggleConfirmer[0] == 0) 
  {
      toggleConfirmer[1] = !toggleConfirmer[1];
      timerToConfirmDisplayAction = millis();
      
      if(toggleConfirmer[0] == 0 && toggleConfirmer[1] == 1)
      {
        Serial.println("Get ready for the pic: ");
        tft.screenshotToConsole();
        toggleConfirmer[0] = !toggleConfirmer[0];
      }
  }
  */
  #endif 
}
 
 
