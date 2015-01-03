/* Name: main.c
 * Author: Alex Feinland
 * Description: Toggles an LED on and off every 500ms (half second)for an 8mhz fCLK_IO.
 */

#include <avr/io.h>
#include <avr/interrupt.h>


/* This function configures Timer/Counter 1 on the ATMega328 to call an ISR every 500ms.
 * Specifically, we are configuring the timer for Mode 4, which increments from 0
 * up to TOP (OCR1A, in this case), get set back to 0, and starts the whole process over again.
 * When the value in TCNT1 equals the value in OCR1A, the Timer/Counter1 Compare Match A interrupt is triggered.
 *
 * NOTE: We are assuming that the chip (specifically fCLK_IO) is running at 8mhz.
 */
void timer_init(void)
{
    /* Enable global interrupts. This allows the ISR to be called.
     *
     * NOTE: "SREG |= 0x80;" will also work. 
     */
    sei();

    /* OCR1A holds the TOP value for TCNT1 in Mode 4. This is the value that determines the resolution of the timer.
     * 1250 is the number of times the timer will increment in 100ms (our desired period),
     * with a CPU frequency of 8mhz and prescaler of 64. The value for OCR1A can be calculated with the equation:
     * (fCLK_IO * desired_period) / prescale_value. In our case, the equation would be 8000000 * 0.5 / 64 = 62500.
     */
    OCR1A = 62500;

    /* The Output Compare Interrupt Enable 1A flag enables the Compare Match A interrupt for Timer/Counter 1.
     * This flag must be set to enable the TIMER1_COMPA_vect ISR.
     */
    TIMSK1 = (1 << OCIE1A);

    /* The CS11 and CS10 (Clock Select) bits set the prescale value to 64. Setting these bits starts the timer.
     * (With a prescale value of 64, TCNT1 will increment every 64 clock cycles, or,
     * with a CPU frequency of 8mhz, 125000 times per second.)
     * 
     * The WGM12 (Waveform Generation Mode)bits sets the timer to Mode 4.
     * See the function comment above or the ATMega328's datasheet for more info.
     */
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
}




ISR(TIMER1_COMPA_vect)
{
    PORTD ^= (1 << PD2); // Toggle PD2, or 0x04 on PORTD.
}


int main(void)
{

    timer_init();

    DDRD = (1 << PD2);    // Set pin PD2 as an output.
    PORTD = 0x00;   // Turn off the LED initially.

    while(1)
    {
        // Do Nothing.
    }

    return 0;   /* never reached */
}
