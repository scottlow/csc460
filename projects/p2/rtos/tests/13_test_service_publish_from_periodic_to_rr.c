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
        Service_Publish(s,5); 
        Task_Next(); 
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
    Task_Create_RR(test2, 1);  
    Task_Create_Periodic(test1, 2, 100, 50, 10);  
    Task_Create_Periodic(abort, 3, 100, 50, 2000);   //periodic task to abort and force trace to be sent over USART.
    return 0; 
}
