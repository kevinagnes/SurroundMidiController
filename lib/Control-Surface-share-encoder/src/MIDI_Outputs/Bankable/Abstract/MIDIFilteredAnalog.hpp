#pragma once

#include <AH/Hardware/FilteredAnalog.hpp>
#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class for potentiometers and faders that send MIDI events and that
 *          can be added to a Bank.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @see     FilteredAnalog
 */
template <class BankAddress, class Sender>
class MIDIFilteredAnalogAddressable : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalog.
     *
     * @param   bankAddress
     *          The bankable MIDI address to send to.
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIFilteredAnalogAddressable(const BankAddress &bankAddress,
                                  pin_t analogPin, const Sender &sender)
        : address{bankAddress}, filteredAnalog{analogPin}, sender(sender) {}

  public:
    void begin() override {}
    void update() override {
        if (filteredAnalog.update())
            sender.send(filteredAnalog.getValue(), address.getActiveAddress());
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of @f$ 16 - 
     *          \mathrm{ANALOG\_FILTER\_SHIFT\_FACTOR} @f$ bits as a parameter, 
     *          and should return a value in the same range.
     * 
     * @see     FilteredAnalog::map     
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

    /// Invert the analog value.
    void invert() { filteredAnalog.invert(); }

    /**
     * @brief   Get the raw value of the analog input (this is the value 
     *          without applying the filter or the mapping function first).
     */
    analog_t getRawValue() const { return filteredAnalog.getRawValue(); }

    /**
     * @brief   Get the value of the analog input (this is the value after first
     *          applying the mapping function).
     */
    analog_t getValue() const { return filteredAnalog.getValue(); }

  private:
    BankAddress address;
    AH::FilteredAnalog<Sender::precision()> filteredAnalog;

  public:
    Sender sender;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for potentiometers and faders that send MIDI events (with
 *          only a channel, no address) and that can be added to a Bank.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @see     FilteredAnalog
 */
template <class BankAddress, class Sender>
class MIDIFilteredAnalog : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalog.
     *
     * @param   bankAddress
     *          The bankable MIDI address to send to.
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIFilteredAnalog(const BankAddress &bankAddress, pin_t analogPin,
                       const Sender &sender)
        : address(bankAddress), filteredAnalog(analogPin), sender(sender) {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            sender.send(filteredAnalog.getValue(), address.getActiveAddress());
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of @f$ 16 - 
     *          \mathrm{ANALOG\_FILTER\_SHIFT\_FACTOR} @f$ bits as a parameter, 
     *          and should return a value in the same range.
     * 
     * @see     FilteredAnalog::map     
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

    /// Invert the analog value.
    void invert() { filteredAnalog.invert(); }

    /**
     * @brief   Get the raw value of the analog input (this is the value 
     *          without applying the filter or the mapping function first).
     */
    analog_t getRawValue() const { return filteredAnalog.getRawValue(); }

    /**
     * @brief   Get the value of the analog input (this is the value after first
     *          applying the mapping function).
     */
    analog_t getValue() const { return filteredAnalog.getValue(); }

  private:
    BankAddress address;
    AH::FilteredAnalog<Sender::precision()> filteredAnalog;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE