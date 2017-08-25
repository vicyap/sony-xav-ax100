#ifndef __1502_216PIN_PINMUX_DVBS_TYPE1_H__
#define __1502_216PIN_PINMUX_DVBS_TYPE1_H__

//*******************************************
// 		following functions are supported
// SPI FLASH
// SUPPORT_3IN1_CARD
// SERIAL DVBT
// SPDIF IN/OUT
// SMART CARD
// SDIO
// SCART TV
// HDMI: CEC,master I2C, HPD
//*******************************************

//*******************************************//
//	pinmux type
//*******************************************//

// 3 IN 1 CARD
#define	CARD_3in1_PINOUT1
//#define	CARD_3in1_PINOUT2

#define	GPIO_CARD_RST_PIN	        	letb1502_PIN_R_MX18 //GPIO_define,CARD_PWR
#define	GPIO_CARD_SENSE1_PIN        	letb1502_PIN_R_MX16 //GPIO_define,SD_INS
#define	GPIO_CARD_SENSE2_PIN        	letb1502_PIN_R_MX17 //GPIO_define,MS_INS
#if defined(CARD_3in1_PINOUT1)//default pinout
#define	GPIO_CARD_CLK				letb1502_PIN_R_MX15
//#define	GPIO_CARD_CLK1				letb1502_PIN_R_MX15// temp for CLK1
#define	GPIO_CARD_CMD				letb1502_PIN_R_MX14
#define	GPIO_CARD_D0				letb1502_PIN_R_MX10
#define	GPIO_CARD_D1				letb1502_PIN_R_MX11
#define	GPIO_CARD_D2				letb1502_PIN_R_MX12
#define	GPIO_CARD_D3				letb1502_PIN_R_MX13
#elif defined(CARD_3in1_PINOUT2)
#define	CARD_3in1_PINOUT2
#define	GPIO_CARD_CLK				letb1502_PIN_B_MX10
//#define	GPIO_CARD_CLK1				letb1502_PIN// temp for CLK1   //disable ci card clock
#define	GPIO_CARD_CMD				letb1502_PIN_B_MX9
#define	GPIO_CARD_D0				letb1502_PIN_B_MX5
#define	GPIO_CARD_D1				letb1502_PIN_B_MX6
#define	GPIO_CARD_D2				letb1502_PIN_B_MX7
#define	GPIO_CARD_D3				letb1502_PIN_B_MX8
#endif


//NAND FLASH
#if (FTL_SUPPORT == 1)
#define	NAND_FLASH_PINOUT1
//#define	NAND_FLASH_PINOUT2
#endif

#if defined(NAND_FLASH_PINOUT1)//default pinout
#define	GPIO_SMC_WP				letb1502_PIN_R_MX33
#define	GPIO_SMC_WE				letb1502_PIN_R_MX32
#define	GPIO_SMC_ALE				letb1502_PIN_R_MX31
#define	GPIO_SMC_CLE				letb1502_PIN_R_MX30
#define	GPIO_SMC_CE0				letb1502_PIN_R_MX28
#define	GPIO_SMC_CE1				letb1502_PIN_R_MX29
#define	GPIO_SMC_RE				letb1502_PIN_R_MX27
#define	GPIO_SMC_RDY				letb1502_PIN_R_MX34
#define	GPIO_SMC_D7				letb1502_PIN_R_MX19
#define	GPIO_SMC_D6				letb1502_PIN_R_MX20
#define	GPIO_SMC_D5				letb1502_PIN_R_MX21
#define	GPIO_SMC_D4				letb1502_PIN_R_MX22
#define	GPIO_SMC_D3				letb1502_PIN_R_MX23
#define	GPIO_SMC_D2				letb1502_PIN_R_MX24
#define	GPIO_SMC_D1				letb1502_PIN_R_MX25
#define	GPIO_SMC_D0				letb1502_PIN_R_MX26
#elif defined(NAND_FLASH_PINOUT2)
#define	GPIO_SMC_WP				letb1502_PIN_R_MX15
#define	GPIO_SMC_WE				letb1502_PIN_R_MX14
#define	GPIO_SMC_ALE				letb1502_PIN_R_MX2
#define	GPIO_SMC_CLE				letb1502_PIN_R_MX1
#define	GPIO_SMC_CE0				letb1502_PIN_R_MX12
#define	GPIO_SMC_CE1				letb1502_PIN_R_MX13
#define	GPIO_SMC_RE				letb1502_PIN_R_MX11
#define	GPIO_SMC_RDY				letb1502_PIN_R_MX0
#define	GPIO_SMC_D7				letb1502_PIN_R_MX3
#define	GPIO_SMC_D6				letb1502_PIN_R_MX4
#define	GPIO_SMC_D5				letb1502_PIN_R_MX5
#define	GPIO_SMC_D4				letb1502_PIN_R_MX6
#define	GPIO_SMC_D3				letb1502_PIN_R_MX7
#define	GPIO_SMC_D2				letb1502_PIN_R_MX8
#define	GPIO_SMC_D1				letb1502_PIN_R_MX9
#define	GPIO_SMC_D0				letb1502_PIN_R_MX10
#endif



