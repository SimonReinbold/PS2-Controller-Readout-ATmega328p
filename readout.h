/***************************************************************
** Definition of Pins, Functions and Constants 
** for readout of PS2 Controllers
***************************************************************/

#define SPI_REG DDRB
#define SPI_PORT PORTB
#define SCK_PIN PB5
#define MISO_PIN PB4
#define MOSI_PIN PB3
#define SS_PIN PB2

void init_hardware( void );
unsigned char SPI_Put(char cData);
void SPI_Transmit(unsigned char *cmd, unsigned char *buff_pointer);
void HextoASCII(unsigned char *hex);