/**
 * @file   test002.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 *
 * @brief  Test 002 - can the RTOS schedule PPP tasks in the correct order
 *
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned char PPP[] = {A, 5, B, 5, C, 5, D, 5, E, 5, F, 5};
const unsigned int PT = sizeof(PPP)/2;

extern uint16_t trace_counter;
EVENT* print_event;

void generic_task(void)
{
    int arg = 0;
    arg = Task_GetArg();

    for(;;)
    {
        add_to_trace(arg);

        Task_Next();
    }
}

int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(2);

    print_event = Event_Init();

   	Task_Create(generic_task, A, PERIODIC, A);
   	Task_Create(generic_task, B, PERIODIC, B);
   	Task_Create(generic_task, C, PERIODIC, C);
   	Task_Create(generic_task, D, PERIODIC, D);
	Task_Create(generic_task, E, PERIODIC, E);
	Task_Create(generic_task, F, PERIODIC, F);

    Event_Wait(print_event);
    print_trace();
}
