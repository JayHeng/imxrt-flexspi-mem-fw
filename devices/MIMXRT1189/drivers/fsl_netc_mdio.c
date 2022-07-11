/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_netc_mdio.h"

/*! @brief MDC standard frequency. */
#define NETC_MDC_FREQUENCY (2500000U)

/*! @brief MDC frequency max clock divisor. */
#define NETC_MDIO_CLK_MAX_DIV_FIELD (0x1FFU)

/*! @brief MDIO hold time in nanosecond. */
#define NETC_MDIO_HOLD_TIME_NS_MIN (10U)

/* TODO: Move to header. */
#define NETC_FEATURE_ENETC_ETH_PORT_NUM (1U)

/*! @brief Pointers to netc function bases for each instance. */
static ENETC_PCI_TYPE0_Type *const s_netcFuncBases[] = ENETC_PCI_TYPE0_BASE_PTRS;

/*! @brief Pointers to eth link bases for each instance. */
static NETC_ETH_LINK_Type *const s_netcEthLinkBases[] = NETC_ETH_LINK_BASE_PTRS;

static uint32_t NETC_MDIOGetFuncInstance(netc_hw_eth_port_idx_t port)
{
    uint32_t instance;

    if ((uint32_t)port > NETC_FEATURE_ENETC_ETH_PORT_NUM - 1U)
    {
        instance = 2;
    }
    else
    {
        instance = 3U + (uint32_t)port;
    }

    return instance;
}

/*
 * Port external MDIO Access functions
 */

static status_t NETC_PEMDIO_Init(NETC_ETH_LINK_Type *base, netc_mdio_config_t *mdioConfig)
{
    bool eHold = false;
    uint32_t holdCycle;
    uint32_t mdioHold;
    uint32_t config;
    uint32_t div;

    /* Set the divisor MDC. */
    div = mdioConfig->srcClockHz / NETC_MDC_FREQUENCY / 2U;
    if (div > NETC_MDIO_CLK_MAX_DIV_FIELD)
    {
        return kStatus_Fail;
    }

    /* A hold time of 10ns is required. */
    holdCycle = (NETC_MDIO_HOLD_TIME_NS_MIN + NETC_NANOSECOND_ONE_SECOND / mdioConfig->srcClockHz - 1U) /
                (NETC_NANOSECOND_ONE_SECOND / mdioConfig->srcClockHz);

    if ((holdCycle > 57U) || (holdCycle == 0U))
    {
        return kStatus_Fail;
    }
    else if (holdCycle > 15U)
    {
        eHold    = true;
        mdioHold = (holdCycle - 2U) / 8U + 1U;
    }
    else
    {
        mdioHold = holdCycle / 2U;
    }

    config = base->PEMDIOCR & NETC_ETH_LINK_PEMDIOCR_WHOAMI_MASK;
    config |= NETC_ETH_LINK_PEMDIOCR_NEG(mdioConfig->isNegativeDriven) | NETC_ETH_LINK_PEMDIOCR_MDIO_CLK_DIV(div) |
              NETC_ETH_LINK_PEMDIOCR_PRE_DIS(mdioConfig->isPreambleDisable) |
              NETC_ETH_LINK_PEMDIOCR_MDIO_HOLD(mdioHold) | NETC_ETH_LINK_PEMDIOCR_EHOLD(eHold);
    base->PEMDIOCR = config;

    return kStatus_Success;
}

static bool NETC_PEMDIO_IsSMIBusy(NETC_ETH_LINK_Type *base)
{
    return ((base->PEMDIOCR & NETC_ETH_LINK_PEMDIOCR_BSY1_MASK) != 0U) ? true : false;
}

static bool NETC_PEMDIO_IsPhyAddrErr(NETC_ETH_LINK_Type *base)
{
    return ((base->PEMDIOCR & NETC_ETH_LINK_PEMDIOCR_ADDR_ERR_MASK) != 0U);
}

