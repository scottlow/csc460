/*
 * @file   test017.c
 * @author Scott Craig and Justin Tanner
 * @date   Mon Oct 29 16:19:32 2007
 *
 * @brief  Test 017 - too many periodic tasks ( designed to crash the OS and produce no output, but an error message )
 *
 */

#include "common.h"
#include "OS/os.h"
#include "uart/uart.h"
#include "trace/trace.h"

enum { A=1, B, C, D, E, F, G, H, I };
const unsigned int PT = 9;
const unsigned char PPP[] = {A, 1, B, 1, C, 1, D, 1, E, 1, F, 1, G, 1, H, 1, I, 1};

EVENT* print_event;

void do_nothing(void)
{
    for(;;)
    {
        Task_Next();
    }
}

int main(void)
{
   	Task_Create(do_nothing, A, PERIODIC, A);
   	Task_Create(do_nothing, B, PERIODIC, B);
   	Task_Create(do_nothing, C, PERIODIC, C);
   	Task_Create(do_nothing, D, PERIODIC, D);
   	Task_Create(do_nothing, E, PERIODIC, E);
   	Task_Create(do_nothing, F, PERIODIC, F);
   	Task_Create(do_nothing, G, PERIODIC, G);
   	Task_Create(do_nothing, H, PERIODIC, H);
   	Task_Create(do_nothing, I, PERIODIC, I);
}
