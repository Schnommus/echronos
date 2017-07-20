#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdint.h>

void serial_init();
uint16_t serial_write(const void *buf, uint16_t count);
uint16_t serial_read(void *buf, uint16_t count);

#endif

