#include <libopencm3/cm3/common.h>
#include <libopencm3/efm32/wdog.h>

#include "rtos-rigel.h"

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

void
systick_isr(void)
{
    rtos_timer_tick();
    WDOG_CMD = WDOG_CMD_CLEAR;
}

/* TODO: Remove the below handlers and redeclare
 * your own handler to use an ISR for your own purposes. */

/* CORTEX-M GENERIC ISRS */

BLOCKING_HANDLER(nmi_isr)
BLOCKING_HANDLER(hardfault_isr)
BLOCKING_HANDLER(svcall_isr)
BLOCKING_HANDLER(pendsv_isr)

/* PERIPHERAL/EXTERNAL ISRS*/

BLOCKING_HANDLER(dma_isr)
BLOCKING_HANDLER(gpio_even_isr)
BLOCKING_HANDLER(timer0_isr)
BLOCKING_HANDLER(acmp0_isr)
BLOCKING_HANDLER(adc0_isr)
BLOCKING_HANDLER(i2c0_isr)
BLOCKING_HANDLER(gpio_odd_isr)
BLOCKING_HANDLER(timer1_isr)
BLOCKING_HANDLER(usart1_rx_isr)
BLOCKING_HANDLER(usart1_tx_isr)
BLOCKING_HANDLER(leuart0_isr)
BLOCKING_HANDLER(pcnt0_isr)
BLOCKING_HANDLER(rtc_isr)
BLOCKING_HANDLER(cmu_isr)
BLOCKING_HANDLER(vcmp_isr)
BLOCKING_HANDLER(msc_isr)
BLOCKING_HANDLER(aes_isr)
BLOCKING_HANDLER(usart0_rx_isr)
BLOCKING_HANDLER(usart0_tx_isr)
BLOCKING_HANDLER(usb_isr)
BLOCKING_HANDLER(timer2_isr)
 