//I2C
#ifdef NIMINTERFACE_FOR_T2//qiuwei20100511add for maints86782
#define GPIO_I2C_SDA_PIN			letb1502_PIN_T_MX10  //TS_SDA
#define GPIO_I2C_SCL_PIN			letb1502_PIN_T_MX11  //TS_SCL

#define GPIO_DEF_SDA_PIN			letb1502_PIN_T_MX10  //TS_SDA
#define GPIO_DEF_SCL_PIN			letb1502_PIN_T_MX11  //TS_SCL
#else
#define GPIO_I2C_SDA_PIN			letb1502_PIN_T_MX10  //TS_SDA
#define GPIO_I2C_SCL_PIN			letb1502_PIN_T_MX11  //TS_SCL

#define GPIO_DEF_SDA_PIN			letb1502_PIN
#define GPIO_DEF_SCL_PIN			letb1502_PIN
#endif


#define GPIO_DEMOD_RESET_PIN		letb1502_PIN_T_MX9//RESET_P

#define GPIO_ANTENNA_POWER			letb1502_PIN_L_MX14 
#define GPIO_ANTENNA_SHORT_PROTECT	letb1502_PIN_L_MX15



//Serial TS : Select demux serial pinout
//Serial pinout condition selection
#if (PRIMARY_LIVE_INPUT==0)
//Demux 0:
//#define DEMUX0_SERIAL_PINOUT1
//#define DEMUX0_SERIAL_PINOUT2
//#define DEMUX0_SERIAL_PINOUT3
#define DEMUX0_SERIAL_PINOUT4
//#define DEMUX0_SERIAL_PINOUT5
//#define DEMUX0_SERIAL_PINOUT8
#elif(PRIMARY_LIVE_INPUT==1)
//Demux 1:
//#define DEMUX1_SERIAL_PINOUT1
#define DEMUX1_SERIAL_PINOUT2
//#define DEMUX1_SERIAL_PINOUT3
//#define DEMUX1_SERIAL_PINOUT4
#endif


#ifdef SUPPORT_DUAL_NIM
#if (PRIMARY_LIVE_INPUT==0)
//secondary is demux1
//#define SUB_DEMUX1_SERIAL_PINOUT1
#define SUB_DEMUX1_SERIAL_PINOUT2
//#define SUB_DEMUX1_SERIAL_PINOUT3
//#define SUB_DEMUX1_SERIAL_PINOUT4
//#define SUB_DEMUX1_SERIAL_PINOUT5
//#define SUB_DEMUX1_SERIAL_PINOUT8
#elif(PRIMARY_LIVE_INPUT==1)
//secondary is demux0
//#define SUB_DEMUX0_SERIAL_PINOUT1
//#define SUB_DEMUX0_SERIAL_PINOUT2
//#define SUB_DEMUX0_SERIAL_PINOUT3
#define SUB_DEMUX0_SERIAL_PINOUT4
#endif
#endif

#if 0
#define GPIO_LNB_POWER_PIN			letb1502_PIN_T_MX6
#define GPIO_LNB_LEVEL_PIN			letb1502_PIN_T_MX5
#else
#define GPIO_LNB_POWER_PIN			letb1502_PIN
#define GPIO_LNB_LEVEL_PIN			letb1502_PIN
#endif

#define GPIO_LNB_SDA_PIN			letb1502_PIN
#define GPIO_LNB_SCL_PIN			letb1502_PIN
#define GPIO_LNB_SDA_PIN2			letb1502_PIN
#define GPIO_LNB_SCL_PIN2			letb1502_PIN

#ifdef SUPPORT_MODULE_TEST
#define DEMUX1_SERIAL_PINOUT2
#endif

//Serial pinout
#define GPIO_NIM_RST				GPIO_DEMOD_RESET_PIN
#define GPIO_NIM_SDA				GPIO_I2C_SDA_PIN
#define GPIO_NIM_SCL				GPIO_I2C_SCL_PIN
#if defined(DEMUX0_SERIAL_PINOUT1)  //pinmux table, demux0,X1
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX15
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX14
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX24
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX23
#elif defined(DEMUX0_SERIAL_PINOUT2)  //pinmux table, demux0,X2
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX15
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX14
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX13
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX16
#elif defined(DEMUX0_SERIAL_PINOUT3)//pinmux table, demux0,X3
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX9
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX10
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX11
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX12
#elif defined(DEMUX0_SERIAL_PINOUT4)//pinmux table, demux0,X4, default demux 0 pinout
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX4
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX1
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX2
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX3
#elif defined(DEMUX0_SERIAL_PINOUT5)//pinmux table, demux0,X5
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX9
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX6
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX7
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX8
#elif defined(DEMUX0_SERIAL_PINOUT8)//pinmux table, demux0,X8
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX24
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX21
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX22
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX23
#endif

