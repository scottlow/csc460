/**
 * Tests the operation of Now() function. 
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

SERVICE * s; 

void test1(){
    PORTH ^= 1 << 5; 
    Service_Publish(s,5); 
}

void test3(){
    while(1){
        PORTH ^= 1 << 4; 
        Task_Create_System(test1, 1);
        Task_Next(); 
    }
}

void test2(){
    int16_t x = 0; 
    Service_Subscribe(s, &x); 
    while(1){
        PORTH ^= 1 << 6; 
        Task_Next(); 
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
    Task_Create_Periodic(test3, 3, 100, 50, 100);   //periodic task to create a system task. 
    Task_Create_Periodic(abort, 4, 100, 50, 2000);   //periodic task to abort and force trace to be sent over USART.
    return 0; 
}
