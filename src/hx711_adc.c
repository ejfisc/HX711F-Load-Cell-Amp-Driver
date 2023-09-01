/* ****************************************************************************/
/** HX711 Load Cell Amplifier Driver

  @File Name
    hx711_adc.c

  @Summary
    Driver Library for the sparkfun hx711f load cell amplifier

  @Description
    Implements functions that allow the user to interact with the ADC
******************************************************************************/

#include "hx711_adc.h"
#include "nrf_gpio.h" // nordic specific gpio library

#define pin_read(pin_no) nrf_gpio_pin_read(pin_no) // macro to redirect to sdk specific pin read function
#define pin_write(pin_no, value) nrf_gpio_pin_write(pin_no, value) // macro to redirect to sdk specific pin write function

#define CHECK_BIT(var, pos) ((var>>pos) & 1)

static uint32_t dout_pin = 0; // data out pin
static uint32_t sck_pin = 0; // serial clock pin
static uint8_t gain; // amplification factor

/*
    @brief Initializes the ADC

    @note sets the data out and serial clock pins

    @param[in] data_pin Data Out pin number

    @param[in] clock_pin Serial Clock pin number
*/
void hx711_adc_init(uint32_t data_pin, uint32_t clock_pin, uint8_t gain_set) {
    // set gain and clock/data
    dout_pin = data_pin;
    sck_pin = clock_pin;
    set_gain(gain_set);
    adc_wait_ready(); // wait for ADC to be ready
}

/*
    @brief Set amplification factor

    @param[in] gain Gain factor, 128, 64, or 32
*/
void set_gain(uint8_t gain_set) {
    switch(gain_set) {
	case 128:
	    gain = 1;
	    break;
	case 64:
	    gain = 3;
	    break;
	case 32:
	    gain = 2;
	    break;
    }
}

/*
    @brief Function for checking if ADC is ready

    @note Reads data out pin

    @ret True if ADC is ready (data out pin reads 0), False if not
*/
bool adc_is_ready() {
    return pin_read(dout_pin) == 0;
}

/*
    @brief Wait for ADC to be ready

    @note Sits in a while loop until ADC is ready
*/
void adc_wait_ready() {
    while(!adc_is_ready()) { /* wait for adc to be ready */ }
}

/*
    @brief Reads data from ADC

    @note pulses clock pin and reads on pos edge

    @ret Signed 32-bit integer ADC code
*/
int32_t adc_read() {
    adc_wait_ready(); // wait for ADC to be ready
    uint8_t i = 0;
    uint32_t result = 0;

    // pulse clock and read data
    for(i = 0; i < 24; i++) {
	pin_write(sck_pin, 1);
	result |= pin_read(dout_pin) << 23 - i; // read data on pos edge
	pin_write(sck_pin, 0);
    }

    // pulse clock to set gain
    for(i = 0; i < gain; i++) {
	pin_write(sck_pin, 1);
	pin_write(sck_pin, 0);
    }

    if(CHECK_BIT(result, 23)) // if negative, fill MSB with 1s
	result |= 0xff000000;
    
    return (int32_t)(result);
}

/*
    @brief Read voltage signal from ADC

    @note Reads ADC code and calculates voltage

    @note (ADC Code / Max Possible Code) * Differential input voltage

    @ret Sense voltage signal from ADC (float)
*/
float adc_read_voltage() {
    int32_t code = adc_read();
    float voltage = (float)code/8388607.0f;
    // 128 gain -> +- 20mV differential input v
    if(gain == 1)
	voltage * 20.0f;
    else if(gain == 3) // 64 gain -> +- 40mV differential input v
	voltage * 40.0f;
    char str[16];
#ifdef DEBUG_OUTPUT
    snprintf(str, 16, "%.6f", voltage);
    NRF_LOG_INFO("calculated adc voltage: %s", str);
    NRF_LOG_FLUSH();
#endif
    return voltage;
}

