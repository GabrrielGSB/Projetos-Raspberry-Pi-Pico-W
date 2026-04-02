#include "../bitDogLabRef.h"
#include "matrizLED.h"

void iniciarMatrizLED(uint pin) {
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
}

void definirMatrizLED(const uint indice, 
                      const uint8_t r, 
                      const uint8_t g, 
                      const uint8_t b){
  leds[indice].R = r;
  leds[indice].G = g;
  leds[indice].B = b;
}

void limparMatrizLED(){
    for (uint i = 0; i < numLEDs; ++i)
    definirMatrizLED(i, 0, 0, 0);
}

void escreverMatrizLED(){
      // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
    for (uint i = 0; i < numLEDs; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}











