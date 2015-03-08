/**
 * Tests whether an interrupt can successfully publish to a service. 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

SERVICE * s; 

int16_t i = 0; 

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

ISR(TIMER3_COMPA_vect){
    if(i > 15){
        i = 0;
    }
    PORTB ^= 1 << 7;  
    Service_Publish(s, i++);     
}

int r_main(){
    DDRB |= 1 << 7; 
    setup_output(); 
    PORTB = 0;
    s = Service_Init(); 

    /*Set up the timer to trigger interrupts*/
    TCCR3A = 0;
    TCCR3B = 0;

    TCCR3B |= (1 << WGM32); //set to CTC mode 4

    TCCR3B |= (1<<CS32); //set prescaler 256
    
    OCR3A = 0x22FF;   //set value for compare on timer 3 approxmiately every 0.25 seconds. 

    TIMSK3 |= (1<<OCIE3A); //enable interrupt
    TCNT3=0;               //reset timer

    /*Set up remainder of tasks*/
    Task_Create_RR(test2, 2);  
    Task_Create_Periodic(abort, 2, 100, 50, 2000);   //periodic task to abort and force trace to be sent over USART.
    return 0; 
}
