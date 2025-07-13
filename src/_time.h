#include "_screen.h"
#include <time.h>

void clearTextLine(int y, int altura = 12) {
    u8g2.setDrawColor(0); // apagar
    u8g2.drawBox(0, y - altura + 1, u8g2.getDisplayWidth(), altura);
    u8g2.setDrawColor(1); // volta ao normal
}

void showTime() {
    static unsigned long ultimaAtualizacao = 0;
    unsigned long agora = millis();

    if (agora - ultimaAtualizacao >= 1000) {  // 1 segundo passou
        ultimaAtualizacao = agora;

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Erro ao obter hora");
            return;  // sem delay!
        }

        // Formata o horário para HH:MM:SS
        char buffer[9];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);

        clearTextLine(60);
        u8g2.drawStr(0, 60, buffer);    // Escreve o texto
        u8g2.sendBuffer();              // Atualiza o display
    }
}