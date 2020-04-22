/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "ShiftRegisterOutBase.hpp"

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <Arduino.h> // MSBFIRST
AH_DIAGNOSTIC_POP()

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for serial-in/parallel-out shift registers, 
 *          like the 74HC595.
 * 
 * @ingroup AH_ExtIO
 */
template <uint8_t N>
class ShiftRegisterOut : public ShiftRegisterOutBase<N> {
  public:
    using BitOrder_t = typename ShiftRegisterOutBase<N>::BitOrder_t;

    /**
     * @brief   Create a new ShiftRegisterOut object with a shift register
     *          connected to the given pins, with a given bit order,
     *          and a given number of outputs.
     * 
     * Multiple shift registers can be cascaded by connecting the serial output
     * of the first one to the input of the second one:
     * ```
     * clockPin >───────────┬──────────────────────┬───────── ⋯
     *              ┏━━━━━━━┷━━━━━━━┓      ┏━━━━━━━┷━━━━━━━┓ 
     *              ┃     SH_CP     ┃      ┃     SH_CP     ┃ 
     * dataPin  >───┨ DS        Q7S ┠──────┨ DS        Q7S ┠─ ⋯
     *              ┃     ST_CP     ┃      ┃     ST_CP     ┃ 
     *              ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛ 
     * latchPin >───────────┴──────────────────────┴───────── ⋯
     * ```
     * 
     * @param   dataPin
     *          The digital output pin connected to the serial data input (DS or
     *          SER) of the shift register.
     * @param   clockPin
     *          The digital output pin connected to the clock input (SH_CP or
     *          SRCLK) of the shift register.
     * @param   latchPin
     *          The digital output pin connected to the latch pin (ST_CP or 
     *          RCLK) of the shift register.
     * @param   bitOrder
     *          Either `MSBFIRST` (most significant bit first) or `LSBFIRST`
     *          (least significant bit first).
     */
    ShiftRegisterOut(pin_t dataPin, pin_t clockPin, pin_t latchPin,
                     BitOrder_t bitOrder = MSBFIRST);

    /**
     * @brief   Initialize the shift register.  
     *          Set the data and clock pins to output mode, 
     *          and set all shift register outputs to `LOW`.
     */
    void begin() override;

    /**
     * @brief   Write the state buffer to the physical outputs.
     */
    void update() override;

  private:
    const pin_t dataPin;
    const pin_t clockPin;
};

END_AH_NAMESPACE

#include "ShiftRegisterOut.ipp"

AH_DIAGNOSTIC_POP()
