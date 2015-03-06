#include <avr/io.h>
#include "stdint.h"

#define FOSC F_CPU // Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

void usart_init(); 

void usart_send(char d); 

void usart_send_bytes(char * bytes, uint16_t n); 
