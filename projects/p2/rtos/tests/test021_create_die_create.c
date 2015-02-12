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
const unsigned int PT = 0;
const unsigned char PPP[] = {};

extern uint16_t trace_counter;
EVENT* print_event;
EVENT* test_event;    

void sys_task(void)
{
        add_to_trace(Task_GetArg());
}

int main(void)
{
    uint8_t i, j
    
    
    ;

    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(21);


    for(i = 0; i < 3; ++i)
    {
        add_to_trace(1111);
        for(j=1; j<=10; ++j)
        {
            add_to_trace(Task_Create(sys_task, j, SYSTEM, j));
        }
        
        add_to_trace(1111);

        Task_Next();
    }  


    add_to_trace(1111);   
    print_trace();
}
