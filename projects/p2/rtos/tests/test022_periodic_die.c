/**
 * @file   test021.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 *
 * @brief  Test 020 - Test Task_Create return values, let tasks die, create some more.
 *
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned char PPP[] = {A, 1, A, 1, B, 1};
const unsigned int PT = sizeof(PPP)/2;

extern uint16_t trace_counter;
EVENT* print_event;

void A_task(void)
{
    add_to_trace(A);
}

void B_task(void)
{
    for(;;)
    {
        add_to_trace(B);
        Task_Create(A_task, A, PERIODIC, A);
        Task_Next();
    }
}


int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(22);

    print_event = Event_Init();

    Task_Create(B_task, B, PERIODIC, B);

    Event_Wait(print_event);
    print_trace();
}
