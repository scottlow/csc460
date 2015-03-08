/**
 * Tests the operation of Now() function. 
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

SERVICE * s; 

void test1(){
    int16_t i = 0; 
    while(1){
        if(i > 15){
            i = 0; 
        }
        PORTB ^= 1 << 7; 
        Service_Publish(s,i++); 
        Task_Next(); 
    }
}

void test2(){
    int16_t x = 0; 
    while(1){
        Service_Subscribe(s, &x);     
        set_output(x); 
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
    Task_Create_Periodic(test1, 1, 100, 50, 10);  
    Task_Create_RR(test2, 2);  
    Task_Create_Periodic(abort, 2, 100, 50, 2000);   //periodic task to abort and force trace to be sent over USART.
    return 0; 
}