#if defined(DEMUX1_SERIAL_PINOUT1)//pinmux table, demux1,X1
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX16
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX13
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX14
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX15
#elif defined(DEMUX1_SERIAL_PINOUT2)//pinmux table, demux1,X2, default demux1 pinout
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX5
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX6
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX7
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX8
#elif defined(DEMUX1_SERIAL_PINOUT3)//pinmux table, demux1,X3
#define	GPIO_S_TS_DAT				letb1502_PIN_T_MX9
#define	GPIO_S_TS_CLK				letb1502_PIN_T_MX6
#define	GPIO_S_TS_DEN 				letb1502_PIN_T_MX7
#define	GPIO_S_TS_SYNC			letb1502_PIN_T_MX8
#elif defined(DEMUX1_SERIAL_PINOUT4)//pinmux table, demux1,X4
#define	GPIO_S_TS_DAT				letb1502_PIN_L_MX3
#define	GPIO_S_TS_CLK				letb1502_PIN_L_MX15
#define	GPIO_S_TS_DEN 				letb1502_PIN_L_MX2
#define	GPIO_S_TS_SYNC			letb1502_PIN_L_MX1
#endif

#if defined(SUB_NIM_SERIAL_PINOUT1)//demux0 default pinout
#define	GPIO_S_TS_DAT2			letb1502_PIN_T_MX4
#define	GPIO_S_TS_CLK2				letb1502_PIN_T_MX1
#define	GPIO_S_TS_DEN2 			letb1502_PIN_T_MX2
#define	GPIO_S_TS_SYNC2			letb1502_PIN_T_MX3
#elif defined(SUB_NIM_SERIAL_PINOUT2)//demux1 default pinout
#define	GPIO_S_TS_DAT2			letb1502_PIN_T_MX5
#define	GPIO_S_TS_CLK2				letb1502_PIN_T_MX6
#define	GPIO_S_TS_DEN2 			letb1502_PIN_T_MX7
#define	GPIO_S_TS_SYNC2			letb1502_PIN_T_MX8
#endif


#if 0 //Not support parallel in 216pin with ETH phy option1
//PARALLEL TS : Select demux parallel pinout
//#define DEMUX0_PARALLEL_PINOUT1
#define DEMUX1_PARALLEL_PINOUT1

#if defined(DEMUX0_PARALLEL_PINOUT1)
#define	GPIO_P_TS_DAT0			letb1502_PIN_T_MX16
#define	GPIO_P_TS_DAT1			letb1502_PIN_T_MX15
#define	GPIO_P_TS_DAT2			letb1502_PIN_T_MX14
#define	GPIO_P_TS_DAT3			letb1502_PIN_T_MX13
#define	GPIO_P_TS_DAT4			letb1502_PIN_T_MX12
#define	GPIO_P_TS_DAT5			letb1502_PIN_T_MX11
#define	GPIO_P_TS_DAT6			letb1502_PIN_T_MX10
#define	GPIO_P_TS_DAT7			letb1502_PIN_T_MX17
#define	GPIO_P_TS_CLK				letb1502_PIN_T_MX18
#define	GPIO_P_TS_DEN 				letb1502_PIN_T_MX19
#define	GPIO_P_TS_SYNC			letb1502_PIN_T_MX20
#elif defined(DEMUX1_PARALLEL_PINOUT1)
#define	GPIO_P_TS_DAT0			letb1502_PIN_T_MX16
#define	GPIO_P_TS_DAT1			letb1502_PIN_T_MX15
#define	GPIO_P_TS_DAT2			letb1502_PIN_T_MX14
#define	GPIO_P_TS_DAT3			letb1502_PIN_T_MX13
#define	GPIO_P_TS_DAT4			letb1502_PIN_T_MX12
#define	GPIO_P_TS_DAT5			letb1502_PIN_T_MX11
#define	GPIO_P_TS_DAT6			letb1502_PIN_T_MX10
#define	GPIO_P_TS_DAT7			letb1502_PIN_T_MX9
#define	GPIO_P_TS_CLK				letb1502_PIN_T_MX6
#define	GPIO_P_TS_DEN 				letb1502_PIN_T_MX7
#define	GPIO_P_TS_SYNC			letb1502_PIN_T_MX8
#endif
#endif

//SPDIF_IN--GPIO
#define SPDIF_IN_PINOUT1
//#define SPDIF_IN_PINOUT2
//#define SPDIF_IN_PINOUT3
//#define SPDIF_IN_PINOUT4
//#define SPDIF_IN_PINOUT5

#if defined(SPDIF_IN_PINOUT1)//X1,default pinout
#define GPIO_SPDIF_IN_PIN			letb1502_PIN_L_MX11 
#elif defined(SPDIF_IN_PINOUT2)//X2
#define GPIO_SPDIF_IN_PIN			letb1502_PIN_L_MX1  
#elif defined(SPDIF_IN_PINOUT3)//X3
#define GPIO_SPDIF_IN_PIN			letb1502_PIN_T_MX27  
#elif defined(SPDIF_IN_PINOUT4)//X4
#define GPIO_SPDIF_IN_PIN			letb1502_PIN_T_MX13  
#elif defined(SPDIF_IN_PINOUT5)//X5
#define GPIO_SPDIF_IN_PIN			letb1502_PIN_B_MX14  
#endif


