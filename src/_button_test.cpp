#include <Arduino.h>

#define PIN5 5
#define BUTTONPIN 19

void blink(int duration, int led, int times)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(led, HIGH); // Liga o LED
    delay(duration);         // Espera
    digitalWrite(led, LOW);  // Desliga o LED
    delay(duration);
  }
}

void startup_blink(int led)
{
  blink(100, led, 5);
}

int ligado;
int buttonState;

void setup()
{
  Serial.begin(9600);
  pinMode(PIN5, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);
  ligado = 0;
  buttonState = HIGH;
  startup_blink(PIN5);
}

void loop()
{

  if (buttonState != digitalRead(BUTTONPIN))
  {
    Serial.println("Botão");
    delay(50); // Debounce
    int newButtonState = digitalRead(BUTTONPIN);
    if (newButtonState == buttonState)
      return; // Ruído, ignora

    buttonState = newButtonState;
  }
  if (ligado)
  {
    digitalWrite(PIN5, HIGH);
  }
  else
  {
    digitalWrite(PIN5, LOW);
  }
}
