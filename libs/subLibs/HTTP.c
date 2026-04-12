//======================================================================
#pragma region //INCLUDES

#include "HTTP.h"
#include "arduinoStyle.h"
#include "/home/GGSB/Desktop/Pico Projects/Teste/HTTPpages/paginasHTTP.h"
#include <stdlib.h> 

#pragma endregion
//======================================================================

//======================================================================
#pragma region //DEFINES LOCAIS

#define LED_Verde 11
#define LED_Vermelho 13

#define TAMANHO_PACOTE_ENVIO 1024 // bytes

#pragma endregion
//======================================================================

//======================================================================
#pragma region  //DEFINIÇÃO DE TIPOS E VARIÁVEIS

typedef struct {
    const char *posicaoAtual; 
    size_t      restante;      
} memoriaDeEnvio;

bool luz1Ligada, luz2Ligada, luz3Ligada, luz4Ligada; 

const char *header_html = "HTTP/1.1 200 OK\r\nContent-Type:" 
                            "text/html\r\nConnection: close\r\n\r\n";                         

#pragma endregion
//======================================================================

//======================================================================
#pragma region //FUNÇÕES INTERNAS

void fecharConexao(struct tcp_pcb *tpcb) {
    const char *resposta = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
    
    tcp_write(tpcb, resposta, strlen(resposta), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    tcp_close(tpcb);
}

err_t streamingCallback(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    memoriaDeEnvio *estado = (memoriaDeEnvio *)arg;
    if (estado == NULL) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    if (estado->restante > 0) {
        size_t numBytesEnvio = (estado->restante > TAMANHO_PACOTE_ENVIO) ? TAMANHO_PACOTE_ENVIO : estado->restante;

        tcp_write(tpcb, estado->posicaoAtual, numBytesEnvio, TCP_WRITE_FLAG_COPY);

        estado->posicaoAtual += numBytesEnvio; estado->restante -= numBytesEnvio;

        tcp_output(tpcb); 
        
        return ERR_OK;
    } 
    else {
        free(estado);            
        tcp_arg(tpcb, NULL);     
        tcp_close(tpcb);        

        return ERR_OK;
    }
}

err_t httpCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        tcp_close(tpcb); 
        return ERR_OK;
    }  

    tcp_recved(tpcb, p->tot_len);

    char *requisicao = (char *)p->payload; 

    if (strstr(requisicao, "GET / ") || strstr(requisicao, "GET /main.html")) {
        memoriaDeEnvio *estado = (memoriaDeEnvio *)malloc(sizeof(memoriaDeEnvio));
        if (estado == NULL) {
            tcp_close(tpcb);
            pbuf_free(p);
            return ERR_MEM; 
        }

        estado->posicaoAtual = APLICACAO;
        estado->restante = strlen(APLICACAO);

        tcp_arg(tpcb, estado);
        tcp_sent(tpcb, streamingCallback);

        tcp_write(tpcb, header_html, strlen(header_html), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE);
        
        size_t numBytesEnvio = (estado->restante > TAMANHO_PACOTE_ENVIO) ? TAMANHO_PACOTE_ENVIO : estado->restante;
        tcp_write(tpcb, estado->posicaoAtual, numBytesEnvio, TCP_WRITE_FLAG_COPY);

        estado->posicaoAtual += numBytesEnvio; 
        estado->restante -= numBytesEnvio;

        tcp_output(tpcb); 
    }
    
    else if (strstr(requisicao, "GET /luz1/on")) {
        luz3Ligada = true;
        fecharConexao(tpcb);
    }
    else if (strstr(requisicao, "GET /luz1/off")) {
        luz3Ligada = false;
        fecharConexao(tpcb);
    }
    else if (strstr(requisicao, "GET /luz2/on")) {
        luz4Ligada = true;
        fecharConexao(tpcb);
    }
    else if (strstr(requisicao, "GET /luz2/off")) {
        luz4Ligada = false;
        fecharConexao(tpcb);
    }
    else if (strstr(requisicao, "GET /luz3/on")) {
        luz2Ligada = true;
        fecharConexao(tpcb);
    }
    else if (strstr(requisicao, "GET /luz3/off")) {
        luz2Ligada = false;
        fecharConexao(tpcb);
    }
    else if (strstr(requisicao, "GET /luz4/on")) {
        luz1Ligada = true;
        fecharConexao(tpcb);
    }
    else if (strstr(requisicao, "GET /luz4/off")) {
        luz1Ligada = false;
        fecharConexao(tpcb);
    }

    else if (strstr(requisicao, "GET /status")) {
        bool estadoPorta = true;
        char json_resposta[150];
        sprintf(json_resposta, "{\"luz1\":\"%s\",\"luz2\":\"%s\",\"luz3\":\"%s\",\"luz4\":\"%s\",\"porta\":\"%s\"}",
                luz3Ligada ? "on" : "off",
                luz4Ligada ? "on" : "off",
                luz2Ligada ? "on" : "off",
                luz1Ligada ? "on" : "off",
                estadoPorta ? "aberta" : "fechada");

        char resposta_completa[300];
        sprintf(resposta_completa, 
                "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s", 
                strlen(json_resposta), json_resposta);

        // 4. Enviamos tudo de uma vez
        tcp_write(tpcb, resposta_completa, strlen(resposta_completa), TCP_WRITE_FLAG_COPY);
        tcp_output(tpcb);
        tcp_close(tpcb);
    }    
    else {
        tcp_close(tpcb); 
    }

    pbuf_free(p); 
    return ERR_OK; 
}

err_t connectionCallback(void *arg, struct tcp_pcb *newpcb, err_t err) {  
    tcp_recv(newpcb, httpCallback); 
        return ERR_OK;
    }

#pragma endregion
//======================================================================

//======================================================================
#pragma region //FUNÇÕES EXTERNAS

void iniciarServerHTTP() {
    struct tcp_pcb *pcb = tcp_new();

    if (!pcb) return;

    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) return;

    pcb = tcp_listen(pcb);

    tcp_accept(pcb, connectionCallback);

    // printf("Servidor HTTP rodando na porta 80...\n");
    // pinMode(LED_Verde, OUTPUT); digitalWrite(LED_Verde, HIGH);
}

#pragma endregion
//======================================================================