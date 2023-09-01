# SparkFun HX711F Load Cell Amplifier Driver

## Getting Started
Currently the `pin_write()` and `pin_read()` macros are mapped to Nordic nRF5 SDK specific gpio functions, if you are using a different micro you'll have to modify these to fit your environment and update the includes. 

Configure data out pin as an input and clock pin as an output. Choose a gain of 32, 64, or 128. Call `hx711_adc_init()`.

I found that sampling the voltage (each call to `adc_read_voltage()`) works best on a 500ms interval. 

## Debug Output
A precompiler directive is used to turn debug output on and off. Currently all of the outputs are using `NRF_LOG_INFO` which is a Nordic nRF5 SDK specific function, change these to printf or whatever your micro environment uses.

