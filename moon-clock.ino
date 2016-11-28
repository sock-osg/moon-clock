#include <math.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include <TM1637Display.h>

const short CTRL_MODE = 14;
const short CTRL_DECREASE = 15;
const short CTRL_INCREASE = 16;

const short DISPLAY_DIO = 12;
const short DISPLAY_CLK = 13;

const short TOTAL_PHASES = 8;
const short bitsMoonClock[] = {
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
};

const byte moonBitsConfiguration[] = {
  B00000000, // = 0
  B00000001, // > 0 < 8
  B00001111, // = 8
  B00111111, // > 8 < 15
  B11111111, // = 16
  B11111100, // > 16 < 23
  B11110000, // = 24
  B11000000, // > 24
};

int lastDay = -1;
int setupModeCounter = -1;

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);
tmElements_t configTime;

void setup(void) {
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet) {
    printTime(0, 0);
  } else {
    printTime(configTime.Hour, configTime.Minute);
  }
  
  for (short index = 0; index < TOTAL_PHASES; index++) {
    pinMode(bitsMoonClock[index], OUTPUT);
  }
  pinMode(CTRL_MODE, INPUT);
  pinMode(CTRL_DECREASE, INPUT);
  pinMode(CTRL_INCREASE, INPUT);
  display.setBrightness(0x0f);
}

void loop(void) {
  int statusMode = digitalRead(CTRL_MODE);

  if (statusMode == HIGH) {
    setupModeCounter += 1;
  }

  // Configuration mode
  if (setupModeCounter >= 0) {
    if (setupModeCounter == 0) {
      // Start config mode, read and set the date into the date parts array
      RTC.read(configTime);
    } else if (setupModeCounter == 5) {
      // End config mode, set config time
      RTC.write(configTime);
      setupModeCounter = -1;
    } else {
      int value = 0;

      if (digitalRead(CTRL_INCREASE)) {
        value += 1;
      } else if (digitalRead(CTRL_DECREASE)) {
        value -= 1;
      }
  
      printTime(0, value);
    }
  } else {
    if (RTC.read(configTime)) {
      // Updates the moon phase when there is a day change
      if (lastDay != configTime.Day) {
        byte phaseDigits = getBitsConfigurationForPhase(getMoonPhase(configTime.Year, configTime.Month, configTime.Day));
        printPhaseToDisplay(phaseDigits);
        lastDay = configTime.Day;
      }

      printTime(configTime.Hour, configTime.Minute);
    } else {
      lastDay = -1;
      printTime(0, 0);
    }
  }
}

void printPhaseToDisplay(byte phaseDigits) {
  for (short index = 0; index < TOTAL_PHASES; index++) {
    digitalWrite(bitsMoonClock[index], getBitConfig(phaseDigits, index));
  }
}

boolean getBitConfig(byte phaseDigits, short index) {
  return (phaseDigits & (1 << index)) != 0;
}

void printTime(int hours, int minutes) {
  display.showNumberDec(hours, false, 2, 0); // 2 = length, 0 = position
  display.showNumberDec(minutes, false, 2, 2); // 2 = length, 0 = position.
}

byte getMoonPhase(int currDay, int currMonth, int currYear) {
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

int getDaysFromMonth(int month, int year) {
  int days = 0;

  switch (month) {
    case 2:
      days = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) == 0 ? 29 : 28;
      break;
    case 4: case 6: case 9: case 11:
      days = 30;
      break;
    default:
      days = 31;
      break;
  }

  return days;
}

byte getBitsConfigurationForPhase(int moonPhase) {
  byte bitsConfig = {};
  if (moonPhase == 0) {
    bitsConfig = moonBitsConfiguration[0];
  } else if (moonPhase > 0 && moonPhase < 8) {
    bitsConfig = moonBitsConfiguration[1];
  } else if (moonPhase == 8) {
    bitsConfig = moonBitsConfiguration[2];
  } else if (moonPhase > 8 && moonPhase < 15) {
    bitsConfig = moonBitsConfiguration[3];
  } else if (moonPhase == 16) {
    bitsConfig = moonBitsConfiguration[4];
  } else if (moonPhase > 16 && moonPhase < 23) {
    bitsConfig = moonBitsConfiguration[5];
  } else if (moonPhase == 24) {
    bitsConfig = moonBitsConfiguration[6];
  } else if (moonPhase > 24) {
    bitsConfig = moonBitsConfiguration[7];
  }
  return bitsConfig;
}

