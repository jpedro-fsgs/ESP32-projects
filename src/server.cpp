#include <Arduino.h>
#include "_wifi.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "_screen.h"

#define REDLIGHT 33
#define WHITEBUTTON 18
#define REDBUTTON 19

bool redLightState = false;
unsigned long lastBlinkTime = 0;
bool loadingState = false;

void blinkNonBlocking(int interval)
{
    unsigned long currentTime = millis();

    // Verifica se passou tempo suficiente
    if (currentTime - lastBlinkTime >= interval)
    {
        redLightState = !redLightState; // Inverte o estado
        digitalWrite(REDLIGHT, redLightState ? HIGH : LOW);
        lastBlinkTime = currentTime; // Atualiza o tempo da última mudança
    }
}

bool whiteButtonPressed()
{
    if (digitalRead(WHITEBUTTON) == LOW)
    {
        delay(200);
        return true;
    }
    return false;
}

bool redButtonPressed()
{
    if (digitalRead(REDBUTTON) == LOW)
    {
        delay(200);
        return true;
    }
    return false;
}

const char *api = "http://192.168.0.28:3333/last_message";
JsonDocument doc;

void requestEndpoint()
{
    digitalWrite(REDLIGHT, HIGH);
    Serial.println("Carregando...");
    printLine("LOADING...");

    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(api);
        int httpCode = http.GET();

        if (httpCode > 0)
        {
            String payload = http.getString();
            deserializeJson(doc, payload);
            printLine(doc["lastMessage"]);
        }
        else
        {
            Serial.print("Erro na requisição HTTP: ");
            Serial.println(http.errorToString(httpCode).c_str());
            printLine("ERROR");
        }

        http.end();
    }
    else
    {
        Serial.println("WiFi desconectado.");
    }
    digitalWrite(REDLIGHT, LOW);
}

void setup()
{
    Serial.begin(115200);
    pinMode(REDLIGHT, OUTPUT);
    pinMode(WHITEBUTTON, INPUT_PULLUP);
    pinMode(REDBUTTON, INPUT_PULLUP);
    setupScreen();
    setupWifi();
    setupScreen();
}

void loop()
{
    if (redButtonPressed())
    {
        requestEndpoint();
    }
}
