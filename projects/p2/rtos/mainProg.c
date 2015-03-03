#include <avr/io.h>
#include <util/delay.h>
#include "os.h"
#include "kernel.h"

/** PPP and PT defined in user application. */
const unsigned char PPP[2] = {1, 255};

/** PPP and PT defined in user application. */
const unsigned int PT = 1;

void foo(){
    uint16_t val = 0; 

    DDRB = 1 << 7;          
    while(1){
        _delay_ms(500);  
        for(;;){
            val = Now()%5; 
            if(val == 3){
                PORTB ^= 1 << 7; 
                break; 
            }
        }
    }
    
    
}

int r_main(){
    Task_Create(foo, 0, PERIODIC, 1); 

    Service_Init(); 
    return 0; 
}
