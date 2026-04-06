#include "botoes.h"

void configurarBotoes() {
    pinMode(botaoA, INPUT);
    gpio_pull_up(botaoA);
    
    pinMode(botaoB, INPUT);
    gpio_pull_up(botaoB);

    gpio_set_irq_enabled_with_callback(botaoA, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled              (botaoB, GPIO_IRQ_EDGE_FALL, true);
}

bool lerBotaoA() { return digitalRead(botaoA); }
bool lerBotaoB() { return digitalRead(botaoB); }

