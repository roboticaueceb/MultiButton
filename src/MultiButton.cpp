#include "MultiButton.h"

MultiButton::MultiButton(uint8_t pinNumber) : pin(pinNumber) {}

void MultiButton::begin() {
  pinMode(pin, INPUT_PULLUP);
}

ButtonEvent MultiButton::update() {
  int reading = digitalRead(pin);
  uint32_t now = millis();
  ButtonEvent event = EVENT_NONE;

  // 1. Antirrebote
  if (reading != lastRawReading) {
    lastDebounceTime = now;
  }
  lastRawReading = reading;

  if ((now - lastDebounceTime) > debounceDelay) {
    if (reading != stableState) {
      stableState = reading;
      if (stableState == LOW) {
        pressStartTime = now;
        longPressHandled = false;
      } else {
        if (!longPressHandled) {
          clickCount++;
          lastReleaseTime = now;
        }
      }
    }
  }

  // 2. Click largo
  if (stableState == LOW && !longPressHandled) {
    if ((now - pressStartTime) >= longPressThreshold) {
      event = EVENT_LONG_PRESS;
      longPressHandled = true;
      clickCount = 0;
    }
  }

  // 3. Ventana multiclic
  if (stableState == HIGH && clickCount > 0) {
    if ((now - lastReleaseTime) >= multiClickWindow) {
      if (clickCount == 1)      event = EVENT_SINGLE_CLICK;
      else if (clickCount == 2) event = EVENT_DOUBLE_CLICK;
      else if (clickCount >= 3) event = EVENT_TRIPLE_CLICK;

      clickCount = 0;
    }
  }

  return event;
}