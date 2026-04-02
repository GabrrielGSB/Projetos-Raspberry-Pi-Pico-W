#include <stdio.h>             
#include "pico/stdlib.h"         
#include "arduinoStyle.h"
#include "bitDogLabRef.h"


int main() {
    stdio_init_all();

    
    while (1) {
        Joystick leituraJoy = lerJoystick();

        printf("%d // %d\n",leituraJoy.eixoX, leituraJoy.eixoY);
    }

    return 0;
}
