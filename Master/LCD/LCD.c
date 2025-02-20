/*
 * LCD.c
 *
 * Created: 2/18/2025 11:32:41 AM
 *  Author: adria
 */ 

#include "LCD.h"

// Funcion para inicializar LCD en modo 8 bits

void initLCD8bits(void) {
	// Configurar PORTD como salida
	DDRD |= 0b11111100;  // Datos D0 a D7
	PORTD = 0x00; // Limpiar puerto

	// Configurar PORTB como salida (RS y E)
	DDRB = 0xFF;
	PORTB = 0x00;

	_delay_ms(20);  // Tiempo de estabilización del LCD
	
	LCD_CMD(0x38);  // Function set: modo 8 bits, 2 líneas, 5x8 puntos
	//LCD_CMD(0b00110000);  
	_delay_ms(4);

	LCD_CMD(0x0C);  // Display ON, cursor OFF
	//LCD_CMD(0b00001100); 
	_delay_ms(1);

	LCD_CMD(0x06);  // Entry mode set: incrementa dirección, sin desplazamiento
	//LCD_CMD(0b00000110); 
	_delay_ms(1);

	LCD_CMD(0x01);  // Clear display
	_delay_ms(2);
}

void LCD_CMD(char a){
	// RS = 0; // => RS = 0 // Dato en el puerto lo va a interpretar como comando
	PORTB &= ~(1 << PORTB0);
	//EN = 1; // => E = 1;
	PORTB |= (1 << PORTB1);
	LCD_Port(a);
	_delay_ms(4);
	//EN = 0; // => E = 0;
	PORTB &= ~(1 << PORTB1);
}

// Funcion para colocar en el puerto un valor
void LCD_Port(char a){
	if (a & 1)
	// D0 = 1;
	PORTD |= (1 << PORTD2);
	else 
	// D0 = 0;
	PORTD &= ~(1 << PORTD2);
	
	if (a & 2)
	// D1 = 1;
	PORTD |= (1 << PORTD3);
	else
	// D1 = 0;
	PORTD &= ~(1 << PORTD3);
	
	if (a & 4)
	// D2 = 1;
	PORTD |= (1 << PORTD4);
	else
	// D2 = 0;
	PORTD &= ~(1 << PORTD4);
	
	if (a & 8)
	// D3 = 1;
	PORTD |= (1 << PORTD5);
	else
	// D3 = 0;
	PORTD &= ~(1 << PORTD5);
	
	if (a & 16)
	// D4 = 1;
	PORTD |= (1 << PORTD6);
	else
	// D4 = 0;
	PORTD &= ~(1 << PORTD6);
	
	if (a & 32)
	// D5 = 1;
	PORTD |= (1 << PORTD7);
	else
	// D5 = 0;
	PORTD &= ~(1 << PORTD7);
	
	if (a & 64)
	// D6 = 1;
	PORTB |= (1 << PORTB2);
	else
	// D6 = 0;
	PORTB &= ~(1 << PORTB2);
	
	if (a & 128)
	// D7 = 1;
	PORTB |= (1 << PORTB3);
	else
	// D7 = 0;
	PORTB &= ~(1 << PORTB3);
}

// Funcion para enviar un caracter
void LCD_Write_Char(char c){
	//RS = 1; // => RS = 1 // Dato en el puerto lo va a interpretar como CARACTER (Dato)
	PORTB |= (1 << PORTB0);
	//EN = 1; // => E = 1
	PORTB |= (1 << PORTB1);
	LCD_Port(c);
	_delay_ms(4);
	//EN = 0; // => E = 0;
	PORTB &= ~(1 << PORTB1);
}

// Funcion para enviar una cadena
void LCD_Write_String(const char *a){
	int i;
	for (i = 0; a[i] != '\0'; i++)
	LCD_Write_Char(a[i]);
	
}

// Desplazamaiento hacia la derecha
void LCD_Shift_Right(void){
	LCD_CMD(0x1C);
}

// Desplazamaiento hacia la izquierda
void LCD_Shift_Left(void){
	LCD_CMD(0x18);
	
}

// Establecer el cursos
void LCD_Set_Cursor(char c, char f){
	char temp;
	if (f == 1){
		temp = 0x80 + (c - 1);
		LCD_CMD(temp);
	}else if (f == 2){
		temp = 0xC0 + (c - 1);
		LCD_CMD(temp);
	}
}