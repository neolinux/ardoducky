#include "duckencoder/script.h"

const unsigned char MOD_SHIFT = 2;
const unsigned char MOD_ALT = 4;

const unsigned char KEY_NUM_1 = 89;
const unsigned char KEY_NUM_0 = 98;

unsigned char buf[8] = {0}; // Keyboard buffer

void pressKey(unsigned char a, unsigned char b) {
  buf[0] = a;
  buf[2] = b;
  Serial.write(buf, 8);
  /*
    Serial.print(">");
    Serial.print(a, DEC);
    Serial.print(", ");
    Serial.print(b, DEC);
    Serial.print("\n");
  */
  delay(1);
}

void pressAltNum(unsigned char a) {
  if (a == 0)
    pressKey(MOD_ALT, KEY_NUM_0);
  else
    pressKey(MOD_ALT, a + KEY_NUM_1 - 1);
}

void setup() {
  Serial.begin(9600);

  int j = 0; // Number counter
  for (int i = 0; i < script_len; i++) {
    char chr = (char)pgm_read_byte(&script_src[i]);
    
    // Delay
    if (chr == (char)255) {
      delay(script_numbers[j]);
      j++;

    // Release
    } else if (chr == (char)254) {
      pressKey(0, 0);

    // Input alt code
    } else if (chr == (char)253) {
      unsigned char n = (char)pgm_read_byte(&script_src[i + 1]);
      i++;

      unsigned char a = n / 100;
      unsigned char b = (n / 10) % 10;
      unsigned char c = n % 10;

      if (a) pressAltNum(a);
      if (a == b) pressKey(MOD_ALT, 0);
      if (a || b) pressAltNum(b);
      if (b == c) pressKey(MOD_ALT, 0);
      pressAltNum(c);
      pressKey(0, 0);

    // Nomod key then release
    } else if (chr == (char)252) {
      pressKey(0, (char)pgm_read_byte(&script_src[i + 1]));
      pressKey(0, 0);
      i++;

    // Shift key then release
    } else if (chr == (char)251) {
      pressKey(MOD_SHIFT, (char)pgm_read_byte(&script_src[i + 1]));
      pressKey(0, 0);
      i++;

    // Any other key press
    } else if (i < script_len - 1) {
      pressKey(chr, (char)pgm_read_byte(&script_src[i + 1]));
      i++;
    }
  }
}

void loop() {
  // Don't do anything :>
}

