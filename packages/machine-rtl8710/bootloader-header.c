#include <stdint.h>

extern uint32_t all_start;
extern uint32_t all_size;

#define FW_VERSION          (0x0100)
#define FW_SUBVERSION       (0x0001)
#define FW_CHIP_ID          (0x8195)
#define FW_CHIP_VER         (0x01)
#define FW_BUS_TYPE         (0x01)          // the iNIC firmware type: USB/SDIO
#define FW_INFO_RSV1        (0x00)          // the firmware information reserved
#define FW_INFO_RSV2        (0x00)          // the firmware information reserved
#define FW_INFO_RSV3        (0x00)          // the firmware information reserved
#define FW_INFO_RSV4        (0x00)          // the firmware information reserved

extern void entry(void);

// These first 2 headers are used to indicate where to copy the flash
// image on boot. They are not used at all for RAM debugging.

__attribute__((section(".image2.header1")))
const uint32_t image2_header1[] = {
    (uint32_t)&all_size, // Size of data to be copied from flash to ram
    (uint32_t)&all_start, // Where to start copying to ram
};

__attribute__((section(".image2.header2")))
const uint8_t image2_header2[] = {
    '8', '1', '9', '5', '8', '7', '1', '1',
};

// Only the second 2 headers are loaded for RAM debugging

__attribute__((section(".image2.header3")))
const uint32_t image2_header3[] = {
    (uint32_t)&entry + 1 //start by executing 'entry', in thumb mode (this was previously InfraStart)
};

__attribute__((section(".image2.header4")))
const uint8_t image2_header4[] = {
	'R', 'T', 'K', 'W', 'i', 'n', 0x0, 0xff, 
	(FW_VERSION&0xff), ((FW_VERSION >> 8)&0xff),
	(FW_SUBVERSION&0xff), ((FW_SUBVERSION >> 8)&0xff),
	(FW_CHIP_ID&0xff), ((FW_CHIP_ID >> 8)&0xff),
	(FW_CHIP_VER),
	(FW_BUS_TYPE),
	(FW_INFO_RSV1),
	(FW_INFO_RSV2),
	(FW_INFO_RSV3),
	(FW_INFO_RSV4)
};
