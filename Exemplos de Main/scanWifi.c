#include "bitDogLabRef.h"
#include "interrupcoes.h"
#include "comunicacao.h"
#include "HTTP.h"
// #include "telasDisplaySSD1306.h"
//======================================================================
#pragma region // FUNÇÕES DE SCAN WI-FI E MEMÓRIA

#define MAX_REDES_SALVAS 30 

// 1. Criamos um "Molde" para guardar todas as informações de uma rede, não só o nome
typedef struct {
    char ssid[33];
    int rssi;
    uint8_t channel;
    uint8_t bssid[6];
} RedeWiFi;

// 2. O nosso "Caderninho" agora é um array dessas Structs
RedeWiFi redes_encontradas[MAX_REDES_SALVAS];
int num_redes_encontradas = 0; 


// 3. O Callback agora trabalha em silêncio, apenas preenchendo a memória
static int callback_resultado_scan(void *env, const cyw43_ev_scan_result_t *resultado) {
    
    char ssid_limpo[33];
    int tamanho_ssid = resultado->ssid_len;
    if (tamanho_ssid > 32) tamanho_ssid = 32;
    
    memcpy(ssid_limpo, resultado->ssid, tamanho_ssid);
    ssid_limpo[tamanho_ssid] = '\0'; 

    if (tamanho_ssid == 0) return 0; // Ignora redes ocultas

    // Verifica se a rede já está na nossa struct
    for (int i = 0; i < num_redes_encontradas; i++) {
        if (strcmp(redes_encontradas[i].ssid, ssid_limpo) == 0) {
            return 0; // Rede já anotada, ignora e sai.
        }
    }

    // Se for uma rede nova, guarda TODOS os dados nela na memória global
    if (num_redes_encontradas < MAX_REDES_SALVAS) {
        strcpy(redes_encontradas[num_redes_encontradas].ssid, ssid_limpo);
        redes_encontradas[num_redes_encontradas].rssi = resultado->rssi;
        redes_encontradas[num_redes_encontradas].channel = resultado->channel;
        memcpy(redes_encontradas[num_redes_encontradas].bssid, resultado->bssid, 6);
        
        num_redes_encontradas++;
    }

    return 0; 
}

// 4. A função principal controla o tempo e lê a memória no final
void escanearRedesDisponiveis() {
    printf("\n--- INICIANDO VARREDURA DE REDES WI-FI ---\n");

    num_redes_encontradas = 0; // Zera a memória para a nova busca

    cyw43_wifi_scan_options_t opcoes_scan = {0};

    int erro = cyw43_wifi_scan(&cyw43_state, &opcoes_scan, NULL, callback_resultado_scan);

    if (erro == 0) {
        printf("Scanner rodando no chip de radio... Aguarde...\n");
        
        // --- O PULO DO GATO ---
        // O Pico fica preso neste loop 'while' até o hardware avisar que o scan terminou.
        // Isso leva cerca de 2 a 3 segundos.
        while (cyw43_wifi_scan_active(&cyw43_state)) {
            sleep_ms(100); 
        }

        // Quando o loop acima termina, significa que a memória está cheia e pronta.
        // Agora nós lemos os dados diretamente do nosso array global!
        printf("\n--- RESULTADO FINAL DO SCAN ---\n");
        printf("Foram encontradas %d redes unicas no ar:\n\n", num_redes_encontradas);
        
        for (int i = 0; i < num_redes_encontradas; i++) {
            printf("[%02d] Rede: %-32s | Sinal: %4d dBm | Canal: %2d | MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                   i + 1,
                   redes_encontradas[i].ssid,
                   redes_encontradas[i].rssi,
                   redes_encontradas[i].channel,
                   redes_encontradas[i].bssid[0], redes_encontradas[i].bssid[1], redes_encontradas[i].bssid[2],
                   redes_encontradas[i].bssid[3], redes_encontradas[i].bssid[4], redes_encontradas[i].bssid[5]);
        }
        printf("---------------------------------------\n");

    } else {
        printf("Falha ao iniciar o scanner de Wi-Fi. Codigo de erro: %d\n", erro);
    }
}

#pragma endregion
//======================================================================
int main() {
    stdio_init_all();

    delay_s(10);

    iniciarWiFi();
   

    while (1) {
        cyw43_arch_poll();  
        escanearRedesDisponiveis();
        sleep_ms(1000);
    }
    
    return 0;
}
