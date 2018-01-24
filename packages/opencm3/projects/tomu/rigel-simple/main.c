#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/efm32/wdog.h>
#include <libopencm3/efm32/gpio.h>
#include <libopencm3/efm32/cmu.h>

#include "rtos-rigel.h"
#include "debug.h"

/* Systick interrupt frequency, Hz */
#define SYSTICK_FREQUENCY 1000

void fn_a(void)
{
    for (;;)
    {
        debug_println("task A: set led A");
        rtos_sleep(50);
        gpio_clear(GPIOA, GPIO0);
        debug_println("task A: clear led A");
        rtos_sleep(50);
        gpio_set(GPIOA, GPIO0);
    }
}

void fn_b(void)
{
    for (;;)
    {
        debug_println("task B: clear led B");
        rtos_sleep(50);
        gpio_set(GPIOB, GPIO7);
        debug_println("task B: set led B");
        rtos_sleep(50);
        gpio_clear(GPIOB, GPIO7);
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
    // TODO: Put CHIP_Init() here...
    libopencm3_pre_main();

    // Relocate the vector table (probably not necessary after the bootloader but
    // doing it anyway)
    SCB_VTOR = 0x4000;

    WDOG_CTRL = WDOG_CTRL_CLKSEL_ULFRCO | WDOG_CTRL_EN | (1 << WDOG_CTRL_PERSEL_SHIFT);

    debug_println("Initializing peripherals...");

    cmu_disable_lock();

    cmu_periph_clock_enable(CMU_GPIO);

    cmu_enable_lock();

    gpio_disable_lock();

    gpio_mode_setup(GPIOA, GPIO_MODE_WIRED_AND, GPIO0);
    gpio_mode_setup(GPIOB, GPIO_MODE_WIRED_AND, GPIO7);

    gpio_enable_lock();

    /* TODO: Setup clocks, initialize peripherals etc. here */

    /* TODO: Set the ahb frequency based on your clock settings above
     * Usually this has the same value as the main clock frequency */

    uint32_t ahb_freq = 14000000;

    systick_start(SYSTICK_FREQUENCY, ahb_freq);

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    debug_println("rtos_start returned? ...");

    for(;;);

    return 0;
}
