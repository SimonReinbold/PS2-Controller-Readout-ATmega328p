#!/bin/bash
avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p  lcd-routines.c readout.c -o run.o
#avr-gcc -mmcu=atmega328p $1.o -o $1
avr-objcopy -O ihex -R .eeprom run.o run.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:run.hex
