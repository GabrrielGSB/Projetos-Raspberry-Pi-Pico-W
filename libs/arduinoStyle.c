#include <stdio.h>
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "arduinoStyle.h"
#include "pico/stdlib.h"


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
void delay_s (int time_s ){ sleep_ms(time_s*1000); }

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

void analogWrite(int pin, float duty) {
    int freq = 2000;
    analogWrite_freq(pin, duty, freq);
    
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