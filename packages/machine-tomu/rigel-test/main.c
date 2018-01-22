#include "rtos-rigel.h"
#include "debug.h"

#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_wdog.h"

/* Systick interrupt frequency, Hz */
#define SYSTICK_FREQUENCY 1000

void fn_a(void)
{
    for (;;)
    {
        debug_println("task A: set led A");
        rtos_sleep(50);
        GPIO_PinOutSet(gpioPortA, 0);
        debug_println("task A: clear led A");
        rtos_sleep(50);
        GPIO_PinOutClear(gpioPortA, 0);
    }
}

void fn_b(void)
{
    for (;;)
    {
        debug_println("task B: clear led B");
        rtos_sleep(50);
        GPIO_PinOutClear(gpioPortB, 7);
        debug_println("task B: set led B");
        rtos_sleep(50);
        GPIO_PinOutSet(gpioPortB, 7);
    }
}

int main(void)
{
    debug_println("Initializing peripherals...");

    // Runs the Silicon Labs chip initialisation stuff, that also deals with
    // errata (implements workarounds, etc).
    CHIP_Init();

    // Relocate the vector table (probably not necessary after the bootloader but
    // doing it anyway)
    SCB->VTOR = 0x4000;

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
