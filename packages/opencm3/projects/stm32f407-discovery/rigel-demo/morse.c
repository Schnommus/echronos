#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include "rtos-rigel.h"
#include "debug.h"

#ifndef ARRAY_LEN
#define ARRAY_LEN(array) (sizeof((array))/sizeof((array)[0]))
#endif

/* Morse standard timings */
#define ELEMENT_TIME 500
#define DIT (1*ELEMENT_TIME)
#define DAH (3*ELEMENT_TIME)
#define INTRA (1*ELEMENT_TIME)
#define INTER (3*ELEMENT_TIME)
#define WORD (7*ELEMENT_TIME)

uint16_t frequency_sequence[] = {
	DIT,
	INTRA,
	DIT,
	INTRA,
	DIT,
	INTER,
	DAH,
	INTRA,
	DAH,
	INTRA,
	DAH,
	INTER,
	DIT,
	INTRA,
	DIT,
	INTRA,
	DIT,
	WORD,
};

int frequency_sel = 0;

void fn_d(void)
{
    debug_println("task d: starting flashing interrupt-driven morse code with TIM2");

    for(;;)
    {
        rtos_signal_wait(RTOS_SIGNAL_ID_TIM2);

        /*
         * Get current timer value to calculate next
         * compare register value.
         */
        uint16_t compare_time = timer_get_counter(TIM2);

        /* Calculate and set the next compare value. */
        uint16_t frequency = frequency_sequence[frequency_sel++];
        uint16_t new_time = compare_time + frequency;

        timer_set_oc_value(TIM2, TIM_OC1, new_time);
        if (frequency_sel == ARRAY_LEN(frequency_sequence))
        {
            frequency_sel = 0;
        }

        /* Toggle LED to indicate compare event. */
        gpio_toggle(GPIOD, GPIO15);
    }
}


void tim2_isr(void)
{
    if (timer_get_flag(TIM2, TIM_SR_CC1IF))
    {
        /* Clear compare interrupt flag. */
        timer_clear_flag(TIM2, TIM_SR_CC1IF);

        rtos_interrupt_event_raise(RTOS_INTERRUPT_EVENT_ID_TIM2);

    }
}
