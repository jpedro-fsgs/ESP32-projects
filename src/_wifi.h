#include <WiFi.h>
#include "config.h"

void setupWifi()
{
    WiFi.begin(ssid, password);

    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 20)
    {
        Serial.print("Tentando conectar... Status: ");
        Serial.println(WiFi.status());
        delay(1000);
        tentativas++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nConectado!");
        Serial.print("IP local: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("Falha ao conectar ao WiFi.");
    }
}
