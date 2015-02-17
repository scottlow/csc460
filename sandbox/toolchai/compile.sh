#sudo avrdude -p m2560 -c stk500 -P /dev/tty.usbmodem1451 -U demo:w:main.hex

echo "Cleaning..."
rm -f *.o
rm -f *.elf
rm -f *.hex

echo "Compiling..."

avr-gcc -Wall -O2 -DF_CPU=16000000UL -mmcu=atmega2560 -c -o demo.o demo.c

echo "Linking..."
avr-gcc -Wall -O2 -DF_CPU=16000000UL -mmcu=atmega2560 -g -o out.elf demo.o 

echo "Making ELF..."
#avr-gcc -Wall -Os -mmcu=atmega2560 -g -o out.elf out.o


echo "Making HEX..."
avr-objcopy -j .text -j .data -O ihex out.elf main.hex



