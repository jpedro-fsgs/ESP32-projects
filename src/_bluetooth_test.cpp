#include <Arduino.h>
#include "BluetoothSerial.h"

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

BluetoothSerial SerialBT; // cria o objeto serial Bluetooth

int ligado;
int buttonState;

void setup()
{
  Serial.begin(9600);
  pinMode(PIN5, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);
  SerialBT.begin("ESP32_Controlador"); // nome Bluetooth visível
  Serial.println("Bluetooth iniciado. Pronto para parear.");
  ligado = 0;
  buttonState = HIGH;
  startup_blink(PIN5);
}

void loop()
{

  if (buttonState != digitalRead(BUTTONPIN))
  {
    delay(50); // Debounce
    int newButtonState = digitalRead(BUTTONPIN);
    if (newButtonState == buttonState)
      return; // Ruído, ignora

    buttonState = newButtonState;

    if (buttonState == LOW)
    { // Botão pressionado
      SerialBT.println("Botão Apertado!");
      blink(100, PIN5, 3);
    }
    else
    {
      SerialBT.println("Botão solto.");
    }
  }
  if (SerialBT.available())
  {
    char recebido = SerialBT.read();
    Serial.print("Recebido: ");
    Serial.println(recebido);

    // Exemplo de controle: ligar LED com comando
    if (recebido == 'T')
      ligado = !ligado;
    if (recebido == 'L')
      ligado = 1;
    if (recebido == 'D')
      ligado = 0;
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
