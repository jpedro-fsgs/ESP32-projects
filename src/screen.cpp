#include <U8g2lib.h>

// Initialize the display (check I2C address)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

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

void setup()
{
    Serial.begin(115200);

    u8g2.begin();
    u8g2.setFont(u8g2_font_ncenB08_tr); // Escolha a fonte
    u8g2.clearBuffer();

    const char *meuTextoLongo = "O Arduino é uma plataforma de prototipagem eletrônica de código aberto. Verdadeiro ou falso?";

    // Chama a nossa função mágica!
    // Posição (0, 12), Largura máxima 128
    drawWrappedText(u8g2, 0, 12, 128, meuTextoLongo);
    u8g2.drawStr(0, 64, "TRUE OR FALSE?");

    u8g2.sendBuffer();
}

void loop()
{
}