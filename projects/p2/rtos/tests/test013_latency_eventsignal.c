/**
 * @file   test013.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 013 - measure the latency of event signaling from a PERIODIC task to a RR
 * 
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 6;
const unsigned char PPP[] = {IDLE, 2, A, 4, IDLE, 2, A, 4, IDLE, 2, A, 4};

uint16_t signal_time = 0;
EVENT* test_event;
EVENT* print_event;

void periodic_task(void)
{
    for(;;)
    {
        Event_Signal(test_event);
        signal_time = TCNT3;
        Task_Next();
    }
}

void round_robin(void)
{
    for(;;)
    {
        Event_Wait(test_event);
   		add_to_trace(TCNT3 - signal_time);
        Task_Next();
    }
}

int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(13);

    test_event = Event_Init();
    print_event = Event_Init();    

    /* Run clock at 8MHz. */
    TCCR3B = _BV(CS30);
      
    Task_Create(periodic_task, A, PERIODIC, A);
	Task_Create(round_robin, 0, RR, 0);
	
    Event_Wait(print_event);
    print_trace();
}
