#include <Arduino.h> 
#include <Control_Surface.h>
#include <Arduino_Helpers.h>
#include <AH/Hardware/Button.hpp>
#include <Keyboard.h>


#define K_UP       0xDA
#define K_DOWN     0xD9
#define K_LEFT     0xD8
#define K_RIGHT    0xD7
#define KEY_RETURN 0xB0

class ArrowJoystick : public MIDIOutputElement 
{
  public:
    ArrowJoystick(PinList<2> analogpins, pin_t buttonPin, pin_t buttonPin2, Button activatePin, MIDICNChannel channel)
      : joyX(analogpins[0]), joyY(analogpins[1]), buttonPin(buttonPin),buttonPin2(buttonPin2), activatePin(activatePin), channel(channel) {}

    void begin() override 
    {
        pinMode(buttonPin, INPUT_PULLUP);
        pinMode(buttonPin2, INPUT_PULLUP);
        activatePin.begin();  
          
    }


    void update() override 
    {
      uint16_t keydelay = 280;
      if (digitalRead(buttonPin) == HIGH && digitalRead(buttonPin2) == HIGH) 
      {
        bool changed = false;
        changed |= joyY.update();

        if (activatePin.update() == Button::Rising)  
        {   
          Control_Surface.MIDI().sendCC({3, channel}, 127);
          Serial.println("I sent a Midi CC: " + String(127));
        }

        if (!changed)
        return;

        constexpr int bias = 512;
        byte     threshh   =  30;
        int y = joyY.getValue() - bias; // Position 0 (is actually 512)

        if (millis() - timerCheck > keydelay)
        {
          if (y > threshh) 
          {
            Control_Surface.MIDI().sendCC({MCU::V_POT_1, channel}, map(y,30,512,0,127)); 
            timerCheck = millis();
          }
          if (y <-threshh) 
          {
            Control_Surface.MIDI().sendCC({MCU::V_POT_2, channel}, map(y,-30,-512,0,127));
            timerCheck = millis();
          }
        }

        
      }

      if (digitalRead(buttonPin) == LOW && digitalRead(buttonPin2) == HIGH)  {
      
      if (activatePin.update() == Button::Rising)  
      {
        Keyboard.press(KEY_RETURN); Keyboard.release(KEY_RETURN);
      }
      
      bool changed = false;
      changed |= joyX.update();
      changed |= joyY.update();

      if (!changed)
        return;

      constexpr int bias = 512;
      int x = joyX.getValue() - bias; // Position 0 (is actually 512)
      int y = joyY.getValue() - bias; // Position 0 (is actually 512)

      byte     thresh   =  70;
      
      
      if (millis() - timerCheck > keydelay)
      {
 
        if (x >  thresh) 
        {
          //Control_Surface.MIDI().sendCC({MCU::V_POT_1, channel},  127);
          Keyboard.press(K_UP); Keyboard.release(K_UP); 
          timerCheck = millis();
           
        }
        if (x < -thresh) 
        {
          //Control_Surface.MIDI().sendCC({MCU::V_POT_1, channel}, 0);
          Keyboard.press(K_DOWN); Keyboard.release(K_DOWN);
          timerCheck = millis();
          
        }
        if (y >  thresh) 
        {
          //Control_Surface.MIDI().sendCC({MCU::V_POT_2, channel},  2);
          Keyboard.press(K_RIGHT); Keyboard.release(K_RIGHT);
          timerCheck = millis();
          
        }
        if (y < -thresh) 
        {
          //Control_Surface.MIDI().sendCC({MCU::V_POT_2, channel}, 66);
          Keyboard.press(K_LEFT); Keyboard.release(K_LEFT);
          timerCheck = millis();
          
        }
      }
    }
  }
    
  private:
    FilteredAnalog<> joyX, joyY;
    pin_t buttonPin;
    pin_t buttonPin2;
    Button activatePin;
    bool  activateJoy;
    unsigned long timerCheck;     
    MIDICNChannel channel;
  public:
    bool takeScreenShoot = 0;  
};
