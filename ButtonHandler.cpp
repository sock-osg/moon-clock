#include "Arduino.h"
#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(int p, int lp) : pin(p), longpress_len(lp) { }

void ButtonHandler::init() {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH); // pull-up
  was_pressed = false;
  pressed_counter = 0;
}

int ButtonHandler::handle() {
  int event;
  int now_pressed = !digitalRead(pin);

  if (!now_pressed && was_pressed) {
    // handle release event
    event = pressed_counter < longpress_len ? EV_SHORTPRESS : EV_LONGPRESS;
  } else {
    event = EV_NONE;
  }

  // update press running duration
  pressed_counter = now_pressed ? pressed_counter + 1 : 0;

  // remember state, and we're done
  was_pressed = now_pressed;
  return event;
}

