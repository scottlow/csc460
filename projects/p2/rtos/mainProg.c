#include <avr/io.h>
#include <util/delay.h>
#include "os.h"
#include "kernel.h"

/** PPP and PT defined in user application. */
const unsigned char PPP[2] = {1, 255};

/** PPP and PT defined in user application. */
const unsigned int PT = 1;

void foo(){
    DDRB = 1 << 7;          
    for(;;){
        _delay_ms(5000);  
        PORTB ^= 1 << 7; 
        Task_Next(); 
    }
}

int r_main(){
    Task_Create(foo, 0, PERIODIC, 1); 
    return 0; 
}
