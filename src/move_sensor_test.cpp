#include <Arduino.h>

#define REDLIGHT 25
#define BUZZER 33
#define WHITEBUTTON 18
#define REDBUTTON 19
#define PIR_PIN 26


void setup()
{
    pinMode(PIR_PIN, INPUT);
    pinMode(REDLIGHT, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(WHITEBUTTON, INPUT_PULLUP);
    pinMode(REDBUTTON, INPUT_PULLUP);
    Serial.begin(115200);
    delay(200);
    Serial.println("start");
}

void loop()
{
    bool motionDetected = digitalRead(PIR_PIN) == HIGH;

    if(motionDetected){
        digitalWrite(BUZZER, HIGH);
        delay(200);
        digitalWrite(BUZZER, LOW);
        delay(200);
    }
    Serial.println(motionDetected ? "Motion detected" : "No motion");
    delay(200);
}
