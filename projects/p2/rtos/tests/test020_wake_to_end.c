/**
 * @file   test020.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 020 - Does woken task go to end of queue?
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
EVENT* test_event;    

void sig_task(void)
{
        add_to_trace(1);
        
        Task_Next();

        add_to_trace(1);
        
        Event_Signal(test_event);

        Task_Next();

        Event_Signal(print_event);
        
}


void wait_task(void)
{
        add_to_trace(2);
        
        Event_Wait(test_event);
        
        add_to_trace(2);
}

void other_task(void)
{
        add_to_trace(3);
        Task_Next();        
        add_to_trace(3);
        Task_Next();
}

void other_other_task(void)
{
        add_to_trace(4);
        Task_Next();        
        add_to_trace(4);
        Task_Next();
}




int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(20);

    /* setup the event */
    test_event = Event_Init();
    print_event = Event_Init();    
      
   	Task_Create(sig_task, 0, RR, 0);
   	Task_Create(wait_task, 0, RR, 0);
    Task_Create(other_task, 0, RR, 0);
    Task_Create(other_other_task, 0, RR, 0);
    	
    Event_Wait(print_event);
    print_trace();
}
