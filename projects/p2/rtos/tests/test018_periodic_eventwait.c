/**
 * @file   test018.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 018 - periodic task waited on a event ( should produce no output and crash the OS )
 * 
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 1;
const unsigned char PPP[] = {A, 2};

EVENT* event;

void do_nothing(void)
{
    for(;;)
    {
        Event_Wait(event);
        Task_Next();
    }
}

int main(void)
{
    event = Event_Init();
    
   	Task_Create(do_nothing, A, PERIODIC, A);
}
