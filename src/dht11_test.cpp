#include<Arduino.h>
#include "DHT.h"

#define DHTPIN 26        // Pino conectado ao DHT11
#define DHTTYPE DHT11    // Tipo do sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(2000);  // DHT11 não suporta leituras rápidas

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.println(dht.readHumidity());
  Serial.println(dht.readTemperature());

  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na leitura do sensor DHT11");
    return;
  }

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C");
}