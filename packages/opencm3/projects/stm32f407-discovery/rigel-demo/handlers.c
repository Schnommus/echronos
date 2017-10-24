#include <libopencm3/cm3/common.h>

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

/* PERIPHERAL/EXTERNAL ISRS (automatically populated)*/

BLOCKING_HANDLER(nvic_wwdg_isr)
BLOCKING_HANDLER(pvd_isr)
BLOCKING_HANDLER(tamp_stamp_isr)
BLOCKING_HANDLER(rtc_wkup_isr)
BLOCKING_HANDLER(flash_isr)
BLOCKING_HANDLER(rcc_isr)
BLOCKING_HANDLER(exti0_isr)
BLOCKING_HANDLER(exti1_isr)
BLOCKING_HANDLER(exti2_isr)
BLOCKING_HANDLER(exti3_isr)
BLOCKING_HANDLER(exti4_isr)
BLOCKING_HANDLER(dma1_stream0_isr)
BLOCKING_HANDLER(dma1_stream1_isr)
BLOCKING_HANDLER(dma1_stream2_isr)
BLOCKING_HANDLER(dma1_stream3_isr)
BLOCKING_HANDLER(dma1_stream4_isr)
BLOCKING_HANDLER(dma1_stream5_isr)
BLOCKING_HANDLER(dma1_stream6_isr)
BLOCKING_HANDLER(adc_isr)
BLOCKING_HANDLER(can1_tx_isr)
BLOCKING_HANDLER(can1_rx0_isr)
BLOCKING_HANDLER(can1_rx1_isr)
BLOCKING_HANDLER(can1_sce_isr)
BLOCKING_HANDLER(exti9_5_isr)
BLOCKING_HANDLER(tim1_brk_tim9_isr)
BLOCKING_HANDLER(tim1_up_tim10_isr)
BLOCKING_HANDLER(tim1_trg_com_tim11_isr)
BLOCKING_HANDLER(tim1_cc_isr)
//BLOCKING_HANDLER(tim2_isr) - we are using this!
BLOCKING_HANDLER(tim3_isr)
BLOCKING_HANDLER(tim4_isr)
BLOCKING_HANDLER(i2c1_ev_isr)
BLOCKING_HANDLER(i2c1_er_isr)
BLOCKING_HANDLER(i2c2_ev_isr)
BLOCKING_HANDLER(i2c2_er_isr)
BLOCKING_HANDLER(spi1_isr)
BLOCKING_HANDLER(spi2_isr)
BLOCKING_HANDLER(usart1_isr)
BLOCKING_HANDLER(usart2_isr)
BLOCKING_HANDLER(usart3_isr)
BLOCKING_HANDLER(exti15_10_isr)
BLOCKING_HANDLER(rtc_alarm_isr)
BLOCKING_HANDLER(usb_fs_wkup_isr)
BLOCKING_HANDLER(tim8_brk_tim12_isr)
BLOCKING_HANDLER(tim8_up_tim13_isr)
BLOCKING_HANDLER(tim8_trg_com_tim14_isr)
BLOCKING_HANDLER(tim8_cc_isr)
BLOCKING_HANDLER(dma1_stream7_isr)
BLOCKING_HANDLER(fsmc_isr)
BLOCKING_HANDLER(sdio_isr)
BLOCKING_HANDLER(tim5_isr)
BLOCKING_HANDLER(spi3_isr)
BLOCKING_HANDLER(uart4_isr)
BLOCKING_HANDLER(uart5_isr)
BLOCKING_HANDLER(tim6_dac_isr)
BLOCKING_HANDLER(tim7_isr)
BLOCKING_HANDLER(dma2_stream0_isr)
BLOCKING_HANDLER(dma2_stream1_isr)
BLOCKING_HANDLER(dma2_stream2_isr)
BLOCKING_HANDLER(dma2_stream3_isr)
BLOCKING_HANDLER(dma2_stream4_isr)
BLOCKING_HANDLER(eth_isr)
BLOCKING_HANDLER(eth_wkup_isr)
BLOCKING_HANDLER(can2_tx_isr)
BLOCKING_HANDLER(can2_rx0_isr)
BLOCKING_HANDLER(can2_rx1_isr)
BLOCKING_HANDLER(can2_sce_isr)
BLOCKING_HANDLER(otg_fs_isr)
BLOCKING_HANDLER(dma2_stream5_isr)
BLOCKING_HANDLER(dma2_stream6_isr)
BLOCKING_HANDLER(dma2_stream7_isr)
BLOCKING_HANDLER(usart6_isr)
BLOCKING_HANDLER(i2c3_ev_isr)
BLOCKING_HANDLER(i2c3_er_isr)
BLOCKING_HANDLER(otg_hs_ep1_out_isr)
BLOCKING_HANDLER(otg_hs_ep1_in_isr)
BLOCKING_HANDLER(otg_hs_wkup_isr)
BLOCKING_HANDLER(otg_hs_isr)
BLOCKING_HANDLER(dcmi_isr)
BLOCKING_HANDLER(cryp_isr)
BLOCKING_HANDLER(hash_rng_isr)
BLOCKING_HANDLER(fpu_isr)
BLOCKING_HANDLER(uart7_isr)
BLOCKING_HANDLER(uart8_isr)
BLOCKING_HANDLER(spi4_isr)
BLOCKING_HANDLER(spi5_isr)
BLOCKING_HANDLER(spi6_isr)
BLOCKING_HANDLER(sai1_isr)
BLOCKING_HANDLER(lcd_tft_isr)
BLOCKING_HANDLER(lcd_tft_err_isr)
BLOCKING_HANDLER(dma2d_isr)
