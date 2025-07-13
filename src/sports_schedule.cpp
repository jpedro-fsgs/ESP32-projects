#include <Arduino.h>
#include "_wifi.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "_time.h"

#define REDLIGHT 33
#define WHITEBUTTON 18
#define REDBUTTON 19

bool redLightState = false;
unsigned long lastBlinkTime = 0;

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

int status = WL_IDLE_STATUS;
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600;
const int daylightOffset_sec = 0;

int whiteButtonPressed()
{
    if (digitalRead(WHITEBUTTON) == LOW)
    {
        delay(200);
        return 1;
    }
    return 0;
}

int redButtonPressed()
{
    if (digitalRead(REDBUTTON) == LOW)
    {
        delay(200);
        return 1;
    }
    return 0;
}

void setup()
{

    Serial.begin(115200);
    // Print a welcome message
    Serial.println("Vai Corinthians!");
    setupScreen();
    pinMode(REDLIGHT, OUTPUT);
    pinMode(WHITEBUTTON, INPUT_PULLUP);
    pinMode(REDBUTTON, INPUT_PULLUP);

    setupWifi();

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLine("Vai Corinthians!");
}

void printSchedule()
{
    u8g2.clearBuffer();
    u8g2.drawStr(0, 12, (teams[currentTeam].strEvent).c_str());
    u8g2.drawStr(0, 24, (teams[currentTeam].strLeague).c_str());
    u8g2.drawStr(0, 36, ("Time: " + teams[currentTeam].strTime).c_str());
    u8g2.drawStr(0, 48, ("Date: " + teams[currentTeam].dateEvent).c_str());
    u8g2.sendBuffer();
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
        printSchedule();
    }
    if (redButtonPressed())
    {
        requestSchedule();
    }
    if (!currentTeam)
    {
        blinkNonBlocking(1000);
    }
    else
    {
        blinkNonBlocking(2000);
    }
    showTime();
}