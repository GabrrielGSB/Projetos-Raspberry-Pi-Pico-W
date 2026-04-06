#include "microfone.h"

uint16_t adcBuffer[numAmostras];

void configurarMicrofone() { configurarDMA_ADC(pinMicrofone); }

void lerMicrofone() { analogRead_DMA(adcBuffer, numAmostras); }
 