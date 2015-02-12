/**
 * @file   test005.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 005 - can the RTOS interleave RR and PERIODIC tasks
 * 
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 2;
const unsigned char PPP[] = {A, 1, B, 1};

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
    set_test(5);
      
    print_event = Event_Init();

   	Task_Create(generic_task, A, PERIODIC, A);
   	Task_Create(generic_task, B, PERIODIC, B);
   	Task_Create(generic_task, 7, RR, 0);
	
    Event_Wait(print_event);
    print_trace();
}
