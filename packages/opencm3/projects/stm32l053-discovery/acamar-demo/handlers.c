#include <libopencm3/cm3/common.h>

#include "rtos-acamar.h"

#include "debug.h"

/* NOTE: these handlers are different to
 * libopencm3's default null/blocking handlers -
 * each isr gets its own function, making it
 * easier to figure out which isr was fired.
 * Additionally they spit out the isr ID.*/

#define BLOCKING_HANDLER(NAME_ISR) \
    void NAME_ISR(void) \
    { \
        debug_println(#NAME_ISR " - blocking..."); \
        for(;;); \
    }

#define NULL_HANDLER(NAME_ISR) \
    void NAME_ISR(void) \
    { \
    }

/* This 'fatal' handler is called by the RTOS
 * if any internal logic checks fail. Possible
 * causes are often memory corruption or
 * impossible scheduling deadlines */

void
fatal(const RtosErrorId error_id)
{
    debug_print("RTOS FATAL ERROR: ");
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

/* PERIPHERAL/EXTERNAL ISRS (automatically populated)*/

BLOCKING_HANDLER(wwdg_isr)
BLOCKING_HANDLER(pvd_isr)
BLOCKING_HANDLER(rtc_isr)
BLOCKING_HANDLER(flash_isr)
BLOCKING_HANDLER(rcc_isr)
BLOCKING_HANDLER(exti0_1_isr)
BLOCKING_HANDLER(exti2_3_isr)
BLOCKING_HANDLER(exti4_15_isr)
BLOCKING_HANDLER(tsc_isr)
BLOCKING_HANDLER(dma1_channel1_isr)
BLOCKING_HANDLER(dma1_channel2_3_isr)
BLOCKING_HANDLER(dma1_channel4_5_isr)
BLOCKING_HANDLER(adc_comp_isr)
BLOCKING_HANDLER(lptim1_isr)
BLOCKING_HANDLER(reserved1_isr)
BLOCKING_HANDLER(tim2_isr)
BLOCKING_HANDLER(reserved2_isr)
BLOCKING_HANDLER(tim6_dac_isr)
BLOCKING_HANDLER(reserved3_isr)
BLOCKING_HANDLER(reserved4_isr)
BLOCKING_HANDLER(tim21_isr)
BLOCKING_HANDLER(reserved5_isr)
BLOCKING_HANDLER(tim22_isr)
BLOCKING_HANDLER(i2c1_isr)
BLOCKING_HANDLER(i2c2_isr)
BLOCKING_HANDLER(spi1_isr)
BLOCKING_HANDLER(spi2_isr)
BLOCKING_HANDLER(usart1_isr)
BLOCKING_HANDLER(usart2_isr)
BLOCKING_HANDLER(lpuart1_isr)
BLOCKING_HANDLER(lcd_isr)
BLOCKING_HANDLER(usb_isr)