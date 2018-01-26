/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2015 Kuldeep Singh Dhaka <kuldeepdhaka9@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBOPENCM3_EFM32_USB_H
#define LIBOPENCM3_EFM32_USB_H

#include <libopencm3/cm3/common.h>
#include <libopencm3/usb/usbd.h>

#define USB_CTRL			MMIO32(USB_BASE + 0x000)
#define USB_STATUS			MMIO32(USB_BASE + 0x004)
#define USB_IF				MMIO32(USB_BASE + 0x008)
#define USB_IFS				MMIO32(USB_BASE + 0x00C)
#define USB_IFC				MMIO32(USB_BASE + 0x010)
#define USB_IEN				MMIO32(USB_BASE + 0x014)
#define USB_ROUTE			MMIO32(USB_BASE + 0x018)

/* USB_CTRL */
#define USB_CTRL_DMPUAP			(1 << 1)

/* USB_ROUTE */
#define USB_ROUTE_DMPUPEN		(1 << 2)
/* Bit 1 - Reserved */
#define USB_ROUTE_PHYPEN		(1 << 0)

/* Core Global Control and Status Registers */
#define USB_OTG_BASE			(USB_BASE + 0x3C000)
#define USB_GOTGCTL			MMIO32(USB_OTG_BASE + 0x000)
#define USB_GOTGINT			MMIO32(USB_OTG_BASE + 0x004)
#define USB_GAHBCFG			MMIO32(USB_OTG_BASE + 0x008)
#define USB_GUSBCFG			MMIO32(USB_OTG_BASE + 0x00C)
#define USB_GRSTCTL			MMIO32(USB_OTG_BASE + 0x010)
#define USB_GINTSTS			MMIO32(USB_OTG_BASE + 0x014)
#define USB_GINTMSK			MMIO32(USB_OTG_BASE + 0x018)
#define USB_GRXSTSR			MMIO32(USB_OTG_BASE + 0x01C)
#define USB_GRXSTSP			MMIO32(USB_OTG_BASE + 0x020)
#define USB_GRXFSIZ			MMIO32(USB_OTG_BASE + 0x024)
#define USB_GNPTXFSIZ			MMIO32(USB_OTG_BASE + 0x028)
#define USB_GNPTXSTS			MMIO32(USB_OTG_BASE + 0x02C)
#define USB_GDFIFOCFG			MMIO32(USB_OTG_BASE + 0x05C)
#define USB_HPTXFSIZ			MMIO32(USB_OTG_BASE + 0x100)
#define USB_DIEPTXF(x)			\
	MMIO32(USB_OTG_BASE + 0x104 + (4 * ((x) - 1)))

/* Host-mode Control and Status Registers */
#define USB_HCFG			MMIO32(USB_OTG_BASE + 0x400)
#define USB_HFIR			MMIO32(USB_OTG_BASE + 0x404)
#define USB_HFNUM			MMIO32(USB_OTG_BASE + 0x408)
#define USB_HPTXSTS			MMIO32(USB_OTG_BASE + 0x410)
#define USB_HAINT			MMIO32(USB_OTG_BASE + 0x414)
#define USB_HAINTMSK			MMIO32(USB_OTG_BASE + 0x418)
#define USB_HPRT			MMIO32(USB_OTG_BASE + 0x440)
#define USB_HCx_CHAR(x)			\
	MMIO32(USB_OTG_BASE + 0x500 + ((x) * 0x20))
#define USB_HCx_INT(x)			\
	MMIO32(USB_OTG_BASE + 0x508 + ((x) * 0x20))
#define USB_HCx_INTMSK(x)		\
	MMIO32(USB_OTG_BASE + 0x50C + ((x) * 0x20))
#define USB_HCx_TSIZ(x)			\
	MMIO32(USB_OTG_BASE + 0x510 + ((x) * 0x20))
#define USB_HCx_DMAADDR(x)		\
	MMIO32(USB_OTG_BASE + 0x514 + ((x) * 0x20))

