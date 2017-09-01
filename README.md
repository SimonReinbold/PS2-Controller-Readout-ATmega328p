# PS2-Controller-Readout-ATmega328p
Extracting Playstation 2 Controller Data using Hardware SPI of an ATmega328p

The ATmega328p has a Hardware SPI which makes the readut pretty compact and easy.

Further Information can be found here:
http://store.curiousinventor.com/guides/PS2/
https://gist.github.com/scanlime/5042071
http://www.gamesx.com/controldata/psxcont/psxcont.htm

For testing purposes I used a LCD screen to display data. Some functions in the upladed C-Code therefore refer to another C-File which handles this LCD communication. 
