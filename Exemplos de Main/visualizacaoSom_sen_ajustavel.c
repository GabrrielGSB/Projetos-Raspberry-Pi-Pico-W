#include "bitDogLabRef.h"
#include "interrupcoes.h"

int main() {
    stdio_init_all();
    configurarBotoes();
    configurarDisplay();
    configurarMicrofone();

    uint res;
    uint in_min, in_max;

    const char* str = "sen ";


    while (1) {
        limparDisplay();

        res    = sensibilidadeSom * 100;
        in_min = 0 + res;
        in_max = 4095 - res;

        desenharString(str, 0, 0);
        
        for (int i = 0; i < sensibilidadeSom; i++) {
            desenharRetangulo(32, 0, 32 + i*5, 8); 
        }
        // desenharString(numToStr(in_min), 0, 0); 
        // desenharString(numToStr(in_max), 40, 0); 

        lerMicrofone();
        
        for (int x = 0; x < numAmostras - 1; x++) {
            int y = (32 + map(adcBuffer[x], in_min, in_max, -32, 32));
            desenharPixel(x, y);
        }

        atualizarDisplay();

        
    }
    
    return 0;
}
