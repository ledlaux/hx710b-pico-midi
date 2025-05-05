#include <Adafruit_TinyUSB.h>  
#include "HX710.h"

const int DOUT_PIN = 12;
const int SCK_PIN = 14;

// MIDI Configuration
const uint8_t MIDI_CC_NUM = 1;  // Mod Wheel
const uint8_t MIDI_CH = 1;       // Channel 1

// Calibration
const long MIN_RAW = 545697;
const long MAX_RAW = 1000000;

HX710 pressureSensor;

//--------------------------------------------------------------------+
// USB MIDI Setup
//--------------------------------------------------------------------+
Adafruit_USBD_MIDI usb_midi;

void setup() {
  // Initialize USB MIDI
  usb_midi.begin();
  while (!TinyUSBDevice.mounted()) delay(10);  // Wait for USB connection

  // Initialize sensor
  pressureSensor.initialize(SCK_PIN, DOUT_PIN);
  pinMode(SCK_PIN, OUTPUT);
  digitalWrite(SCK_PIN, LOW);

  Serial.begin(115200);
  Serial.println("USB MIDI Ready");
}

uint8_t pressure_to_midi(long raw) {
  raw = constrain(raw, MIN_RAW, MAX_RAW);
  return map(raw, MIN_RAW, MAX_RAW, 0, 127);
}

void loop() {
  if (pressureSensor.isReady()) {
    pressureSensor.readAndSelectNextData(HX710_DIFFERENTIAL_INPUT_40HZ);
    long raw = pressureSensor.getLastDifferentialInput();
    uint8_t midi_val = pressure_to_midi(raw);

    // Send only if value changed (with threshold)
    static uint8_t last_val = 0;
    if (abs(midi_val - last_val) >= 1) {
      uint8_t cc_msg[4] = {
      0x0B,                     // Cable number 0, CIN = 0x0B (Control Change)
      uint8_t(0xB0 | (MIDI_CH - 1)),  // Status byte: Control Change on channel
      MIDI_CC_NUM,              // Controller number
      midi_val                  // Controller value
      };

usb_midi.write(cc_msg, 4);
      last_val = midi_val;

      // Debug output
      Serial.print("Raw: "); Serial.print(raw);
      Serial.print(" | MIDI: "); Serial.println(midi_val);
    }
  }
  delay(10);
}
