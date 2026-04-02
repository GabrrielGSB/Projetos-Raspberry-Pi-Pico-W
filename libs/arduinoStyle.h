#ifndef ARDUINO_STYLE_H
#define ARDUINO_STYLE_H

#include <stdbool.h>

#define INPUT  0
#define OUTPUT 1

#define LOW    0
#define HIGH   1

// Oitava 4 (Oitava Central)
#define NOTE_C4  262 // Dó
#define NOTE_CS4 277 // Dó Sustenido
#define NOTE_D4  294 // Ré
#define NOTE_DS4 311 // Ré Sustenido
#define NOTE_E4  330 // Mi
#define NOTE_F4  349 // Fá
#define NOTE_FS4 370 // Fá Sustenido
#define NOTE_G4  392 // Sol
#define NOTE_GS4 415 // Sol Sustenido
#define NOTE_A4  440 // Lá (Frequência de afinação padrão)
#define NOTE_AS4 466 // Lá Sustenido
#define NOTE_B4  494 // Si

// Oitava 5 (Mais aguda)
#define NOTE_C5  523 // Dó
#define NOTE_CS5 554 // Dó Sustenido
#define NOTE_D5  587 // Ré
#define NOTE_DS5 622 // Ré Sustenido
#define NOTE_E5  659 // Mi
#define NOTE_F5  698 // Fá
#define NOTE_FS5 740 // Fá Sustenido
#define NOTE_G5  784 // Sol
#define NOTE_GS5 831 // Sol Sustenido
#define NOTE_A5  880 // Lá
#define NOTE_AS5 932 // Lá Sustenido
#define NOTE_B5  988 // Si

#define REST     0 

void pinMode(int pin, bool state);

void digitalWrite(int pin, bool state);
bool digitalRead(int pin);

void analogWrite(int pin, float duty);
void analogWrite_freq(int pin, float duty, int freq);

void delay_us(int time_us);
void delay(int time_ms);
void delay_s(int time_s);

void tone(int pin, int freq, float intensidade);
void noTone(int pin);


#endif