/* Device-mode Control and Status Registers */
#define USB_DCFG			MMIO32(USB_OTG_BASE + 0x800)
#define USB_DCTL			MMIO32(USB_OTG_BASE + 0x804)
#define USB_DSTS			MMIO32(USB_OTG_BASE + 0x808)
#define USB_DIEPMSK			MMIO32(USB_OTG_BASE + 0x810)
#define USB_DOEPMSK			MMIO32(USB_OTG_BASE + 0x814)
#define USB_DAINT			MMIO32(USB_OTG_BASE + 0x818)
#define USB_DAINTMSK			MMIO32(USB_OTG_BASE + 0x81C)
#define USB_DVBUSDIS			MMIO32(USB_OTG_BASE + 0x828)
#define USB_DVBUSPULSE			MMIO32(USB_OTG_BASE + 0x82C)
#define USB_DIEPEMPMSK			MMIO32(USB_OTG_BASE + 0x834)

#define USB_DIEPx_CTL(x)		\
	MMIO32(USB_OTG_BASE + 0x900 + ((x) * 0x20))
#define USB_DIEPx_INT(x)		\
	MMIO32(USB_OTG_BASE + 0x908 + ((x) * 0x20))
#define USB_DIEPx_TSIZ(x)		\
	MMIO32(USB_OTG_BASE + 0x910 + ((x) * 0x20))
#define USB_DIEP0CTL			USB_DIEPx_CTL(0)
#define USB_DIEP0TSIZ			USB_DIEPx_TSIZ(0)
#define USB_DIEP0INT			USB_DIEPx_INT(0)

#define USB_DOEPx_CTL(x)		\
	MMIO32(USB_OTG_BASE + 0xB00 + ((x) * 0x20))
#define USB_DOEPx_INT(x)		\
	MMIO32(USB_OTG_BASE + 0xB08 + ((x) * 0x20))
#define USB_DOEPx_TSIZ(x)		\
	MMIO32(USB_OTG_BASE + 0xB10 + ((x) * 0x20))
#define USB_DOEP0CTL			USB_DOEPx_CTL(0)
#define USB_DOEP0TSIZ			USB_DOEPx_TSIZ(0)
#define USB_DOEP0INT			USB_DOEPx_INT(0)

/* Power and clock gating control and status register */
#define USB_PCGCCTL			MMIO32(USB_OTG_BASE + 0xE00)

/* Data FIFO */
#define USB_FIFOxD(x)			\
	(&MMIO32(USB_OTG_BASE + (((x) + 1) << 12)))

/* Global CSRs */
/* USB control registers (OTG_HS_GOTGCTL) */
#define USB_GOTGCTL_BSVLD		(1 << 19)
#define USB_GOTGCTL_ASVLD		(1 << 18)
#define USB_GOTGCTL_DBCT		(1 << 17)
#define USB_GOTGCTL_CIDSTS		(1 << 16)
#define USB_GOTGCTL_DHNPEN		(1 << 11)
#define USB_GOTGCTL_HSHNPEN		(1 << 10)
#define USB_GOTGCTL_HNPRQ		(1 << 9)
#define USB_GOTGCTL_HNGSCS		(1 << 8)
#define USB_GOTGCTL_SRQ			(1 << 1)
#define USB_GOTGCTL_SRQSCS		(1 << 0)

/* AHB configuration register (USB_GAHBCFG) */
#define USB_GAHBCFG_GLBLINTRMSK		0x0001
#define USB_GAHBCFG_TXFELVL		0x0080

/* USB configuration register (USB_GUSBCFG) */
#define USB_GUSBCFG_CURRUPTTXPKT         (1 << 31)
/* Bits 30:29 - Reserved */
#define USB_GUSBCFGTXENDDELAY           (1 << 28)
/* Bits 27:23 - Reserved */
#define USB_GUSBCFG_TERMSELDLPULSE       (1 << 22)
/* Bits 21:14 - Reserved */
#define USB_GUSBCFG_TRDT_MASK		(0xf << 10)
#define USB_GUSBCFG_TRDT_16BIT		(0x5 << 10)
#define USB_GUSBCFG_TRDT_8BIT		(0x9 << 10)
/* Bits 9:6 - Reserved */
#define USB_GUSBCFG_FSINTF      (1 << 5)
/* Bits 4:3 - Reserved */
#define USB_GUSBCFG_TOCAL_MASK		0x3

