#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIChordButton.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Note** events.
 * 
 * A Note On event is sent when the button is pressed, and a Note Off
 * event is sent when the button is released.  
 * The button is debounced in software.  
 * This version can be banked.  
 *
 * @ingroup BankableMIDIOutputElements
 */
class NoteChordButton : public MIDIChordButton<DigitalNoteSender> {
  public:
    /**
     * @brief   Create a new Bankable NoteChordButton object with the given pin, 
     *          note number, channel and chord.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address of the base note, containing the note
     *          number [0, 127], channel [CHANNEL_1, CHANNEL_16], and optional 
     *          cable number [0, 15].
     * @param   chord
     *          The chord containing the intervals of the other notes to play.
     * @param   velocity
     *          The velocity of the MIDI Note events.
     * 
     * @tparam  N
     *          The number of notes in the chord.
     */
    template <uint8_t N>
    NoteChordButton(const OutputBankConfig &config, pin_t pin,
                    const MIDICNChannelAddress &address, const Chord<N> &chord,
                    uint8_t velocity = 0x7F)
        : MIDIChordButton<DigitalNoteSender>{
              config, pin, address, chord, {velocity},
          } {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

} // namespace Bankable

END_CS_NAMESPACE