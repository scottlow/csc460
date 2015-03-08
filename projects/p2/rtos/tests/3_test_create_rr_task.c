/**
 * Tests the operation of Now() function. 
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

void test(){
    int i = 0; 
    while(1){
        for(i = 0; i < 16; i++){
            set_output(i); 
            _delay_ms(100); 
        }
    }
}


int r_main(){
    DDRB |= 1 << 7; 
    setup_output(); 
    PORTB = 0;
    Task_Create_RR(test, 1); 
    return 0; 
}
