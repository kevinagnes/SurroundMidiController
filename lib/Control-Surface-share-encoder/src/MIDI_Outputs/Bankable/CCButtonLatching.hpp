#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonLatching.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **latching
 *          push button or toggle switch**, and send out MIDI **Control Change**
 *          events.
 * 
 * When the switch changes state, two MIDI events are sent: the first
 * one with a value of 0x7F, followed immediately by a second one with
 * a value of 0x00.  
 * The switch is debounced in software.  
 * This version can be banked.
 *
 * @ingroup BankableMIDIOutputElements
 */
class CCButtonLatching
    : public MIDIButtonLatching<SingleAddress, DigitalCCSender> {
  public:
    /**
     * @brief   Create a new Bankable CCButtonLatching object with the given
     *          pin, the given controller number and channel.
     *
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [0, 15].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButtonLatching(const OutputBankConfig &config, pin_t pin,
                     const MIDICNChannelAddress &address,
                     const DigitalCCSender &sender = {})
        : MIDIButtonLatching<SingleAddress, DigitalCCSender>{
              {config, address}, pin, sender} {}
};

} // namespace Bankable

END_CS_NAMESPACE