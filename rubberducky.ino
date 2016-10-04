#include <Keyboard.h>
#include "duckencoder/script.h"
void runCode();
bool RUN_ON_START = true;
//

bool pInput = false;

void setup() {
  // Custom code:
  pinMode(3, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pInput = digitalRead(3);

  // Init
  Keyboard.begin();
  delay(1000);
  if (RUN_ON_START) runCode();
}

void loop() {
  // Check if our button is pressed
  if (!RUN_ON_START && digitalRead(3)) {
    if (!pInput) {
      pInput = true;
      runCode();
    }
  } else {
    pInput = false;
  }

  delay(50);
}


//// MAIN CODE ////

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
  int num = 0;

  for (int i = 0; i < script_len; i++) {
    char cmd = (char)pgm_read_byte(&script_src[i]);
    
    // String input
    if (cmd >= 1 && cmd <= 250) {
      int j;
      for (j = i + 1; j <= i + cmd; j++) {
        char chr = (char)pgm_read_byte(&script_src[j]);

        // If it's a normal letter or number
        if ((chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) || (chr >= 48 && chr <= 57)) {
          Keyboard.write(chr);
        } else {
          altCode(chr);
        }
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
