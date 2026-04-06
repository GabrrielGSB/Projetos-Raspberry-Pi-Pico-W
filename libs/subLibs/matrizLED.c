#include "matrizLED.h"
#include "ws2818b.pio.h"

struct repeating_timer timerMatrizLED;
configMatrizLed configMatrizLED;
infoMatrizLED leds[numLEDs];
PIO np_pio;
uint sm;

/**
 * @brief Inicializa a matriz de LEDs WS2812B.
 * * Configura o periférico PIO (Programmable I/O) para gerar o protocolo de timing 
 * dos LEDs e inicia um timer de hardware para atualização periódica.
 * * @param pin O número do pino GPIO da Raspberry Pi Pico onde o sinal de dados está conectado.
 * * @note Esta função tentará encontrar uma máquina de estado (SM) livre no pio0. 
 * Caso esteja cheio, ela tentará o pio1 automaticamente.
 * * @note O timer é configurado para rodar a cada 200 microssegundos, o que é adequado para atualizar os LEDs sem causar flickering perceptível.
 */
void iniciarMatrizLED(const uint pin) {
    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    // Toma posse de uma máquina PIO.
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }

    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    configMatrizLED.contador = resolucaoIntensidade;

    // Configura o timer para rodar a cada 200 microssegundos
    add_repeating_timer_us(200, preencherMatrizLED_loop, &configMatrizLED, &timerMatrizLED);

    limparMatrizLED(); 
    preencherMatrizLED();
}

/**
 * @brief Define a cor de um LED na matriz.
 * * @param indice O número da posição do LED (0 a 24).
 * @param r Intensidade da Vermelho (0-255).
 * @param g Intensidade do Verde (0-255).
 * @param b Intensidade do Azul (0-255).
 */
void definirLED_matriz(const uint8_t indice, 
                       const uint8_t r, 
                       const uint8_t g, 
                       const uint8_t b){
  leds[indice].R = r;
  leds[indice].G = g;
  leds[indice].B = b;
}

/**
 * @brief Define a intensidade de brilho de um LED específico da matriz.
 * * @param indice O índice do LED na matriz (0 a 24).
 * @param intensidade O valor da intensidade desejada (0 a "resolucaoIntensidade").
 * * @attention O valor da intensidade é automaticamente limitado entre 0 e a "resolucaoIntensidade".
 * Se o valor passado for maior que o limite, ele será truncado para o valor máximo permitido.
 */
void definirIntensidadeLED_matriz(const uint8_t indice, uint8_t intensidade){
    if (intensidade < 0) intensidade = 0;
    else if (intensidade > resolucaoIntensidade) intensidade = resolucaoIntensidade;

    configMatrizLED.intensidade[indice] = intensidade;
}

void limparMatrizLED(){
    for (uint i = 0; i < numLEDs; ++i)
    definirLED_matriz(i, 0, 0, 0);
}

void preencherMatrizLED(){
    for (uint i = 0; i < numLEDs; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(157); // sinal de RESET do datasheet.
}

bool preencherMatrizLED_loop(struct repeating_timer *t){
    configMatrizLed *config = (configMatrizLed *) t->user_data;

    for (uint i = 0; i < numLEDs; ++i) {
        if (config->contador > config->intensidade[i]) {
            pio_sm_put_blocking(np_pio, sm, 0);
            pio_sm_put_blocking(np_pio, sm, 0);
            pio_sm_put_blocking(np_pio, sm, 0);
        } else {
            pio_sm_put_blocking(np_pio, sm, leds[i].G);
            pio_sm_put_blocking(np_pio, sm, leds[i].R);
            pio_sm_put_blocking(np_pio, sm, leds[i].B);
        }   
    }

    config->contador--; 
    if (config->contador == 0) config->contador = resolucaoIntensidade; 
    
    return true;
}











