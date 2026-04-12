#include "pico/multicore.h"
#include "telasDisplaySSD1306.h"
#include "bitDogLabRef.h"
#include "interrupcoes.h"
#include "comunicacao.h"
#include "HTTP.h"


Joystick joy;
int selecaoRegiao = -1;

// =================================================================
#pragma region // ROTINA DO CORE 1 

void main1() {
    configurarMatrizLED();
    configurarJoystick();
    configurarDisplay(); 
    configurarBotoes();

    while(1) {
        logicaMovimentoTelaLampadas(&joy, &selecaoRegiao);
        logicaSelecaoTelaLampadas(&joy, &selecaoRegiao,
                                  &luz1Ligada,&luz2Ligada,
                                  &luz3Ligada,&luz4Ligada);
        
        controlarRegiao2x2MatrizLED(selecaoRegiao, 
                                    luz1Ligada,  luz2Ligada, 
                                    luz3Ligada,  luz4Ligada);
            
        seletorDeTela(luz3Ligada,luz4Ligada,luz2Ligada,luz1Ligada);

        delay(10); 
    }
}
#pragma endregion
// =================================================================

// =================================================================
#pragma region // ROTINA DO CORE 0 

int main() {
    multicore_launch_core1(main1);
    stdio_init_all();

    if (!noWIFI) iniciarWiFi();
    if (!noHTTP) iniciarServerHTTP();

    while (1) {
        if (!noWIFI) {
            cyw43_arch_poll();  
        }
        delay(100); 
    }
    
    return 0;
}

#pragma endregion
// =================================================================