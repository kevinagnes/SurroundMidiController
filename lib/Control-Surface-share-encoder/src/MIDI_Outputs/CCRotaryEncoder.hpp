#pragma once

#include <MIDI_Outputs/Abstract/MIDIRotaryEncoder.hpp>
#include <MIDI_Senders/RelativeCCSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **quadrature
 *          (rotary) encoder** and send out relative MIDI **Control Change**
 *          events.
 * 
 * This version cannot be banked.
 *
 * @note    To use this class, include the [PJRC Encoder library]
 *          (https://github.com/PaulStoffregen/Encoder) before the
 *          Control-Surface library.
 *
 * @ingroup MIDIOutputElements
 */
class CCRotaryEncoder : public MIDIRotaryEncoder<RelativeCCSender> {
  public:
    /**
     * @brief   Construct a new CCRotaryEncoder object with the given pins, 
     *          address, channel, speed factor, and number of pulses per step.
     * 
     * @param   encoder
     *          The Encoder object to use.  
     *          Usually passed as a list of the two pins connected to the 
     *          A and B outputs of the encoder, e.g. `{2, 3}`.  
     *          The internal pull-up resistors will be enabled by the Encoder
     *          library.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [0, 15].
     * @param   speedMultiply
     *          A constant factor to increase the speed of the rotary encoder.
     *          The difference in position will just be multiplied by this 
     *          factor. 
     * @param   pulsesPerStep
     *          The number of pulses per physical click of the encoder.
     *          For a normal encoder, this is 4. If you want to increase the
     *          resolution, for the use of Jog wheels, for example, you can go
     *          as 1.  
     *          Whereas a greater speedMultiplier factor will increase the 
     *          speed, increasing the number of pulsesPerStep will result in a 
     *          lower speed.
     * @param   sender
     *          The MIDI sender to use.
     */
    CCRotaryEncoder(Encoder &&encoder, const MIDICNChannelAddress &address,
                    int8_t speedMultiply = 1, uint8_t pulsesPerStep = 4,
                    const RelativeCCSender &sender = {})
        : MIDIRotaryEncoder<RelativeCCSender>(std::move(encoder), address,
                                              speedMultiply, pulsesPerStep,
                                              sender) {}

// For tests only (PJRC Encoder library's copy constructor doesn't work)
#ifndef ARDUINO
    CCRotaryEncoder(const Encoder &encoder, const MIDICNChannelAddress &address,
                    int8_t speedMultiply, uint8_t pulsesPerStep,
                    const RelativeCCSender &sender = {})
        : MIDIRotaryEncoder<RelativeCCSender>(encoder, address, speedMultiply,
                                              pulsesPerStep, sender) {}
#endif
};

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **quadrature
 *          (rotary) encoder** and send out relative MIDI **Control Change**
 *          events. This class just saves a reference to the encoder you give
 *          it, so it can be shared with other classes.
 * 
 * This version cannot be banked.
 *
 * @note    To use this class, include the [PJRC Encoder library]
 *          (https://github.com/PaulStoffregen/Encoder) before the
 *          Control-Surface library.
 *
 * @ingroup MIDIOutputElements
 */
class BorrowedCCRotaryEncoder
    : public BorrowedMIDIRotaryEncoder<RelativeCCSender> {
  public:
    /**
     * @brief   Construct a new CCRotaryEncoder object with the given pins, 
     *          address, channel, speed factor, and number of pulses per step.
     * 
     * @param   encoder
     *          The Encoder object to use. Saved by reference, so it must 
     *          out-live this instance.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [0, 15].
     * @param   speedMultiply
     *          A constant factor to increase the speed of the rotary encoder.
     *          The difference in position will just be multiplied by this 
     *          factor. 
     * @param   pulsesPerStep
     *          The number of pulses per physical click of the encoder.
     *          For a normal encoder, this is 4. If you want to increase the
     *          resolution, for the use of Jog wheels, for example, you can go
     *          as 1.  
     *          Whereas a greater speedMultiplier factor will increase the 
     *          speed, increasing the number of pulsesPerStep will result in a 
     *          lower speed.
     * @param   sender
     *          The MIDI sender to use.
     */
    BorrowedCCRotaryEncoder(Encoder &encoder,
                            const MIDICNChannelAddress &address,
                            int8_t speedMultiply = 1, uint8_t pulsesPerStep = 4,
                            const RelativeCCSender &sender = {})
        : BorrowedMIDIRotaryEncoder<RelativeCCSender>(
              encoder, address, speedMultiply, pulsesPerStep, sender) {}
};

END_CS_NAMESPACE