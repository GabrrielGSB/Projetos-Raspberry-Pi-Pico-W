#include "comunicacao.h"

void configurarI2C() {
    i2c_init(i2c1, 400000); 
    gpio_set_function(pinSDA, GPIO_FUNC_I2C); 
    gpio_set_function(pinSCL, GPIO_FUNC_I2C); 
    gpio_pull_up(pinSDA); 
    gpio_pull_up(pinSCL); 
}

bool iniciarWiFi() {
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return false;
    }
    else {
        printf("Conectado!\n");

        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("Endereço IP %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }
}