//Smart Card interface
//#define SMART_CARD_PINOUT1
//#define SMART_CARD_PINOUT2
#define SMART_CARD_PINOUT3
//#define SMART_CARD_PINOUT4

#if defined(SMART_CARD_PINOUT1)//X1
#define	GPIO_SMART_CARD_IO		letb1502_PIN_T_MX17
#define	GPIO_SMART_CARD_CLK		letb1502_PIN_T_MX18
#define	GPIO_SMART_CARD_PRES	letb1502_PIN_T_MX19
#define	GPIO_SMART_CARD_RSTB	letb1502_PIN_T_MX20
#define	GPIO_SMART_CARD_VCCB	letb1502_PIN_T_MX21
#elif defined(SMART_CARD_PINOUT2)//X2, conflict with NAND
#define	GPIO_SMART_CARD_IO		letb1502_PIN_R_MX19
#define	GPIO_SMART_CARD_CLK		letb1502_PIN_R_MX20
#define	GPIO_SMART_CARD_PRES	letb1502_PIN_R_MX21
#define	GPIO_SMART_CARD_RSTB	letb1502_PIN_R_MX22
#define	GPIO_SMART_CARD_VCCB	letb1502_PIN_R_MX23
#elif defined(SMART_CARD_PINOUT3)//X3, conflict with SPV611
#define	GPIO_SMART_CARD_IO		letb1502_PIN_R_MX36
#define	GPIO_SMART_CARD_CLK		letb1502_PIN_R_MX37
#define	GPIO_SMART_CARD_PRES	letb1502_PIN_R_MX35
#define	GPIO_SMART_CARD_RSTB	letb1502_PIN_R_MX38
#define	GPIO_SMART_CARD_VCCB	letb1502_PIN_R_MX39
#elif defined(SMART_CARD_PINOUT4)//X4
#define	GPIO_SMART_CARD_IO		letb1502_PIN_L_MX9
#define	GPIO_SMART_CARD_CLK		letb1502_PIN_L_MX10
#define	GPIO_SMART_CARD_PRES	letb1502_PIN_L_MX11
#define	GPIO_SMART_CARD_RSTB	letb1502_PIN_L_MX12
#define	GPIO_SMART_CARD_VCCB	letb1502_PIN_L_MX13
#endif


//SMART CARD 1 interface
//#define SMART_CARD1_PINOUT1
#define SMART_CARD1_PINOUT2
//#define SMART_CARD1_PINOUT3
//#define SMART_CARD1_PINOUT4

#if defined(SMART_CARD1_PINOUT1)//X1
#define	GPIO_SMART_CARD1_IO		letb1502_PIN_T_MX4
#define	GPIO_SMART_CARD1_CLK		letb1502_PIN_T_MX5
#define	GPIO_SMART_CARD1_PRES	letb1502_PIN_T_MX6
#define	GPIO_SMART_CARD1_RSTB	letb1502_PIN_T_MX7
#define	GPIO_SMART_CARD1_VCCB	letb1502_PIN_T_MX8
#elif defined(SMART_CARD1_PINOUT2)//X2,SMARTCARD1 default smartcard pinout
#define	GPIO_SMART_CARD1_IO		letb1502_PIN_T_MX12
#define	GPIO_SMART_CARD1_CLK		letb1502_PIN_T_MX13
#define	GPIO_SMART_CARD1_PRES	letb1502_PIN_T_MX14
#define	GPIO_SMART_CARD1_RSTB	letb1502_PIN_T_MX15
#define	GPIO_SMART_CARD1_VCCB	letb1502_PIN_T_MX16
#elif defined(SMART_CARD1_PINOUT3)//X3
#define	GPIO_SMART_CARD1_IO		letb1502_PIN_L_MX2
#define	GPIO_SMART_CARD1_CLK		letb1502_PIN_L_MX3
#define	GPIO_SMART_CARD1_PRES	letb1502_PIN_L_MX4
#define	GPIO_SMART_CARD1_RSTB	letb1502_PIN_L_MX5
#define	GPIO_SMART_CARD1_VCCB	letb1502_PIN_L_MX6
#elif defined(SMART_CARD1_PINOUT4)//X4
#define	GPIO_SMART_CARD1_IO		letb1502_PIN_R_MX38
#define	GPIO_SMART_CARD1_CLK		letb1502_PIN_R_MX39
#define	GPIO_SMART_CARD1_PRES	letb1502_PIN_T_MX0
#define	GPIO_SMART_CARD1_RSTB	letb1502_PIN_T_MX1
#define	GPIO_SMART_CARD1_VCCB	letb1502_PIN_T_MX2
#endif


//HDMI I2C interface--master
#define HDMI_I2C_PINOUT1
#if defined(HDMI_I2C_PINOUT1)//X1
#define GPIO_HDMI_I2C_DAT			letb1502_PIN_T_MX32
#define GPIO_HDMI_I2C_CLK			letb1502_PIN_T_MX33
#endif


//HDMI hot-plug detect interface
#define GPIO_HDMI_HPD_PIN			letb1502_PIN_IV_MX5

