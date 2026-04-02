#include "bitDogLabRef.h"
#include "arduinoStyle.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"


Joystick lerJoystick(){
    Joystick valores;

    pinMode(bttJoystick, INPUT);
    gpio_pull_up(bttJoystick);

    adc_init();

    adc_gpio_init(26);
    adc_gpio_init(27);

    adc_select_input(0); valores.eixoY = adc_read();
    adc_select_input(1); valores.eixoX = adc_read();

    valores.botao = digitalRead(bttJoystick);

    return valores;
}