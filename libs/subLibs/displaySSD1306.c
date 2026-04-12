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

//======================================================================
#pragma region //FUNÇÕES DE AÇÃO NO DISPLAY

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

#pragma endregion
//======================================================================

//======================================================================
#pragma region //FUNÇÕES DE DESENHOS SIMPLES 

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

void desenharCirculoPreenchido(uint8_t x0, uint8_t y0, uint8_t radius) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    // 1. Desenha a linha central horizontal (o "equador" do círculo)
    desenharLinha(x0 - radius, y0, x0 + radius, y0);

    // 2. Preenche o restante do círculo fatiando de cima para baixo
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        // Em vez de desenhar os 8 pixels das bordas, nós desenhamos 4 linhas 
        // horizontais que conectam as bordas espelhadas do círculo.

        // Linhas mais largas (próximas ao centro)
        desenharLinha(x0 - y, y0 + x, x0 + y, y0 + x);
        desenharLinha(x0 - y, y0 - x, x0 + y, y0 - x);
        
        // Linhas mais estreitas (próximas aos polos)
        desenharLinha(x0 - x, y0 + y, x0 + x, y0 + y);
        desenharLinha(x0 - x, y0 - y, x0 + x, y0 - y);
    }
}

void desenharArco_90deg(uint8_t x0, uint8_t y0, uint8_t radius) {
    if (radius == 0) return;
    
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    desenharPixel(x0, y0 - radius); // Ponto mais alto do arco

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        // Ao pintar apenas estas duas coordenadas, criamos uma fatia 
        // de exatos -45° a +45° a partir do centro (formato do Wi-Fi)
        desenharPixel(x0 + x, y0 - y);
        desenharPixel(x0 - x, y0 - y);
    }
}

void desenharSorriso(uint8_t x0, uint8_t y0, uint8_t radius) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    desenharPixel(x0, y0 + radius); // Ponto mais baixo (centro do lábio inferior)

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        // Pintamos APENAS os quadrantes inferiores (y0 + algo)
        desenharPixel(x0 + x, y0 + y);
        desenharPixel(x0 - x, y0 + y);
        desenharPixel(x0 + y, y0 + x);
        desenharPixel(x0 - y, y0 + x);
    }
}

#pragma endregion
//======================================================================

//======================================================================
#pragma region //FUNÇÕES DE ESCRITA

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
    for (int col = 0; col < 8; col++) {
        uint8_t linha_vertical = font[offset + col];
        
        for (int bit = 0; bit < 8; bit++) {
            if (linha_vertical & (1 << bit)) { 
                desenharPixel(x + col, y + bit);
            }
        }
    }
}

void desenharString(const char* str, uint8_t x, uint8_t y) {
    while (*str) {
        desenharSimbolo(*str, x, y);
        x += 8; 
        str++;
    }
}

#pragma endregion
//======================================================================

//======================================================================
#pragma region //FUNÇÕES DE DESENHOS SIMBÓLICOS

void desenharSimboloWiFi(uint8_t x, uint8_t y, uint8_t tamanho, bool sem_conexao) {
    // 1. Desenha o ponto base (a bolinha do transmissor)
    // Usamos um pequeno retângulo preenchido para ficar bem visível no OLED
    uint8_t raio_ponto = (tamanho >= 10) ? 2 : 1;
    desenharRetangulo(x - raio_ponto, y - raio_ponto, x + raio_ponto, y + raio_ponto);

    // 2. Desenha os Arcos concêntricos
    // Dividimos o tamanho total por 3 para dar o espaçamento das ondas
    uint8_t espaco = tamanho / 3;

    if (tamanho >= 4) {
        // Arco Interno
        desenharArco_90deg(x, y, espaco);
        desenharArco_90deg(x, y, espaco + 1); // +1 para deixar a linha mais grossa
    }
    
    if (tamanho >= 8) {
        // Arco do Meio
        desenharArco_90deg(x, y, espaco * 2);
        desenharArco_90deg(x, y, espaco * 2 + 1);
    }
    
    if (tamanho >= 12) {
        // Arco Externo
        desenharArco_90deg(x, y, tamanho);
        desenharArco_90deg(x, y, tamanho - 1);
    }

    // 3. Desenha o risco transversal caso não haja conexão
    if (sem_conexao) {
        // Faz um traço diagonal cortando o símbolo do canto superior esquerdo ao inferior direito
        desenharLinha(x - tamanho, y - tamanho + 2, x + tamanho, y + 2);
    }
}

