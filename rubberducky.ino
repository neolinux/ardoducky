#include <Keyboard.h>
#include "duckencoder/script.h"

int mods[] = {
  KEY_LEFT_CTRL,    // 0
  KEY_LEFT_SHIFT,
  KEY_LEFT_ALT,
  KEY_LEFT_GUI,
  KEY_LEFT_ARROW,   // 4
  KEY_DOWN_ARROW,
  KEY_UP_ARROW,
  KEY_RIGHT_ARROW,
  KEY_BACKSPACE,
  KEY_RETURN,       // 9
  KEY_ESC,
  KEY_DELETE,
  KEY_TAB
};

void setup() {
  Keyboard.begin();

  delay(1000);

  int num = 0;

  for (int i = 0; i < script_len; i++) {
    char cmd = (char)pgm_read_byte(&script_src[i]);
    
    // String input
    if (cmd >= 1 && cmd <= 250) {
      int j;
      for (j = i + 1; j <= i + cmd; j++) {
        char chr = (char)pgm_read_byte(&script_src[j]);
        Keyboard.write(chr);
        delay(1);
      }
      i = j - 1;

    // Delay
    } else if (cmd == (char)251) {
      delay(script_numbers[num]);
      num++;

    // Modifier input
    } else if (cmd == (char)0) {
      char mod = (char)pgm_read_byte(&script_src[i + 1]);
      char chr = (char)pgm_read_byte(&script_src[i + 2]);
      Keyboard.press(mods[mod]);
      delay(1);
      if (chr) {
        Keyboard.write(chr);
        delay(1);
      }
      Keyboard.release(mods[mod]);
      delay(1);
      i += 2;
    }
  }
}

void loop() {
  // Don't do anything :>
  delay(1000);
}