/* reset register (USB_GRSTCTL) */
#define USB_GRSTCTL_AHBIDL		(1 << 31)
#define USB_GRSTCTL_DMAREQ		(1 << 30)
/* Bits 29:11 - Reserved */
#define USB_GRSTCTL_TXFNUM_MASK	(0x1f << 6)
#define USB_GRSTCTL_TXFFLSH		(1 << 5)
#define USB_GRSTCTL_RXFFLSH		(1 << 4)
/* Bit 3:2 - Reserved */
#define USB_GRSTCTL_HSRST		(1 << 1)
#define USB_GRSTCTL_CSRST		(1 << 0)

/* interrupt status register (USB_GINTSTS) */
#define USB_GINTSTS_WKUPINT		(1 << 31)
/* Bits 30:24 - Reserved */
#define USB_GINTSTS_RESETDET	(1 << 23)
#define USB_GINTSTS_FETSUSP		(1 << 22)
#define USB_GINTSTS_INCOMPLP   	(1 << 21)
#define USB_GINTSTS_INCOMPISOIN	(1 << 20)
#define USB_GINTSTS_OEPINT		(1 << 19)
#define USB_GINTSTS_IEPINT		(1 << 18)
/* Bits 17:16 - Reserved */
#define USB_GINTSTS_EOPF		(1 << 15)
#define USB_GINTSTS_ISOODRP		(1 << 14)
#define USB_GINTSTS_ENUMDNE		(1 << 13)
#define USB_GINTSTS_USBRST		(1 << 12)
#define USB_GINTSTS_USBSUSP		(1 << 11)
#define USB_GINTSTS_ESUSP		(1 << 10)
/* Bits 9:8 - Reserved */
#define USB_GINTSTS_GONAKEFF	(1 << 7)
#define USB_GINTSTS_GINAKEFF	(1 << 6)
/* Bit 5 - Reserved */
#define USB_GINTSTS_RXFLVL		(1 << 4)
#define USB_GINTSTS_SOF			(1 << 3)
/* Bits 2:1 - Reserved */
#define USB_GINTSTS_CMOD		(1 << 0)

/* interrupt mask register (USB_GINTMSK) */
#define USB_GINTMSK_MMISM		(1 << 1)
#define USB_GINTMSK_SOFM		(1 << 3)
#define USB_GINTMSK_RXFLVLM		(1 << 4)

#define USB_GINTMSK_GINAKEFFM	(1 << 6)
#define USB_GINTMSK_GONAKEFFM	(1 << 7)

#define USB_GINTMSK_ESUSPM		(1 << 10)
#define USB_GINTMSK_USBSUSPM	(1 << 11)
#define USB_GINTMSK_USBRST		(1 << 12)
#define USB_GINTMSK_ENUMDNEM	(1 << 13)
#define USB_GINTMSK_ISOODRPM	(1 << 14)
#define USB_GINTMSK_EOPFM		(1 << 15)

#define USB_GINTMSK_IEPINT	   	   (1 << 18)
#define USB_GINTMSK_OEPINT	       (1 << 19)
#define USB_GINTMSK_INCOMPISOINMSK (1 << 20)
#define USB_GINTMSK_INCOMPLPMSK	   (1 << 21)
#define USB_GINTMSK_FETSUSPMSK	   (1 << 22)
#define USB_GINTMSK_RESETDETMSK	   (1 << 23)
#define USB_GINTMSK_WUIM	       (1 << 31)

/* Receive Status Pop Register (USB_GRXSTSP) */
/* Bits 31:25 - Reserved */
#define USB_GRXSTSP_FRMNUM_MASK		(0xf << 21)
#define USB_GRXSTSP_PKTSTS_MASK		(0xf << 17)
#define USB_GRXSTSP_PKTSTS_GOUTNAK	(0x1 << 17)
#define USB_GRXSTSP_PKTSTS_OUT		(0x2 << 17)
#define USB_GRXSTSP_PKTSTS_OUT_COMP	(0x3 << 17)
#define USB_GRXSTSP_PKTSTS_SETUP_COMP	(0x4 << 17)
#define USB_GRXSTSP_PKTSTS_SETUP	(0x6 << 17)
#define USB_GRXSTSP_DPID_MASK		(0x3 << 15)
#define USB_GRXSTSP_DPID_DATA0		(0x0 << 15)
#define USB_GRXSTSP_DPID_DATA1		(0x2 << 15)
#define USB_GRXSTSP_DPID_DATA2		(0x1 << 15)
#define USB_GRXSTSP_DPID_MDATA		(0x3 << 15)
#define USB_GRXSTSP_BCNT_MASK		(0x7ff << 4)
#define USB_GRXSTSP_EPNUM_MASK		(0xf << 0)

