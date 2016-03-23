#include <math.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)

byte moonPases[] = {
  B00000001,
  B00000011,
  B00000111,
  B00011111,
  B00111111,
  B00000001,
  B00000001,
  B00000001,
};

void setup(void) {
  Serial.begin(9600);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }
}

void loop(void) {
  // digitalClockDisplay();
  getBitsConfigurationForPhase(getPhase(year(), month(), day()));
  printPhaseToDisplay();
}

void printPhaseToDisplay(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(':');
  if (digits < 10) {
    Serial.print('0');
  }
  Serial.print(digits);
}

/**
 * calculate the current phase of the moon based on the current date algorithm adapted from Stephen R. Schmitt's
 * Lunar Phase Computation program, originally written in the Zeno programming language http://home.att.net/~srschmitt/lunarphasecalc.html
 */
byte getPhase(int Y, int M, int D) {
  double AG, IP;
  byte phase;
  long YY, MM, K1, K2, K3, JD;

  // calculate julian date
  YY = Y - floor((12 - M) / 10);
  MM = M + 9;
  if (MM >= 12) {
    MM = MM - 12;
  }

  K1 = floor(365.25 * (YY + 4712));
  K2 = floor(30.6 * MM + 0.5);
  K3 = floor(floor((YY / 100) + 49) * 0.75) - 38;

  JD = K1 + K2 + D + 59;
  if (JD > 2299160) {
    JD = JD - K3;
  }

  IP = normalize((JD - 2451550.1) / 29.530588853);
  AG = IP * 29.53;
  return getPhaseBits(AG);
}

byte getBitsConfigurationForPhase(double AG) {
  Serial.println(AG);
  if (AG < 1.20369) {
    //phase = B00000000;
    lightLeds (0, 0, 0, 0, 0, 0);
  } else if (AG < 3.61108) {
    //phase = B00000001;
    lightLeds (0, 0, 0, 0, 0, I);
  } else if (AG < 6.01846) {
    //phase = B00000011;
    lightLeds (0, 0, 0, 0, I, I);
  } else if (AG < 8.42595) {
    //phase = B00000111;
    lightLeds (0, 0, 0, I, I, I);
  } else if (AG < 10.83323) {
    //phase = B00001111;
    lightLeds (0, 0, I, I, I, I);
  } else if (AG < 13.24062) {
    //phase = B00011111;
    lightLeds (0, I, I, I, I, I);
  } else if (AG < 15.64800) {
    //phase = B00111111;
    lightLeds (I, I, I, I, I, I);
  } else if (AG < 18.05539) {
    //phase = B00111110;
    lightLeds (I, I, I, I, I, 0);
  } else if (AG < 20.46277) {
    //phase = B00111100;
    lightLeds (I, I, I, I, 0, 0);
  } else if (AG < 22.87016) {
    //phase = B00111000;
    lightLeds (I, I, I, 0, 0, 0);
  } else if (AG < 25.27754) {
    //phase = B00110000;
    lightLeds (I, I, 0, 0, 0, 0);
  } else if (AG < 27.68493) {
    //phase = B00100000;
    lightLeds (I, 0, 0, 0, 0, 0);
  } else {
    //phase = 0;
    lightLeds (0, 0, 0, 0, 0, 0);
  }
}

double normalize(double v) {           // normalize moon calculation between 0-1
  v = v - floor(v);
  if (v < 0) {
    v = v + 1;
  }
  return v;
}

