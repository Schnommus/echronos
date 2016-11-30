/*| headers |*/

/*| object_like_macros |*/

/* MPU control registers */
#define MPU_TYPE                    0xE000ED90  /* MPU Type                        */
#define MPU_CTRL                    0xE000ED94  /* MPU Control                     */
#define MPU_NUMBER                  0xE000ED98  /* MPU Region Number               */
#define MPU_BASE                    0xE000ED9C  /* MPU Region Base Address         */
#define MPU_ATTR                    0xE000EDA0  /* MPU Region Attribute and Size   */
#define MPU_BASE1                   0xE000EDA4  /* MPU Region Base Address Alias 1 */
#define MPU_ATTR1                   0xE000EDA8  /* MPU Region Attribute and Size ^ */
#define MPU_BASE2                   0xE000EDAC  /* MPU Region Base Address Alias 2 */
#define MPU_ATTR2                   0xE000EDB0  /* MPU Region Attribute and Size ^ */
#define MPU_BASE3                   0xE000EDB4  /* MPU Region Base Address Alias 3 */
#define MPU_ATTR3                   0xE000EDB8  /* MPU Region Attribute and Size ^ */

/* MPU type bitfields */
#define MPU_TYPE_IREGION_M          0x00FF0000  /* Number of I Regions             */
#define MPU_TYPE_DREGION_M          0x0000FF00  /* Number of D Regions             */
#define MPU_TYPE_SEPARATE           0x00000001  /* Separate or Unified MPU         */
#define MPU_TYPE_IREGION_S          16
#define MPU_TYPE_DREGION_S          8

/* MPU base bitfields */
#define MPU_BASE_ADDR_M             0xFFFFFFE0  /* Base Address Mask               */
#define MPU_BASE_VALID              0x00000010  /* Region Number Valid             */
#define MPU_BASE_REGION_M           0x00000007  /* Region Number                   */
#define MPU_BASE_ADDR_S             5
#define MPU_BASE_REGION_S           0

/* More MPU-related constants */
#define MPU_MAX_REGIONS             8

/* MPU control flags */
#define MPU_CTRL_ENABLE             0x00000001  /* MPU Enable                      */

/* MPU enable flags */
#define MPU_CONFIG_PRIV_DEFAULT     4
#define MPU_CONFIG_HARDFLT_NMI      2
#define MPU_CONFIG_NONE             0

/* MPU region enable flags */
#define MPU_RGN_ENABLE              1
#define MPU_RGN_DISABLE             0

/* MPU region permission flags */
#define MPU_RGN_PERM_EXEC           0x00000000
#define MPU_RGN_PERM_NOEXEC         0x10000000
#define MPU_RGN_PERM_PRV_NO_USR_NO  0x00000000
#define MPU_RGN_PERM_PRV_RW_USR_NO  0x01000000
#define MPU_RGN_PERM_PRV_RW_USR_RO  0x02000000
#define MPU_RGN_PERM_PRV_RW_USR_RW  0x03000000
#define MPU_RGN_PERM_PRV_RO_USR_NO  0x05000000
#define MPU_RGN_PERM_PRV_RO_USR_RO  0x06000000

/* MPU special region permission flags */
#define MPU_ATTR_TEX_M              0x00380000
#define MPU_ATTR_SHAREABLE          0x00040000
#define MPU_ATTR_CACHEABLE          0x00020000
#define MPU_ATTR_BUFFRABLE          0x00010000

/* MPU sub-region flags */
#define MPU_SUB_RGN_DISABLE_0       0x00000100
#define MPU_SUB_RGN_DISABLE_1       0x00000200
#define MPU_SUB_RGN_DISABLE_2       0x00000400
#define MPU_SUB_RGN_DISABLE_3       0x00000800
#define MPU_SUB_RGN_DISABLE_4       0x00001000
#define MPU_SUB_RGN_DISABLE_5       0x00002000
#define MPU_SUB_RGN_DISABLE_6       0x00004000
#define MPU_SUB_RGN_DISABLE_7       0x00008000

/* Allowed MPU region sizes */
#define MPU_ATTR_SIZE_M             0x0000003E  /* Region Size Mask */
#define MPU_RGN_SIZE_32B            (4 << 1)
#define MPU_RGN_SIZE_64B            (5 << 1)
#define MPU_RGN_SIZE_128B           (6 << 1)
#define MPU_RGN_SIZE_256B           (7 << 1)
#define MPU_RGN_SIZE_512B           (8 << 1)

#define MPU_RGN_SIZE_1K             (9 << 1)
#define MPU_RGN_SIZE_2K             (10 << 1)
#define MPU_RGN_SIZE_4K             (11 << 1)
#define MPU_RGN_SIZE_8K             (12 << 1)
#define MPU_RGN_SIZE_16K            (13 << 1)
#define MPU_RGN_SIZE_32K            (14 << 1)
#define MPU_RGN_SIZE_64K            (15 << 1)
#define MPU_RGN_SIZE_128K           (16 << 1)
#define MPU_RGN_SIZE_256K           (17 << 1)
#define MPU_RGN_SIZE_512K           (18 << 1)

