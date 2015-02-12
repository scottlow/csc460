/**
 * @file   test016.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 *
 * @brief  Test 016 - How long does it take for an event to be signaled from an ISR
 *
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G };
const unsigned int PT = 0;
const unsigned char PPP[] = {};

uint16_t volatile inside_interrupt = 0;

EVENT* volatile event;
EVENT* volatile print_event;

void round_robin(void)
{
    for(;;)
    {
        Event_Wait(event);
        add_to_trace(TCNT3 - inside_interrupt);
        Task_Next();
    }
}

int main(void)
{
    /* setup the test */
	uart_init();
	uart_write((uint8_t*)"\r\nSTART\r\n", 9);
    set_test(16);

    event = Event_Init();
    print_event = Event_Init();

    Task_Create(round_robin, 0, RR, 0);

    /* Run clock at 1MHz. */
    TCCR3B = _BV(CS31);
    OCR3A = 0x10;

    /* Clear flag. */
    TIFR3 = _BV(OCF3A);

    /* Set up Timer 3 Output Compare interrupt */
    TIMSK3 |= _BV(OCIE3A);

    Event_Wait(print_event);
    print_trace();
}

ISR(TIMER3_COMPA_vect)
{
    inside_interrupt = TCNT3;
    Event_Signal(event);
}