/* general core configuration register (USB_GCCFG) */
/* Bits 31:22 - Reserved */
#define USB_GCCFG_NOVBUSSENS		(1 << 21)
#define USB_GCCFG_SOFOUTEN		(1 << 20)
#define USB_GCCFG_VBUSBSEN		(1 << 19)
#define USB_GCCFG_VBUSASEN		(1 << 18)
/* Bit 17 - Reserved */
#define USB_GCCFG_PWRDWN		(1 << 16)
/* Bits 15:0 - Reserved */


/* Device-mode CSRs */
/* device control register (USB_DCTL) */
/* Bits 31:12 - Reserved */
#define USB_DCTL_POPRGDNE		(1 << 11)
#define USB_DCTL_CGONAK			(1 << 10)
#define USB_DCTL_SGONAK			(1 << 9)
#define USB_DCTL_SGINAK			(1 << 8)
#define USB_DCTL_TCTL_MASK		(7 << 4)
#define USB_DCTL_GONSTS			(1 << 3)
#define USB_DCTL_GINSTS			(1 << 2)
#define USB_DCTL_SDIS			(1 << 1)
#define USB_DCTL_RWUSIG			(1 << 0)

/* device configuration register (USB_DCFG) */
#define USB_RESVALID_MASK           (0x3F << 26)
/* Bits 25:16 - Reserved */
#define USB_DCFG_ERRATICINTMASK      (1 << 15)
/* Bits 14:13 - Reserved */
#define USB_DCFG_PERFRINT_MASK    	(0x2 << 11)
#define USB_DCFG_DEVADDR_MASK		(0x7F << 4)
#define USB_DCFG_ENA32KHZUSP		(1 << 3)
#define USB_DCFG_NZSTSOUTHSHK		(1 << 2)
#define USB_DCFG_DEVSPD_MASK		(0x3 << 0)
#define USB_DCFG_DEVSPD_FS	    	(0x3 << 0)
#define USB_DCFG_DEVSPD_LS	    	(0x2 << 0)

/* Device IN Endpoint Common Interrupt Mask Register (USB_DIEPMSK) */
/* Bits 31:10 - Reserved */
#define USB_DIEPMSK_BIM			(1 << 9)
#define USB_DIEPMSK_TXFURM		(1 << 8)
/* Bit 7 - Reserved */
#define USB_DIEPMSK_INEPNEM		(1 << 6)
#define USB_DIEPMSK_INEPNMM		(1 << 5)
#define USB_DIEPMSK_ITTXFEMSK		(1 << 4)
#define USB_DIEPMSK_TOM			(1 << 3)
/* Bit 2 - Reserved */
#define USB_DIEPMSK_EPDM		(1 << 1)
#define USB_DIEPMSK_XFRCM		(1 << 0)

/* Device OUT Endpoint Common Interrupt Mask Register (USB_DOEPMSK) */
/* Bits 31:10 - Reserved */
#define USB_DOEPMSK_BOIM		(1 << 9)
#define USB_DOEPMSK_OPEM		(1 << 8)
/* Bit 7 - Reserved */
#define USB_DOEPMSK_B2BSTUP		(1 << 6)
/* Bit 5 - Reserved */
#define USB_DOEPMSK_OTEPDM		(1 << 4)
#define USB_DOEPMSK_STUPM		(1 << 3)
/* Bit 2 - Reserved */
#define USB_DOEPMSK_EPDM		(1 << 1)
#define USB_DOEPMSK_XFRCM		(1 << 0)

