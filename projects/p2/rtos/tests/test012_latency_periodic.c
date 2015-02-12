/**
 * @file   test012.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 012 - test the latency of periodic tasks
 * 
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 10;
const unsigned char PPP[] = {A, 1, B, 1, A, 1, B, 1, A, 1, B, 1, A, 1, B, 1, A, 1, B, 1};

uint16_t last = 0;
uint16_t time = 0;
EVENT* print_event;

void periodic_task(void)
{
    for(;;)
    {
        time = TCNT3;
        
		add_to_trace(time - last);

        last = time;

        Task_Next();
    }
}

int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(12);

    print_event = Event_Init();
    
    /* Run clock at 1MHz. */
    TCCR3B = _BV(CS31);
      
    Task_Create(periodic_task, A, PERIODIC, A);
	Task_Create(periodic_task, B, PERIODIC, B);
    
    Event_Wait(print_event);
    print_trace();
}
