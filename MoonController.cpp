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
  Serial.print("Moon phase: ");
  Serial.println(moonPhase);

  if (moonPhase == 0) {
    digitalWrite(pinOuts[0], LOW);
    digitalWrite(pinOuts[1], LOW);
    digitalWrite(pinOuts[2], LOW);
    digitalWrite(pinOuts[3], LOW);
    digitalWrite(pinOuts[4], LOW);
    digitalWrite(pinOuts[5], LOW);
    digitalWrite(pinOuts[6], LOW);
    digitalWrite(pinOuts[7], LOW);
  } else if (moonPhase > 0 && moonPhase < 8) {
    digitalWrite(pinOuts[0], HIGH);
    digitalWrite(pinOuts[1], HIGH);
    digitalWrite(pinOuts[2], LOW);
    digitalWrite(pinOuts[3], LOW);
    digitalWrite(pinOuts[4], LOW);
    digitalWrite(pinOuts[5], LOW);
    digitalWrite(pinOuts[6], LOW);
    digitalWrite(pinOuts[7], LOW);
  } else if (moonPhase == 8) {
    digitalWrite(pinOuts[0], HIGH);
    digitalWrite(pinOuts[1], HIGH);
    digitalWrite(pinOuts[2], HIGH);
    digitalWrite(pinOuts[3], HIGH);
    digitalWrite(pinOuts[4], LOW);
    digitalWrite(pinOuts[5], LOW);
    digitalWrite(pinOuts[6], LOW);
    digitalWrite(pinOuts[7], LOW);
  } else if (moonPhase > 8 && moonPhase <= 15) {
    digitalWrite(pinOuts[0], HIGH);
    digitalWrite(pinOuts[1], HIGH);
    digitalWrite(pinOuts[2], HIGH);
    digitalWrite(pinOuts[3], HIGH);
    digitalWrite(pinOuts[4], HIGH);
    digitalWrite(pinOuts[5], HIGH);
    digitalWrite(pinOuts[6], LOW);
    digitalWrite(pinOuts[7], LOW);
  } else if (moonPhase == 16) {
    digitalWrite(pinOuts[0], HIGH);
    digitalWrite(pinOuts[1], HIGH);
    digitalWrite(pinOuts[2], HIGH);
    digitalWrite(pinOuts[3], HIGH);
    digitalWrite(pinOuts[4], HIGH);
    digitalWrite(pinOuts[5], HIGH);
    digitalWrite(pinOuts[6], HIGH);
    digitalWrite(pinOuts[7], HIGH);
  } else if (moonPhase > 16 && moonPhase < 23) {
    digitalWrite(pinOuts[0], LOW);
    digitalWrite(pinOuts[1], LOW);
    digitalWrite(pinOuts[2], HIGH);
    digitalWrite(pinOuts[3], HIGH);
    digitalWrite(pinOuts[4], HIGH);
    digitalWrite(pinOuts[5], HIGH);
    digitalWrite(pinOuts[6], HIGH);
    digitalWrite(pinOuts[7], HIGH);
  } else if (moonPhase == 24) {
    digitalWrite(pinOuts[0], LOW);
    digitalWrite(pinOuts[1], LOW);
    digitalWrite(pinOuts[2], LOW);
    digitalWrite(pinOuts[3], LOW);
    digitalWrite(pinOuts[4], HIGH);
    digitalWrite(pinOuts[5], HIGH);
    digitalWrite(pinOuts[6], HIGH);
    digitalWrite(pinOuts[7], HIGH);
  } else if (moonPhase > 24) {
    digitalWrite(pinOuts[0], LOW);
    digitalWrite(pinOuts[1], LOW);
    digitalWrite(pinOuts[2], LOW);
    digitalWrite(pinOuts[3], LOW);
    digitalWrite(pinOuts[4], LOW);
    digitalWrite(pinOuts[5], LOW);
    digitalWrite(pinOuts[6], HIGH);
    digitalWrite(pinOuts[7], HIGH);
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