#define MPU_RGN_SIZE_1M             (19 << 1)
#define MPU_RGN_SIZE_2M             (20 << 1)
#define MPU_RGN_SIZE_4M             (21 << 1)
#define MPU_RGN_SIZE_8M             (22 << 1)
#define MPU_RGN_SIZE_16M            (23 << 1)
#define MPU_RGN_SIZE_32M            (24 << 1)
#define MPU_RGN_SIZE_64M            (25 << 1)
#define MPU_RGN_SIZE_128M           (26 << 1)
#define MPU_RGN_SIZE_256M           (27 << 1)
#define MPU_RGN_SIZE_512M           (28 << 1)

#define MPU_RGN_SIZE_1G             (29 << 1)
#define MPU_RGN_SIZE_2G             (30 << 1)
#define MPU_RGN_SIZE_4G             (31 << 1)

/* Registers used for enabling the MPU interrupt */
#define SYS_HND_CTRL                0xE000ED24  /* System Handler Control and State */
#define SYS_HND_CTRL_MEM            0x00010000  /* Memory Management Fault Enable   */

/* Registers used to handle the MPU interrupt */
#define NVIC_FAULT_STAT             0xE000ED28  /* Configurable Fault Status        */
#define NVIC_MM_ADDR                0xE000ED34  /* Memory Management Fault Address  */

/* Armv7m system address map (from TRM) */
#define CODE_BASE                   0x00000000  /* 512 MB */
#define SRAM_BASE                   0x20000000  /* 512 MB */
#define SRAM_BITBAND_END            0x20100000  /* ^      */
#define SRAM_BITBAND_ALIAS_BASE     0x22000000  /* ^      */
#define SRAM_BITBAND_ALIAS_END      0x24000000  /* ^      */
#define PERIPH_BASE                 0x40000000  /* 512 MB */
#define PERIPH_BITBAND_END          0x40100000  /* ^      */
#define PERIPH_BITBAND_ALIAS_BASE   0x42000000  /* ^      */
#define PERIPH_BITBAND_ALIAS_END    0x44000000  /* ^      */
#define EXTERNAL_RAM_BASE           0x60000000  /* 1 GB   */
#define EXTERNAL_DEVICE_BASE        0xA0000000  /* 1 GB   */
#define INT_PERIPH_BASE             0xE0000000  /* 256 KB */
#define INT_PERIPH_NVIC             0xE000E000  /* ^      */
#define EXT_PERIPH_BASE             0xE0040000  /* 768 KB */
#define SYSTEM_RESERVED_BASE        0xE0100000  /* rest   */

/*| types |*/

/*| structures |*/

/*| extern_declarations |*/

