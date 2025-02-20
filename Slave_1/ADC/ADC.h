/*
 * ADC.h
 *
 *  Author:VICTOR VANEGAS
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdlib.h>

// Funci�n para inicializar el ADC
void ADC_init(void);

// Funci�n para leer un canal ADC (0-255)
uint8_t ADC_read(uint8_t canal);

#endif