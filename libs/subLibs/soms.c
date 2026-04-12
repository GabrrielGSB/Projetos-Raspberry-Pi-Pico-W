#include "soms.h"

#define buzzerEsquerdo 21
#define buzzerDireito  10

void somNavegacaoMovimento() {
    tone(buzzerEsquerdo, NOTE_C5, 500.0); sleep_ms(20); 
    tone(buzzerDireito, NOTE_E5, 500.0); sleep_ms(25); 

    noTone(buzzerEsquerdo); noTone(buzzerDireito);
}

void somSelecao() {
    tone(buzzerEsquerdo, NOTE_C5, 500.0); sleep_ms(60); 
    tone(buzzerDireito, NOTE_G5, 500.0); sleep_ms(120); 
    
    noTone(buzzerEsquerdo); noTone(buzzerDireito);
}