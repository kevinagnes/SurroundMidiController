#define debugging false
#define NOSCREEN 0
#define ScreenFrameRate 30   // Refresh rate for the screen 

#include <Encoder.h>         // Include the Encoder library.
#include <ClickEncoder.h>    // For the Encoder Buttons
#include <Control_Surface.h> // Include the Control Surface library
#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>
#include <AH/Hardware/Button.hpp>
#include <joystickClass.hpp> 
#include <JoystickClassII.hpp> 

// DISPLAY
#include <SPI.h>
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include <SystemFont5x7.h>
#include <fonts/Arial_bold_14.h>
#define TFT_CS 11 // 10
#define TFT_DC 10 // 9
#define TFT_RST 9 // 8
#define TFT_LED 8 // 7
#define BLACK           0x0000
#define RED             0xF800
#define GREEN           0x07E0
#define BLUE            0x001F
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define ORANGE          0xFD20
#define GREENYELLOW     0xAFE5 
#define DARKGREEN       0x03E0
#define WHITE           0xFFFF
#define PINK            0xC177
#define GREYGREEN       0xDF1A


// ENCODER
#define speedMultiplier 1         // encoder rotation speed
#define encoderType 4             // type of encoder
#define timeBackToBankZero 500    // timer
#define logCurveDb 8

unsigned long timer[10], 
              txtTimer,
              frameTimer, 
              backToLevelsTimer, 
              timerViewChange,
              refreshTime,
              previousDecay[6],
              decayTime[6],
              fpsTimer;
                 
const byte ccEnc[10] = {20,24,28,52,56,60,85,102,106,110},  // CC messages
           bEnc[10]  = {43,42,45,44,47,46,49,48,51,50},     // Button Pins
           buttonMidi[] = {119,118,117,116,115,114};
           

const int xCompPos[8] = { 0, 0,-120,-120, 90, 90, 0, 0},
          yCompPos[8] = { 0, 0, -40,  60,-40, 60, 0, 0};
      
bool toggle[10], 
     timerToggle[10],
     boolmode=1,
     sleepState = false,
     lcdState = 1,
     light,
     changeViewMode = false,
     decaying[6],
     transportToggle[6],
     oldtransportToggle[6],
     EqIsOn[8],
     ViewChange,
     surroundToggle[2], 
     isTimecode;        

uint8_t ccMode[10],
        blockheight = 8, //8
        spacing = 2, // 2
        circleSize = 20,
        frameMultiplier = 1;
        
int soloColour[6], peakValue[6], cccConverted[10];

const uint16_t xPos[8] = { 35, 55, 120, 120, 190, 190, 260, 285},
               yPos[8] = {180, 110, 80, 170,  80, 170, 110, 180},
               circleColour[] = {BLACK,RED,BLUE,YELLOW},
               vuXpos[6] = {10, 60, 110, 160, 210, 260}, 
               vunameXpos[6] = {260,210,160,110,60,10}, 
               vuYpos = 160, 
               vuWidth = 30;           

uint16_t vuColor[6] = {GREEN,GREEN,GREEN,GREEN,GREEN,GREEN},
         frameCount,
         x,
         y = 172, // 172
         previousxx,
         previousyy,
         width = 30,
         color = GREEN,
         TRANSPORT_COLOUR = ORANGE,
         timeCodeColour[3] = {ORANGE,WHITE,RED},
         peak[6],
         newPeak[6];
         
float vuValue[6];

String EQText1[8] = {"HPF"," LS"," P1"," P2"," P3"," P4"," HS","LPF"},
       EQText2[4] = {"Freq","Gain","Qfac","    "},
       EqFreqToPrint[8], EqGainToPrint[8], EqQToPrint[8],EqONOFFToPrint[8],
       CompText1[10]= {"NUDGE 1F","SCRUB","THRESHOLD","ATTACK","RATIO","RELEASE","H. ZOOM","TRACK","SEND A","S. SPREAD" }, // -50,0db | 0,200ms | 1,30:1 | 5,5000ms
       CompText1scale[10] = {"","","db","ms",":1","ms","","db","",""},
       CompText2scale[10] = {"","","","","db","%","db","","","db"},
       CompType[10] = {"DIGITAL","VCA","FET","VCA 1","VCA 2","FET","OPTO"},
       CompText2[10]= {"NUDGE .5F","FRAME","CIRCUIT","MAKEUP","KNEE","OUT MIX"," V. ZOOM","MASTER","SEND B","S. LFE" }, //DIGITAL,VCA,FET,VCA1,VCA2,FET,OPTO | 0,1 linear | -20,50db | 0,100%
       vuNames[6] = {"LFE", "Rs", "Ls", "R", "C", "L"},
       modes[2] = {"EQ MODE", "GENERAL MODE"},
       tDisplay,
       blankTxt = "       ",
       timerToDisplay, 
       GeneralPrinter;

  char barStr[6], 
       beatStr[3],
       frameStr[4];

byte oldcc[10],
     oldccc[10],
     oldSurroundJoy[2], 
     oldccmuxTransport[6],
     oldccmuxChannel[6];


#if debugging==true
unsigned long debugDelay,
              olddebugDelay;
#endif     
