/* ✔ */

#pragma once

#include <MIDI_Outputs/Abstract/MIDIButtonLatched.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or toggle switch**, and send out MIDI **Controller 
 *          Change** events.
 * 
 * It latches the input, so press once to enable, press again to 
 * disable (toggle).
 * 
 * The switch is debounced in software.  
 * This version cannot be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class CCButtonLatched : public MIDIButtonLatched<DigitalCCSender> {
  public:
    /**
     * @brief   Create a new CCButtonLatched object on the given pin and with 
     *          address.
     * 
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [0, 15].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButtonLatched(pin_t pin, const MIDICNChannelAddress &address,
                    const DigitalCCSender &sender = {})
        : MIDIButtonLatched{pin, address, sender} {}
};

END_CS_NAMESPACE