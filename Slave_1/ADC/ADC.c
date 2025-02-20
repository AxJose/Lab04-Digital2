/*
 * ADC.c
 *
 *  Author: VICTOR VANEGAS
 */ 
#define F_CPU 16000000UL
#include "ADC.h"
#include <util/delay.h>

void ADC_init(void) {
	ADMUX = 0;
	// Configuracion de los pines PC0 y PC1 como entrada
	DDRC &= ~(1 << PORTC0)|(1 << PORTC1);
	ADMUX = (1 << REFS0); // Voltaje de referencia AVcc (5V)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC y preescaler de 128 (125kHz)
}

uint8_t ADC_read(uint8_t canal) {
	canal &= 0x0F;  // Asegurar que esté en el rango correcto (0-15)
	ADMUX = (ADMUX & 0xF0) | canal;  // Seleccionar canal ADC

	ADCSRA |= (1 << ADSC);  // Iniciar conversión
	while (ADCSRA & (1 << ADSC));  // Esperar a que termine

	return (ADC << 2);
}