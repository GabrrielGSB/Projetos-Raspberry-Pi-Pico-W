#include "displaySSD1306.h"

struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
};

uint8_t display[ssd1306_buffer_length];

const char* numToStr(uint num) {
    static char buffer[12];  

    snprintf(buffer, sizeof(buffer), "%u", num); 

    return buffer;
}

void configurarDisplay() {
    configurarI2C();
    ssd1306_init();

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    memset(display, 0, ssd1306_buffer_length);
    render_on_display(display, &frame_area);
}

void atualizarDisplay() {
    render_on_display(display, &frame_area);
}

void limparDisplay() {
    memset(display, 0, ssd1306_buffer_length);
}  

void desenharPixel(uint8_t x, uint8_t y) {
    if (x >= ssd1306_width || y >= ssd1306_height) {
        return; // Ignora coordenadas fora dos limites do display
    }

    uint16_t byte_index = (y / 8) * ssd1306_width + x;
    uint8_t bit_mask = 1 << (y % 8);

    display[byte_index] |= bit_mask; // Define o bit correspondente para acender o pixel
}

void apagarPixel(uint8_t x, uint8_t y) {
    if (x >= ssd1306_width || y >= ssd1306_height) {
        return; // Ignora coordenadas fora dos limites do display
    }

    uint16_t byte_index = (y / 8) * ssd1306_width + x;
    uint8_t bit_mask = ~(1 << (y % 8));

    display[byte_index] &= bit_mask; // Limpa o bit correspondente para apagar o pixel
}

void desenharLinha(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        desenharPixel(x0, y0);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int err2 = err * 2;

        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void apagarLinha(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        apagarPixel(x0, y0);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int err2 = err * 2;

        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void desenharRetangulo(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    for (uint8_t x = x0; x <= x1; x++) {
        for (uint8_t y = y0; y <= y1; y++) {
            desenharPixel(x, y);
        }
    }
}

void apagarRetangulo(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    for (uint8_t x = x0; x <= x1; x++) {
        for (uint8_t y = y0; y <= y1; y++) {
            apagarPixel(x, y);
        }
    }
}

void desenharCirculo(uint8_t x0, uint8_t y0, uint8_t radius) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    desenharPixel(x0, y0 + radius);
    desenharPixel(x0, y0 - radius);
    desenharPixel(x0 + radius, y0);
    desenharPixel(x0 - radius, y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        desenharPixel(x0 + x, y0 + y);
        desenharPixel(x0 - x, y0 + y);
        desenharPixel(x0 + x, y0 - y);
        desenharPixel(x0 - x, y0 - y);
        desenharPixel(x0 + y, y0 + x);
        desenharPixel(x0 - y, y0 + x);
        desenharPixel(x0 + y, y0 - x);
        desenharPixel(x0 - y, y0 - x);
    }
}

void apagarCirculo(uint8_t x0, uint8_t y0, uint8_t radius) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    apagarPixel(x0, y0 + radius);
    apagarPixel(x0, y0 - radius);
    apagarPixel(x0 + radius, y0);
    apagarPixel(x0 - radius, y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        apagarPixel(x0 + x, y0 + y);
        apagarPixel(x0 - x, y0 + y);
        apagarPixel(x0 + x, y0 - y);
        apagarPixel(x0 - x, y0 - y);
        apagarPixel(x0 + y, y0 + x);
        apagarPixel(x0 - y, y0 + x);
        apagarPixel(x0 + y, y0 - x);
        apagarPixel(x0 - y, y0 - x);
    }
}

void desenharSimbolo(const char simbolo, uint8_t x, uint8_t y) {
    char c = simbolo;
    int offset = -1;

    // 1. Mapeamento do Índice
    if      (simbolo >= 'A' && simbolo <= 'Z') { offset = (simbolo - 'A' + 1) * 8;  } 
    else if (simbolo >= 'a' && simbolo <= 'z') { offset = (simbolo - 'a' + 1) * 8;  } 
    else if (simbolo >= '0' && simbolo <= '9') { offset = (simbolo - '0' + 27) * 8; } 
    else if (simbolo == ' ')                 { offset = 0; }
    if (offset == -1) return;

    // 2. Renderização do Bitmap
    // Cada byte no seu array representa uma COLUNA vertical de 8 pixels
    for (int col = 0; col < 8; col++) {
        uint8_t linha_vertical = font[offset + col];
        
        for (int bit = 0; bit < 8; bit++) {
            if (linha_vertical & (1 << bit)) { // Verifica se o bit específico está ligado (1)
                desenharPixel(x + col, y + bit);
            }
        }
    }
}

void desenharString(const char* str, uint8_t x, uint8_t y) {
    while (*str) {
        desenharSimbolo(*str, x, y);
        x += 8; // Move para a próxima posição de caractere (8 pixels de largura)
        str++;
    }
}