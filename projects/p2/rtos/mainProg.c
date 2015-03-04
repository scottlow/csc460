#include <avr/io.h>
#include <util/delay.h>
#include "os.h"
#include "kernel.h"

/** PPP and PT defined in user application. */
const unsigned char PPP[6] = {1, 200, 2, 100, 3, 100};

/** PPP and PT defined in user application. */
const unsigned int PT = 3;

SERVICE * s;
uint16_t test_int = 10;

void foo(){
    uint16_t val = 0; 

    while(1){
        _delay_ms(20);
        PORTB ^= 1 << 5;
    } 
}

void bar(){
    uint16_t val = 0;

    while(1){
        _delay_ms(200);
        PORTB ^= 1 << 6;
    } 
}

void baz(){
    uint16_t val = 0;
    int i = 0;

    while(1){
        for(i = 0; i < 4; i++){
            PORTB ^= 1 << 7;
            _delay_ms(400);
        }
        Service_Subscribe(s, &test_int);
    } 
}

int r_main(){
    DDRB |= 1 << 7; 
    DDRB |= 1 << 6;             
    DDRB |= 1 << 5; 
    PORTB = 0;
    
    s = Service_Init();

    //Task_Create(foo, 0, RR, 1);
    Task_Create(bar, 0, RR, 2);
    Task_Create(baz, 0, RR, 3);

    return 0; 
}
