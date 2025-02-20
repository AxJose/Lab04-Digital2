/*
 * main.c
 *
 * Created: 2/18/2025 11:32:41 AM
 *  Author: Adrián Pascual - 22045 y Victor Vanegas - 22839
 * MASTER
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C/I2C.h"
#include "LCD/LCD.h"

#define slave1 0x40
#define slave2 0x30

uint8_t direccion;
uint8_t temp;
uint8_t bufferI2C;
uint8_t valorI2C = 0;


void initPorts(void);
void refreshPORT(uint8_t valor);

int main(void)
{
	I2C_Master_Init(100000, 1); // Inicializar como Master Fscl 100kHz, prescaler 1
	//initPorts();
	
	initLCD8bits();
	LCD_CMD(0x01);
	LCD_CMD(0x06);
	_delay_ms(100);
	
	char buffer[16];
	// Mostrar en la LCD
	LCD_Set_Cursor(1, 1);
	LCD_Write_String("S1:");
	LCD_Set_Cursor(8, 1);
	LCD_Write_String("S2:");
	
    while(1)
    {
	
       /******* ESCLAVO 1 ******/
		PORTB |= (1 << PORTB5);
		
		I2C_Master_Start();
		// Escritura
		bufferI2C = slave1 << 1 & 0b11111110;
		
		temp = I2C_Master_Write(bufferI2C);
		if (temp != 1){
			I2C_Master_Stop();
		}else{
			I2C_Master_Write('R');
			I2C_Master_Stop();
		}
		_delay_ms(500);
		
		PORTB &= ~(1 << PORTB5);
		I2C_Master_Start();
		// Lectura
		bufferI2C = slave1 << 1 | 0b00000001;
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
		}else{
		
		TWCR = (1 << TWINT) | (1 << TWEN);
		while(!(TWCR & (1 << TWINT)));
		
		valorI2C = TWDR;
		
		I2C_Master_Stop();
		}
		
		sprintf(buffer, "%02d", valorI2C);
		LCD_Set_Cursor(1,2);
		LCD_Write_String(buffer);
		
		/******* ESCLAVO 2 ******/
		PORTB |= (1 << PORTB5);
		
		I2C_Master_Start();
		// Escritura
		bufferI2C = slave2 << 1 & 0b11111110;
		
		temp = I2C_Master_Write(bufferI2C);
		if (temp != 1){
			I2C_Master_Stop();
			}else{
			I2C_Master_Write('R');
			I2C_Master_Stop();
		}
		_delay_ms(500);
		
		PORTB &= ~(1 << PORTB5);
		I2C_Master_Start();
		// Lectura
		bufferI2C = slave2 << 1 | 0b00000001;
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
			}else{
			
			TWCR = (1 << TWINT) | (1 << TWEN);
			while(!(TWCR & (1 << TWINT)));
			
			valorI2C = TWDR;
			
			I2C_Master_Stop();
		}
		
		float contador = ((5.06 / 255) * valorI2C);
		
		sprintf(buffer, "%.2fV", contador);
		LCD_Set_Cursor(8,2);
		LCD_Write_String(buffer);
    }
}

void initPorts(void){
	// Salidas
	DDRB |= (1 << DDB5);
}

void refreshPORT(uint8_t valor){
	if (valor & 0b10000000){
		PORTB |= (1 << PORTB1);
		}else{
		PORTB &= ~(1 << PORTB1);
	}
	if (valor & 0b01000000){
		PORTB |= (1 << PORTB0);
		}else{
		PORTB &= ~(1 << PORTB0);
	}
	if (valor & 0b00100000){
		PORTD |= (1 << PORTD7);
		}else{
		PORTD &= ~(1 << PORTD7);
	}
	if (valor & 0b00010000){
		PORTD |= (1 << PORTD6);
		}else{
		PORTD &= ~(1 << PORTD6);
	}
	if (valor & 0b00001000){
		PORTD |= (1 << PORTD5);
		}else{
		PORTD &= ~(1 << PORTD5);
	}
	if (valor & 0b00000100){
		PORTD |= (1 << PORTD4);
		}else{
		PORTD &= ~(1 << PORTD4);
	}
	if (valor & 0b00000010){
		PORTD |= (1 << PORTD3);
		}else{
		PORTD &= ~(1 << PORTD3);
	}
	if (valor & 0b00000001){
		PORTD |= (1 << PORTD2);
		}else{
		PORTD &= ~(1 << PORTD2);
	}
}