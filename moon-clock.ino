#include <math.h>
#include <Wire.h>

#include "ButtonHandler.h"
#include "MoonController.h"

// I2C Config
#define I2C_RTC_ADDR 0x68

#define MOON_PIN_0        2  // rightmost light if facing clock
#define MOON_PIN_1        3
#define MOON_PIN_2        4
#define MOON_PIN_3        5
#define MOON_PIN_4        6
#define MOON_PIN_5        7
#define MOON_PIN_6        8
#define MOON_PIN_7        9

// Digits controll
#define CTRL_DIGIT_ONE    10
#define CTRL_DIGIT_TWO    11
#define CTRL_DIGIT_THREE  12
#define CTRL_DIGIT_FOUR   13
#define CTRL_DOTS         A0

//
#define SHIFT_REGISTER_DATA     A1
#define SHIFT_REGISTER_LATCH    A2
#define SHIFT_REGISTER_CLOCK    A3

// Configuration pins
#define CTRL_24_HRS_MODE   A6
#define _24_HRS_MODE       A7

// digit pattern for a 7-segment display
const byte seven_segments_config[16] = {
// .GFEDCBA
  B11000000,  // 0
  B11111001,  // 1
  B10100100,  // 2
  B10110000,  // 3
  B10011001,  // 4
  B10010010,  // 5
  B10000010,  // 6
  B11111000,  // 7
  B10000000,  // 8
  B10010000,  // 9
  B10001000,  // A
  B10000011,  // b
  B11000110,  // C
  B10100001,  // d
  B10000110,  // E
  B10001110   // F
};

const int digit_controllers[] = {
  CTRL_DIGIT_ONE, CTRL_DIGIT_TWO, CTRL_DIGIT_THREE, CTRL_DIGIT_FOUR
};
int BASE_DELAY = 2;
const int DOTS_TOGGLE = 1000;

int lastDay = 0;
int lastMonth = 0;
int lastYear = 0;
int dotCounter = 0;

void setup(void) {
  //Serial.begin(57600);
  pinMode(MOON_PIN_0, OUTPUT);
  pinMode(MOON_PIN_1, OUTPUT);
  pinMode(MOON_PIN_2, OUTPUT);
  pinMode(MOON_PIN_3, OUTPUT);
  pinMode(MOON_PIN_4, OUTPUT);
  pinMode(MOON_PIN_5, OUTPUT);
  pinMode(MOON_PIN_6, OUTPUT);
  pinMode(MOON_PIN_7, OUTPUT);

  pinMode(CTRL_DIGIT_ONE, OUTPUT);
  pinMode(CTRL_DIGIT_TWO, OUTPUT);
  pinMode(CTRL_DIGIT_THREE, OUTPUT);
  pinMode(CTRL_DIGIT_FOUR, OUTPUT);
  pinMode(CTRL_DOTS, OUTPUT);

  pinMode(CTRL_24_HRS_MODE, INPUT);
  pinMode(_24_HRS_MODE, OUTPUT);

  pinMode(SHIFT_REGISTER_DATA, OUTPUT);
  pinMode(SHIFT_REGISTER_LATCH, OUTPUT);
  pinMode(SHIFT_REGISTER_CLOCK, OUTPUT);
}

void loop(void) {
  byte second, minute, hour, dayOfWeek, currentDay, currentMonth, currentYear;
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &currentDay, &currentMonth, &currentYear);

  if (lastDay == 0 & lastMonth == 0 && lastYear == 0) {
    lastDay = currentDay;
    lastMonth = currentMonth;
    lastYear = currentYear;

    printPhase(currentDay, currentMonth, currentYear);
  } else if (lastDay != currentDay || lastMonth != currentMonth || lastYear != currentYear) {
    printPhase(currentDay, currentMonth, currentYear);
  }

  //printDateTime(now);
  printSegments(hour, minute);
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
  Wire.beginTransmission(I2C_RTC_ADDR);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(I2C_RTC_ADDR, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

byte bcdToDec(byte val) {
  return (val / 16*10) + (val % 16);
}

boolean rtcPresent() {
  Wire.beginTransmission(I2C_RTC_ADDR);
  Wire.write(0x00);
  return !(Wire.endTransmission() != 0);
}

void printSegments(int hour, int minutes) {
  int hours_mode = analogRead(CTRL_24_HRS_MODE);

  if (hours_mode >= 100) {
    if (hour > 12) {
      digitalWrite(_24_HRS_MODE, 255);
      hour -= 12;
    } else {
      digitalWrite(_24_HRS_MODE, 0);
    }
  }
  
  int hour_parts[] = {
    minutes % 10,
    minutes / 10,
    hour % 10,
    hour / 10
  };

  // get the digit pattern to be updated
  byte pattern;
  pattern = seven_segments_config[hour_parts[0]];
  digitalWrite(SHIFT_REGISTER_LATCH, LOW);
  shiftOut(SHIFT_REGISTER_DATA, SHIFT_REGISTER_CLOCK, MSBFIRST, ~pattern);
  digitalWrite(SHIFT_REGISTER_LATCH, HIGH);

  digitalWrite(digit_controllers[0], LOW);
  digitalWrite(digit_controllers[1], HIGH);
  digitalWrite(digit_controllers[2], HIGH);
  digitalWrite(digit_controllers[3], HIGH);
  delay(BASE_DELAY); // ten 100ms

  // get the digit pattern to be updated
  pattern = seven_segments_config[hour_parts[1]];
  digitalWrite(SHIFT_REGISTER_LATCH, LOW);
  shiftOut(SHIFT_REGISTER_DATA, SHIFT_REGISTER_CLOCK, MSBFIRST, ~pattern);
  digitalWrite(SHIFT_REGISTER_LATCH, HIGH);

  digitalWrite(digit_controllers[0], HIGH);
  digitalWrite(digit_controllers[1], LOW);
  digitalWrite(digit_controllers[2], HIGH);
  digitalWrite(digit_controllers[3], HIGH);
  delay(BASE_DELAY); // ten 100ms

  // get the digit pattern to be updated
  pattern = seven_segments_config[hour_parts[2]];
  digitalWrite(SHIFT_REGISTER_LATCH, LOW);
  shiftOut(SHIFT_REGISTER_DATA, SHIFT_REGISTER_CLOCK, MSBFIRST, ~pattern);
  digitalWrite(SHIFT_REGISTER_LATCH, HIGH);

  digitalWrite(digit_controllers[0], HIGH);
  digitalWrite(digit_controllers[1], HIGH);
  digitalWrite(digit_controllers[2], LOW);
  digitalWrite(digit_controllers[3], HIGH);
  delay(BASE_DELAY); // ten 100ms

  if (hour >= 10) {
    // get the digit pattern to be updated
    pattern = seven_segments_config[hour_parts[3]];
    digitalWrite(SHIFT_REGISTER_LATCH, LOW);
    shiftOut(SHIFT_REGISTER_DATA, SHIFT_REGISTER_CLOCK, MSBFIRST, ~pattern);
    digitalWrite(SHIFT_REGISTER_LATCH, HIGH);
  
    digitalWrite(digit_controllers[0], HIGH);
    digitalWrite(digit_controllers[1], HIGH);
    digitalWrite(digit_controllers[2], HIGH);
    digitalWrite(digit_controllers[3], LOW);
    delay(BASE_DELAY); // ten 100ms
  }

  dotCounter += (BASE_DELAY * 4);
  if (dotCounter >= DOTS_TOGGLE) {
    digitalWrite(CTRL_DOTS, !digitalRead(CTRL_DOTS));
    dotCounter = 0;
  }
}

