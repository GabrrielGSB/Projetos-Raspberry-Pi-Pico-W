#include "comunicacao.h"

void configurarI2C() {
    i2c_init(i2c1, 400000); 
    gpio_set_function(pinSDA, GPIO_FUNC_I2C); 
    gpio_set_function(pinSCL, GPIO_FUNC_I2C); 
    gpio_pull_up(pinSDA); 
    gpio_pull_up(pinSCL); 
}