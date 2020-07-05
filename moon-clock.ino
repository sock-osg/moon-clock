#include <DS3231.h>
#include <Wire.h>
#include <TM1637Display.h>

#include "ButtonHandler.h"

#define DIO              2 // can be any digital pin
#define CLK              3 // can be any digital pin

#define BTN_SET          4  // Button SET
#define BTN_INCREASE     5  // Button INCREASE

#define _24_HRS_LED_PIN  6

// Init the DS3231 using the hardware interface
TM1637Display display(CLK, DIO);
DS3231 rtc;

// Instantiate button objects
ButtonHandler btn_set(BTN_SET);
ButtonHandler btn_increase(BTN_INCREASE);

int event_counter = 0;
bool Century = false;
bool h12 = false;
bool PM;
bool timer_setup;

void setup() {
  Serial.begin(9600);

  // Initialize the rtc object
  Wire.begin();
  display.setBrightness(0x0a);

  btn_set.init();
  btn_increase.init();
}

byte getHour() {
  byte hour = rtc.getHour(h12, PM);

  if (hour > 12) {
    digitalWrite(_24_HRS_LED_PIN, HIGH); // This means PM hour
  } else {
    digitalWrite(_24_HRS_LED_PIN, LOW);
  }

  if (hour == 0) {
    hour = 12;
  } else if (hour >= 13) {
    hour = hour - 12;
  }

  return hour;
}

void print_current_time() {
  byte current_hour = getHour();
  byte current_minute = rtc.getMinute();

  byte tens_curr_hour = current_hour / 10;
  byte units_curr_hour = current_hour % 10;

  uint8_t data[] = {
    tens_curr_hour == 0 ? 0x0 : display.encodeDigit(tens_curr_hour),
    rtc.getSecond() % 2 == 0 ? 0x80 | display.encodeDigit(units_curr_hour) : display.encodeDigit(units_curr_hour),
    display.encodeDigit(current_minute / 10),
    display.encodeDigit(current_minute % 10)
  };

  if (timer_setup && rtc.getSecond() % 2 == 1) {
    if (event_counter == 1) {
      data[0] = 0x00;
      data[1] = 0x00;
    } else if (event_counter == 2) {
      data[2] = 0x00;
      data[3] = 0x00;
    }
  }

  display.setSegments(data);
}

void print_temperature() {
  byte temperature = rtc.getTemperature();
  uint8_t data[] = {
    display.encodeDigit(temperature / 10),
    display.encodeDigit(temperature % 10),
    0x63, // Degrees symbol
    0x39
  };
  display.setSegments(data);
}

void print_date() {
  byte symbol = 0x00;
  byte measure = 0x00;

  if (event_counter == 3) {
    symbol = 0x5e;
    measure = rtc.getDate();
  } else if (event_counter == 4) {
    symbol = 0x54;
    measure = rtc.getMonth(Century);
  } else if (event_counter == 5) {
    symbol = 0x6e;
    measure = rtc.getYear();
  }

  uint8_t data[] = {
    symbol, // Solved symbol
    0x00,   // empty
    display.encodeDigit(measure / 10),
    display.encodeDigit(measure % 10),
  };

  if (timer_setup && rtc.getSecond() % 2 == 1) {
    data[2] = 0x00;
    data[3] = 0x00;
  }
  
  display.setSegments(data);
}

bool print_time = true;
bool print_day = false;
bool print_month = false;
bool print_year = false;

void reset_date_parts_flags() {
  bool print_time = false;
  bool print_day = false;
  bool print_month = false;
  bool print_year = false;
}

void loop() {
  // handle button
  int event_btn_set = btn_set.handle();
  int event_btn_increase = btn_increase.handle();

  if (event_btn_set == EV_SHORTPRESS) {
    timer_setup = true;
    event_counter = event_counter + 1;

    switch (event_counter) {
      case 1:
        reset_date_parts_flags();
        print_time = true;
        break;
      case 2: // Hours or Minutes
        reset_date_parts_flags();
        print_time = true;
        break;
      case 3: // Day
        reset_date_parts_flags();
        print_day = true;
        break;
      case 4: // Month
        reset_date_parts_flags();
        print_month = true;
        break;
      case 5: // Year
        reset_date_parts_flags();
        print_year = true;
        break;
      default: // Reset
        reset_date_parts_flags();
        timer_setup = false;
        print_time = true;
        event_counter = 0;
        break;
    }
  } else if (event_btn_increase == EV_SHORTPRESS) {
    if (event_counter == 1) {
      Serial.println("Setting Hours");
      int current_hour = rtc.getHour(h12, PM);
      int new_hour = current_hour < 23 ? current_hour + 1 : 0;
      rtc.setHour(new_hour);
    } else if (event_counter == 2) {
      Serial.println("Setting Minutes");
      int current_minute = rtc.getMinute();
      int new_minute = current_minute < 59 ? current_minute + 1 : 0;
      rtc.setMinute(new_minute);
    } else if (event_counter == 3) {
      Serial.println("Setting Day");
      int current_day = rtc.getDate();
      int new_day = current_day < 31 ? current_day + 1 : 1;
      rtc.setDate(new_day);
    } else if (event_counter == 4) {
      Serial.println("Setting Month");
      int current_month = rtc.getMonth(Century);
      int new_month = current_month < 11 ? current_month + 1 : 1;
      rtc.setMonth(new_month);
    } else if (event_counter == 5) {
      Serial.println("Old year");
      Serial.println(rtc.getYear());
      int current_year = rtc.getYear();
      int new_year = current_year < 98 ? current_year + 1 : 0;
      rtc.setYear(new_year);
      Serial.println("New year");
      Serial.println(rtc.getYear());
    } else {
      Serial.println("Invalid option");
    }
  }

  if (event_counter == 0 && rtc.getSecond() > 57 && !timer_setup) {
    print_temperature();
  } else if (event_counter >= 3 && event_counter <= 5) {
    print_date();
  } else {
    print_current_time();
  }

  delay(DELAY);
}