void desenharSimboloHTTP(uint8_t x, uint8_t y, bool sem_conexao) {
    // Parâmetros de tamanho do nosso "vetor" (Você pode alterar para crescer ou encolher)
    uint8_t h = 14; // Altura das letras
    uint8_t w = 10; // Largura das letras
    uint8_t s = 4;  // Espaçamento entre as letras

    // --- Letra H ---
    uint8_t x_H = x;
    desenharLinha(x_H, y, x_H, y + h);             // Traço vertical esquerdo
    desenharLinha(x_H + w, y, x_H + w, y + h);     // Traço vertical direito
    desenharLinha(x_H, y + (h/2), x_H + w, y + (h/2)); // Traço horizontal do meio

    // --- Letra T (Primeiro) ---
    uint8_t x_T1 = x_H + w + s;
    desenharLinha(x_T1, y, x_T1 + w, y);                   // Traço superior (telhado)
    desenharLinha(x_T1 + (w/2), y, x_T1 + (w/2), y + h);   // Traço vertical central

    // --- Letra T (Segundo) ---
    uint8_t x_T2 = x_T1 + w + s;
    desenharLinha(x_T2, y, x_T2 + w, y);                   // Traço superior (telhado)
    desenharLinha(x_T2 + (w/2), y, x_T2 + (w/2), y + h);   // Traço vertical central

    // --- Letra P ---
    uint8_t x_P = x_T2 + w + s;
    desenharLinha(x_P, y, x_P, y + h);                     // Traço vertical principal
    desenharLinha(x_P, y, x_P + w, y);                     // Traço superior
    desenharLinha(x_P + w, y, x_P + w, y + (h/2));         // Traço vertical da barriga
    desenharLinha(x_P, y + (h/2), x_P + w, y + (h/2));     // Traço horizontal do meio da barriga

    // --- Risco Transversal (Offline) ---
    if (sem_conexao) {
        // Corta da ponta inferior esquerda do 'H' até a ponta superior direita do 'P'
        uint8_t x_final = x_P + w;
        
        // Desenhamos duas linhas com 1 pixel de diferença no eixo Y para criar 
        // um traço mais "grosso", deixando claro que é um erro ou bloqueio.
        desenharLinha(x - 2, y + h + 2, x_final + 2, y - 2);
        desenharLinha(x - 2, y + h + 3, x_final + 2, y - 1);
    }
}

void desenharCarinhaSorridente(uint8_t x, uint8_t y, uint8_t tamanho) {
    if (tamanho < 5) return; // Muito pequeno para desenhar detalhes

    // 1. Desenha o contorno do rosto
    desenharCirculo(x, y, tamanho);

    // 2. Calcula as posições proporcionais
    uint8_t distancia_olhos = tamanho / 3;
    uint8_t altura_olhos = tamanho / 4;
    
    // Os olhos ficam um pouco maiores se o rosto for grande
    uint8_t raio_olho = (tamanho >= 12) ? 2 : 1; 

    // 3. Desenha os Olhos
    desenharCirculoPreenchido(x - distancia_olhos, y - altura_olhos, raio_olho); // Olho Esquerdo
    desenharCirculoPreenchido(x + distancia_olhos, y - altura_olhos, raio_olho); // Olho Direito

    // 4. Desenha o Sorriso (Um arco centralizado, um pouco menor que o rosto)
    uint8_t raio_sorriso = tamanho - (tamanho / 3);
    desenharSorriso(x, y, raio_sorriso);
}

#pragma endregion
//======================================================================
