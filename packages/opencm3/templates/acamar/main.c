#include <libopencm3/cm3/common.h>

#include "rtos-acamar.h"
#include "debug.h"

void fn_a(void)
{
    for (;;)
    {
        rtos_yield_to(RTOS_TASK_ID_B);
        debug_println("-> in task A");
    }
}

void fn_b(void)
{
    for (;;)
    {
        rtos_yield_to(RTOS_TASK_ID_A);
        debug_println("-> in task B");
    }
}

int main(void) {

    debug_println("Initializing peripherals...");

    /* TODO: Setup clocks, initialize peripherals etc. here */

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    while(1);

	return 0;
}
