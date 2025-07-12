#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

int status = WL_IDLE_STATUS;
char *api = "https://opentdb.com/api.php?amount=1&difficulty=easy&type=boolean";

#define REDLIGHT 5
#define WHITEBUTTON 19
#define REDBUTTON 21

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

void setup()
{

    Serial.begin(9600);
    pinMode(REDLIGHT, OUTPUT);
    pinMode(WHITEBUTTON, INPUT_PULLUP);
    pinMode(REDBUTTON, INPUT_PULLUP);
    startup_blink(REDLIGHT);

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
        startup_blink(REDLIGHT);
    }
    else
    {
        Serial.println("Falha ao conectar ao WiFi.");
    }

    startup_blink(REDLIGHT);
}

void realizarPergunta(String pergunta, int resposta){
    Serial.println(pergunta);
}

void loop()
{
    if (digitalRead(WHITEBUTTON) == LOW)
    {
        Serial.println("WHITE");
        // blink(100, REDLIGHT, 7);

        if (WiFi.status() == WL_CONNECTED)
        {
            HTTPClient http;
            http.begin(api); // Substitua pela URL da sua API
            int httpCode = http.GET();

            if (httpCode > 0)
            {
                String payload = http.getString();
                Serial.println("Resposta do servidor:");
                Serial.println(payload);
            }
            else
            {
                Serial.print("Erro na requisição HTTP: ");
                Serial.println(http.errorToString(httpCode).c_str());
            }

            http.end();
        }
        else
        {
            Serial.println("WiFi desconectado.");
        }
    }
    if (digitalRead(REDBUTTON) == LOW)
    {
        Serial.println("RED");
        blink(250, REDLIGHT, 3);
    }
}
