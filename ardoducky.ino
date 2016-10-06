#include "HID-Project.h"
#include "script.h"
void runCode();

char sysvars[3];

void setup() {
  // Init
  BootKeyboard.begin();
  delay(1000);

  // Reset caps lock and num lock
  uint8_t leds = BootKeyboard.getLeds();
  if (leds & LED_CAPS_LOCK) BootKeyboard.write(KEY_CAPS_LOCK);
  if (!(leds & LED_NUM_LOCK)) BootKeyboard.write(KEY_NUM_LOCK);

  // Set sysvars
  sysvars[0] = leds & LED_CAPS_LOCK;
  sysvars[1] = leds & LED_NUM_LOCK;
  sysvars[2] = leds & LED_SCROLL_LOCK;

  // Run
  runCode();
}

void loop() {
  delay(50);
}

//// MAIN CODE ////

void altCode(uint8_t n) {
  // Get digits
  uint8_t a = n / 100;
  uint8_t b = (n / 10) % 10;
  uint8_t c = n % 10;

  // Press alt
  BootKeyboard.press(KEY_LEFT_ALT);
  delay(1);

  // Type alt code
  if (a) {
    BootKeyboard.write((KeyboardKeycode)(KEYPAD_1 + (a + 9) % 10));
    delay(1);
  }

  if (a || b) {
    BootKeyboard.write((KeyboardKeycode)(KEYPAD_1 + (b + 9) % 10));
    delay(1);
  }

  BootKeyboard.write((KeyboardKeycode)(KEYPAD_1 + (c + 9) % 10));
  delay(1);

  // Release alt
  BootKeyboard.release(KEY_LEFT_ALT);
  delay(1);
}

void runCode() {
  // Iterate script
  for (int i = 0; i < ads_len; i++) {
    uint8_t cmd = pgm_read_byte(&ads_data[i]);

    // Press key
    if (cmd == 1) {
      BootKeyboard.write((KeyboardKeycode)pgm_read_byte(&ads_data[++i]));
      delay(1);
    }

    // Wait
    else if (cmd == 2) {
      uint8_t idx = pgm_read_byte(&ads_data[++i]);
      delay(ads_consts[idx]);
    }

    // Press
    else if (cmd == 3) {
      BootKeyboard.press((KeyboardKeycode)pgm_read_byte(&ads_data[++i]));
      delay(1);
    }

    // Release
    else if (cmd == 4) {
      BootKeyboard.release((KeyboardKeycode)pgm_read_byte(&ads_data[++i]));
      delay(1);
    }

    // Press alt code
    else if (cmd == 5) {
      altCode(pgm_read_byte(&ads_data[++i]));
      delay(1);
    }

    // Jump
    else if (cmd == 6) {
      uint8_t idx = pgm_read_byte(&ads_data[++i]);
      i = ads_consts[idx] - 1;
    }

    // Jump if sysvar is true
    else if (cmd == 7) {
      uint8_t sysvar_idx = pgm_read_byte(&ads_data[++i]);
      uint8_t data_idx = pgm_read_byte(&ads_data[++i]);
      if (sysvars[sysvar_idx])
        i = ads_consts[data_idx] - 1;
    }
  }
}
