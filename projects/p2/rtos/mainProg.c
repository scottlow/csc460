#include <avr/io.h>
#include <util/delay.h>
#include "os.h"
#include "kernel.h"

/** PPP and PT defined in user application. */
const unsigned char PPP[4] = {1, 200, 2, 100};

/** PPP and PT defined in user application. */
const unsigned int PT = 2;

void foo(){
    uint16_t val = 0; 

    while(1){
        _delay_ms(20);
        PORTB ^= 1 << 7;
    } 
}

void bar(){
    uint16_t val = 0;

    while(1){
        _delay_ms(200);
        PORTB ^= 1 << 6;
    } 
}

int r_main(){
    DDRB |= 1 << 7; 
    DDRB |= 1 << 6;             
    PORTB = 0;

    Task_Create(foo, 0, RR, 1);
    Task_Create(bar, 0, RR, 2);

    //Service_Init(); 
    return 0; 
}
