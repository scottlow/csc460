#include <avr/io.h>
#include <util/delay.h>
#include "os.h"
#include "kernel.h"

/** PPP and PT defined in user application. */
const unsigned char PPP[6] = {1, 200, 2, 100, 3, 100};

/** PPP and PT defined in user application. */
const unsigned int PT = 3;

SERVICE * s;
uint16_t test_int = 10;
uint16_t publish_int = 16;

void foo(){
    uint16_t val = 0; 

     for(;;){
        _delay_ms(1);
        PORTB ^= 1 << 7;
        Service_Publish(s, &publish_int);
        PORTB ^= 1 << 7;
        Task_Next();
    }
}

void bar(){
    uint16_t val = 0;
    while(1){
        //Service_Subscribe(s, &test_int);
        PORTB ^= 1 << 7;
        Task_Next(); 
    } 
}

void baz(){
    uint16_t val = 0;
    int i = 0;

    while(1){
        for(i = 0; i < 4; i++){
            PORTB ^= 1 << 5;
            _delay_ms(400);
        }
        Service_Subscribe(s, &test_int);
    } 
}

void foobar(){
    int j = 0;
    while(1) {
        for(j = 0; j < 10; j++)
            _delay_ms(300);
        Service_Publish(s, &publish_int);
    }
}

void daz(){
    uint8_t i = 0; 
    for(;;){
        for(i = 0; i < 16; i++){
            set_output(i); 
            Task_Next(); 
        }
    }
}

int r_main(){
    DDRB |= 1 << 7; 
    DDRB |= 1 << 6;             
    DDRB |= 1 << 5; 
    setup_output(); 
    PORTB = 0;
    
    s = Service_Init();

    //Task_Create(foo, 0, RR, 1);
    //Task_Create_RR(bar);
    //Task_Create_RR(baz);
    //Task_Create_RR(foobar);

    //Task_Create_System(bar, 2); 
    //Task_Create_Periodic(foo, 1, 3000, 100, 1000);
    Task_Create_Periodic(bar, 1, 25, 5, 10);
    Task_Create_Periodic(daz, 2, 100, 5, 0);


    return 0; 
}
