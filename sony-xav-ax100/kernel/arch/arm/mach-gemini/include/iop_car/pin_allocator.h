//#include "types.h" //Kaney mark
#include <mach/common.h> //Kaney add
#ifndef __ALLOCATE_8600_PIN_FUNCTION_H
#define __ALLOCATE_8600_PIN_FUNCTION_H

//#include "config.h"
//#include "gpio_pin_map.h"
#include "gpio_func_sel.h"

#define GPIO_DBG

#ifdef GPIO_DBG
void GPIO_CONFLICT(UINT8 gpio_number);
int gpio_conflict_check(UINT8 gpio_number,UINT8 function_index);
//for using gpio_array to determine the conflict or not
int gpio_conflict_check_array(UINT8 *gpio_array, UINT32 array_size,UINT8 function_index);
#endif
int deallocate_gpio(UINT8 gpio_number);
int deallocate_gpio_array(UINT8 *gpio_array, UINT32 array_size);

//PC HSINK AND VSINK
int allocate_hvsync_pin(void);

//UART FOR DEBUG AND NORMAL USE
int allocate_uart_dbg_pin(void);
int allocate_uart_normal_pin(void);

// 3 IN 1 CARD
int allcoate_3in1_card_pin(void);
int plt_card_switch_pin(UINT32 cardtype);

// SPV611
int allcoate_SPV611_sd_pin(void);
int plt_SPV611_switch_pin(void);

//NAND FLASH
int allcoate_nand_flash_pin(void);
void plt_nand_setting_pin(void);
void deallocate_nand_flash_pin(void);

//SPDIF IN/OUT
int allocate_spdifin_pin(void);
int allocate_spdifout_pin(void);

//#ifdef	SUPPORT_USB_POWER_SAVE
int allocate_usb_power_save_pin(void);
int turn_off_usb_power(void);

#ifdef	SUPPORT_NIM_RF
int allcoate_nim_rf_pin(void);
#endif

//SERIAL DVBT
int allcoate_serial_ts_pin(void);

//PARALLEL DVB
int allcoate_parallel_ts_pin(void);

//DEMUX Serial/parallel switch
int plt_demux_switch_pin(UINT32 demuxtype);

//SMART CARD
int allcoate_smart_card_pin(void);

//SMART CARD1
int allcoate_smart_card1_pin(void);

//SDIO
int allcoate_sdio_pin(void);

//NET EPHY
int allcoate_ephy_pin(void);

//HDMI I2C HPD CEC
int allocate_hdmi_i2c_pin(void);
int allocate_hdmi_hpd_pin(void);
int allocate_hdmi_cec_pin(void);

//SCART TV
int allocate_scart_control_pin(void);

//AUDIO MUTE
int allocate_gpio_mute_pin(void);
int allocate_gpio_demute_pin(void);

//IR AND VFD FRONT PANEL
int allocate_ir_pin(void);
int allocate_vfd_pin(void);

//GAME PAD FUNCTION
int allocate_gamepad_pin(void);

//SERVO
int allcoate_servo_function_pin(void);

//FOR USER TO ALLOCATE DESIRED GPIO
int allocate_other_gpio_pin(UINT8 chip_pin_number);

int allcoate_pcmcia_pin(void);

//FOR CCIR656 initial, CCIR656 on/off
int allcoate_ccir656_pin(void);		//CCIR656 initial
int allcoate_ccir656_out_on(void);	//CCIR656 on
int allcoate_ccir656_out_off(void);	//CCIR656 off

//ATA
int allcoate_ata_function_pin(void);
void plt_ata_setting_pin(void);

int allocate_dvbs_gpio_pin(void);

int allocate_sbtvd_gpio_pin(void);//haleliu 20100505 add for sbtvd 

#ifdef SUPPORT_DVBST_NIM
int allocate_dvbst_gpio_pin(void); //lijin 20101228 add for DVB_S&T
#endif

#ifdef SUPPORT_SBTVD_DVBS_NIM
int allocate_sbtvd_dvbs_gpio_pin(void); //yanan added 20120130 for SBTVD+DVBS
#endif
#ifdef SUPPORT_Dual_DVBS                //lijin 20110808  add for Dual DVBS
int allocate_Dualdvbs_gpio_pin(void);
#endif

int init_i2c_gpio_pins(void);

//iPod Detect
int allocate_ipod_detect_pin(void);
#ifdef SUPPORT_PLAYER_STB_DVB_T  
#ifdef SUPPORT_SPDC2XX_POWER_DOWN
int allcoate_serial_dvbt_Tuner_Power(BYTE bPowerOn);
#endif
#endif

int allocate_i2sout_pin(void);

int allocate_i2sin_pin(void);

#endif/*__ALLOCATE_8600_PIN_FUNCTION_H*/

