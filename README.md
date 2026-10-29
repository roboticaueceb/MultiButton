# MultiButton (ESP32 / Arduino) 🔘

Librería en C++ ligera y no bloqueante para microcontroladores. Permite gestionar botones físicos mediante un solo pin con lógica activa en bajo (Pull-Up interno), filtrando rebotes mecánicos (*debounce*) y reconociendo múltiples patrones de pulsación mediante `millis()`.

Diseñada para el desarrollo de menús, interfaces y robots de competencia en el **Club de Robótica UECEB**.

---

## ⚡ Características

* **Cero bloqueos:** No utiliza `delay()`. El bucle principal `loop()` se mantiene libre para lazos de control de motores, sensores y pantallas OLED.
* **Filtro antirrebote bidireccional:** Elimina falsos disparos por vibración de láminas metálicas (ventana de 35 ms).
* **Gestión por eventos:** Devuelve un tipo formal `ButtonEvent` listo para evaluar con `if` o `switch`.
* **Eventos soportados:**
  * Clic simple (`EVENT_SINGLE_CLICK`)
  * Doble clic (`EVENT_DOUBLE_CLICK`)
  * Triple clic (`EVENT_TRIPLE_CLICK`)
  * Pulsación larga / Hold (`EVENT_LONG_PRESS`)

---

## 🔌 Conexión del Circuito

La librería utiliza la resistencia interna de **Pull-Up** del microcontrolador. No requieres resistencias externas adicionales en la protoboard.

![Diagrama de conexión](docs/assets/arduino%20y%20boton.png)

```text
       ┌──────────────┐
       │Arduino/ESP32 │
       │              │
       │       GPIO 5 ├─────────┐
       │              │         │
       │          GND ├───┐     │
       └──────────────┘   │   ┌─┴─┐
                          │   │   │  Pulsador (Push Button)
                          └───┤   │
                              └───┘
```

* **Terminal 1 del botón:** Conectado al pin configurado (ej. GPIO 5).
* **Terminal 2 del botón:** Conectado directamente a **GND**.

* **En reposo (suelto):** El pin lee `HIGH` (3.3V).  
* **Presionado:** El pin lee `LOW` (0V).

---

## 🧠 ¿Cómo funciona por dentro? (La regla del árbitro)

Para entender la lógica sin marearse con el código, imagina que el microcontrolador es un árbitro observando el botón:

1. **El filtro de vibración (35 ms):** Si las láminas del botón tiemblan al hacer contacto, el árbitro no anota nada hasta que el voltaje se mantenga quieto durante 35 milisegundos seguidos.
2. **El Clic Largo (se resuelve abajo / LOW):** Si mantienes el botón presionado más de 600 ms, el árbitro grita `EVENT_LONG_PRESS` en caliente. No espera a que levantes el dedo.
3. **Los Clics Múltiples (se resuelven arriba / HIGH):** Cada vez que sueltas el botón, el árbitro anota un toque en su libreta y abre una **ventana de gracia de 280 ms**. Si en esos 280 ms vuelves a tocar, suma otro clic y reinicia el tiempo de gracia. Cuando pasan 280 ms de silencio total, evalúa cuántos toques diste (1, 2 o 3) y entrega el evento final.

---

## 📦 Instalación

1. Descarga este repositorio como archivo comprimido:  
   Clic en el botón verde **Code** -> **Download ZIP**.
2. En el **Arduino IDE**:
   * Menú superior: `Programa` -> `Incluir librería` -> `Añadir biblioteca .ZIP...`
   * Selecciona el archivo descargado.
3. Reinicia el entorno de desarrollo si es necesario.

---

## 🚀 Ejemplo Rápido: Detectar Doble Clic

Este ejemplo muestra la forma más limpia de reaccionar a un solo evento en específico (ideal para confirmar opciones o avanzar opciones en un robot):

```cpp
#include <MultiButton.h>

#define BTN_PIN 5  // Pin conectado al botón (el otro extremo a GND)

MultiButton btn(BTN_PIN);

void setup() {
  Serial.begin(115200);
  btn.begin();
  Serial.println("MultiButton iniciado. Da dos toques rapidos...");
}

void loop() {
  // update() devuelve EVENT_NONE casi todo el tiempo.
  // Solo devolvera EVENT_DOUBLE_CLICK tras 280 ms de silencio luego del 2do toque.
  if (btn.update() == EVENT_DOUBLE_CLICK) {
    Serial.println(">>> DOBLE CLIC CONFIRMADO <<<");
  }
}
```

---

## 🎮 Ejemplo Avanzado: Interfaz con 2 Botones

Gracias a la orientación a objetos, puedes controlar múltiples botones sin duplicar variables de tiempo ni funciones. Cada objeto gestiona su propio cronómetro interno y su máquina de estados de manera independiente:

```cpp
#include <MultiButton.h>

MultiButton btnNext(4);    // Pin 4 a GND
MultiButton btnSelect(5);  // Pin 5 a GND

void setup() {
  Serial.begin(115200);
  btnNext.begin();
  btnSelect.begin();
}

void loop() {
  ButtonEvent evNext   = btnNext.update();
  ButtonEvent evSelect = btnSelect.update();

  // Control del primer botón (Navegación)
  if (evNext == EVENT_SINGLE_CLICK) {
    Serial.println("Opcion siguiente");
  }

  // Control del segundo botón (Selección / Salida)
  if (evSelect == EVENT_SINGLE_CLICK) {
    Serial.println("Confirmar opcion");
  } else if (evSelect == EVENT_LONG_PRESS) {
    Serial.println("Atras / Cancelar");
  }
}
```

---

## 📊 Lista de Eventos Disponibles

| Evento | Descripción | Caso de uso típico |
| :--- | :--- | :--- |
| `EVENT_NONE` | Sin actividad o en espera de tiempo | Ignorar en el bucle principal |
| `EVENT_SINGLE_CLICK` | 1 toque corto seguido de 280 ms de silencio | Seleccionar opción / Siguiente |
| `EVENT_DOUBLE_CLICK` | 2 toques rápidos seguidos de 280 ms de silencio | Confirmar / Entrar a menú |
| `EVENT_TRIPLE_CLICK` | 3 toques rápidos | Calibración de sensores / Reset |
| `EVENT_LONG_PRESS` | Mantenido pulsado por más de 600 ms | Salir / Cancelar / Detener robot |
