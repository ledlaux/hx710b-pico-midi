#include <Adafruit_TinyUSB.h>  
#include "HX710.h"

const int DOUT_PIN = 12;
const int SCK_PIN = 14;
const int POT_PIN = 29;  // Potentiometer input

// MIDI Configuration
const uint8_t MIDI_CC_NUM = 1;  // Breath Controller
const uint8_t MIDI_CH = 1;      // Channel 1

// Calibration
const long MIN_RAW = 600000;
const long MAX_RAW = 2000000;

HX710 pressureSensor;
Adafruit_USBD_MIDI usb_midi;

void setup() {
  usb_midi.begin();
  while (!TinyUSBDevice.mounted()) delay(10);

  pressureSensor.initialize(SCK_PIN, DOUT_PIN);
  pinMode(SCK_PIN, OUTPUT);
  digitalWrite(SCK_PIN, LOW);

  Serial.begin(115200);
  Serial.println("USB MIDI Ready");

  pinMode(POT_PIN, INPUT);  // Potentiometer input
}

uint8_t pressure_to_midi(long raw) {
  raw = constrain(raw, MIN_RAW, MAX_RAW);
  return map(raw, MIN_RAW, MAX_RAW, 0, 127);
}

void loop() {
  if (pressureSensor.isReady()) {
    pressureSensor.readAndSelectNextData(HX710_DIFFERENTIAL_INPUT_40HZ);
    long raw = pressureSensor.getLastDifferentialInput();
    uint8_t target_val = pressure_to_midi(raw);

    static uint8_t last_val = 0;
    static uint32_t last_send = 0;

    // Read potentiometer
    int pot_val = analogRead(POT_PIN);  // 0–1023

    // Map to fall factor range: 0.90–0.99
    float fall_factor = map(pot_val, 0, 1023, 90, 99) / 100.0;

    // Map to send interval: 10–50 ms
    uint32_t send_interval = map(pot_val, 0, 1023, 10, 50);

    const uint8_t rise_step = 2;

    if (millis() - last_send >= send_interval) {
      if (target_val > last_val) {
        last_val = min(target_val, last_val + rise_step);
      } else if (target_val < last_val) {
        last_val = max(target_val, uint8_t(last_val * fall_factor));
        if (last_val < target_val) last_val = target_val;
      } else {
        return;
      }

      uint8_t cc_msg[4] = {
        0x0B,
        uint8_t(0xB0 | (MIDI_CH - 1)),
        MIDI_CC_NUM,
        last_val
      };

      usb_midi.write(cc_msg, 4);
      last_send = millis();

      Serial.print("Raw: "); Serial.print(raw);
      Serial.print(" | MIDI: "); Serial.print(last_val);
      Serial.print(" | Fall: "); Serial.print(fall_factor, 2);
      Serial.print(" | Interval: "); Serial.println(send_interval);
    }
  }

  delay(1);
}
