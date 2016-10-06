#include <Keyboard.h>
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
int numpad_1 = 225;

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
    Keyboard.write(numpad_1 + (a + 9) % 10);
    delay(1);
  }

  if (a || b) {
    Keyboard.write(numpad_1 + (b + 9) % 10);
    delay(1);
  }

  Keyboard.write(numpad_1 + (c + 9) % 10);
  delay(1);

  // Release alt
  Keyboard.release(KEY_LEFT_ALT);
  delay(1);
}

void runCode() {
  // Iterate script
  for (int i = 0; i < ads_len; i++) {
    uint8_t cmd = pgm_read_byte(&ads_data[i]);

    // Write string
    if (cmd == 1) {
      uint8_t len = pgm_read_byte(&ads_data[i + 1]);
      int j;
      for (j = i + 2; j <= i + len + 1; j++) {
        uint8_t chr = pgm_read_byte(&ads_data[j]);

        // If it's a normal letter or number
        if ((chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) || (chr >= 48 && chr <= 57))
          Keyboard.write(chr);
        else
          altCode(chr);

        delay(1);
      }
      i = j - 1;
    }

    // Wait
    else if (cmd == 2) {
      uint8_t idx = pgm_read_byte(&ads_data[++i]);
      delay(ads_consts[idx]);
    }

    // Press
    else if (cmd == 3) {
      uint8_t key = pgm_read_byte(&ads_data[++i]);
      Keyboard.press(key);
      delay(1);
    }

    // Release
    else if (cmd == 4) {
      uint8_t key = pgm_read_byte(&ads_data[++i]);
      Keyboard.release(key);
      delay(1);
    }

    // Jump
    else if (cmd == 5) {
      uint8_t idx = pgm_read_byte(&ads_data[++i]);
      i = ads_consts[idx] - 1;
    }

    // Write string raw
    else if (cmd == 6) {
      uint8_t len = pgm_read_byte(&ads_data[i + 1]);
      int j;
      for (j = i + 2; j <= i + len + 1; j++) {
        uint8_t chr = pgm_read_byte(&ads_data[j]);
        Keyboard.write(chr);
        delay(1);
      }
      i = j - 1;
    }
  }
}
