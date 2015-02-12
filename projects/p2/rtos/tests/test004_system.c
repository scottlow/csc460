/**
 * @file   test004.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 004 - can schedule SYSTEM tasks in the expected order
 * 
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 0;
const unsigned char PPP[] = {};

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
    set_test(4);
      
    print_event = Event_Init();

   	Task_Create(generic_task, 1, SYSTEM, 0);
   	Task_Create(generic_task, 2, SYSTEM, 0);
   	Task_Create(generic_task, 3, SYSTEM, 0);
   	Task_Create(generic_task, 4, SYSTEM, 0);
	Task_Create(generic_task, 5, SYSTEM, 0);
	Task_Create(generic_task, 6, SYSTEM, 0);    

    Event_Wait(print_event);
    print_trace();
}
