#include "rtl8710.h"
#include "lib/mask.h"

void serial_init(){
	PERI_ON->PESOC_CLK_CTRL |= PERI_ON_CLK_CTRL_ACTCK_LOG_UART_EN | PERI_ON_CLK_CTRL_SLPCK_LOG_UART_EN; // enable LOG_UART peripheral clock
	PERI_ON->SOC_FUNC_EN |= PERI_ON_SOC_FUNC_EN_LOG_UART; // enable LOG_UART peripheral
	while(!(LOG_UART->LSR & LOG_UART_LSR_TEMT)); // wait until bootloader messages are printed
	mask32_set(PERI_ON->CPU_PERIPHERAL_CTRL, PERI_ON_CPU_PERIPHERAL_CTRL_LOG_UART_PIN_SEL, 0); // default pins are B0/B1
	PERI_ON->CPU_PERIPHERAL_CTRL |= PERI_ON_CPU_PERIPHERAL_CTRL_LOG_UART_PIN_EN; // enable LOG_UART pins

	LOG_UART->IER = 0;
	LOG_UART->LCR |= LOG_UART_LCR_DLAB;
	LOG_UART->DLL = 0x22; // default firmware sets DLL to 0x22 and DLH to 0... -> baudrate to 38400 (used mww and mdw commands with openocd to set DLAB_EN bit and read values)
	// LOG_UART->DLL = 0x11; // bootloader sets DLL to 0x11 and DLH to 0... -> baudrate to 38400 (used mww and mdw commands with openocd to set DLAB_EN bit and read values)
	LOG_UART->DLH = 0x00; // look rtl8195a_uart.c file in the SDK how to calculate baud rate
	LOG_UART->LCR = mask32(LOG_UART_LCR_DLS, 3);
	LOG_UART->FCR = 0;
}

uint16_t serial_write(const void *buf, uint16_t count){
	if(!count || !(LOG_UART->LSR & LOG_UART_LSR_THRE))return(0);
	LOG_UART->THR = ((uint8_t *)buf)[0];
	return(1);
}

uint16_t serial_read(void *buf, uint16_t count){
	if(!count || !(LOG_UART->LSR & LOG_UART_LSR_DR))return(0);
	((uint8_t *)buf)[0] = LOG_UART->RBR;
	return(1);
}

