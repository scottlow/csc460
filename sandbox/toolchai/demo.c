#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRB = 1 << 7;           /* make the LED pin an output */
    for(;;){
        char i;
        _delay_ms(5000);  /* max is 262.14 ms / F_CPU in MHz */
        PORTB ^= 1 << 7;    /* toggle the LED */
    }
    return 0;               /* never reached */
}
