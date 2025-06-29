# Air Pressure-MIDI Controller (HX710b + Raspberry Pi Pico)

This project uses a **Raspberry Pi Pico** running Arduino code to read air pressure data from an MPS20N0040D sensor using **HX710b** 24-bit ADC and convert the data into **MIDI Control Change (CC)** messages over USB using **TinyUSB** stack.

Perfect for DIY musical interfaces such as breath controllers, pressure pads, or force-sensitive MIDI instruments.

## Arduino Library Dependencies

Make sure to install the following libraries via the Arduino Library Manager or manually:
- Adafruit TinyUSB Library 
- HX710 Arduino library
- MIDI library 

## Wiring

| HX710B Pin | Connect To (Raspberry Pi Pico) |
|------------|-------------------------------|
| VCC        | 3.3V *(recommended)*           |
| GND        | GND                           |
| DT OUT     | GPIO12 (or any digital pin)    |
| SCK        | GPIO14 (or any digital pin)    |

## Calibration

Adjust MIN_RAW, MAX_RAW,  
POT_PIN 29 is mapped to FALL (release) and SEND interval factor  

## Output

When pressure is applied:

1. The serial console prints:   
   Raw: value | Midi: value (0-127)

2. Pico sends MIDI CC messages over usb. Default settings:

- **CC Number**: 2 (Breath Control)
- **MIDI Channel**: 1
- **Update Rate**: ~10–50ms (adjustable)






