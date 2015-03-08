/**
 * Tests the operation of Now() function. 
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

SERVICE * s; 

void test1(){
    while(1){
        PORTH ^= 1 << 4; 
        _delay_ms(25);
        Service_Publish(s,5); 
    }
}

void test2(){
    int16_t x = 0; 
    while(1){
        PORTH ^= 1 << 6; 
        Service_Subscribe(s, &x); 
    }
}

void abort(){
    while(1){
        OS_Abort(); 
    }
}

int r_main(){
    DDRB |= 1 << 7; 
    setup_output(); 
    PORTB = 0;
    s = Service_Init(); 
    Task_Create_RR(test2, 2);  
    Task_Create_RR(test1, 1);  
    //Task_Create_Periodic(abort, 3, 100, 50, 2000);   //periodic task to abort and force trace to be sent over USART.
    return 0; 
}
