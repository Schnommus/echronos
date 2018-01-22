#include "rtos-rigel.h"
#include "debug.h"

#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_wdog.h"

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
        GPIO_PinOutClear(gpioPortA, 0);
        debug_println("task b: sleeping for 7");
        rtos_sleep(7);
        GPIO_PinOutSet(gpioPortA, 0);
    }
}

int main(void)
{
    debug_println("Initializing peripherals...");

    // Runs the Silicon Labs chip initialisation stuff, that also deals with
    // errata (implements workarounds, etc).
    CHIP_Init();

    WDOG->CTRL = WDOG_CTRL_CLKSEL_ULFRCO | WDOG_CTRL_EN | (1 << _WDOG_CTRL_PERSEL_SHIFT);

    // Switch on the clock for GPIO. Even though there's no immediately obvious
    // timing stuff going on beyond the SysTick below, it still needs to be
    // enabled for the GPIO to work.
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Sets up and enable the `SysTick_Handler' interrupt to fire once every 1ms.
    // ref: http://community.silabs.com/t5/Official-Blog-of-Silicon-Labs/Chapter-5-MCU-Clocking-Part-2-The-SysTick-Interrupt/ba-p/145297
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / SYSTICK_FREQUENCY)) {
    // Something went wrong.
    while (1);
    }

    // Set up two pins with the GPIO controller and configure them to be open
    // drain:
    //  - PA0 == green
    //  - PB7 == red
    GPIO_PinModeSet(gpioPortA, 0, gpioModeWiredAnd, 0);
    GPIO_PinModeSet(gpioPortB, 7, gpioModeWiredAnd, 0);

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    debug_println("rtos_start returned? ...");

    for(;;);

    return 0;
}
