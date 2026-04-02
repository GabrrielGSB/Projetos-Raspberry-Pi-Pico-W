#ifndef BIT_DOG_LAB_REF_H
#define BIT_DOG_LAB_REF_H

#include <stdbool.h>
#include <stdint.h>

#define buzzerEsquerdo 21
#define buzzerDireito  10

#define botaoB 6
#define botaoA 5

#define LED_Verde    11
#define LED_Azul     12
#define LED_Vermelho 13

#define bttJoystick 22 
#define joystickX   27
#define joystickY   26

#define matrizLED 7

typedef struct Joystick{ uint16_t eixoX, eixoY; bool botao; } Joystick;

typedef struct matrizLEDs{ uint8_t R, G, B; } matrizLEDs;

Joystick lerJoystick();



#endif