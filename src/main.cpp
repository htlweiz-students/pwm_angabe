#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>

char detectEdgeC(char pin_number) {
  char return_value = 0;
  static unsigned char old_values = PINC;
  unsigned char new_values = PINC;
  if ((old_values & 1 << pin_number) != (new_values & 1 << pin_number)) {
    // edge detected;
    if (new_values & 1 << pin_number) {
      return_value = 1;
      old_values |= (1 << pin_number);
    } else {
      return_value = -1;
      old_values &= ~(1 << pin_number);
    }
  }
  return return_value;
}

char detectRotary() {
  char return_value = 0;
  static unsigned char old_values = PINC & (1 << PINC3 | 1 << PINC4);
  unsigned char new_values = PINC & (1 << PINC3 | 1 << PINC4);
  if (new_values == 0 && old_values == (1 << PINC4)) {
    return_value = 1;
  }
  if (new_values == 0 && old_values == (1 << PINC3)) {
    return_value = -1;
  }
  old_values = new_values;
  return return_value;
}

void PWMInit() {}

int main(void) {
  for (auto i = DD2; i <= DD7; i++) {
    DDRD |= (1 << i);
  }
  PWMInit();

  while (1) {
    // Your main loop code here
    char rotary = detectRotary();
    if (rotary < 0) {
      if (OCR0A > 20) {
        OCR0A -= 20;
      } else {
        OCR0A = 0;
      }
    }
    if (rotary > 0) {
      if (OCR0A < 235) {
        OCR0A += 20;
      } else {
        OCR0A = 255;
      }
    }
    OCR0B = 255 - OCR0A;
  }

  return 0;
}
