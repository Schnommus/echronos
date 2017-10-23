#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>

#include "rtos-acamar.h"
#include "debug.h"

void fn_a(void)
{
    debug_println("Started task A");
    for (;;)
    {
        debug_println("-> in task A");
        rtos_yield_to(RTOS_TASK_ID_B);
    }
}

void fn_b(void)
{
    debug_println("Started task B");
    for (;;)
    {
        debug_println("-> in task B");
        rtos_yield_to(RTOS_TASK_ID_A);
    }
}

int main(void) {

    libopencm3_pre_main();

    debug_println("Initializing peripherals...");

    /* TODO: Setup clocks, initialize peripherals etc. here */

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    debug_println("rtos_start returned? ...");

    while(1);

    return 0;
}
