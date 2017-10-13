#include <libopencm3/cm3/common.h>

#include "rtos-acamar.h"

#include "debug.h"

/* NOTE: these handlers are different to
 * libopencm3's default null/blocking handlers -
 * each isr gets its own function, making it
 * easier to figure out which isr was fired.
 * Additionally they spit out the isr ID.*/

#define BLOCKING_HANDLER(handler) \
    void ##handler(void) \
    { \
        debug_println(#handler " - blocking..."); \
        for(;;); \
    }

#define NULL_HANDLER(handler) \
    void ##handler(void) \
    { \
    }

/* This 'fatal' handler is called by the RTOS
 * if any internal logic checks fail. Possible
 * causes are often memory corruption or
 * impossible scheduling deadlines */

void
fatal(const RtosErrorId error_id)
{
    debug_print("FATAL RTOS ERROR: 0x");
    debug_printhex32(error_id);
    debug_println("");
    for (;;)
    {
    }
}

/* TODO: Remove the below handlers and redeclare
 * your own handler to use an ISR for your own purposes. */

/* CORTEX-M GENERIC ISRS */

BLOCKING_HANDLER(nmi_isr)
BLOCKING_HANDLER(hardfault_isr)
BLOCKING_HANDLER(memmanage_isr)
BLOCKING_HANDLER(busfault_isr)
BLOCKING_HANDLER(usagefault_isr)
BLOCKING_HANDLER(svcall_isr)
BLOCKING_HANDLER(debug_monitor_isr)
BLOCKING_HANDLER(pendsv_isr)
BLOCKING_HANDLER(systick_isr)

/* PERIPHERAL/EXTERNAL ISRS */
