#include <Arduino.h>

#define LEDPIN 2
#define PIN5 5
#define BUTTONPIN 19

void blink(int duration, int led, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(led, HIGH);  // Liga o LED
    delay(duration);          // Espera
    digitalWrite(led, LOW);   // Desliga o LED
    delay(duration);
  }
}

void startup_blink(int led) {
  blink(100, led, 5);
}

void sos(int led) {
  blink(250, led, 3);
  blink(500, led, 3);
}

int buttonState;

void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);  // Configura o pino como saída
  pinMode(PIN5, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);
  buttonState = HIGH;
  startup_blink(LEDPIN);
}

void loop() {

  if (buttonState != digitalRead(BUTTONPIN)) {
    buttonState = digitalRead(BUTTONPIN);
    delay(50);
    if (buttonState == LOW) {  // Botão pressionado
      Serial.println("Botão Apertado!");
      digitalWrite(PIN5, HIGH);

    } else {
      Serial.println("Botão solto.");
      blink(100, PIN5, 3);
      // digitalWrite(PIN5, LOW);
    }
  }
}