/*| function_declarations |*/
void mpu_enable(void);
void mpu_disable(void);
uint32_t mpu_regions_supported_get(void) {
void mpu_region_enable(uint32_t mpu_region);
void mpu_region_disable(uint32_t mpu_region);
void mpu_region_set(uint32_t mpu_region, uint32_t mpu_addr, uint32_t mpu_flags);
void mpu_region_get(uint32_t mpu_region, uint32_t *mpu_addr_ptr, uint32_t *mpu_flags_ptr);
void mpu_memmanage_interrupt_enable(void);
void mpu_memmanage_interrupt_disable(void);

/*| state |*/

/*| function_like_macros |*/
#define HWREG(x) \
        (*((volatile uint32_t *)(x)))

/*| functions |*/
void
mpu_enable(void) {
    api_assert( !(HWREG(MPU_CTRL) & MPU_CTRL_ENABLE),
                ERROR_ID_MPU_ALREADY_ENABLED );

    /* Make the MPU fault in privileged mode, but disable it during a hard fault */
    HWREG(MPU_CTRL) &= ~(MPU_CONFIG_PRIV_DEFAULT | MPU_CONFIG_HARDFLT_NMI)

    /* Turn on the MPU */
    HWREG(MPU_CTRL) = mpu_config | MPU_CTRL_ENABLE;
}

void
mpu_disable(void) {
    api_assert( HWREG(MPU_CTRL) & MPU_CTRL_ENABLE,
                ERROR_ID_MPU_ALREADY_DISABLED );

    HWREG(MPU_CTRL) &= ~MPU_CTRL_ENABLE;
}

/* Gets the number of hardware regions supported by this MPU */
uint32_t
mpu_regions_supported_get(void) {
    /* Read the DREGION field of the MPU type register and mask off   */
    /* the bits of interest to get the count of regions.              */
    return((HWREG(MPU_TYPE) & MPU_TYPE_DREGION_M) >> MPU_TYPE_DREGION_S);
}

void
mpu_region_enable(uint32_t mpu_region) {
    api_assert(mpu_region < MPU_MAX_REGIONS,
               ERROR_ID_MPU_INTERNAL_INVALID_REGION_INDEX);

    /* Pick the region we want to modify */
    HWREG(MPU_NUMBER) = mpu_region;

    /* Enable this region */
    HWREG(MPU_ATTR) |= MPU_ATTR_ENABLE;
}

void
mpu_region_disable(uint32_t mpu_region) {
    api_assert(mpu_region < MPU_MAX_REGIONS,
               ERROR_ID_MPU_INTERNAL_INVALID_REGION_INDEX);

    HWREG(MPU_NUMBER) = mpu_region;
    HWREG(MPU_ATTR) &= ~MPU_ATTR_ENABLE;
}

void
mpu_region_set(uint32_t mpu_region, uint32_t mpu_addr, uint32_t mpu_flags) {
    api_assert(mpu_region < MPU_MAX_REGIONS,
               ERROR_ID_MPU_INTERNAL_INVALID_REGION_INDEX);

    /* Check that the address is size-aligned as required */
    api_assert(mpu_addr == (mpu_addr & ~0 << (((mpu_flags & MPU_ATTR_SIZE_M) >> 1) + 1)),
               ERROR_ID_MPU_INTERNAL_MISALIGNED_ADDR);

    /* Select the region and set the base address at the same time */
    HWREG(MPU_BASE) = mpu_addr | mpu_region | MPU_BASE_VALID;

    /* Set region attributes, with fixed values for:
     * Type Extension Mask = 0
     * Cacheable = 0
     * Shareable = 1
     * Bufferable = 1
     * These options should be fine on most devices, with a performance
     * penalty on armv7m processors that have a cache. This is rare though. */
    HWREG(MPU_ATTR) = ((mpu_flags & ~(MPU_ATTR_TEX_M | MPU_ATTR_CACHEABLE)) |
                        MPU_ATTR_SHAREABLE | MPU_ATTR_BUFFRABLE);
}

void
mpu_region_get(uint32_t mpu_region, uint32_t *mpu_addr_ptr, uint32_t *mpu_flags) {
    api_assert(mpu_region < MPU_MAX_REGIONS,
               ERROR_ID_MPU_INTERNAL_INVALID_REGION_INDEX);

    api_assert(mpu_addr && mpu_flags,
               ERROR_ID_MPU_INTERNAL_INVALID_PTR);

    /* Set the MPU region and then grab our data */
    HWREG(MPU_NUMBER) = mpu_region;
    *mpu_addr_ptr = HWREG(MPU_BASE) & MPU_BASE_ADDR_M;
    *mpu_flags_ptr = HWREG(MPU_ATTR);
}

void
mpu_memmanage_interrupt_enable(void) {
    /* Clear the NVIC FSR as it starts off as junk */
    uint32_t fault_stat = HWREG(NVIC_FAULT_STAT);
    HWREG(NVIC_FAULT_STAT) = fault_stat;

    /* Enable the interrupt */
    HWREG(SYS_HND_CTRL) |= SYS_HND_CTRL_MEM;
}

void
mpu_memmanage_interrupt_disable(void) {
    HWREG(SYS_HND_CTRL) &= ~SYS_HND_CTRL_MEM;
}

void
mpu_initialize(void) {

    /* TODO: Instead of doing this, only give tasks access to their own stack!
     * We shouldn't really be protecting the RTOS from itself, that's not necessary. */

    /* Create a read-only executable region of size 256K -> FLASH */
    mpu_region_set(0, CODE_BASE,
                   MPU_RGN_SIZE_256K | MPU_RGN_PERM_EXEC |
                   MPU_RGN_PERM_PRV_RO_USR_RO |
                   MPU_RGN_ENABLE);

    /* Create an RW non-executable SRAM region of size 32K -> SRAM */
    mpu_region_set(1, SRAM_BASE,
                   MPU_RGN_SIZE_32K | MPU_RGN_PERM_NOEXEC |
                   MPU_RGN_PERM_PRV_RW_USR_RW | MPU_SUB_RGN_DISABLE_4 |
                   MPU_RGN_ENABLE);


    /* Enable the memmanage interrupt */
    mpu_memmanage_interrupt_enable(void);

    /* The MPU itself will only enforce memory protection rules
     * whilst it is enabled. We only enable the MPU when we are
     * inside a task - it is not enabled here. */
}

/*| public_functions |*/

bool
{{prefix_func}}handle_memmanage(void) {
    /* Grab fault address and status */
    uint32_t fault_address = HWREG(NVIC_MM_ADDR);
    uint32_t fault_status  = HWREG(NVIC_FAULT_STAT);

    /* Clear the fault status register */
    HWREG(NVIC_FAULT_STAT) = fault_status;

    /* Turn off the MPU so that the RTOS (outside this handler)
     * is able to operate normally */
    mpu_disable();

    /* An MPU policy violation is a fatal error (for now) */
    {{fatal_error}}(ERROR_ID_MPU_VIOLATION);
}

