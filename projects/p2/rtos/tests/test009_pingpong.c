/**
 * @file   test009.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 009 - Multiple events with multiple signals ( similar to ping pong )
 * 
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 1;
const unsigned char PPP[] = {IDLE, 10};

EVENT* print_event;
EVENT* event_one;
EVENT* event_two;

void wait_task_one(void)
{
    for(;;)
    {
        add_to_trace(10);

        Event_Signal(event_two);
        Event_Wait(event_one);
        
        add_to_trace(11);

        Task_Next();
    }
}

void wait_task_two(void)
{
    for(;;)
    {
        add_to_trace(20);

        Event_Signal(event_one);
        Event_Wait(event_two);

        add_to_trace(21);

        Task_Next();
    }
}

int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(9);

    /* setup the event */
    event_one = Event_Init();
    event_two = Event_Init();
    print_event = Event_Init();        
      
   	Task_Create(wait_task_one, 0, RR, 0);
   	Task_Create(wait_task_two, 0, RR, 0);
	
    Event_Wait(print_event);
    print_trace();
}
