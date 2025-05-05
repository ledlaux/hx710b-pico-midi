# Pressure-to-MIDI Controller (HX710B + Raspberry Pi Pico W)

This project uses a **Raspberry Pi Pico W** running Arduino code to read pressure data from an MPS20N0040D sensor using **HX710B** 24-bit ADC and convert the data into **MIDI Control Change (CC)** messages over USB using **TinyUSB MIDI**.

Perfect for DIY musical interfaces such as breath controllers, pressure pads, or force-sensitive MIDI instruments.


## Arduino Library Dependencies

Make sure to install the following libraries via the Arduino Library Manager or manually:
- [`Adafruit TinyUSB Library`](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)
- `HX710B` or `HX711` compatible library (whichever you’re using or adapting)
- `MIDI` library (optional if using USB directly via TinyUSB)

## Wiring

| HX710B Pin | Connect To         |
|-----------|--------------------|
| VCC       | 3.3V (recomended) on Raspberry Pico   |
| GND       | GND on Pico W      |
| DT        | GPIO (e.g., GP12)   |
| SCK       | GPIO (e.g., GP14)   |


