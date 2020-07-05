#ifndef MoonController_h
#define MoonController_h

#include "Arduino.h"

#define DEFAULT_PHASES    8

#define MOON_PIN_0        2  // rightmost light if facing clock
#define MOON_PIN_1        3
#define MOON_PIN_2        4
#define MOON_PIN_3        5
#define MOON_PIN_4        6
#define MOON_PIN_5        7
#define MOON_PIN_6        8
#define MOON_PIN_7        9

class MoonController {

  public:
    // Constructor
    MoonController(int pinOuts[]);

    // Initialization done after construction, to permit static instances
    void init();

    void printPhase(int currDay, int currMonth, int currYear);

  protected:
    int* pinOuts;

  private:
    int getMoonPhase(int currDay, int currMonth, int currYear);
};

#endif
