#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR()

#include <AH/Containers/ArrayHelpers.hpp>
#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/PitchBendSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {
namespace ManyAddresses {

/**
 * @brief   A class of MIDIOutputElement%s that read the analog input from a
 *          **potentiometer or fader**, and send out 14-bit MIDI **Pitch Bend** 
 *          events.
 * 
 * The analog input is filtered and hysteresis is applied for maximum
 * stability.  
 * The actual precision is "only" 10 bits, because this is the resolution of the
 * built-in ADC, and this is the default resolution used by the Mackie Control
 * Universal protocol.  
 * This version can be banked using an arbitrary list of alternative
 * addresses.
 *          
 * @tparam  N
 *          The number of variants/alternative addresses the element has.
 *
 * @ingroup ManyAddressesMIDIOutputElements
 */
template <setting_t N>
class PBPotentiometer
    : public Bankable::MIDIFilteredAnalog<ManyAddresses<N>,
                                          PitchBendSender<10>> {
  public:
    /** 
     * @brief   Create a new Bankable PBPotentiometer object with the given 
     *          analog pin and channel.
     * 
     * @param   bank
     *          The bank that selects the address to use.
     * @param   analogPin
     *          The analog input pin to read from.
     * @param   addresses
     *          The list of MIDI channels [CHANNEL_1, CHANNEL_16] and optional 
     *          Cable Numbers [0, 15].
     * @param   sender
     *          The MIDI sender to use.
     */
    PBPotentiometer(const Bank<N> &bank, pin_t analogPin,
                    const Array<MIDICNChannel, N> &addresses,
                    const PitchBendSender<10> &sender = {})
        : MIDIFilteredAnalog<ManyAddresses<N>, PitchBendSender<10>>{
              {bank, AH::copyAs<MIDICNChannelAddress>(addresses)},
              analogPin,
              sender,
          } {}
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
