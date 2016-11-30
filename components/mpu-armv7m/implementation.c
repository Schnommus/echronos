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

/*| types |*/

/*| structures |*/

/*| extern_declarations |*/

/*| function_declarations |*/
void mpu_enable(uint32_t mpu_config);
void mpu_disable(void);
uint32_t mpu_regions_supported_get(void) {
void mpu_region_enable(uint32_t mpu_region);
void mpu_region_disable(uint32_t mpu_region);
void mpu_region_set(uint32_t mpu_region, uint32_t mpu_addr, uint32_t mpu_flags);
void mpu_region_get(uint32_t mpu_region, uint32_t *mpu_addr_ptr, uint32_t *mpu_flags_ptr);

/*| state |*/

/*| function_like_macros |*/
#define HWREG(x) \
        (*((volatile uint32_t *)(x)))

/*| functions |*/
void
mpu_enable(uint32_t mpu_config) {
    /* Check we configure the MPU to use one of the correct flags */
    api_assert(!(mpu_config & ~(MPU_CONFIG_PRIV_DEFAULT | MPU_CONFIG_HARDFLT_NMI)),
               ERROR_ID_MPU_INTERNAL_INVALID_ENABLE_ARGUMENTS );

    HWREG(MPU_CTRL) = mpu_config | MPU_CTRL_ENABLE;
}

void
mpu_disable(void) {
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

/*| public_functions |*/
