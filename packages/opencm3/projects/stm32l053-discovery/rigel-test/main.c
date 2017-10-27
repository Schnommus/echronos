#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/systick.h>

#include "rtos-rigel.h"
#include "debug.h"

/* Systick interrupt frequency, Hz */
#define SYSTICK_FREQUENCY 100

void fn_a(void)
{
    volatile int i;
    uint8_t count;

    rtos_task_start(RTOS_TASK_ID_B);

    if (rtos_task_current() != RTOS_TASK_ID_A)
    {
        debug_println("task a: wrong task??");
        for (;;)
        {
        }
    }


    debug_println("task a: taking lock");
    rtos_mutex_lock(RTOS_MUTEX_ID_TEST);
    rtos_yield();
    if (rtos_mutex_try_lock(RTOS_MUTEX_ID_TEST))
    {
        debug_println("task a: ERROR: unexpected mutex not locked.");
    }
    debug_println("task a: releasing lock");
    rtos_mutex_unlock(0);
    rtos_yield();

    for (count = 0; count < 10; count++)
    {
        debug_println("task a");
        if (count % 5 == 0)
        {
            debug_println("task a: unblocking b");
            rtos_signal_send(RTOS_TASK_ID_B, RTOS_SIGNAL_ID_TEST);
        }
        debug_println("task a: yield");
        rtos_yield();
    }

    /* Do some sleeps */
    debug_println("task a: sleep 10");
    rtos_sleep(10);
    debug_println("task a: sleep done - sleep 5");
    rtos_sleep(5);
    debug_println("task a: sleep done");


    do {
        debug_print("task a: remaining test - ");
        debug_printhex32(rtos_timer_remaining(RTOS_TIMER_ID_TEST));
        debug_print(" - remaining supervisor - ");
        debug_printhex32(rtos_timer_remaining(RTOS_TIMER_ID_SUPERVISOR));
        debug_print(" - ticks - ");
        debug_printhex32(rtos_timer_current_ticks);
        debug_println("");
        /* Sleep here for a bit as semihosting takes up all the CPU time,
         * not allowing the systick interrupt to occur */
        rtos_sleep(1);
        rtos_yield();
    } while (!rtos_timer_check_overflow(RTOS_TIMER_ID_TEST));



    if (!rtos_signal_poll(RTOS_SIGNAL_ID_TIMER))
    {
        debug_println("ERROR: couldn't poll expected timer.");
    }

    debug_println("task a: sleep for 100");
    rtos_sleep(100);

    /* Spin for a bit - force a missed ticked */
    debug_println("task a: start delay");
    for (i = 0 ; i < 50000000; i++)
    {

    }
    debug_println("task a: complete delay");
    rtos_yield();

    debug_println("task a: now waiting for ticks");
    for (;;)
    {
        rtos_signal_wait(RTOS_SIGNAL_ID_TIMER);
        debug_println("task a: timer tick");
    }
}

void fn_b(void)
{
    for (;;)
    {
        debug_println("task b: sleeping for 7");
        rtos_sleep(7);
    }
}

void systick_start(uint32_t systick_frequency, uint32_t ahb_frequency)
{
    if(!ahb_frequency)
    {
        debug_println("systick_start failed (ahb_frequency not set)");
        for(;;);
    }

    bool succeeded = systick_set_frequency(systick_frequency, ahb_frequency);

    if(!succeeded)
    {
        debug_println("systick_start failed (systick_frequency is too low)");
        for(;;);
    }

    systick_counter_enable();
    systick_interrupt_enable();
}

int main(void)
{
    libopencm3_pre_main();

    debug_println("Initializing peripherals...");

    /* TODO: Setup clocks, initialize peripherals etc. here */

    /* TODO: Set the ahb frequency based on your clock settings above
     * Usually this has the same value as the main clock frequency */

    systick_start(SYSTICK_FREQUENCY, 1000000);

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    debug_println("rtos_start returned? ...");

    for(;;);

    return 0;
}