static bool NETC_PEMDIO_IsReadErr(NETC_ETH_LINK_Type *base)
{
    return ((base->PEMDIOCR & NETC_ETH_LINK_PEMDIOCR_MDIO_RD_ER_MASK) != 0U);
}

static status_t NETC_PEMDIO_Write(NETC_ETH_LINK_Type *base, uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    status_t result = kStatus_Success;

    base->PEMDIOCR &= ~NETC_ETH_LINK_PEMDIOCR_ENC45_MASK;
    base->PEMDIOICR = NETC_ETH_LINK_PEMDIOICR_PORT_ADDR(phyAddr) | NETC_ETH_LINK_PEMDIOICR_DEV_ADDR(regAddr);
    base->PEMDIOIDR = data;

    while (NETC_PEMDIO_IsSMIBusy(base))
    {
    }

    if (NETC_PEMDIO_IsPhyAddrErr(base))
    {
        result = kStatus_Fail;
    }

    return result;
}

static status_t NETC_PEMDIO_Read(NETC_ETH_LINK_Type *base, uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    status_t result = kStatus_Success;

    base->PEMDIOCR &= ~NETC_ETH_LINK_PEMDIOCR_ENC45_MASK;
    base->PEMDIOICR = NETC_ETH_LINK_PEMDIOICR_READ_MASK | NETC_ETH_LINK_PEMDIOICR_PORT_ADDR(phyAddr) |
                      NETC_ETH_LINK_PEMDIOICR_DEV_ADDR(regAddr);

    while (NETC_PEMDIO_IsSMIBusy(base))
    {
    }

    if (NETC_PEMDIO_IsReadErr(base) || NETC_PEMDIO_IsPhyAddrErr(base))
    {
        result = kStatus_Fail;
    }
    *pData = base->PEMDIOIDR;

    return result;
}

static status_t NETC_PEMDIO_C45Write(
    NETC_ETH_LINK_Type *base, uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t data)
{
    base->PEMDIOCR |= NETC_ETH_LINK_PEMDIOCR_ENC45_MASK;
    base->PEMDIOICR = NETC_ETH_LINK_PEMDIOICR_PORT_ADDR(portAddr) | NETC_ETH_LINK_PEMDIOICR_DEV_ADDR(devAddr);
    base->PEMDIORAR = NETC_ETH_LINK_PEMDIORAR_REGADDR(regAddr);

    while (NETC_PEMDIO_IsSMIBusy(base))
    {
    }

    if (NETC_PEMDIO_IsPhyAddrErr(base))
    {
        return kStatus_Fail;
    }

    base->PEMDIOIDR = data;
    while (NETC_PEMDIO_IsSMIBusy(base))
    {
    }

    return kStatus_Success;
}

static status_t NETC_PEMDIO_C45Read(
    NETC_ETH_LINK_Type *base, uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t *pData)
{
    base->PEMDIOCR |= NETC_ETH_LINK_PEMDIOCR_ENC45_MASK;
    base->PEMDIOICR = NETC_ETH_LINK_PEMDIOICR_PORT_ADDR(portAddr) | NETC_ETH_LINK_PEMDIOICR_DEV_ADDR(devAddr);
    base->PEMDIORAR = NETC_ETH_LINK_PEMDIORAR_REGADDR(regAddr);
    while (NETC_PEMDIO_IsSMIBusy(base))
    {
    }

    if (NETC_PEMDIO_IsPhyAddrErr(base))
    {
        return kStatus_Fail;
    }

    base->PEMDIOICR = NETC_ETH_LINK_PEMDIOICR_READ_MASK | NETC_ETH_LINK_PEMDIOICR_PORT_ADDR(portAddr) |
                      NETC_ETH_LINK_PEMDIOICR_DEV_ADDR(devAddr);
    while (NETC_PEMDIO_IsSMIBusy(base))
    {
    }

    if (NETC_PEMDIO_IsReadErr(base))
    {
        return kStatus_Fail;
    }

    *pData = base->PEMDIOIDR;
    return kStatus_Success;
}

