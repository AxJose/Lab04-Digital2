/*
 * Laboratorio_4_Slave.c
 *
 * Author : VICTOR VANEGAS
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C/I2C.h"

#define SlaveAddress 0x40

volatile uint8_t contador = 0;
uint8_t buffer = 0;

//*********************************************************************************
// Prototipos de funcion
//*********************************************************************************
void Ports_init(void);

//*********************************************************************************
// Codigo principal
//*********************************************************************************
int main(void)
{
	Ports_init();
	DDRB |= (1<<DDB5);
	DDRB &= ~(1<<DDB5);
	I2C_Slave_Init(SlaveAddress);
	
    while (1) 
    {
		PORTB = contador;
		if(buffer == 'R'){
			PORTB |= (1<<PINB5);  // Activar el pin PB5 (LED, relay, etc.)
			buffer = 0;         // Reiniciar el buffer
		}
    }
}

//*********************************************************************************
// Sub Rutinas
//*********************************************************************************
void Ports_init(void){
	// Marcar como entrada el puerto B
	DDRB |= (1<< PORTB0)|(1<< PORTB1)|(1<< PORTB2)|(1<< PORTB3)|(1<< PORTB4);
	// Los ponemos el puerto en 0
	PORTB = 0x00;
	// Marcar como salida el puerto C
	DDRC &= ~(1<< PORTC0)|(1<< PORTC1);
	// Ponemos los botones como pullups
	PORTC |= (1<<DDC0)|(1<<DDC1);
	// Habilitar interrupcion PCIE0
	PCICR |= (1<<PCIE1);
	// Seleccionamos el PC0 y PC1 que sea la interrucion
	PCMSK1 |= (1<<PCINT8)|(1<<PCINT9);
	
	// Activar las interrupciones
	sei();
}

//*********************************************************************************
// ISR
//*********************************************************************************
ISR(PCINT1_vect){
	if(PINC & (1<<PORTC0)){
		// Si no esta presionado, que no haga nada
		}else{
			contador++;
			_delay_ms(150);
			if(contador == 16){
				contador = 15;
			}
		}
	
	if(PINC & (1<<PORTC1)){
		// Si no esta presionado, que no haga nada
		}else{
			contador--;
			_delay_ms(150);
			if(contador == 255){
				contador = 0;
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
		TWDR = contador; // Cargar el dato
		TWCR = (1 << TWEN)|(1 << TWIE)|(1 << TWINT)|(1 << TWEA); // Inicia el envío
		break;
		default: // Se libera el bus de cualquier error
		TWCR |= (1 << TWINT)|(1 << TWSTO);
		break;
	}
}