#include <math.h>

#include "Arduino.h"
#include "MoonController.h"

MoonController::MoonController(int* pO) : pinOuts(pO) {
  // NONE
}

void MoonController::init() {
  int index;

  int* pin = &pinOuts[0];
  for (int i = 0; i < DEFAULT_PHASES; i++) {
    pin = pin + i;

    pinMode(*pin, OUTPUT);
    digitalWrite(*pin, LOW);

    pin = &pinOuts[0];
  }
}

void MoonController::printPhase(int currDay, int currMonth, int currYear) {
  int moonPhase = getMoonPhase(currDay, currMonth, currYear);

  if (moonPhase == 0) {
    digitalWrite(MOON_PIN_0, LOW);
    digitalWrite(MOON_PIN_1, LOW);
    digitalWrite(MOON_PIN_2, LOW);
    digitalWrite(MOON_PIN_3, LOW);
    digitalWrite(MOON_PIN_4, LOW);
    digitalWrite(MOON_PIN_5, LOW);
    digitalWrite(MOON_PIN_6, LOW);
    digitalWrite(MOON_PIN_7, LOW);
  } else if (moonPhase > 0 && moonPhase < 8) {
    digitalWrite(MOON_PIN_0, HIGH);
    digitalWrite(MOON_PIN_1, HIGH);
    digitalWrite(MOON_PIN_2, LOW);
    digitalWrite(MOON_PIN_3, LOW);
    digitalWrite(MOON_PIN_4, LOW);
    digitalWrite(MOON_PIN_5, LOW);
    digitalWrite(MOON_PIN_6, LOW);
    digitalWrite(MOON_PIN_7, LOW);
  } else if (moonPhase == 8) {
    digitalWrite(MOON_PIN_0, HIGH);
    digitalWrite(MOON_PIN_1, HIGH);
    digitalWrite(MOON_PIN_2, HIGH);
    digitalWrite(MOON_PIN_3, HIGH);
    digitalWrite(MOON_PIN_4, LOW);
    digitalWrite(MOON_PIN_5, LOW);
    digitalWrite(MOON_PIN_6, LOW);
    digitalWrite(MOON_PIN_7, LOW);
  } else if (moonPhase > 8 && moonPhase < 15) {
    digitalWrite(MOON_PIN_0, HIGH);
    digitalWrite(MOON_PIN_1, HIGH);
    digitalWrite(MOON_PIN_2, HIGH);
    digitalWrite(MOON_PIN_3, HIGH);
    digitalWrite(MOON_PIN_4, HIGH);
    digitalWrite(MOON_PIN_5, HIGH);
    digitalWrite(MOON_PIN_6, LOW);
    digitalWrite(MOON_PIN_7, LOW);
  } else if (moonPhase == 16) {
    digitalWrite(MOON_PIN_0, HIGH);
    digitalWrite(MOON_PIN_1, HIGH);
    digitalWrite(MOON_PIN_2, HIGH);
    digitalWrite(MOON_PIN_3, HIGH);
    digitalWrite(MOON_PIN_4, HIGH);
    digitalWrite(MOON_PIN_5, HIGH);
    digitalWrite(MOON_PIN_6, HIGH);
    digitalWrite(MOON_PIN_7, HIGH);
  } else if (moonPhase > 16 && moonPhase < 23) {
    digitalWrite(MOON_PIN_0, LOW);
    digitalWrite(MOON_PIN_1, LOW);
    digitalWrite(MOON_PIN_2, HIGH);
    digitalWrite(MOON_PIN_3, HIGH);
    digitalWrite(MOON_PIN_4, HIGH);
    digitalWrite(MOON_PIN_5, HIGH);
    digitalWrite(MOON_PIN_6, HIGH);
    digitalWrite(MOON_PIN_7, HIGH);
  } else if (moonPhase == 24) {
    digitalWrite(MOON_PIN_0, LOW);
    digitalWrite(MOON_PIN_1, LOW);
    digitalWrite(MOON_PIN_2, LOW);
    digitalWrite(MOON_PIN_3, LOW);
    digitalWrite(MOON_PIN_4, HIGH);
    digitalWrite(MOON_PIN_5, HIGH);
    digitalWrite(MOON_PIN_6, HIGH);
    digitalWrite(MOON_PIN_7, HIGH);
  } else if (moonPhase > 24) {
    digitalWrite(MOON_PIN_0, LOW);
    digitalWrite(MOON_PIN_1, LOW);
    digitalWrite(MOON_PIN_2, LOW);
    digitalWrite(MOON_PIN_3, LOW);
    digitalWrite(MOON_PIN_4, LOW);
    digitalWrite(MOON_PIN_5, LOW);
    digitalWrite(MOON_PIN_6, HIGH);
    digitalWrite(MOON_PIN_7, HIGH);
  }
}

int MoonController::getMoonPhase(int currDay, int currMonth, int currYear) {
  int r = currYear % 100;
  r %= 19;
  if (r > 9) {
    r -= 19;
  }
  r = ((r * 11) % 30) + currMonth + currDay;
  if (currMonth < 3) {
    r += 2;
  }
  int s = currYear < 2000 ? 4 : 8.3;
  r -= s;
  r = ((int) floor(r + 0.5) % 30);
  return r < 0 ? r + 30 : r;
}

