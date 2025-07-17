#include <Arduino.h>
#include "_wifi.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "_screen.h"

#define REDLIGHT 25
#define WHITEBUTTON 18
#define REDBUTTON 19
#define BUZZER 33



void blinkNonBlocking(int interval)
{
    static bool redLightState = false;
    static unsigned long lastBlinkTime = 0;
    unsigned long currentTime = millis();

    // Verifica se passou tempo suficiente
    if (currentTime - lastBlinkTime >= interval)
    {
        redLightState = !redLightState; // Inverte o estado
        digitalWrite(REDLIGHT, redLightState ? HIGH : LOW);
        lastBlinkTime = currentTime; // Atualiza o tempo da última mudança
    }
}

void buzzNonBlocking(int interval)
{
    static bool buzzerState = false;
    static unsigned long lastBlockTime = 0;
    unsigned long currentTime = millis();

    // Verifica se passou tempo suficiente
    if (currentTime - lastBlockTime >= interval)
    {
        buzzerState = !buzzerState; // Inverte o estado
        digitalWrite(BUZZER, buzzerState ? HIGH : LOW);
        lastBlockTime = currentTime; // Atualiza o tempo da última mudança
    }
}
struct team
{
    String strEvent;
    String strLeague;
    String strTime;
    String dateEvent;
    const char *api;
};

struct team teams[] = {
    {"", "", "", "", "https://www.thesportsdb.com/api/v1/json/123/eventsnext.php?id=134284"}, // Corinthians
    {"", "", "", "", "https://www.thesportsdb.com/api/v1/json/123/eventsnext.php?id=133616"}  // Tottenham
};

int currentTeam = 0;

bool whiteButtonPressed()
{
    if (digitalRead(WHITEBUTTON) == LOW)
    {
        digitalWrite(BUZZER, HIGH);
        delay(200);
        return true;
    }
    digitalWrite(BUZZER, LOW);
    return false;
}

bool redButtonPressed()
{
    if (digitalRead(REDBUTTON) == LOW)
    {
        digitalWrite(BUZZER, HIGH);
        delay(200);
        return true;
    }
    digitalWrite(BUZZER, LOW);
    return false;
}

void setup()
{

    Serial.begin(115200);
    // Print a welcome message
    Serial.println("Vai Corinthians!");
    setupScreen();
    pinMode(REDLIGHT, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(WHITEBUTTON, INPUT_PULLUP);
    pinMode(REDBUTTON, INPUT_PULLUP);

    setupWifi();
    printLine("Vai Corinthians!");
}

void printSchedule()
{
    // u8g2.clearBuffer();
    // u8g2.drawStr(0, 12, (teams[currentTeam].strEvent).c_str());
    // u8g2.drawStr(0, 24, (teams[currentTeam].strLeague).c_str());
    // u8g2.drawStr(0, 36, ("Time: " + teams[currentTeam].strTime).c_str());
    // u8g2.drawStr(0, 48, ("Date: " + teams[currentTeam].dateEvent).c_str());

    printLineRoll((teams[currentTeam].strEvent).c_str(), 12);
    printLineRoll((teams[currentTeam].strLeague).c_str(), 24);
    printLineRoll(("Time: " + teams[currentTeam].strTime).c_str(), 36);
    printLineRoll(("Date: " + teams[currentTeam].dateEvent).c_str(), 48);
    // u8g2.sendBuffer();

}

JsonDocument doc;

void requestSchedule()
{
    Serial.println("Carregando...");
    printLine("LOADING...");

    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(teams[currentTeam].api);
        int httpCode = http.GET();

        if (httpCode > 0)
        {
            String payload = http.getString();
            deserializeJson(doc, payload);
            teams[currentTeam].strEvent = doc["events"][0]["strEvent"].as<String>();
            teams[currentTeam].strLeague = doc["events"][0]["strLeague"].as<String>();
            teams[currentTeam].strTime = doc["events"][0]["strTime"].as<String>();
            teams[currentTeam].dateEvent = doc["events"][0]["dateEvent"].as<String>();

            printSchedule();
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

// Loop function runs repeatedly
void loop()
{
    if (whiteButtonPressed())
    {
        currentTeam = !currentTeam;
    }
    if (redButtonPressed())
    {
        requestSchedule();
    }
    if (!currentTeam)
    {
        blinkNonBlocking(500);
        // buzzNonBlocking(250);
    }
    else
    {
        blinkNonBlocking(250);
        // buzzNonBlocking(100);
    }
    printSchedule();
    showTime();
}