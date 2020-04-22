#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonMatrix.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input from a **matrix
 *          of momentary push buttons or switches**, and send out MIDI **Note**
 *          events.
 * 
 * A Note On event is sent when a button is pressed, and a Note Off
 * event is sent when a button is released.  
 * Crude software debouncing is implemented by limiting the refresh rate.  
 * This version can be banked.  
 * 
 * @tparam  nb_rows
 *          The number of rows of the matrix.
 * @tparam  nb_cols
 *          The number of columns of the matrix.
 *
 * @ingroup BankableMIDIOutputElements
 */
template <uint8_t nb_rows, uint8_t nb_cols>
class NoteButtonMatrix
    : public MIDIButtonMatrix<MatrixAddress<nb_rows, nb_cols>,
                              DigitalNoteSender, nb_rows, nb_cols> {
  public:
    /**
     * @brief   Create a new Bankable NoteButtonMatrix object with the given 
     *          pins, note numbers and channel.
     *
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix.  
     *          **⚠** These pins will be driven LOW as outputs (Lo-Z).
     * @param   colPins
     *          A list of pin numbers connected to the columns of the button
     *          matrix.  
     *          These pins will be used as inputs (Hi-Z), and the
     *          internal pull-up resistor will be enabled.
     * @param   notes
     *          A 2-dimensional array of the same dimensions as the button
     *          matrix that contains the note number of each button. [0, 127]
     * @param   channelCN
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and Cable Number 
     *          [0, 15].
     * @param   velocity
     *          The velocity of the MIDI Note events.
     */
    NoteButtonMatrix(const OutputBankConfig &config,
                     const PinList<nb_rows> &rowPins,
                     const PinList<nb_cols> &colPins,
                     const AddressMatrix<nb_rows, nb_cols> &notes,
                     MIDICNChannel channelCN = {CHANNEL_1, 0},
                     uint8_t velocity = 0x7F)
        : MIDIButtonMatrix<MatrixAddress<nb_rows, nb_cols>, DigitalNoteSender,
                           nb_rows, nb_cols>{
              {config, notes, channelCN},
              rowPins,
              colPins,
              {velocity},
          } {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

} // namespace Bankable

END_CS_NAMESPACE