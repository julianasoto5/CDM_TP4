#ifndef PWM_H
#define PWM_H
#include "lib_delay.h"
#include <avr/io.h>
#include <avr/interrupt.h>


typedef enum {RED, GREEN, BLUE,INIT} rgb;
void PWM_Init();
void PWM_Change_DC_RGB(rgb color, int8_t new_value);
uint8_t* PWM_GetRGB();

#define PORT_RED PORTB5
#define PORT_GREEN PORTB2
#define PORT_BLUE PORTB1

#define PWM_START DDRB |= ((1<<PORT_GREEN) | (1<<PORT_RED) | (1<<PORT_BLUE))

#define PWM_ON_RED PORTB &=~(1<<PORT_RED)
#define PWM_OFF_RED PORTB |= (1<<PORT_RED)

#endif