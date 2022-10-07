/*-
 * Copyright (c) 2010 Adrian Chadd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* $FreeBSD$ */

#ifndef	__AR72XX_REG_H__
#define	__AR72XX_REG_H__

#define	AR724X_PLL_REG_CPU_CONFIG	AR71XX_PLL_CPU_BASE + 0x00
#define	AR724X_PLL_REG_PCIE_CONFIG	AR71XX_PLL_CPU_BASE + 0x18

#define	AR724X_PLL_DIV_SHIFT		0
#define	AR724X_PLL_DIV_MASK		0x3ff
#define	AR724X_PLL_REF_DIV_SHIFT	10
#define	AR724X_PLL_REF_DIV_MASK		0xf
#define	AR724X_AHB_DIV_SHIFT		19
#define	AR724X_AHB_DIV_MASK		0x1
#define	AR724X_DDR_DIV_SHIFT		22
#define	AR724X_DDR_DIV_MASK		0x3

#define	AR724X_PLL_VAL_1000		0x00110000
#define	AR724X_PLL_VAL_100		0x00001099
#define	AR724X_PLL_VAL_10		0x00991099

#define	AR724X_BASE_FREQ		5000000

#define	AR724X_DDR_REG_FLUSH_GE0	(AR71XX_DDR_CONFIG + 0x7c)
#define	AR724X_DDR_REG_FLUSH_GE1	(AR71XX_DDR_CONFIG + 0x80)
#define	AR724X_DDR_REG_FLUSH_USB	(AR71XX_DDR_CONFIG + 0x84)
#define	AR724X_DDR_REG_FLUSH_PCIE	(AR71XX_DDR_CONFIG + 0x88)

#define	AR724X_RESET_REG_RESET_MODULE	AR71XX_RST_BLOCK_BASE + 0x1c
#define	AR724X_RESET_USB_HOST			(1 << 5)
#define	AR724X_RESET_USB_PHY			(1 << 4)
#define	AR724X_RESET_MODULE_USB_OHCI_DLL	(1 << 3)

#define	AR724X_RESET_GE1_MDIO			(1 << 23)
#define	AR724X_RESET_GE0_MDIO			(1 << 22)
#define	AR724X_RESET_PCIE_PHY_SERIAL		(1 << 10)
#define	AR724X_RESET_PCIE_PHY			(1 << 7)
#define	AR724X_RESET_PCIE			(1 << 6)
#define	AR724X_RESET_USB_HOST			(1 << 5)
#define	AR724X_RESET_USB_PHY			(1 << 4)
#define	AR724X_RESET_USBSUS_OVERRIDE		(1 << 3)

/* XXX so USB requires different init code? -adrian */
#define	AR7240_OHCI_BASE		0x1b000000
#define	AR7240_OHCI_SIZE		0x01000000

#define	AR724X_PCI_CRP_BASE		(AR71XX_APB_BASE + 0x000C0000)
#define	AR724X_PCI_CRP_SIZE		0x100
#define	AR724X_PCI_CFG_BASE		0x14000000
#define	AR724X_PCI_CFG_SIZE		0x1000

#define	AR724X_PCI_CTRL_BASE		(AR71XX_APB_BASE + 0x000F0000)
#define	AR724X_PCI_CTRL_SIZE		0x100

/* PCI config registers - AR724X_PCI_CTRL_BASE */
#define	AR724X_PCI_APP			0x180f0000
#define	AR724X_PCI_APP_LTSSM_ENABLE	(1 << 0)
#define	AR724X_PCI_RESET		0x180f0018
#define	AR724X_PCI_RESET_LINK_UP	(1 << 0)
#define	AR724X_PCI_INTR_STATUS		0x180f004c
#define	AR724X_PCI_INTR_MASK		0x180f0050
#define	AR724X_PCI_INTR_DEV0		(1 << 14)

#define	AR724X_GPIO_FUNC_GE0_MII_CLK_EN		(1 << 19)
#define	AR724X_GPIO_FUNC_SPI_EN			(1 << 18)
#define	AR724X_GPIO_FUNC_SPI_CS_EN2		(1 << 14)
#define	AR724X_GPIO_FUNC_SPI_CS_EN1		(1 << 13)
#define	AR724X_GPIO_FUNC_CLK_OBS5_EN		(1 << 12)
#define	AR724X_GPIO_FUNC_CLK_OBS4_EN		(1 << 11)
#define	AR724X_GPIO_FUNC_CLK_OBS3_EN		(1 << 10)
#define	AR724X_GPIO_FUNC_CLK_OBS2_EN		(1 << 9)
#define	AR724X_GPIO_FUNC_CLK_OBS1_EN		(1 << 8)
#define	AR724X_GPIO_FUNC_ETH_SWITCH_LED4_EN	(1 << 7)
#define	AR724X_GPIO_FUNC_ETH_SWITCH_LED3_EN	(1 << 6)
#define	AR724X_GPIO_FUNC_ETH_SWITCH_LED2_EN	(1 << 5)
#define	AR724X_GPIO_FUNC_ETH_SWITCH_LED1_EN	(1 << 4)
#define	AR724X_GPIO_FUNC_ETH_SWITCH_LED0_EN	(1 << 3)
#define	AR724X_GPIO_FUNC_UART_RTS_CTS_EN	(1 << 2)
#define	AR724X_GPIO_FUNC_UART_EN		(1 << 1)
#define	AR724X_GPIO_FUNC_JTAG_DISABLE		(1 << 0)

#endif
