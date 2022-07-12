/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */
#include "fsl_iomuxc.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get debug console frequency. */
uint32_t BOARD_DebugConsoleSrcFreq(void)
{
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart0102);
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t lpi2cConfig = {0};

    /*
     * lpi2cConfig.debugEnable = false;
     * lpi2cConfig.ignoreAck = false;
     * lpi2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * lpi2cConfig.baudRate_Hz = 100000U;
     * lpi2cConfig.busIdleTimeout_ns = 0;
     * lpi2cConfig.pinLowTimeout_ns = 0;
     * lpi2cConfig.sdaGlitchFilterWidth_ns = 0;
     * lpi2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cConfig);
    LPI2C_MasterInit(base, &lpi2cConfig, clkSrc_Hz);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subAddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize)
{
    status_t status;
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = NULL;
    xfer.dataSize       = 0;

    status = LPI2C_MasterTransferBlocking(base, &xfer);

    if (kStatus_Success == status)
    {
        xfer.subaddressSize = 0;
        xfer.direction      = kLPI2C_Read;
        xfer.data           = rxBuff;
        xfer.dataSize       = rxBuffSize;

        status = LPI2C_MasterTransferBlocking(base, &xfer);
    }

    return status;
}

void BOARD_Accel_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_ACCEL_I2C_BASEADDR, BOARD_ACCEL_I2C_CLOCK_FREQ);
}

status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff)
{
    uint8_t data = (uint8_t)txBuff;

    return BOARD_LPI2C_Send(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, &data, 1);
}

status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}

void BOARD_Codec_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_CODEC_I2C_BASEADDR, BOARD_CODEC_I2C_CLOCK_FREQ);
}

status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

void BOARD_Camera_I2C_Init(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);

    BOARD_LPI2C_Init(BOARD_CAMERA_I2C_BASEADDR, CLOCK_GetRootClockFreq(BOARD_CAMERA_I2C_CLOCK_ROOT));
}

status_t BOARD_Camera_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_SendSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                                txBuffSize);
}

status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_ReceiveSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                                   rxBuffSize);
}

void BOARD_MIPIPanelTouch_I2C_Init(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_SOURCE,
        .div      = BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_ROOT, &lpi2cClockConfig);

    BOARD_LPI2C_Init(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR,
                     CLOCK_GetRootClockFreq(BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_ROOT));
}

status_t BOARD_MIPIPanelTouch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize,
                            (uint8_t *)txBuff, txBuffSize);
}

status_t BOARD_MIPIPanelTouch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}
#endif /* SDK_I2C_BASED_COMPONENT_USED */

/* MPU configuration. */
#if __CORTEX_M == 7
void BOARD_ConfigMPU(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t size          = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                        0 :
                        ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
#if defined(__USE_SHMEM)
    extern uint32_t __base_rpmsg_sh_mem;
    extern uint32_t __top_rpmsg_sh_mem;
    uint32_t nonCacheStart = (uint32_t)(&__base_rpmsg_sh_mem);
    uint32_t size          = (uint32_t)(&__top_rpmsg_sh_mem) - nonCacheStart;
#else
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t size          = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
#endif
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t size          = (uint32_t)__NCACHE_REGION_SIZE;
#endif
    volatile uint32_t i = 0;

#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }
#endif

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in mpu_armv7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * mpu_armv7.h.
     */

    /*
     * Add default region to deny access to whole address space to workaround speculative prefetch.
     * Refer to Arm errata 1013783-B for more details.
     *
     */
    /* Region 0 setting: Instruction access disabled, No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_NONE, 0, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2GB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */ /*ITCM*/
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */ /*DTCM*/
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */ /*OCRAM1*/
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20480000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);

    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */ /*OCRAM2*/
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20500000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner write back. */ /*FSPI1*/
    MPU->RBAR = ARM_MPU_RBAR(8, 0x04000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
#endif

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 2)
    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner write back. */ /*FSPI2*/
    MPU->RBAR = ARM_MPU_RBAR(8, 0x28000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128MB);
#endif

#ifdef USE_SDRAM
    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
