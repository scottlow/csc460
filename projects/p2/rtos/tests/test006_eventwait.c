/**
 * @file   test006.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 *
 * @brief  Test 006 - can the RTOS wait on an event
 *
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned char PPP[] = {IDLE, 10, A, 1};
const unsigned int PT = sizeof(PPP)/2;

extern uint16_t trace_counter;
EVENT* print_event;
EVENT* test_event;

void wait_task(void)
{
    for(;;)
    {
        add_to_trace(1);

        Event_Wait(test_event);

        add_to_trace(2);

        Task_Next();
    }
}

void A_task(void)
{
    for(;;)
    {
        add_to_trace(3);

        Signal_And_Next(test_event);
    }
}

int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(6);

    /* setup the event */
    test_event = Event_Init();
    print_event = Event_Init();

   	Task_Create(wait_task, 0, RR, 0);
   	Task_Create(A_task, A, PERIODIC, A);

    Event_Wait(print_event);
    print_trace();
}
