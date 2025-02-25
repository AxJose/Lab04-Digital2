/*
 * Laboratorio_4_Slave1.c
 *
 * Author : VICTOR VANEGAS
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ADC/ADC.h"
#include "I2C/I2C.h"

#define SlaveAddress 0x30

uint8_t buffer = 0;
volatile uint8_t valorADC = 0;

int main(void)
{
	DDRB |= (1<<DDB5);
	DDRB &= ~(1<<DDB5);
	ADC_init();
	I2C_Slave_Init(SlaveAddress);
	
	sei();	//Habilitar interrupciones
	
    while (1) 
    {
		valorADC = ADC_read(0);
		if(buffer == 'R'){
			PORTB |= (1<<PINB5);  // Activar el pin PB5 (LED, relay, etc.)
			buffer = 0;         // Reiniciar el buffer
		}
    }
}
ISR(TWI_vect){
	uint8_t estado;
	estado = TWSR & 0xFC;
	switch(estado){
		case 0x60:
		case 0x70:
			TWCR |= (1<< TWINT);
		break;
		case 0x80:
		case 0x90:
			buffer = TWDR;
			TWCR |= (1<< TWINT); // Se limpia la bandera
		break;
		case 0xA8:
		case 0xB8:
			TWDR = valorADC; // Cargar el dato
			TWCR = (1 << TWEN)|(1 << TWIE)|(1 << TWINT)|(1 << TWEA); // Inicia el env�o
		break;
		default: // Se libera el bus de cualquier error
		TWCR |= (1 << TWINT)|(1 << TWSTO);
		break;
		}
}
