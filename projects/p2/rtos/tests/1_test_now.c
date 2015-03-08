#include <avr/io.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

void test(){
    uint16_t val = 0; 
    for(;;){
        val = Now(); 
        set_output(val>>8);
    }
}


int r_main(){
    DDRB |= 1 << 7; 
    DDRB |= 1 << 6;             
    DDRB |= 1 << 5; 
    setup_output(); 
    PORTB = 0;
    Task_Create_Periodic(test, 1, 100, 10, 0); 
    return 0; 
}