/*
 * Port internal MDIO Access functions
 */

static status_t NETC_PIMDIO_Init(NETC_ETH_LINK_Type *base, netc_mdio_config_t *mdioConfig)
{
    uint32_t holdCycle;
    uint32_t mdioHold;
    uint32_t div;

    /* Set the divisor MDC. */
    div = mdioConfig->srcClockHz / NETC_MDC_FREQUENCY / 2U;
    if (div > NETC_MDIO_CLK_MAX_DIV_FIELD)
    {
        return kStatus_Fail;
    }

    /* A hold time of 10ns is required. */
    holdCycle = (NETC_MDIO_HOLD_TIME_NS_MIN + NETC_NANOSECOND_ONE_SECOND / mdioConfig->srcClockHz - 1U) /
                (NETC_NANOSECOND_ONE_SECOND / mdioConfig->srcClockHz);

    if ((holdCycle > 15U) || (holdCycle == 0U))
    {
        return kStatus_Fail;
    }
    else
    {
        mdioHold = holdCycle / 2U;
    }

    base->PM0_MDIO_CFG = NETC_ETH_LINK_PM0_MDIO_CFG_MDIO_CLK_DIV(div) |
                         NETC_ETH_LINK_PM0_MDIO_CFG_PRE_DIS(mdioConfig->isPreambleDisable) |
                         NETC_ETH_LINK_PM0_MDIO_CFG_MDIO_HOLD(mdioHold);

    return kStatus_Success;
}

static bool NETC_PIMDIO_IsSMIBusy(NETC_ETH_LINK_Type *base)
{
    return ((base->PM0_MDIO_CTL & NETC_ETH_LINK_PM0_MDIO_CTL_BSY_MASK) != 0U) ? true : false;
}

static void NETC_PIMDIO_Write(NETC_ETH_LINK_Type *base, uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    base->PM0_MDIO_CTL  = NETC_ETH_LINK_PM0_MDIO_CTL_PORT_ADDR(phyAddr) | NETC_ETH_LINK_PM0_MDIO_CTL_DEV_ADDR(regAddr);
    base->PM0_MDIO_DATA = data;
    while (NETC_PIMDIO_IsSMIBusy(base))
    {
    }
}

static void NETC_PIMDIO_Read(NETC_ETH_LINK_Type *base, uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    base->PM0_MDIO_CTL = NETC_ETH_LINK_PM0_MDIO_CTL_READ_MASK | NETC_ETH_LINK_PM0_MDIO_CTL_PORT_ADDR(phyAddr) |
                         NETC_ETH_LINK_PM0_MDIO_CTL_DEV_ADDR(regAddr);
    while (NETC_PEMDIO_IsSMIBusy(base))
    {
    }

    *pData = base->PEMDIOIDR;
}

/*
 * MDIO API Layer MDIO Access functions
 */

status_t NETC_MDIOInit(netc_mdio_handle_t *handle, netc_mdio_config_t *config)
{
    assert(config->srcClockHz != 0U);

    NETC_ETH_LINK_Type *base;
    uint32_t instance;
    status_t result;

    handle->mdio = config->mdio;

    /* Enable master bus and memory access for PCIe and MSI-X */
    instance = NETC_MDIOGetFuncInstance(handle->mdio.port);
    s_netcFuncBases[instance]->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    base = s_netcEthLinkBases[handle->mdio.port];

    if (handle->mdio.type == kNETC_ExternalMdio)
    {
        result = NETC_PEMDIO_Init(base, config);
    }
    else
    {
        result = NETC_PIMDIO_Init(base, config);
    }

    return result;
}

