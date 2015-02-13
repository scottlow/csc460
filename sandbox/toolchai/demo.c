#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRB = 1 << 7;           /* make the LED pin an output */
    for(;;){
        char i;
        for(i = 0; i < 10; i++){
            _delay_ms(100);  /* max is 262.14 ms / F_CPU in MHz */
        }
        PORTB ^= 1 << 7;    /* toggle the LED */
    }
    return 0;               /* never reached */
}
