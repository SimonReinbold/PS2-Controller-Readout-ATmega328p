/***************************************************************
**
** PS2 Controller readout functions
** 
***************************************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "lcd-routines.h"
#include "readout.h"

void main(){
	lcd_init();

	init_hardware();
	
	unsigned char redMode_cmd[9] = {0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char redMode_ModeCheck = 0x73;
	unsigned char filler_byte = 0x5A;

	unsigned char buffer[9] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	unsigned char *buff_pointer;
	unsigned char *cmd_pointer;

	unsigned char n_bytes;

	while(1){
		// Reset variables for transmission
		buff_pointer = &buffer[0];
		cmd_pointer = &redMode_cmd[0];

		// Send request and read response
		SPI_Transmit(redMode_cmd,buff_pointer);

		// Analogue Controller in Red Mode

	 	//    BYTE    CMND    DATA

		//     01     0x01    idle
		//     02     0x42    0x73
		//     03     idle    0x5A    Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
		//     04     idle    data    SLCT JOYR JOYL STRT UP   RGHT DOWN LEFT
		//     05     idle    data    L2   R2   L1   R1   /\   O    X    |_|
		//     06     idle    data    Right Joy 0x00 = Left  0xFF = Right
		//     07     idle    data    Right Joy 0x00 = Up    0xFF = Down
		//     08     idle    data    Left Joy  0x00 = Left  0xFF = Right
		//     09     idle    data    Left Joy  0x00 = Up    0xFF = Down

		// Display Data on LCD
		// First Line command
		lcd_setcursor( 0, 1 );
		n_bytes = sizeof(redMode_cmd);
		for(int i=0; i<n_bytes; i++){
			HextoASCII(cmd_pointer);
			cmd_pointer++;
		}

		// Second line response
		lcd_setcursor( 0, 2 );
		n_bytes = sizeof(buffer);
		for(int i=0; i<n_bytes; i++){
			HextoASCII(buff_pointer);
			buff_pointer++;
		}

		// Test Data Right Joystick
		//if(*buffer[5] > 0x80){
		//	lcd_setcursor( 0, 2 );
		//	lcd_string("Right");
		//}
		//if(*buffer[5] = 0x80){
		//	lcd_setcursor( 0, 2 );
		//	lcd_string("Center");
		//}
		//if(*buffer[5] < 0x80){
		//	lcd_setcursor( 0, 2 );
		//	lcd_string("Left");
		//}
	}
}

void init_hardware(){
	// Inport
	// SPI_REG |= (1<<MISO_PIN); -> automatically configured according to Atmel
	
	// Outports
	SPI_REG &= ~( (1<<SCK_PIN) & (1<<MOSI_PIN) & (1<<SS_PIN) );

	// Set Slave Select pin to high
	SPI_PORT |= (1<<SS_PIN);

	// Set Master Mode in SPI Control Register
	// Enable SPI
	// LSB first
	// Clock Speed ATmega328p = 16MHz, desired SCK speed 250kHz -> Prescaler = 64
	// CLOCK Leading Edge is falling
	// Sample on Trailing Edge
	SPCR |= (1<<MSTR) & (1<<SPE) & (1<<DORD) & (1<<SPR1) & (1<<CPOL) & (1<<CPHA);

	// Interrupts needed ??
	// sei();
	// SPCR0 |= SPIE0;
}

unsigned char SPI_Put(char cData){
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while((SPSR & (1<<SPIF)));

	return SPDR;
}

void SPI_Transmit(unsigned char *cmd, unsigned char *buff_pointer){	
	for(signed int i=0; i<9; i++){
		*buff_pointer = SPI_Put(*cmd++);
		// Next buffer byte
		buff_pointer++;
	}
}

void HextoASCII(unsigned char *hex)
{
	unsigned char temp_L;           // dummy variable that will hold LSBs of Hex number
	unsigned char temp_H;           // dummy variable that will hold MSBs of Hex number

	temp_H = *hex & 0xF0;           // Obtain the upper 4 bits (MSBs) of hex number
	temp_H = temp_H >> 4;           //
	if(temp_H >9)                   // Check if the number is a letter
		temp_H += 0x37;             // Convert the number to a letter in ASCII
	else
		temp_H += 0x30;             // Convert the number to ASCII number

	lcd_data(temp_H);           // Display the number
	temp_L = *hex & 0x0F;           // Obtain the lower 4 bits (LSBs) of hex number
	if(temp_L >9)                   // Check if the the number is a letter
		temp_L += 0x37;             // Convert the number to a letter in ASCII
	else
		temp_L += 0x30;             // Convert the number to ASCII number

	lcd_data(temp_L);           // Display the number
}