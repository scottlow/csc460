/**
 * Tests the operation of Now() function. 
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

void test(){
    while(1){
        PORTH ^= 1 << 6; 
        _delay_ms(10); 
        Task_Next(); 
    }
}

void test2(){
    while(1){
        PORTH ^= 1 << 5; 
        _delay_ms(10); 
        Task_Next(); 
    }
}


int r_main(){
    DDRB |= 1 << 7; 
    setup_output(); 
    PORTB = 0;
    Task_Create_Periodic(test, 1, 100, 50, 0);  //period of 500 ms, with 250 ms WCET. 
    Task_Create_Periodic(test2, 2, 100, 50, 50);  //period of 500 ms, with 250 ms WCET. 
    return 0; 
}