// HDMI CEC
#define GPIO_HDMI_CEC_PIN			letb1502_PIN_IV_MX4

//SPDIF OUT
#define SPDIF_OUT_PINOUT1
//#define SPDIF_OUT_PINOUT2
#if defined(SPDIF_OUT_PINOUT1)
#define GPIO_SPDIF_OUT				letb1502_PIN_L_MX6 //default pinout
#elif defined(SPDIF_OUT_PINOUT2)
#define GPIO_SPDIF_OUT				letb1502_PIN_L_MX0
#endif


//GPIO mute function--GPIO
#define GPIO_AMUTE_PIN				letb1502_PIN_IV_MX7


//SCART TV--GPIO
#define GPIO_SCART_UNUSED			(0) //set the unused io

#define GPIO_SCART_RELEASE			(letb1502_PIN_IV_MX9) //SCART_RES:release-1/disable release-0
#define GPIO_SCART_TVMODE			(letb1502_PIN_IV_MX8) //SCART_SIG:CVBS-0/RGB-1
#define GPIO_SCART_TVDISPLAY		(letb1502_PIN_IV_MX10) //SCART_DIS:TV Display 16:9-1/4:3-0
#define GPIO_SCART_TVVCR			(GPIO_SCART_UNUSED)//Switch TV&VCR
#define GPIO_SCART_VCRINSERT		(GPIO_SCART_UNUSED)//Detect VCR insert
#define GPIO_SCART_VCRMODE		(GPIO_SCART_UNUSED)
#define GPIO_SCART_VCRDISPLAY		(GPIO_SCART_UNUSED)

// IR 
#define GPIO_IR_PIN					letb1502_PIN_IV_MX0

// VFD
#define GPIO_VFD_STB_PIN			letb1502_PIN_IV_MX2//letb1502_PIN_IV_MX1
#define GPIO_VFD_CLK_PIN			letb1502_PIN_IV_MX1//letb1502_PIN_IV_MX2  //zhouyu for mantis 0159701
#define GPIO_VFD_DAT_PIN			letb1502_PIN_IV_MX3

//debug UART TX/RX pair
#define UART_DBG_PINOUT1
#if defined(UART_DBG_PINOUT1)//default pinout, X1
#define GPIO_DBG_TX				letb1502_PIN_L_MX7
#define GPIO_DBG_RX				letb1502_PIN_L_MX8
#endif

//normal UART TX/RX pair
//#define UART_NORMAL_PINOUT1
//#define UART_NORMAL_PINOUT2
//#define UART_NORMAL_PINOUT3
#define UART_NORMAL_PINOUT4
//#define UART_NORMAL_PINOUT5
//#define UART_NORMAL_PINOUT6

#if defined(UART_NORMAL_PINOUT1)//X1
#define GPIO_GENERAL_TX			letb1502_PIN_T_MX20
#define GPIO_GENERAL_RX			letb1502_PIN_T_MX21
#elif defined(UART_NORMAL_PINOUT2)//X2, default pinout, conflict with SMC0, SYNC_PC and SPV611
#define GPIO_GENERAL_TX			letb1502_PIN_R_MX38
#define GPIO_GENERAL_RX			letb1502_PIN_R_MX39
#elif defined(UART_NORMAL_PINOUT3)//X3
#define GPIO_GENERAL_TX			letb1502_PIN_L_MX9
#define GPIO_GENERAL_RX			letb1502_PIN_L_MX10
#elif defined(UART_NORMAL_PINOUT4)//X4
#define GPIO_GENERAL_TX			letb1502_PIN_R_MX25
#define GPIO_GENERAL_RX			letb1502_PIN_R_MX26
#elif defined(UART_NORMAL_PINOUT5)//X5
#define GPIO_GENERAL_TX			letb1502_PIN_T_MX0
#define GPIO_GENERAL_RX			letb1502_PIN_T_MX1
#elif defined(UART_NORMAL_PINOUT6)//X6
#define GPIO_GENERAL_TX			letb1502_PIN_T_MX17
#define GPIO_GENERAL_RX			letb1502_PIN_T_MX18
#endif

//VGA interface
//#define H_V_SINK_PINOUT1
#define H_V_SINK_PINOUT2
//#define H_V_SINK_PINOUT3
//#define H_V_SINK_PINOUT4 //no use in 216pin w/ETH phy option 1 !!

#if defined(H_V_SINK_PINOUT1)  //X1
#define	GPIO_VGA_HSYNC			letb1502_PIN_T_MX20	
#define	GPIO_VGA_VSYNC			letb1502_PIN_T_MX21
#elif defined(H_V_SINK_PINOUT2)//X2, conflict with UART0
#define	GPIO_VGA_HSYNC			letb1502_PIN_L_MX7	
#define	GPIO_VGA_VSYNC			letb1502_PIN_L_MX8
#elif defined(H_V_SINK_PINOUT3)//X3,,conflict with UART1,SMC0, SPV611 pinout
#define	GPIO_VGA_HSYNC			letb1502_PIN_R_MX38	
#define	GPIO_VGA_VSYNC			letb1502_PIN_R_MX39
#elif defined(H_V_SINK_PINOUT4)//X4 
#define	GPIO_VGA_HSYNC			letb1502_PIN_L_MX9	
#define	GPIO_VGA_VSYNC			letb1502_PIN_L_MX10
#endif


