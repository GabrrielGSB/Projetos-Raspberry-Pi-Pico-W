#include "interrupcoes.h"

#define botaoB 6
#define botaoA 5

#define numTELAS 2

volatile int telaAtualint = 1;
volatile size_t timerBotao = 0;

volatile bool botaoAiniciouWIFI;
volatile bool botaoBiniciouServerHTTP;

void gpio_callback(uint gpio, uint32_t events) {
    if ((millis() - timerBotao) < 200) return; 
    timerBotao = millis();

    if      (gpio == botaoA) {
        if (!botaoAiniciouWIFI) botaoAiniciouWIFI = true;
        else                    telaAtualint = (telaAtualint % numTELAS) + 1;
    } 
    else if (gpio == botaoB) {
        if (!botaoBiniciouServerHTTP) botaoBiniciouServerHTTP = true;
        else                    telaAtualint = ((telaAtualint + numTELAS - 2) % numTELAS) + 1;
    }
}