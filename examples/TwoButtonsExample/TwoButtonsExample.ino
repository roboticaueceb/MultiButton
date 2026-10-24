#include <MultiButton.h>

// Definición de pines (ambos con el otro extremo a GND)
#define BTN_NEXT_PIN   4  // Botón para navegar / cambiar
#define BTN_SELECT_PIN 5  // Botón para confirmar / seleccionar

// Instancias independientes de la clase
MultiButton btnNext(BTN_NEXT_PIN);
MultiButton btnSelect(BTN_SELECT_PIN);

void setup() {
  Serial.begin(115200);

  // Inicializar ambos pines con su Pull-Up interno
  btnNext.begin();
  btnSelect.begin();

  Serial.println("=====================================");
  Serial.println("  SISTEMA DE CONTROL DE 2 BOTONES    ");
  Serial.println("  BTN 1 (Pin 4): Navegacion          ");
  Serial.println("  BTN 2 (Pin 5): Accion / Salida     ");
  Serial.println("=====================================");
}

void loop() {
  // 1. Actualizar el estado de cada botón de forma no bloqueante
  ButtonEvent evNext   = btnNext.update();
  ButtonEvent evSelect = btnSelect.update();

  // 2. Lógica del Botón 1: Navegación (Pin 4)
  switch (evNext) {
    case EVENT_SINGLE_CLICK:
      Serial.println("[BTN NEXT] -> Siguiente opcion (+1)");
      break;

    case EVENT_DOUBLE_CLICK:
      Serial.println("[BTN NEXT] -> Avance rapido (+5)");
      break;

    case EVENT_LONG_PRESS:
      Serial.println("[BTN NEXT] -> Reiniciar contador a 0");
      break;

    default:
      break;
  }

  // 3. Lógica del Botón 2: Acción / Selección (Pin 5)
  switch (evSelect) {
    case EVENT_SINGLE_CLICK:
      Serial.println("[BTN SELECT] -> Confirmar seleccion [OK]");
      break;

    case EVENT_LONG_PRESS:
      Serial.println("[BTN SELECT] -> Salir al menu principal [BACK]");
      break;

    default:
      break;
  }
}