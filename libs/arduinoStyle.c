#include "arduinoStyle.h"

#define ADC_CLOCK_DIV 96.0f

uint dma_channel;
dma_channel_config dma_cfg;

void pinMode(int pin, bool state) {
    gpio_init(pin);
    gpio_set_dir(pin, state);
}

void digitalWrite(int pin, bool state){ 
    gpio_set_function(pin, GPIO_FUNC_SIO);
    gpio_put(pin, state); 
}
bool digitalRead(int pin){ return gpio_get(pin); }

void delay_us(int time_us){ sleep_us(time_us);     }
void delay   (int time_ms){ sleep_ms(time_ms);     }
void delay_s (float time_s ){ sleep_ms(time_s*1000); }

void tone(int pin, int freq, float intensidade) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint chan = pwm_gpio_to_channel(pin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);
    
    uint32_t wrap = 1000000 / freq;

    pwm_config_set_wrap(&config, wrap);
    pwm_init(slice_num, &config, true);
    
    pwm_set_chan_level(slice_num, chan, (wrap * intensidade) / 1000);
}
void noTone(int pin) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    
    pwm_set_enabled(slice_num, false);
    
    gpio_set_function(pin, GPIO_FUNC_SIO);
    gpio_put(pin, 0); 
}


int analogRead(int pin) {
    adc_init();
    adc_gpio_init(pin);
    adc_select_input(pin - 26); 

    return adc_read();
}

void configurarDMA_ADC(int pin) {
    adc_init();
    adc_gpio_init(pin);
    adc_select_input(pin - 26); 

    adc_set_clkdiv(ADC_CLOCK_DIV);

    adc_fifo_setup(
    true,  // Habilitar FIFO
    true,  // Habilitar request de dados do DMA
    1,     // Threshold para ativar request DMA é 1 leitura do ADC
    false, // Não usar bit de erro
    false  // Não fazer downscale das amostras para 8-bits, manter 12-bits.
    );

    dma_channel = dma_claim_unused_channel(true);
    dma_cfg     = dma_channel_get_default_config(dma_channel);

    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment    (&dma_cfg, false);
    channel_config_set_write_increment   (&dma_cfg, true);
    channel_config_set_dreq              (&dma_cfg, DREQ_ADC);
}
void analogRead_DMA(uint16_t *buffer, size_t numAmostras) {
    adc_fifo_drain(); // Limpa o FIFO do ADC.
    adc_run(false); // Desliga o ADC (se estiver ligado) para configurar o DMA.

    dma_channel_configure(dma_channel, &dma_cfg,
        buffer,          // Escreve no buffer.
        &(adc_hw->fifo), // Lê do ADC.
        numAmostras,     // Faz numAmostras amostras.
        true             // Liga o DMA.
    );

    // Liga o ADC e espera acabar a leitura.
    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_channel);

    // if (!dma_channel_is_busy(dma_channel)) {
    // // Só entra aqui se o DMA já terminou!
    // }
    
    // Acabou a leitura, desliga o ADC de novo.
    adc_run(false);
}

void analogWrite(int pin, float duty) {
    analogWrite_freq(pin, duty, 2000);
    
    // if (duty < 0.0f)   duty = 0.0f;
    // if (duty > 100.0f) duty = 100.0f;

    // gpio_set_function(pin, GPIO_FUNC_PWM);
    
    // uint slice_num = pwm_gpio_to_slice_num(pin);
    // uint chan = pwm_gpio_to_channel(pin);

    // // freq = 1.9kHz
    // pwm_set_wrap(slice_num, 65535);
    // pwm_set_clkdiv(slice_num, 1.0f);

    // uint32_t nivel_pwm = (uint32_t)((duty / 100.0f) * 65535.0f);

    // pwm_set_chan_level(slice_num, chan, nivel_pwm);
    // pwm_set_enabled(slice_num, true);
}
void analogWrite_freq(int pin, float duty, int freq){
    if (duty < 0.0f)   duty = 0.0f;
    if (duty > 100.0f) duty = 100.0f;

    if (freq < 8) freq = 8;

    gpio_set_function(pin, GPIO_FUNC_PWM);
    
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint chan      = pwm_gpio_to_channel(pin);

    uint32_t clock_freq = clock_get_hz(clk_sys);
    
    uint32_t ciclos_totais = clock_freq / freq;
    uint32_t wrap;
    float clkdiv;

    if (ciclos_totais < 65535) {
        wrap = ciclos_totais;
        clkdiv = 1.0f;
    } else {
        wrap = 65535;
        clkdiv = (float)ciclos_totais / 65535.0f;
    }

    pwm_set_wrap(slice_num, wrap);
    pwm_set_clkdiv(slice_num, clkdiv);

    uint32_t nivel_pwm = (uint32_t)((duty / 100.0f) * wrap);

    pwm_set_chan_level(slice_num, chan, nivel_pwm);
    pwm_set_enabled(slice_num, true);
}

uint32_t millis(){ return to_ms_since_boot(get_absolute_time()); }
uint32_t micros(){ return time_us_32(); }

    long map(long x, long in_min, long in_max, long out_min, long out_max) {
        long valor = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        if (valor > out_max) return out_max;
        if (valor < out_min) return out_min;
        return valor;
    }

