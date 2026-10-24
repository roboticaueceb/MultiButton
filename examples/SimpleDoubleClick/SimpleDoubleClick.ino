#include <MultiButton.h>

#define BTN_PIN 5  // Pin conectado al botón (el otro extremo a GND)

MultiButton btn(BTN_PIN);

void setup() {
  Serial.begin(115200);
  btn.begin();
  Serial.println("Listo. Da dos toques rapidos al boton...");
}

void loop() {
  // update() devuelve EVENT_NONE casi todo el tiempo
  // Solo devolverá EVENT_DOUBLE_CLICK tras 280 ms de silencio después del segundo toque
  if (btn.update() == EVENT_DOUBLE_CLICK) {
    Serial.println(">>> DOBLE CLIC CONFIRMADO <<<");
  }
}