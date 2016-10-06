#include "HID-Project.h"
#include "script.h"
void runCode();

void setup() {
  // Init
  Keyboard.begin();
  delay(1000);

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
  Keyboard.press(KEY_LEFT_ALT);
  delay(1);

  // Type alt code
  if (a) {
    Keyboard.write((KeyboardKeycode)(KEYPAD_1 + (a + 9) % 10));
    delay(1);
  }

  if (a || b) {
    Keyboard.write((KeyboardKeycode)(KEYPAD_1 + (b + 9) % 10));
    delay(1);
  }

  Keyboard.write((KeyboardKeycode)(KEYPAD_1 + (c + 9) % 10));
  delay(1);

  // Release alt
  Keyboard.release(KEY_LEFT_ALT);
  delay(1);
}

void runCode() {
  // Iterate script
  for (int i = 0; i < ads_len; i++) {
    uint8_t cmd = pgm_read_byte(&ads_data[i]);

    // Press key
    if (cmd == 1) {
      Keyboard.write((KeyboardKeycode)pgm_read_byte(&ads_data[++i]));
      delay(1);
    }

    // Wait
    else if (cmd == 2) {
      uint8_t idx = pgm_read_byte(&ads_data[++i]);
      delay(ads_consts[idx]);
    }

    // Press
    else if (cmd == 3) {
      Keyboard.press((KeyboardKeycode)pgm_read_byte(&ads_data[++i]));
      delay(1);
    }

    // Release
    else if (cmd == 4) {
      Keyboard.release((KeyboardKeycode)pgm_read_byte(&ads_data[++i]));
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
  }
}
