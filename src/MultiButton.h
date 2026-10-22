#ifndef MULTIBUTTON_H
#define MULTIBUTTON_H

#include <Arduino.h>

enum ButtonEvent {
  EVENT_NONE,
  EVENT_SINGLE_CLICK,
  EVENT_DOUBLE_CLICK,
  EVENT_TRIPLE_CLICK,
  EVENT_LONG_PRESS
};

class MultiButton {
  private:
    uint8_t pin;
    const uint32_t debounceDelay = 35;
    const uint32_t longPressThreshold = 600;
    const uint32_t multiClickWindow = 280;

    int lastRawReading = HIGH;
    int stableState = HIGH;
    uint32_t lastDebounceTime = 0;

    uint32_t pressStartTime = 0;
    uint32_t lastReleaseTime = 0;
    uint8_t clickCount = 0;
    bool longPressHandled = false;

  public:
    MultiButton(uint8_t pinNumber);
    void begin();
    ButtonEvent update();
};

#endif