/**
 * Tests the operation of Now() function. 
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../os.h"
#include "../kernel.h"

void test(){
    for(;;){
    	set_output(0);
        PORTB = 0;
    	if(Now()%5 == 0){
    		PORTB |= 1 << 7;
    	}else{
    		PORTB = 0;
    	}
    	if(Now()%5 == 1){
            PORTH |= 1 << 6;     		
    	}else{
            PORTH &=0xDF;     		
    	}
    	if(Now()%5 == 2){
            PORTH |= 1 << 5;     		
    	}else{
            PORTH &=0xEF;     		
    	}
    	if(Now()%5 == 3){
            PORTH |= 1 << 4;     		

    	}else{
            PORTH &=0xF7;     		
    	}
    	if(Now()%5 == 4){
            PORTH |= 1 << 3;     		
    	}else{
            PORTH &=0xFB;     		
    	}

    	Task_Next();
    }
}


int r_main(){
    DDRB |= 1 << 7; 
    setup_output(); 
    PORTB = 0;
    Task_Create_RR(test, 1); 
    return 0; 
}
