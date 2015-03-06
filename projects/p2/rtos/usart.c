#include "usart.h"

void usart_init(){
    UBRR0H = (unsigned char)(MYUBRR>>8);
    UBRR0L = (unsigned char)MYUBRR;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void usart_send(char d){
    while(!( UCSR0A & (1<<UDRE0))); 
    UDR0 = d; 
}

void usart_send_bytes(char * bytes, uint16_t n){
    uint16_t i = 0;
    for(i=0; i<n; i++){
        usart_send(bytes[i]); 
        usart_send(32); 
    }
    usart_send('\r');
    usart_send('\n');
}
