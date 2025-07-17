#include <Arduino.h>

#define BUZZER 33
#define WHITEBUTTON 18
#define REDBUTTON 19
#define SENSOR 26

#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

int canalPWM = 0;  // Canal de 0 a 15
int freq = 1000;   // Frequência de 1kHz
int resolucao = 8; // 8 bits: 0-255 de duty cycle

bool whiteButtonPressed()
{
    if (digitalRead(WHITEBUTTON) == LOW)
    {
        return true;
    }
    return false;
}

bool redButtonPressed()
{

    if (digitalRead(REDBUTTON) == LOW)
    {
        return true;
    }
    return false;
}

void setup()
{
    Serial.begin(115200);
    pinMode(WHITEBUTTON, INPUT_PULLUP);
    pinMode(REDBUTTON, INPUT_PULLUP);
    pinMode(SENSOR, INPUT);
    ledcSetup(canalPWM, freq, resolucao); // Configura o PWM
    ledcAttachPin(BUZZER, canalPWM);      // Associa o pino ao canal
    // ledcWrite(canalPWM, 200);
    Serial.println("Ready");
}

int lastTone = 0;

void loop()
{
    // if (redButtonPressed())
    // {
    //     if (lastTone != C4)
    //     {
    //         ledcWriteTone(canalPWM, C4);
    //         ledcWrite(canalPWM, 200);
    //         lastTone = C4;
    //         Serial.println("Dó");
    //     }
    // }
    // else if (whiteButtonPressed())
    // {
    //     if (lastTone != G4)
    //     {
    //         ledcWriteTone(canalPWM, G4);
    //         ledcWrite(canalPWM, 200);
    //         lastTone = G4;
    //         Serial.println("Sol");
    //     }
    // }
    // else
    // {
    //     ledcWrite(canalPWM, 0); // Silencia
    //     lastTone = 0;
    // }

    if (digitalRead(SENSOR) == LOW)
    {
            Serial.println("!!!");

            ledcWriteTone(canalPWM, C4);
            ledcWrite(canalPWM, 200);
            delay(200);
            ledcWriteTone(canalPWM, G4);
            ledcWrite(canalPWM, 200);
            delay(200);
            ledcWrite(canalPWM, 0);

    }
}
