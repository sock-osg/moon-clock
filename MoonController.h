#ifndef MoonController_h
#define MoonController_h

#include "Arduino.h"

#define DEFAULT_PHASES    8

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
