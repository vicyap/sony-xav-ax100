#ifndef __GPIO_FUNC_SEL_DVB_EVB_TYPE1_H
#define __GPIO_FUNC_SEL_DVB_EVB_TYPE1_H

//********************************************************************
//							function select area
//********************************************************************

#define	SUPPORT_3IN1_CARD
#define	SUPPORT_3IN1_CARD_TWO_SENSE_PIN

#if (FTL_SUPPORT == 1)
	#define	SUPPORT_NAND_FLASH
#endif
#define	SUPPORT_SPDIF_IN

#define SUPPORT_SPDIF_OUT

#define	SUPPORT_SERIAL_TS

#if defined(SMARTCARD_USE_UART1) || defined(SUPPORT_DUAL_SMARTCARD)
#define	SUPPORT_SMART_CARD
#endif
#if defined(SMARTCARD_USE_UART2) || defined(SUPPORT_DUAL_SMARTCARD)
#define	SUPPORT_SMART_CARD1
#endif

#define SUPPORT_HDMI

#define SUPPORT_SCART_TV	//maolin.yang 100209 OPEN
#ifdef SUPPORT_SCART_TV
#define SUPPORT_SCART_AUTO_LOOP
#endif

#define SUPPORT_GPIO_MUTE

#define STB_GPIO_MUTE

#define SUPPORT_IR

//#define EPHY_LED_PINOUT1

#ifdef RISC_CFG_IOP_USE_GPAD_GPIO
#define SUPPORT_GAMEPAD_FUNCTION  // lijin open for Game pad 
#else 
#define SUPPORT_VFD //lizhan open for 1500 vfd 
#endif

//#define ADD_PC_VGA_OUT

#define SUPPORT_UART_NORMAL

#ifndef DVDRELEASE
	#define SUPPORT_UART_DBG
#endif

//#define SUPPORT_ATA_FUNCTION
//#define SUPPORT_CCIR656_OUTPUT
#ifndef  SUPPORT_ATA_FUNCTION
//#ifdef CI_WITH_PCMCIA
//#define SUPPORT_PCMCIA
//#endif
#ifdef CI_WITH_SPV611
	#define SUPPORT_SPV611
#endif
#endif

//#if ((PLT_PINMUX_TYPE == DVBS_216PIN_TYPE_1))
	#define SUPPORT_DVBS_NIM
//#endif

//#if (PLT_PINMUX_TYPE == SBTVD_DEMO_TYPE_1)	//haleliu 20100505 add for SBTVD
//	#define SUPPORT_SBTVD_NIM
//#endif

//#define SUPPORT_I2S_OUT
//#define SUPPORT_I2S_IN

#endif//__GPIO_FUNC_SEL_DVB_EVB_TYPE1_H