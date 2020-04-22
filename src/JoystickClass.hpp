#include <Control_Surface.h>

class PanJoystick : public MIDIOutputElement 
{
  public:
    PanJoystick(PinList<2> pins, pin_t buttonPin, MIDICNChannel channel)
      : joyXX(pins[0]), joyYY(pins[1]), buttonPin(buttonPin), channel(channel) {}

    void begin() override {
        pinMode(buttonPin, INPUT_PULLUP);
    }


    void update() override {
      if (digitalRead(buttonPin) == HIGH)
        return;
      
      changed = false;
      changed |= joyXX.update();
      changed |= joyYY.update();

      constexpr int bias = 512;
      xx = joyXX.getValue() - bias; // Position 0 (is actually 512)
      yy = joyYY.getValue() - bias; // Position 0 (is actually 512)

      if (!changed)
        return;

      
      

      // Math to convert X,Y position to Angle and Distance from the center position
      constexpr float PI_f    = M_PI;
      float angleRad          = atan2(yy, xx);      // [-π, +π]
      angleDeg = angleRad;
      angleRad               += PI_f;               // [0, +2π]
      diversity         = hypot(xx, yy)/bias;       // [0, 1]
      #if (debugging==true)
      Serial.println("diversity: " + (String)diversity);
      #endif
      if (0.96F < diversity > 1.0F) diversity = 1.0F;

      // Map to MIDI CC range
      uint8_t angleMapped     = round(angleRad * 127 / (2 * PI_f)); // [0, 127]
      uint8_t diversityMapped = round(diversity * 127);             // [0, 127]
      if (diversityMapped>127) diversityMapped = 127;               // constraint max val to 127
      if (diversityMapped<0)   diversityMapped = 0;                 // constraint min val to 0

      // Sending the CC MIDI messages (Both CC must be sent at the same time)
      Control_Surface.MIDI().sendCC({MCU::V_POT_1, channel}, angleMapped);
      Control_Surface.MIDI().sendCC({MCU::V_POT_2, channel}, diversityMapped);
    }
    

  private:
    MIDICNChannel channel;
  public:
    bool changed;
    FilteredAnalog<> joyXX, joyYY;
    pin_t buttonPin;
    float angleDeg;
    float diversity;
    int xx;
    int yy;  
};
