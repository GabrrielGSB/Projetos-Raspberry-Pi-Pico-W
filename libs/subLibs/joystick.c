#include "joystick.h"

size_t timerBotaoJoy = 0;
bool acaoJoyRealizada = false;
bool somMovimentoTocado = false;

void configurarJoystick() {
    pinMode(bttJoystick, INPUT);
    gpio_pull_up(bttJoystick);

    adc_init();

    adc_gpio_init(26);
    adc_gpio_init(27);
}

void lerJoystick(Joystick *joy) {
    adc_select_input(0); joy->eixoY = adc_read();
    adc_select_input(1); joy->eixoX = adc_read();

    joy->botao = digitalRead(bttJoystick);
}

void logicaMovimentoTelaLampadas(Joystick *joy, int *estadoSelecao) {
    lerJoystick(joy);

    if (acaoJoyRealizada && !somMovimentoTocado) {
        somMovimentoTocado = true;
        somNavegacaoMovimento();
    }

    if ((joy->eixoX > 1900 && joy->eixoX < 2200) && 
        (joy->eixoY > 1900 && joy->eixoY < 2200)) {
        acaoJoyRealizada = false;
        somMovimentoTocado = false;
    }

    if (!acaoJoyRealizada) {
        if      (*estadoSelecao == 1) {
            if      (joy->eixoX > DIREITA)  { *estadoSelecao =-1; acaoJoyRealizada = true; }
            else if (joy->eixoX < ESQUERDA) { *estadoSelecao = 2; acaoJoyRealizada = true; }
            else if (joy->eixoY > CIMA)     { *estadoSelecao = 4; acaoJoyRealizada = true; }
            else if (joy->eixoY < BAIXO)    { *estadoSelecao =-1; acaoJoyRealizada = true; }
        } 
        else if (*estadoSelecao == 2) {
            if      (joy->eixoX > DIREITA)  { *estadoSelecao = 1; acaoJoyRealizada = true; }
            else if (joy->eixoX < ESQUERDA) { *estadoSelecao =-2; acaoJoyRealizada = true; }
            else if (joy->eixoY > CIMA)     { *estadoSelecao = 3; acaoJoyRealizada = true; }
            else if (joy->eixoY < BAIXO)    { *estadoSelecao =-2; acaoJoyRealizada = true; }
        } 
        else if (*estadoSelecao == 3) {
            if      (joy->eixoX > DIREITA)  { *estadoSelecao = 4; acaoJoyRealizada = true; }
            else if (joy->eixoX < ESQUERDA) { *estadoSelecao =-3; acaoJoyRealizada = true; }
            else if (joy->eixoY > CIMA)     { *estadoSelecao =-3; acaoJoyRealizada = true; }
            else if (joy->eixoY < BAIXO)    { *estadoSelecao = 2; acaoJoyRealizada = true; }
        } 
        else if (*estadoSelecao == 4) {
            if      (joy->eixoX > DIREITA)  { *estadoSelecao =-4; acaoJoyRealizada = true; }
            else if (joy->eixoX < ESQUERDA) { *estadoSelecao = 3; acaoJoyRealizada = true; }
            else if (joy->eixoY > CIMA)     { *estadoSelecao =-4; acaoJoyRealizada = true; }
            else if (joy->eixoY < BAIXO)    { *estadoSelecao = 1; acaoJoyRealizada = true; }
        }
        else if (*estadoSelecao == -1) {
            if      (joy->eixoX > DIREITA)  { *estadoSelecao =-1; acaoJoyRealizada = true; }
            else if (joy->eixoX < ESQUERDA) { *estadoSelecao = 1; acaoJoyRealizada = true; }
            else if (joy->eixoY > CIMA)     { *estadoSelecao = 1; acaoJoyRealizada = true; }
            else if (joy->eixoY < BAIXO)    { *estadoSelecao =-1; acaoJoyRealizada = true; }
        } 
        else if (*estadoSelecao == -2) {
            if      (joy->eixoX > DIREITA)  { *estadoSelecao = 2; acaoJoyRealizada = true; }
            else if (joy->eixoX < ESQUERDA) { *estadoSelecao =-2; acaoJoyRealizada = true; }
            else if (joy->eixoY > CIMA)     { *estadoSelecao = 2; acaoJoyRealizada = true; }
            else if (joy->eixoY < BAIXO)    { *estadoSelecao =-2; acaoJoyRealizada = true; }
        } 
        else if (*estadoSelecao == -3) {
            if      (joy->eixoX > DIREITA)  { *estadoSelecao = 3; acaoJoyRealizada = true; }
            else if (joy->eixoX < ESQUERDA) { *estadoSelecao =-3; acaoJoyRealizada = true; }
            else if (joy->eixoY > CIMA)     { *estadoSelecao =-3; acaoJoyRealizada = true; }
            else if (joy->eixoY < BAIXO)    { *estadoSelecao = 3; acaoJoyRealizada = true; }
        } 
        else if (*estadoSelecao == -4) {
            if      (joy->eixoX > DIREITA)  { *estadoSelecao =-4; acaoJoyRealizada = true; }
            else if (joy->eixoX < ESQUERDA) { *estadoSelecao = 4; acaoJoyRealizada = true; }
            else if (joy->eixoY > CIMA)     { *estadoSelecao =-4; acaoJoyRealizada = true; }
            else if (joy->eixoY < BAIXO)    { *estadoSelecao = 4; acaoJoyRealizada = true; }
        }
    }
}

void logicaSelecaoTelaLampadas(Joystick *joy, int *estadoSelecao,
                              bool *luz1Ligada, bool *luz2Ligada,
                              bool *luz3Ligada, bool *luz4Ligada) {

    if ((millis() - timerBotaoJoy) > 200) {
        timerBotaoJoy = millis();

        lerJoystick(joy);

        if (joy->botao == 0) {
            somSelecao();
            if      (*estadoSelecao == 1) *luz1Ligada = !*luz1Ligada;
            else if (*estadoSelecao == 2) *luz2Ligada = !*luz2Ligada;
            else if (*estadoSelecao == 3) *luz3Ligada = !*luz3Ligada;
            else if (*estadoSelecao == 4) *luz4Ligada = !*luz4Ligada;
        }
        
    }

}