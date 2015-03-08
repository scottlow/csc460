/**
 * Tests the operation of Now() function. 
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

void test1(){
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

void test3(){
    while(1){
        PORTH ^= 1 << 3; 
        _delay_ms(10); 
        Task_Next(); 
    }
}

void test4(){
    while(1){
        PORTH ^= 1 << 4; 
        _delay_ms(10); 
        Task_Next(); 
    }
}

void test5(){
    PORTB ^= 1 << 7; 
    _delay_ms(10); 
    Task_Next(); 
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
    Task_Create_Periodic(abort, 1, 100, 50, 1010);  //period of 500 ms, with 250 ms WCET. 
    Task_Create_Periodic(test1, 5, 100, 50, 0);  //period of 500 ms, with 250 ms WCET. 
    Task_Create_Periodic(test2, 2, 100, 50, 50);  //period of 500 ms, with 250 ms WCET. 
    Task_Create_RR(test3, 3);  //period of 500 ms, with 250 ms WCET. 
    Task_Create_RR(test4, 4);  //period of 500 ms, with 250 ms WCET. 
    return 0; 
}
