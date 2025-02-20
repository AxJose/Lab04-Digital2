/*
 * I2C.c
 *
 *  Author: VICTOR VANEGAS
 */ 

#include "I2C.h"
#define F_CPU 16000000

// Funcion para inicializar I2C Maestro

void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler){
	DDRC &= ~((1 << DDC4) | (1 << DDC5)); // Pines de I2C como entradas
	
	switch(Prescaler){
		case 1:
			TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
		break;
		case 4:
			TWSR &= ~(1 << TWPS1);
			TWSR |= (1 << TWPS0);
		break;
		case 16:
			TWSR &= ~(1 << TWPS0);
			TWSR |= (1 << TWPS1);
		case 64:
			TWSR |= (1 << TWPS1) | (1 << TWPS0);
		break;
		default:
			TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
			Prescaler = 1;
			break;
	}
	TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaler); /* must be > 10 for stable operation */
	TWCR |= (1 << TWEN);
}

// Funcion de inicio de la comunicacion I2C
void I2C_Master_Start(void){
	uint8_t estado;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Iniciar condicion de start
	while(!(TWCR & (1 << TWINT))); // Esperar a que termine la flag TWINT
}

void I2C_Master_Stop(void){
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO); // Secuencia de parada
	
}	

// Funcion de transmision de datos del maestro al esclavo
uint8_t I2C_Master_Write(uint8_t dato){
	uint8_t estado;
	
	TWDR = dato; // Cargar el dato
	TWCR = (1 << TWEN) | (1 << TWINT); // Inicia el envio
	
	while(!(TWCR & (1 << TWINT))); // Esperar al flag
	estado = TWSR & 0xF8; // Verificar el estado
	
	// Verificar si se transmitio una SLA + W con ACK, SLA + R con ACK, o un Dato con ACK
	if(estado == 0x18 || estado == 0x28 || estado == 0x40){
		return 1;
	}else{
		return estado;
	}
}

// Funcion de recepcion de datos enviados por el esclavo al maestro
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	if(ack){
		TWCR |= (1 << TWEA); // Lectura con ACK
	}else{
		TWCR &= ~(1 << TWEA); // Sin ACK
	}
	
	TWCR |= (1 << TWINT); // Inicia la lectura
	while(!(TWCR & (1 << TWINT))); // Esperar al flag
	estado = TWSR & 0xF8; // Verificar estado
	// Verificar dato leido con ACK o sin ACK
	if(estado == 0x58 || estado == 0x50){
		*buffer = TWDR;
		return 1;
	}else{
		return estado;
	}
}

void I2C_Slave_Init(uint8_t address){
	DDRC &= ~((1 << DDC4) | (1 << DDC5)); // Pines I2C como entradas
	
	TWAR = address << 1; // Se asigna la direccion que entra
	
	// Se habilia la interfaz, ACK automatico, se habuilia la ISR
	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
}