/* Device Control IN Endpoint 0 Control Register (USB_DIEP0CTL) */
#define USB_DIEP0CTL_EPENA		(1 << 31)
#define USB_DIEP0CTL_EPDIS		(1 << 30)
/* Bits 29:28 - Reserved */
#define USB_DIEP0CTL_SD0PID		(1 << 28)
#define USB_DIEP0CTL_SNAK		(1 << 27)
#define USB_DIEP0CTL_CNAK		(1 << 26)
#define USB_DIEP0CTL_TXFNUM_MASK	(0xf << 22)
#define USB_DIEP0CTL_STALL		(1 << 21)
/* Bit 20 - Reserved */
#define USB_DIEP0CTL_EPTYP_MASK		(0x3 << 18)
#define USB_DIEP0CTL_NAKSTS		(1 << 17)
/* Bit 16 - Reserved */
#define USB_DIEP0CTL_USBAEP		(1 << 15)
/* Bits 14:2 - Reserved */
#define USB_DIEP0CTL_MPSIZ_MASK		(0x3 << 0)
#define USB_DIEP0CTL_MPSIZ_64		(0x0 << 0)
#define USB_DIEP0CTL_MPSIZ_32		(0x1 << 0)
#define USB_DIEP0CTL_MPSIZ_16		(0x2 << 0)
#define USB_DIEP0CTL_MPSIZ_8		(0x3 << 0)

/* Device Control OUT Endpoint 0 Control Register (USB_DOEP0CTL) */
#define USB_DOEP0CTL_EPENA		(1 << 31)
#define USB_DOEP0CTL_EPDIS		(1 << 30)
/* Bits 29:28 - Reserved */
#define USB_DOEP0CTL_SD0PID		(1 << 28)
#define USB_DOEP0CTL_SNAK		(1 << 27)
#define USB_DOEP0CTL_CNAK		(1 << 26)
/* Bits 25:22 - Reserved */
#define USB_DOEP0CTL_STALL		(1 << 21)
#define USB_DOEP0CTL_SNPM		(1 << 20)
#define USB_DOEP0CTL_EPTYP_MASK		(0x3 << 18)
#define USB_DOEP0CTL_NAKSTS		(1 << 17)
/* Bit 16 - Reserved */
#define USB_DOEP0CTL_USBAEP		(1 << 15)
/* Bits 14:2 - Reserved */
#define USB_DOEP0CTL_MPSIZ_MASK		(0x3 << 0)
#define USB_DOEP0CTL_MPSIZ_64		(0x0 << 0)
#define USB_DOEP0CTL_MPSIZ_32		(0x1 << 0)
#define USB_DOEP0CTL_MPSIZ_16		(0x2 << 0)
#define USB_DOEP0CTL_MPSIZ_8		(0x3 << 0)

/* Device IN Endpoint Interrupt Register (USB_DIEPINTx) */
/* Bits 31:8 - Reserved */
#define USB_DIEP_INT_TXFE		(1 << 7)
#define USB_DIEP_INT_INEPNE		(1 << 6)
/* Bit 5 - Reserved */
#define USB_DIEP_INT_ITTXFE		(1 << 4)
#define USB_DIEP_INT_TOC		(1 << 3)
/* Bit 2 - Reserved */
#define USB_DIEP_INT_EPDISD		(1 << 1)
#define USB_DIEP_INT_XFRC		(1 << 0)

/* Device IN Endpoint Interrupt Register (USB_DOEPINTx) */
/* Bits 31:7 - Reserved */
#define USB_DOEP_INT_B2BSTUP		(1 << 6)
/* Bit 5 - Reserved */
#define USB_DOEP_INT_OTEPDIS		(1 << 4)
#define USB_DOEP_INT_SETUP		(1 << 3)
/* Bit 2 - Reserved */
#define USB_DOEP_INT_EPDISD		(1 << 1)
#define USB_DOEP_INT_XFRC		(1 << 0)

/* Device OUT Endpoint 0 Transfer Size Register (USB_DOEP0TSIZ) */
/* Bit 31 - Reserved */
#define USB_DIEP0TSIZ_STUPCNT_1		(0x1 << 29)
#define USB_DIEP0TSIZ_STUPCNT_2		(0x2 << 29)
#define USB_DIEP0TSIZ_STUPCNT_3		(0x3 << 29)
#define USB_DIEP0TSIZ_STUPCNT_MASK	(0x3 << 29)
/* Bits 28:20 - Reserved */
#define USB_DIEP0TSIZ_PKTCNT		(1 << 19)
/* Bits 18:7 - Reserved */
#define USB_DIEP0TSIZ_XFRSIZ_MASK	(0x7f << 0)

#endif