#define PCMCIA_PINOUT_1
//#define PCMCIA_PINOUT_2
//#define PCMCIA_PINOUT_3
#if defined(PCMCIA_PINOUT_1)
#define	GPIO_CI_D7				letb1502_PIN_R_MX35
#define	GPIO_CI_D6				letb1502_PIN_R_MX36
#define	GPIO_CI_D5				letb1502_PIN_R_MX37
#define	GPIO_CI_D4				letb1502_PIN_R_MX38
#define	GPIO_CI_D3				letb1502_PIN_R_MX39
#define	GPIO_CI_D2				letb1502_PIN_T_MX0
#define	GPIO_CI_D1				letb1502_PIN_T_MX1
#define	GPIO_CI_D0				letb1502_PIN_T_MX2
#define	GPIO_CI_A14			letb1502_PIN_R_MX34
#define	GPIO_CI_A13			letb1502_PIN_R_MX33
#define	GPIO_CI_A12			letb1502_PIN_R_MX32
#define	GPIO_CI_A11			letb1502_PIN_R_MX31
#define	GPIO_CI_A10			letb1502_PIN_R_MX30
#define	GPIO_CI_A9				letb1502_PIN_R_MX29
#define	GPIO_CI_A8				letb1502_PIN_R_MX28
#define	GPIO_CI_A7				letb1502_PIN_R_MX27
#define	GPIO_CI_A6				letb1502_PIN_R_MX26
#define	GPIO_CI_A5				letb1502_PIN_R_MX25
#define	GPIO_CI_A4				letb1502_PIN_R_MX24
#define	GPIO_CI_A3				letb1502_PIN_R_MX23
#define	GPIO_CI_A2				letb1502_PIN_R_MX22
#define	GPIO_CI_A1				letb1502_PIN_R_MX21
#define	GPIO_CI_A0				letb1502_PIN_R_MX20

#define	GPIO_CI_CS				letb1502_PIN_T_MX8 //chip select //CEN
#define	GPIO_CI_OE				letb1502_PIN_T_MX7 //OEN
#define	GPIO_CI_IOR			letb1502_PIN_T_MX6 //IO Read //IORN
#define	GPIO_CI_IOW			letb1502_PIN_T_MX5 //IO Write //IOWN
#define	GPIO_CI_WE				letb1502_PIN_T_MX4 //WEN
#define	GPIO_CI_WAIT			letb1502_PIN_T_MX3 //WAITN
#define	GPIO_CI_CD1			letb1502_PIN_R_MX19  //CIIN_CD1 //CI_PLUS_INT

#define	GPIO_CI_CD2			letb1502_PIN //CIIN_CD2
#define	GPIO_CI_RST			letb1502_PIN  //RESET_CI
#define	GPIO_CI_RDY			letb1502_PIN //CI_READY
#define	GPIO_CI_PWR			letb1502_PIN //CI_PWR_EN
#elif defined(PCMCIA_PINOUT_2)
#define	GPIO_CI_D7				letb1502_PIN_R_MX35
#define	GPIO_CI_D6				letb1502_PIN_R_MX36
#define	GPIO_CI_D5				letb1502_PIN_R_MX37
#define	GPIO_CI_D4				letb1502_PIN_R_MX38
#define	GPIO_CI_D3				letb1502_PIN_R_MX39
#define	GPIO_CI_D2				letb1502_PIN_T_MX0
#define	GPIO_CI_D1				letb1502_PIN_T_MX1
#define	GPIO_CI_D0				letb1502_PIN_T_MX2
#define	GPIO_CI_A14			letb1502_PIN_R_MX34
#define	GPIO_CI_A13			letb1502_PIN_R_MX33
#define	GPIO_CI_A12			letb1502_PIN_R_MX32
#define	GPIO_CI_A11			letb1502_PIN_R_MX31
#define	GPIO_CI_A10			letb1502_PIN_R_MX30
#define	GPIO_CI_A9				letb1502_PIN_R_MX29
#define	GPIO_CI_A8				letb1502_PIN_R_MX28
#define	GPIO_CI_A7				letb1502_PIN_R_MX27
#define	GPIO_CI_A6				letb1502_PIN_R_MX26
#define	GPIO_CI_A5				letb1502_PIN_R_MX25
#define	GPIO_CI_A4				letb1502_PIN_R_MX24
#define	GPIO_CI_A3				letb1502_PIN_R_MX23
#define	GPIO_CI_A2				letb1502_PIN_R_MX22
#define	GPIO_CI_A1				letb1502_PIN_R_MX21
#define	GPIO_CI_A0				letb1502_PIN_R_MX20

