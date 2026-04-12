#include "telasDisplaySSD1306.h"

bool telaBoasVindasAconteceu = false;
bool telaConexaoPressionada  = false;

bool noWIFI = false;
bool noHTTP = false;

int telaAtual = 0;

void telaBoasVindas() {
    limparDisplay();

    for (int i=5; i<=75; i+=5){
        desenharCirculo(62, 32, i);
        atualizarDisplay();
        delay(150);
    }
    
    for (int i=75; i>=5; i-=5){
        apagarCirculo(62, 32, i);
        atualizarDisplay();
        delay(150);
    }
    
    desenharString("bem vindo", 30,10); atualizarDisplay();
    delay(500);
    desenharCarinhaSorridente(62,45,15); atualizarDisplay();
    delay(2000);

    telaBoasVindasAconteceu = true; telaAtual = telaAtualint;
}

void telaConexao() {
    noWIFI = false;
    noHTTP = false;

    if (!botaoAiniciouWIFI) {
        noWIFI = true;

        limparDisplay();

        desenharString("Pressione A",   20,18);
        desenharString("para conectar", 12,30);
        desenharString("ao WIFI",       35,42);

        atualizarDisplay();
        delay(2000);
    }

    if (!botaoBiniciouServerHTTP) {
        noHTTP = true;

        limparDisplay();

        desenharString("Pressione B",  20,18);
        desenharString("para iniciar", 16,30);
        desenharString("server HTTP",  20,42);

        atualizarDisplay();
        delay(2000);
    }

    limparDisplay();

    desenharString("CONEXAO",35,0);
    desenharSimboloWiFi(30, 50, 20, noWIFI); 
    desenharSimboloHTTP(65, 35, noHTTP);

    atualizarDisplay();

    if ((!botaoAiniciouWIFI) || (!botaoBiniciouServerHTTP))  delay(2000);
    else                                                     telaAtual = telaAtualint;
}

void telaLampadas(bool Luz1Ligada, bool Luz2Ligada, bool Luz3Ligada, bool Luz4Ligada) {
    limparDisplay();

    desenharString("lampadas",35,0);

    if (Luz1Ligada) desenharCirculoPreenchido(30,25,6);
    else            desenharCirculo(30,25,6);

    if (Luz2Ligada) desenharCirculoPreenchido(100,25,6);
    else            desenharCirculo(100,25,6);

    if (Luz3Ligada) desenharCirculoPreenchido(30,50,6);
    else            desenharCirculo(30,50,6);

    if (Luz4Ligada) desenharCirculoPreenchido(100,50,6);
    else            desenharCirculo(100,50,6);

    atualizarDisplay();

    telaAtual = telaAtualint;
}

void seletorDeTela(bool Luz1Ligada, bool Luz2Ligada, 
                   bool Luz3Ligada, bool Luz4Ligada) {

    if      ((telaAtual == 0) && !telaBoasVindasAconteceu) telaBoasVindas();
    else if (telaAtual == 1) telaConexao();
    else if (telaAtual == 2) telaLampadas(Luz1Ligada,Luz2Ligada,Luz3Ligada,Luz4Ligada);
}

