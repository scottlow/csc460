/**
 * @file   test014.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 * 
 * @brief  Test 014 - test the latency of creating PERIODIC and RR tasks
 * 
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 3;
const unsigned char PPP[] = {A, 1, B, 1, C, 1};

uint16_t create_start = 0;
EVENT* print_event;

void periodic_task(void)
{
    Task_Next();
}

void round_robin(void)
{
    Task_Next();
}

int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(14);

    /* just for compatiblity with trace.c, not used*/
    print_event = Event_Init();

    /* Run clock at 8MHz. */
    TCCR3B = _BV(CS30);

    create_start = TCNT3;
    Task_Create(periodic_task, A, PERIODIC, A);
    add_to_trace(TCNT3 - create_start);

    create_start = TCNT3;
    Task_Create(periodic_task, B, PERIODIC, B);
    add_to_trace(TCNT3 - create_start);

    create_start = TCNT3;
    Task_Create(periodic_task, C, PERIODIC, C);
    add_to_trace(TCNT3 - create_start);

    create_start = TCNT3;
    Task_Create(round_robin, 0, RR, 0);
    add_to_trace(TCNT3 - create_start);

    create_start = TCNT3;
    Task_Create(round_robin, 1, RR, 0);
    add_to_trace(TCNT3 - create_start);

    create_start = TCNT3;
    Task_Create(round_robin, 3, RR, 0);
    add_to_trace(TCNT3 - create_start);

    print_trace();
}
