#include <math.h>
#include <Wire.h>
#include <RtcDS3231.h>

#define MOON_PIN_0        2  // rightmost light if facing clock
#define MOON_PIN_1        3
#define MOON_PIN_2        4
#define MOON_PIN_3        5
#define MOON_PIN_4        6
#define MOON_PIN_5        7
#define MOON_PIN_6        8
#define MOON_PIN_7        9

const short TOTAL_PHASES = 8;

RtcDS3231<TwoWire> Rtc(Wire);

void setup(void) {
  Serial.begin(57600);
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  //RtcDateTime compiled = RtcDateTime(2017, 12, 4, 23, 22, 0);

  Rtc.SetDateTime(compiled);

  pinMode(MOON_PIN_0, OUTPUT);
  pinMode(MOON_PIN_1, OUTPUT);
  pinMode(MOON_PIN_2, OUTPUT);
  pinMode(MOON_PIN_3, OUTPUT);
  pinMode(MOON_PIN_4, OUTPUT);
  pinMode(MOON_PIN_5, OUTPUT);
  pinMode(MOON_PIN_6, OUTPUT);
  pinMode(MOON_PIN_7, OUTPUT);
}

void loop(void) {
  RtcDateTime now = Rtc.GetDateTime();
  int phase = getMoonPhase(now.Day(), now.Month(), now.Year());

  printPhase(phase);

  //printDateTime(now);
  delay(10000); // ten seconds
}

int getMoonPhase(int currDay, int currMonth, int currYear) {
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

void printPhase(int moonPhase) {
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

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring, 
          countof(datestring),
          PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
          dt.Month(),
          dt.Day(),
          dt.Year(),
          dt.Hour(),
          dt.Minute(),
          dt.Second() );
  Serial.print(datestring);
  Serial.print('\n');
}