status_t NETC_MDIOWrite(netc_mdio_handle_t *handle, uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    NETC_ETH_LINK_Type *base = s_netcEthLinkBases[handle->mdio.port];
    status_t result;

    if (handle->mdio.type == kNETC_ExternalMdio)
    {
        result = NETC_PEMDIO_Write(base, phyAddr, regAddr, data);
    }
    else
    {
        NETC_PIMDIO_Write(base, phyAddr, regAddr, data);
        result = kStatus_Success;
    }

    return result;
}

status_t NETC_MDIORead(netc_mdio_handle_t *handle, uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    NETC_ETH_LINK_Type *base = s_netcEthLinkBases[handle->mdio.port];
    status_t result;

    if (handle->mdio.type == kNETC_ExternalMdio)
    {
        result = NETC_PEMDIO_Read(base, phyAddr, regAddr, pData);
    }
    else
    {
        NETC_PIMDIO_Read(base, phyAddr, regAddr, pData);
        result = kStatus_Success;
    }

    return result;
}

status_t NETC_MDIOC45Write(
    netc_mdio_handle_t *handle, uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t data)
{
    NETC_ETH_LINK_Type *base = s_netcEthLinkBases[handle->mdio.port];
    status_t result;

    if (handle->mdio.type == kNETC_ExternalMdio)
    {
        result = NETC_PEMDIO_C45Write(base, portAddr, devAddr, regAddr, data);
    }
    else
    {
        result = kStatus_Fail;
    }

    return result;
}

status_t NETC_MDIOC45Read(
    netc_mdio_handle_t *handle, uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t *pData)
{
    NETC_ETH_LINK_Type *base = s_netcEthLinkBases[handle->mdio.port];
    status_t result;

    if (handle->mdio.type == kNETC_ExternalMdio)
    {
        result = NETC_PEMDIO_C45Read(base, portAddr, devAddr, regAddr, pData);
    }
    else
    {
        result = kStatus_Fail;
    }

    return result;
}

status_t NETC_MDIOSetPhyStatusCheck(netc_mdio_handle_t *handle,
                                    netc_mdio_phy_status_t *config,
                                    netc_mdio_phy_status_callback_t callback)
{
    NETC_ETH_LINK_Type *base = s_netcEthLinkBases[handle->mdio.port];

    if (handle->mdio.type == kNETC_InternalMdio)
    {
        return kStatus_Fail;
    }
    else
    {
        handle->callback = callback;

        base->PPSCR    = NETC_ETH_LINK_PPSCR_STATUS_INTERVAL(config->interval);
        base->PPSCTRLR = NETC_ETH_LINK_PPSCTRLR_PORT_ADDR(config->phyOrPortAddr) |
                         NETC_ETH_LINK_PPSCTRLR_DEV_ADDR(config->regiOrDevAddr);
        base->PPSRAR = NETC_ETH_LINK_PPSRAR_REGADDR(config->c45RegiAddr);
        base->PPSMR  = (uint32_t)((config->enableIntrLow2High << 16U) + config->enableIntrHigh2Low);

        return kStatus_Success;
    }
}

status_t NETC_MDIOPhyStatusCleanEvent(netc_mdio_handle_t *handle, uint16_t eventLow2High, uint16_t eventHigh2Low)
{
    NETC_ETH_LINK_Type *base = s_netcEthLinkBases[handle->mdio.port];

    if (handle->mdio.type == kNETC_InternalMdio)
    {
        return kStatus_Fail;
    }

    base->PPSER = (eventLow2High << 16U) + eventHigh2Low;

    return kStatus_Success;
}

void NETC_MDIOPhyStatusIRQHandler(netc_mdio_handle_t *handle)
{
    NETC_ETH_LINK_Type *base = s_netcEthLinkBases[handle->mdio.port];
    uint32_t statusEvent     = base->PPSER;

    if (handle->callback != NULL)
    {
        handle->callback(handle, (uint16_t)(statusEvent >> 16U), (uint16_t)(statusEvent && 0xFFFFU));
    }
}
