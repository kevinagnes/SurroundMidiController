/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for collections of LEDs that can display ranges.
 * 
 * @tparam  N
 *          The number of LEDs in the collection.
 * 
 * @ingroup AH_HardwareUtils
 */
template <uint8_t N>
class LEDs {
  public:
    /**
     * @brief   Create a LEDs object.
     * 
     * @param   ledPins
     *          An array of pins with the LEDs connected.
     */
    LEDs(const PinList<N> &ledPins) : ledPins(ledPins) {}

    /**
     * @brief   Initialize (set LED pins as outputs).
     */
    void begin() const {
        for (const pin_t &pin : ledPins)
            ExtIO::pinMode(pin, OUTPUT);
    }

    /**
     * @brief   Turn on a range of the LEDs.
     * 
     * @param   startOn
     *          The first LED of the range to turn on (the LEDs before this one
     *          are turned off).
     * @param   startOff
     *          The first LED after the range to turn off.
     */
    void displayRange(uint8_t startOn, uint8_t startOff) const {
        for (uint8_t pin = 0; pin < startOn; pin++)
            clear(pin);
        for (uint8_t pin = startOn; pin < startOff; pin++)
            set(pin);
        for (uint8_t pin = startOff; pin < N; pin++)
            clear(pin);
    }

    /// Turn on the given LED.
    void set(uint8_t index) const {
        // TODO: bounds check?
        ExtIO::digitalWrite(ledPins[index], HIGH);
    }

    /// Turn off the given LED.
    void clear(uint8_t index) const {
        // TODO: bounds check?
        ExtIO::digitalWrite(ledPins[index], LOW);
    }

    /**
     * @brief   Turn on a single LED, and turn off all others.
     * 
     * @param   led
     *          The LED to turn on.
     */
    void displayDot(uint8_t led) const { displayRange(led, led + 1); }

    /**
     * @brief   Turn off all LEDs.
     */
    void clear() const {
        for (pin_t pin : ledPins)
            ExtIO::digitalWrite(pin, LOW);
    }

  private:
    const PinList<N> ledPins;
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
