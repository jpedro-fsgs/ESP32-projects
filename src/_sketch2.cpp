#include <Arduino.h>
#include "BluetoothSerial.h"

#define PIN5 5

BluetoothSerial SerialBT;  // cria o objeto serial Bluetooth

int ligado;

void setup() {
  Serial.begin(9600);
  pinMode(PIN5, OUTPUT);
  SerialBT.begin("ESP32_Controlador");  // nome Bluetooth visível
  Serial.println("Bluetooth iniciado. Pronto para parear.");
  ligado = 0;
}

void loop() {
  if (SerialBT.available()) {
    char recebido = SerialBT.read();
    Serial.print("Recebido: ");
    Serial.println(recebido);

    // Exemplo de controle: ligar LED com comando
    if (recebido == 'T') ligado = !ligado;
    if (recebido == 'L') ligado = 1;
    if (recebido == 'D') ligado = 0;

    if (ligado) {
      digitalWrite(PIN5, HIGH);
      SerialBT.println("LED Ligado");
    }
    else {
      digitalWrite(PIN5, LOW);
      SerialBT.println("LED Desligado");
    }
  }
}
