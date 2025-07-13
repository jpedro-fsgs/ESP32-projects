#include<Arduino.h>
#include"_screen.h"

#define REDLIGHT 2
#define WHITEBUTTON 19
#define REDBUTTON 18

int counter;

void setup(){

    Serial.begin(115200);
    Serial.println("init");
    counter = 0;
    setupScreen();
    pinMode(REDLIGHT, OUTPUT);
    pinMode(WHITEBUTTON, INPUT_PULLUP);
    pinMode(REDBUTTON, INPUT_PULLUP);
    printText(std::to_string(counter).c_str());
    
}

int whiteButtonPressed(){
    if(digitalRead(WHITEBUTTON) == LOW){
        delay(200);
        return 1;
    }
    return 0;
}

int redButtonPressed(){
    if(digitalRead(REDBUTTON) == LOW){
        delay(200);
        return 1;
    }
    return 0;
}


void loop() {
    if(whiteButtonPressed()){
        counter--;
        printText(std::to_string(counter).c_str());
        Serial.println(counter);
    }
    if(redButtonPressed()){
        counter++;
        printText(std::to_string(counter).c_str());
        Serial.println(counter);
    }

}