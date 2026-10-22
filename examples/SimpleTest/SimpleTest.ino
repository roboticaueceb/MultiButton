#include <Arduino.h>
#include <MultiButton.h>

#define BTN_PIN 7

MultiButton boton(BTN_PIN);

void setup() {
  Serial.begin(115200);
  boton.begin();
  Serial.println("MultiButton listo");
}

void loop() {
  ButtonEvent evento = boton.update();

  switch (evento) {
    case EVENT_SINGLE_CLICK:
      Serial.println("Click simple");
      break;
    case EVENT_DOUBLE_CLICK:
      Serial.println("Doble click");
      break;
    case EVENT_TRIPLE_CLICK:
      Serial.println("Triple click");
      break;
    case EVENT_LONG_PRESS:
      Serial.println("Pulsacion larga");
      break;
    default:
      break;
  }
}