#define	GPIO_CI_CS				letb1502_PIN_R_MX19 //chip select //CEN
#define	GPIO_CI_OE				letb1502_PIN_R_MX18 //OEN
#define	GPIO_CI_IOR			letb1502_PIN_R_MX17 //IO Read //IORN
#define	GPIO_CI_IOW			letb1502_PIN_T_MX5 //IO Write //IOWN
#define	GPIO_CI_WE				letb1502_PIN_T_MX4 //WEN
#define	GPIO_CI_WAIT			letb1502_PIN_T_MX3 //WAITN
#define	GPIO_CI_CD1			letb1502_PIN_R_MX16  //CIIN_CD1 //CI_PLUS_INT

#define	GPIO_CI_CD2			letb1502_PIN_IV_MX10 //CIIN_CD2
#define	GPIO_CI_RST			letb1502_PIN_IV_MX9  //RESET_CI
#define	GPIO_CI_RDY			letb1502_PIN_B_MX15 //CI_READY
#define	GPIO_CI_PWR			letb1502_PIN_L_MX3 //CI_PWR_EN
#elif defined(PCMCIA_PINOUT_3)
#define	GPIO_CI_D7				letb1502_PIN_R_MX35
#define	GPIO_CI_D6				letb1502_PIN_R_MX36
#define	GPIO_CI_D5				letb1502_PIN_R_MX37
#define	GPIO_CI_D4				letb1502_PIN_R_MX38
#define	GPIO_CI_D3				letb1502_PIN_R_MX39
#define	GPIO_CI_D2				letb1502_PIN_T_MX0
#define	GPIO_CI_D1				letb1502_PIN_T_MX1
#define	GPIO_CI_D0				letb1502_PIN_T_MX2
#define	GPIO_CI_A14			letb1502_PIN_R_MX34
#define	GPIO_CI_A13			letb1502_PIN_R_MX33
#define	GPIO_CI_A12			letb1502_PIN_R_MX32
#define	GPIO_CI_A11			letb1502_PIN_R_MX31
#define	GPIO_CI_A10			letb1502_PIN_R_MX30
#define	GPIO_CI_A9				letb1502_PIN_R_MX29
#define	GPIO_CI_A8				letb1502_PIN_R_MX28
#define	GPIO_CI_A7				letb1502_PIN_R_MX27
#define	GPIO_CI_A6				letb1502_PIN_R_MX26
#define	GPIO_CI_A5				letb1502_PIN_R_MX25
#define	GPIO_CI_A4				letb1502_PIN_R_MX24
#define	GPIO_CI_A3				letb1502_PIN_R_MX23
#define	GPIO_CI_A2				letb1502_PIN_R_MX22
#define	GPIO_CI_A1				letb1502_PIN_R_MX21
#define	GPIO_CI_A0				letb1502_PIN_R_MX20

#define	GPIO_CI_CS				letb1502_PIN_R_MX19 //chip select //CEN
#define	GPIO_CI_OE				letb1502_PIN_R_MX18 //OEN
#define	GPIO_CI_IOR			letb1502_PIN_R_MX17 //IO Read //IORN
#define	GPIO_CI_IOW			letb1502_PIN_R_MX16 //IO Write //IOWN
#define	GPIO_CI_WE				letb1502_PIN_R_MX15 //WEN
#define	GPIO_CI_WAIT			letb1502_PIN_R_MX14 //WAITN
#define	GPIO_CI_CD1			letb1502_PIN_R_MX13  //CIIN_CD1 //CI_PLUS_INT

#define	GPIO_CI_CD2			letb1502_PIN //CIIN_CD2
#define	GPIO_CI_RST			letb1502_PIN  //RESET_CI
#define	GPIO_CI_RDY			letb1502_PIN //CI_READY
#define	GPIO_CI_PWR			letb1502_PIN //CI_PWR_EN
#endif


//SPV611PINOUT
#define	SPV611_SD_PINOUT5
#if defined(SPV611_SD_PINOUT5)
#define	GPIO_SPV611_SD_CLK				letb1502_PIN_R_MX39
#define	GPIO_SPV611_SD_CMD				letb1502_PIN_T_MX0
#define	GPIO_SPV611_SD_D0				letb1502_PIN_R_MX35
#define	GPIO_SPV611_SD_D1				letb1502_PIN_R_MX36
#define	GPIO_SPV611_SD_D2				letb1502_PIN_R_MX37
#define	GPIO_SPV611_SD_D3				letb1502_PIN_R_MX38
#endif


//CCIR656
//#define	CCIR656_PINOUT1
#define	CCIR656_PINOUT2
//#define	CCIR656_PINOUT3
//#define	CCIR656_PINOUT4
//#define	CCIR656_PINOUT5

