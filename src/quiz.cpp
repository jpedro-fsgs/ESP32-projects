#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

int status = WL_IDLE_STATUS;
const char *api = "https://opentdb.com/api.php?amount=1&difficulty=easy&type=boolean";
#define REDLIGHT 5
#define WHITEBUTTON 18
#define REDBUTTON 19

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

JsonDocument doc;

int perguntaAtiva = 0;
String perguntaAtual;
int respostaAtual;

void realizarPergunta(String pergunta, int resposta)
{
    Serial.println(perguntaAtual);
}

void checarResposta()
{

    if (digitalRead(REDBUTTON) == LOW)
    {
        Serial.println("Pressionado: Falso");
        blink(100, REDLIGHT, 5);
        if (!respostaAtual)
        {
            Serial.println("Certa Resposta!");
            blink(500, REDLIGHT, 1);
        }
        else
        {
            Serial.println("Resposta Incorreta!");
            blink(500, REDLIGHT, 1);
        }
        perguntaAtiva = 0;
    }
    else if (digitalRead(WHITEBUTTON) == LOW)
    {
        Serial.println("Pressionado: Verdadeiro");
        blink(100, REDLIGHT, 5);
        if (respostaAtual)
        {
            Serial.println("Certa Resposta!");
            blink(500, REDLIGHT, 1);
        }
        else
        {
            Serial.println("Resposta Incorreta!");
            blink(500, REDLIGHT, 1);
        }
        perguntaAtiva = 0;
    }
}

void loop()
{
    if (!perguntaAtiva)
    {
        if (digitalRead(WHITEBUTTON) == LOW || digitalRead(REDBUTTON) == LOW)
        {
            Serial.println("Carregando Pergunta...");
            blink(5, REDLIGHT, 5);

            if (WiFi.status() == WL_CONNECTED)
            {
                HTTPClient http;
                http.begin(api); // Substitua pela URL da sua API
                int httpCode = http.GET();

                if (httpCode > 0)
                {
                    String payload = http.getString();
                    deserializeJson(doc, payload);
                    String question = doc["results"][0]["question"];
                    String answer = doc["results"][0]["correct_answer"];

                    perguntaAtiva = 1;
                    perguntaAtual = question;
                    respostaAtual = answer == "True";
                    Serial.println(perguntaAtual);
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
    }
    else
    {
        checarResposta();
    }
}
