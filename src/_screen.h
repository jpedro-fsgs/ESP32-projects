#include <U8g2lib.h>

// Initialize the display (check I2C address)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600;
const int daylightOffset_sec = 0;

// =======================================================================
// FUNÇÃO PARA DESENHAR TEXTO COM QUEBRA DE LINHA AUTOMÁTICA
// Parâmetros:
// u8g2: A referência para o seu objeto u8g2
// x, y: A posição inicial do texto (canto superior esquerdo)
// maxWidth: A largura máxima em pixels (para sua tela, será 128)
// text: O texto longo que você quer desenhar
// =======================================================================
void drawWrappedText(U8G2 &u8g2, int x, int y, int maxWidth, const char *text)
{
    char *str = strdup(text); // Cria uma cópia do texto para poder modificá-la
    if (str == NULL)
        return;

    char *word = strtok(str, " ");
    char line[50] = ""; // Armazena a linha atual
    int lineHeight = u8g2.getMaxCharHeight();

    while (word != NULL)
    {
        char testLine[50] = "";
        strcpy(testLine, line);
        if (strlen(line) > 0)
        {
            strcat(testLine, " ");
        }
        strcat(testLine, word);

        if (u8g2.getStrWidth(testLine) > maxWidth)
        {
            // A linha atual mais a nova palavra excedem a largura
            // Então, desenha a linha atual
            u8g2.drawStr(x, y, line);

            // Move para a próxima linha
            y += lineHeight;

            // Começa uma nova linha com a palavra que não coube
            strcpy(line, word);
        }
        else
        {
            // A palavra cabe, então adiciona à linha atual
            strcpy(line, testLine);
        }

        word = strtok(NULL, " "); // Pega a próxima palavra
    }

    // Desenha a última linha que sobrou
    u8g2.drawStr(x, y, line);

    free(str); // Libera a memória alocada pela cópia
}

void setupScreen()
{
    u8g2.begin();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.clearBuffer();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void clearTextLine(int y, int altura = 12)
{
    u8g2.setDrawColor(0); // apagar
    u8g2.drawBox(0, y - altura + 1, u8g2.getDisplayWidth(), altura);
    u8g2.setDrawColor(1); // volta ao normal
}

void printText(const char *text)
{

    u8g2.clearBuffer();
    drawWrappedText(u8g2, 0, 12, 128, text);
    u8g2.sendBuffer();
}

void printLine(const char *text, int y = 32)
{
    u8g2.clearBuffer();
    u8g2.drawStr(0, y, text);
    u8g2.sendBuffer();
}

// Suponha que a largura do display seja 128 pixels
#define SCREEN_WIDTH 128

void printLineRoll(const char *text, int y = 32)
{
    static int x = SCREEN_WIDTH; // posição horizontal inicial
    static unsigned long lastUpdate = 0;
    const int scrollSpeed = 10; // em milissegundos

    int textWidth = u8g2.getStrWidth(text);

    if (millis() - lastUpdate > scrollSpeed)
    {
        lastUpdate = millis();

        clearTextLine(y);

        if (textWidth > SCREEN_WIDTH)
        {
            u8g2.setCursor(x, y);
            u8g2.print(text);

            x--;

            if (x < -textWidth)
            {
                x = SCREEN_WIDTH;
            }
        }
        else
        {
            // Texto cabe na tela, centraliza ou alinha à esquerda
            u8g2.setCursor(0, y);
            u8g2.print(text);
        }

        u8g2.sendBuffer();
    }
}



void showTime()
{

    static unsigned long ultimaAtualizacao = 0;
    unsigned long agora = millis();

    if (agora - ultimaAtualizacao >= 1000)
    { // 1 segundo passou
        ultimaAtualizacao = agora;

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Erro ao obter hora");
            return; // sem delay!
        }

        // Formata o horário para HH:MM:SS
        char buffer[9];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);

        clearTextLine(60);
        u8g2.drawStr(0, 60, buffer); // Escreve o texto
        u8g2.sendBuffer();           // Atualiza o display
    }
}