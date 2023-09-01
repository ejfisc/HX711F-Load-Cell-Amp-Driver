/* ****************************************************************************/
/** HX711 Load Cell Amplifier Driver

  @File Name
    hx711_adc.h

  @Summary
    Driver Library for the sparkfun hx711f load cell amplifier

  @Description
    Defines functions that allow the user to interact with the ADC
******************************************************************************/

#ifndef ANALOG_TO_DIGITAL_H
#define ANALOG_TO_DIGITAL_H

#include <inttypes.h>
#include <stdbool.h>

// #define DEBUG_OUTPUT // comment this line to turn off debug output
#ifdef DEBUG_OUTPUT
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#endif 

#define GAIN_128 128
#define GAIN_64 64
#define GAIN_32 32

/*
    @brief Initializes the ADC

    @note sets the data out and serial clock pins

    @param[in] data_pin Data Out pin number

    @param[in] clock_pin Serial Clock pin number
*/
void hx711_adc_init(uint32_t data_pin, uint32_t clock_pin, uint8_t gain);

/*
    @brief Set amplification factor

    @param[in] gain Gain factor, 128, 64, or 32
*/
void set_gain(uint8_t gain);

/*
    @brief Function for checking if ADC is ready

    @note Reads data out pin

    @ret True if ADC is ready (data out pin reads 0), False if not
*/
bool adc_is_ready();

/*
    @brief Wait for ADC to be ready

    @note Sits in a while loop until ADC is ready
*/
void adc_wait_ready();

/*
    @brief Reads data from ADC

    @note pulses clock pin and reads on pos edge

    @ret Signed 32-bit integer ADC code
*/
long adc_read();

/*
    @brief Read voltage signal from ADC

    @note Reads ADC code and calculates voltage

    @note (ADC Code / Max Possible Code) * Differential input voltage

    @ret Sense voltage signal from ADC (float)
*/
float adc_read_voltage();

#endif // ANALOG_TO_DIGITAL_H