#if defined(CCIR656_PINOUT1) //X1
#define	GPIO_CCIR_CLK		letb1502_PIN_R_MX19
#define	GPIO_CCIR_D7		letb1502_PIN_R_MX20
#define	GPIO_CCIR_D6		letb1502_PIN_R_MX21
#define	GPIO_CCIR_D5		letb1502_PIN_R_MX22
#define	GPIO_CCIR_D4		letb1502_PIN_R_MX23
#define	GPIO_CCIR_D3		letb1502_PIN_R_MX24
#define	GPIO_CCIR_D2		letb1502_PIN_R_MX25
#define	GPIO_CCIR_D1		letb1502_PIN_R_MX26
#define	GPIO_CCIR_D0		letb1502_PIN_R_MX27
#elif defined(CCIR656_PINOUT2) //X2, default pinout
#define	GPIO_CCIR_CLK		letb1502_PIN_R_MX32
#define	GPIO_CCIR_D7		letb1502_PIN_R_MX24
#define	GPIO_CCIR_D6		letb1502_PIN_R_MX25
#define	GPIO_CCIR_D5		letb1502_PIN_R_MX26
#define	GPIO_CCIR_D4		letb1502_PIN_R_MX27
#define	GPIO_CCIR_D3		letb1502_PIN_R_MX28
#define	GPIO_CCIR_D2		letb1502_PIN_R_MX29
#define	GPIO_CCIR_D1		letb1502_PIN_R_MX30
#define	GPIO_CCIR_D0		letb1502_PIN_R_MX31
#elif defined(CCIR656_PINOUT3)//X3
#define	GPIO_CCIR_CLK		letb1502_PIN_R_MX27
#define	GPIO_CCIR_D7		letb1502_PIN_R_MX28
#define	GPIO_CCIR_D6		letb1502_PIN_R_MX29
#define	GPIO_CCIR_D5		letb1502_PIN_R_MX30
#define	GPIO_CCIR_D4		letb1502_PIN_R_MX31
#define	GPIO_CCIR_D3		letb1502_PIN_R_MX32
#define	GPIO_CCIR_D2		letb1502_PIN_R_MX33
#define	GPIO_CCIR_D1		letb1502_PIN_R_MX34
#define	GPIO_CCIR_D0		letb1502_PIN_R_MX35
#elif defined(CCIR656_PINOUT4)//X4
#define	GPIO_CCIR_CLK		letb1502_PIN_R_MX31
#define	GPIO_CCIR_D7		letb1502_PIN_R_MX32
#define	GPIO_CCIR_D6		letb1502_PIN_R_MX33
#define	GPIO_CCIR_D5		letb1502_PIN_R_MX34
#define	GPIO_CCIR_D4		letb1502_PIN_R_MX35
#define	GPIO_CCIR_D3		letb1502_PIN_R_MX36
#define	GPIO_CCIR_D2		letb1502_PIN_R_MX37
#define	GPIO_CCIR_D1		letb1502_PIN_R_MX38
#define	GPIO_CCIR_D0		letb1502_PIN_R_MX39
#elif defined(CCIR656_PINOUT5)//X5
#define	GPIO_CCIR_CLK		letb1502_PIN_R_MX21
#define	GPIO_CCIR_D7		letb1502_PIN_R_MX13
#define	GPIO_CCIR_D6		letb1502_PIN_R_MX14
#define	GPIO_CCIR_D5		letb1502_PIN_R_MX15
#define	GPIO_CCIR_D4		letb1502_PIN_R_MX16
#define	GPIO_CCIR_D3		letb1502_PIN_R_MX17
#define	GPIO_CCIR_D2		letb1502_PIN_R_MX18
#define	GPIO_CCIR_D1		letb1502_PIN_R_MX19
#define	GPIO_CCIR_D0		letb1502_PIN_R_MX20
#endif

//*******************************************//
//	pin level
//*******************************************//
#define	HI_LEVEL							(1)
#define	LOW_LEVEL							(0)

//*******************************************//
//	audio dac mute/demute level
//*******************************************//
#define AUDIO_MUTE_LEVLE		(LOW_LEVEL)
#define AUDIO_DEMUTE_LEVLE		(HI_LEVEL)

//*******************************************//
//	lnb power level
//*******************************************//
#define LNB_POWER_CONTROL_ON_LEVEL			LOW_LEVEL
#define LNB_POWER_CONTROL_OFF_LEVEL			(!(LNB_POWER_CONTROL_ON_LEVEL))

#define LNB_POWER_LEVEL_14V					HI_LEVEL
#define LNB_POWER_LEVEL_18V					(!(LNB_POWER_LEVEL_14V))

//*******************************************//
//	dac current and on/off mode;
//*******************************************//
#define  DAC_A_ON_OFF_MODE		DAC_ENABLE
#define  DAC_B_ON_OFF_MODE		DAC_ENABLE
#define  DAC_C_ON_OFF_MODE		DAC_ENABLE
#define  DAC_D_ON_OFF_MODE		DAC_ENABLE

#define  DAC_A_CURRENT_MODE		QUARTER_CURRENT
#define  DAC_B_CURRENT_MODE		FULL_CURRENT
#define  DAC_C_CURRENT_MODE		FULL_CURRENT
#define  DAC_D_CURRENT_MODE		FULL_CURRENT

#define  DAC_A_AVPD_MODE		SEL_ON
#define  DAC_B_AVPD_MODE		SEL_ON
#define  DAC_C_AVPD_MODE		SEL_ON
#define  DAC_D_AVPD_MODE		SEL_ON

#endif//__1502_216PIN_PINMUX_DVBS_TYPE1_H__