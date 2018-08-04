#ifndef ButtonHandler_h
#define ButtonHandler_h

#include "Arduino.h"

#define DEFAULT_LONGPRESS_LEN    25

enum { EV_NONE = 0, EV_SHORTPRESS, EV_LONGPRESS };

class ButtonHandler {

  public:
    // Constructor
    ButtonHandler(int pin, int longpress_len = DEFAULT_LONGPRESS_LEN);

    // Initialization done after construction, to permit static instances
    void init();

    // Handler, to be called in the loop()
    int handle();

  protected:
    boolean was_pressed;     // previous state
    int pressed_counter;     // press running duration
    const int pin;           // pin to which button is connected
    const int longpress_len; // longpress duration
};

#endif

