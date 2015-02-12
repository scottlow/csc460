/**
 * @file   test010.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 010 - Many things at once, event's, RR's and Periodic's
 * 
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 2;
const unsigned char PPP[] = {A, 2, B, 1};

uint8_t big_int = 200;
EVENT* test_event;
EVENT* print_event;

void wait_task_one(void)
{
    for(;;)
    {
        add_to_trace(10);

        Event_Wait(test_event);
        
        add_to_trace(11);

        Task_Next();
    }
}

void multiple_instances(void)
{
    int arg;
    arg = Task_GetArg();
    for(;;)
    {
        add_to_trace(arg);
        Task_Next();
    }
}

void A_task(void)
{
    for(;;)
    {
        add_to_trace(2);
        
        big_int++;
        Task_Create(multiple_instances, big_int, RR, 0);
        
        Signal_And_Next(test_event);
    }
}

void B_task(void)
{
    for(;;)
    {
        add_to_trace(3);
        
        Broadcast_And_Next(test_event);
    }
}

int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(10);

    /* setup the event_one */
    test_event = Event_Init();
    print_event = Event_Init();    
      
   	Task_Create(wait_task_one, 0, RR, 0);
   	Task_Create(A_task, A, PERIODIC, A);
   	Task_Create(B_task, B, PERIODIC, B);    
	
    Event_Wait(print_event);
    print_trace();
}
