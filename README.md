# DS1820
One Wire Protocol library to read the temperature sensor Dallas Maxim DS18S20 (and other DS1820). Written in Arduino C++.

Read out each sensor address individually with isolated connection and store the list of sensor addresses in 'rom_codes.h'. After that you can poll temperatures from your entire bus network.
Implemented according to documentation. All command codes are defined in here, although only the ones relevant to configure temperature polling are fully implemented.

You could read the USB Serial temperature output of the example Arduino program from another computer to e.g. create a Heating Control for your home: https://github.com/MarvinBaral/WebHeatingControl