#endif

    while ((size >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(nonCacheStart % size));
        assert(size == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 10 setting: Memory with Normal type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(10, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
    }

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_EnableDCache();
#endif
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    SCB_EnableICache();
#endif
}
#elif __CORTEX_M == 33
void BOARD_ConfigMPU(void)
{
    uint8_t attr;

    if ((XCACHE_PC->CCR & XCACHE_CCR_ENCACHE_MASK) == 1U) /* disabled if enabled */
    {
        /* Enable the to push all modified lines. */
        XCACHE_PC->CCR |= XCACHE_CCR_PUSHW0_MASK | XCACHE_CCR_PUSHW1_MASK | XCACHE_CCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((XCACHE_PC->CCR & XCACHE_CCR_GO_MASK) != 0x00U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        XCACHE_PC->CCR &= ~(XCACHE_CCR_PUSHW0_MASK | XCACHE_CCR_PUSHW1_MASK);
        XCACHE_PC->CCR &= ~XCACHE_CCR_ENCACHE_MASK;
        __ISB();
        __DSB();
    }

    if ((XCACHE_PS->CCR & XCACHE_CCR_ENCACHE_MASK) == 1U) /* disabled if enabled */
    {
        /* Enable the to push all modified lines. */
        XCACHE_PS->CCR |= XCACHE_CCR_PUSHW0_MASK | XCACHE_CCR_PUSHW1_MASK | XCACHE_CCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((XCACHE_PS->CCR & XCACHE_CCR_GO_MASK) != 0x00U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        XCACHE_PS->CCR &= ~(XCACHE_CCR_PUSHW0_MASK | XCACHE_CCR_PUSHW1_MASK);
        XCACHE_PS->CCR &= ~XCACHE_CCR_ENCACHE_MASK;
        __ISB();
        __DSB();
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* Attr0: device. */
    ARM_MPU_SetMemAttr(0U, ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE));

    /* Attr1: non cacheable. */
    ARM_MPU_SetMemAttr(1U, ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));

    /* Attr2: non transient, write through, read allocate. */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 0U, 1U, 0U);
    ARM_MPU_SetMemAttr(2U, ARM_MPU_ATTR(attr, attr));

    /* Attr3: non transient, write back, read/write allocate. */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 1U);
    ARM_MPU_SetMemAttr(3U, ARM_MPU_ATTR(attr, attr));

    /* NOTE1: All SRAM is non-cacheable regardless of MPU setting. */
    /* Region 0/1 (Code TCM): [0x1FFE0000, 0x1FFFFFFF], non-shareable, read/write, non-privileged, executable. Attr 1 */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0x0FFE0000U, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x0FFFFFFFU, 1U));
    ARM_MPU_SetRegion(1U, ARM_MPU_RBAR(0x1FFE0000U, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x1FFFFFFFU, 1U));

    /* NOTE: All SRAM is non-cacheable regardless of MPU setting. */
    /* Region 2/3 (System TCM): [0x20000000, 0x2001FFFF], non-shareable, read/write,  non-privileged, executable. Attr 1
     */
    ARM_MPU_SetRegion(2U, ARM_MPU_RBAR(0x20000000U, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x2001FFFF, 1U));
    ARM_MPU_SetRegion(3U, ARM_MPU_RBAR(0x30000000U, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x3001FFFF, 1U));

    /* Region 4/5 (FlexSPI2): [0x14000000, 0x14FFFFFF], non-shareable, read-only,  non-privileged, executable. Attr 3 */
    ARM_MPU_SetRegion(4U, ARM_MPU_RBAR(0x04000000, ARM_MPU_SH_NON, 1U, 1U, 0U), ARM_MPU_RLAR(0x04FFFFFF, 3U));
    ARM_MPU_SetRegion(5U, ARM_MPU_RBAR(0x14000000, ARM_MPU_SH_NON, 1U, 1U, 0U), ARM_MPU_RLAR(0x14FFFFFF, 3U));

    /* Region 6/7 (OCRAM1/OCRAM2): [0x30480000, 0x3053FFFF], non-shareable, read-write,  non-privileged, executable.
     * Attr 1 (non-cacheable)*/
    ARM_MPU_SetRegion(6U, ARM_MPU_RBAR(0x20480000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x2053FFFF, 1U));
    ARM_MPU_SetRegion(7U, ARM_MPU_RBAR(0x30480000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x3053FFFF, 1U));

    /* Region 8 (Peripherals): [0x40000000, 0x7FFFFFFF], non-shareable, read/write, non-privileged, executable / eXecute
     * never. Attr 0 (device). */
    ARM_MPU_SetRegion(8U, ARM_MPU_RBAR(0x40000000U, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x7FFFFFFF, 0U));

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    // Enable Code XCACHE
    if ((XCACHE_PC->CCR & XCACHE_CCR_ENCACHE_MASK) == 0U) /* set XCACHE if not configured */
    {
        /* set command to invalidate all ways and write GO bit to initiate command */
        XCACHE_PC->CCR = XCACHE_CCR_INVW1_MASK | XCACHE_CCR_INVW0_MASK;
        XCACHE_PC->CCR |= XCACHE_CCR_GO_MASK;
        /* Wait until the command completes */
        while ((XCACHE_PC->CCR & XCACHE_CCR_GO_MASK) != 0U)
        {
        }
        /* Enable cache */
        XCACHE_PC->CCR = XCACHE_CCR_ENCACHE_MASK;

        __ISB();
        __DSB();
    }

    // Enable System XCACHE
    if ((XCACHE_PS->CCR & XCACHE_CCR_ENCACHE_MASK) == 0U) /* set XCACHE if not configured */
    {
        /* set command to invalidate all ways and write GO bit to initiate command */
        XCACHE_PS->CCR = XCACHE_CCR_INVW1_MASK | XCACHE_CCR_INVW0_MASK;
        XCACHE_PS->CCR |= XCACHE_CCR_GO_MASK;
        /* Wait until the command completes */
        while ((XCACHE_PS->CCR & XCACHE_CCR_GO_MASK) != 0U)
        {
        }
        /* Enable cache */
        XCACHE_PS->CCR = XCACHE_CCR_ENCACHE_MASK;

        __ISB();
        __DSB();
    }
}
#endif

void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength)
{
}

void BOARD_MMC_Pin_Config(uint32_t speed, uint32_t strength)
{
}
