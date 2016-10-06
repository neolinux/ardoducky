#include <Keyboard.h>
#include "script.h"
void runCode();

void setup() {
  // Init
  Keyboard.begin();
  delay(500);

  // Make sure numlock is on and caps lock is off
  // Requires patched Keyboard and HID library
  uint8_t leds = Keyboard.getLedStatus();
  if (!(leds & LED_NUM_LOCK)) Keyboard.write(KEY_NUM_LOCK);
  if (leds & LED_CAPS_LOCK) Keyboard.write(KEY_CAPS_LOCK);

  // Run
  runCode();
}

void loop() {
  delay(50);
}


//// MAIN CODE ////
int numpad_1 = 225;

void altCode(char n) {
  // Get digits
  char a = n / 100;
  char b = (n / 10) % 10;
  char c = n % 10;

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
    char cmd = (char)pgm_read_byte(&ads_data[i]);

    // Write string
    if (cmd == (char)1) {
      char len = (char)pgm_read_byte(&ads_data[i + 1]);
      int j;
      for (j = i + 2; j <= i + len + 1; j++) {
        char chr = (char)pgm_read_byte(&ads_data[j]);

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
    else if (cmd == (char)2) {
      char idx = (char)pgm_read_byte(&ads_data[++i]);
      delay(ads_consts[idx]);
    }

    // Press
    else if (cmd == (char)3) {
      char key = (char)pgm_read_byte(&ads_data[++i]);
      Keyboard.press(key);
      delay(1);
    }

    // Release
    else if (cmd == (char)4) {
      char key = (char)pgm_read_byte(&ads_data[++i]);
      Keyboard.release(key);
      delay(1);
    }
  }
}
