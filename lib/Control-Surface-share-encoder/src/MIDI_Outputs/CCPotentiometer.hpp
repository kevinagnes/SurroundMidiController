#pragma once

#include <MIDI_Outputs/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/ContinuousCCSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 7-bit MIDI 
 *          **Control Change** events.
 * 
 * The analog input is filtered and hysteresis is applied for maximum
 * stability.  
 * This version cannot be banked.
 *
 * @ingroup MIDIOutputElements
 */
class CCPotentiometer
    : public MIDIFilteredAnalogAddressable<ContinuousCCSender> {
  public:
    /** 
     * @brief   Create a new CCPotentiometer object with the given analog pin, 
     *          controller number and channel.
     * 
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [0, 15].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCPotentiometer(pin_t analogPin, const MIDICNChannelAddress &address,
                    const ContinuousCCSender &sender = {})
        : MIDIFilteredAnalogAddressable(analogPin, address, sender) {}
};

END_CS_NAMESPACE