#include "sp_hw_sdio.h"

inline bool is_sd_cmp_int(void)
{
	return (sd_int_control(DEV_SDCTRL_BASE_ADRS) & BIT_N(1));
}
inline bool is_sdio_int(void)
{
	return (sd_int_control(DEV_SDCTRL_BASE_ADRS) & BIT_N(5));
}
inline bool is_dma_int(void)
{
	return (dma_cmp(DEV_SDCTRL_BASE_ADRS) & BIT_N(0));
}

inline void disable_sd_cmp_int(void)
{
	u32 value = SD_INT_CONTROL_GET();
	SD_INT_CONTROL_SET(value & (~(BIT_N(0)))); 
}

inline void enable_sd_cmp_int(void)
{
	u32 value = SD_INT_CONTROL_GET();
	SD_INT_CONTROL_SET(value | (BIT_N(0))); 
}

inline void enable_sdio_int(void)
{
	//g131.reg12[4]
	u32 value = SD_INT_CONTROL_GET();	
	//CLR_BIT_N(value, 6);
	SET_BIT_N(value, 4);
	SD_INT_CONTROL_SET(value); 

}

inline void sdio_int_clr(void)
{
	u32 val = SD_INT_CONTROL_GET();
	SET_BIT_N(val, 6);
	SD_INT_CONTROL_SET(val); 
}


inline void disable_sdio_int(void)
{
	//g131.reg12[4]
	u32 value = SD_INT_CONTROL_GET();
	CLR_BIT_N(value, 4);
	SD_INT_CONTROL_SET(value); 
}

inline void clear_sd_cmp_int(void)
{
	u32 value = SD_INT_CONTROL_GET();
	SD_INT_CONTROL_SET(value | (BIT_N(2))); 
}


inline void reset_dut_sdio(void)
{
	// grp131.19 = 0x07
	SDIO_RST();
}

inline u32 hw_sd_check_rsp_timeout(void)
{
	u32 value = SD_STATUS0_GET(); 
	if((value & BIT_N(6))){ // bit 6 rsp time out
		return false;
	}
	return true;
}

inline void config0_for_send_cmd(u32 cmd, u32 rsp_type, sdio_cmd_type_t cmd_type)
{
	// reg131.14
	u32 val = SD_GET_CONFIG0();
	// bit1 set 0 SDR mode
	CLR_BIT_N(val, 1); 

	/* bit2 SD length is specified(sd_len_mode), 
	 *	1: it means theCMDwith a specified data block.
	 *     So, after page number is transfered, there are
	 *	not send stop cmd and end state machine.
	 */
	// sd_len_mode 
	if((18 == cmd) || (25 == cmd)){
		CLR_BIT_N(val, 2);
	}else{
		SET_BIT_N(val, 2);
	}

	// Transaction mode select bit[5:4] set 00: only cmd/rsp
	if(SDIO_CMD_OLY_CMD == cmd_type){
		CLR_BIT_N(val, 4);	
		CLR_BIT_N(val, 5);
	}else if(SDIO_CMD_WRITE == cmd_type){ // 01 : write
		SET_BIT_N(val, 4);	
		CLR_BIT_N(val, 5);
	}else if(SDIO_CMD_READ== cmd_type){ // 10: read
		CLR_BIT_N(val, 4);	
		SET_BIT_N(val, 5);
	}

	if((RSP_TYPE_NORSP != rsp_type) && (SDIO_CMD_READ != cmd_type) ){
		SET_BIT_N(val, 6);// Auto response after cmd bit[6]	enable
	}else{
		CLR_BIT_N(val, 6);// disable
	}

	if((RSP_TYPE_R3 == rsp_type) || ((RSP_TYPE_NORSP == rsp_type)) || ((RSP_TYPE_R2 == rsp_type))){	
		CLR_BIT_N(val, 8);// disable
	}else{
		SET_BIT_N(val, 8);// HW check Response CRC7  enable
	}
	//Auto Tx dummy after cmd bit[7]	enable
	SET_BIT_N(val, 7);
	SD_SET_CONFIG0(val);
}




inline void config0_for_dma(u32 wr)
{
	u32 val = SD_GET_CONFIG0();	
	if(wr){
		SET_BIT_N(val, 8);// enable
		//Auto Tx dummy after cmd bit[7]	enabale
		SET_BIT_N(val, 7);

		// g131.reg14 bit[5:4] wr or rd  01:wr 10: read
		CLR_BIT_N(val,5);
		SET_BIT_N(val,4);
	}else{	
		CLR_BIT_N(val, 8);// disable
		//Auto Tx dummy after cmd bit[7]	disbale
		CLR_BIT_N(val, 7);
		SET_BIT_N(val,5);
		CLR_BIT_N(val,4);
	}

	// g131.reg14 bit[2] sd_len_mode 
	// set  1 (finite)  temporary
	//SET_BIT_N(val, 2);
	// g131.reg14 bit[1]  
	// set 0 (SDR mode)
	CLR_BIT_N(val,1);
	// bit[0]  set to dma mode	
	CLR_BIT_N(val,0);
	SD_SET_CONFIG0(val);
}


inline void config_send_buf(u32 cmd, u32 arg)
{
	cmd = (cmd & 0x0FFFFFFF);
	SD_CMD_BUF0_SET( (u8) (cmd + 0x40));
	SD_CMD_BUF1_SET( (u8) ((arg >> 24) & 0x000000ff));
	SD_CMD_BUF2_SET( (u8) ((arg >> 16) & 0x000000ff));
	SD_CMD_BUF3_SET( (u8) ((arg >> 8) & 0x000000ff));
	SD_CMD_BUF4_SET( (u8) ((arg) & 0x000000ff));
}

inline void hw_start_transation(void)
{
	SD_CTRL_SET(0x01);
}

inline bool sd_rsp_buff_full(void)
{
	if(SD_STATUS0_GET() & BIT_N(1))	{
		return true;
	}else{
		return false;
	}
}

inline bool is_nomal_dma_err_timeout(void)
{
	u32 val = SD_STATUS1_GET();
	if(val & BIT_N(13)){
		return true;
	}else{
		return false;
	}
}

inline void sw_rst_channel(void)
{
	// g136.reg3 = 0x04
	DMA_HW_STOP_RST_SET(0x04);
}

inline u32 get_rst_channel_stat(void)
{
	return ((DMA_HW_STOP_RST_GET() & BIT_N(2)) >> 2);
}



inline void  set_sd_tx_dummy_num(u32 rounds)
{
	DUMMY_CLOCK_NUMBER_SET(rounds);
}


inline u32 hw_prepare_xfer_data_pio(u32 wr)
{
	u32 val = 0; 

	//reset_dut_sdio();
	val = SD_GET_CONFIG0();	
	// g131.reg14 bit[5:4] wr or rd
	if(wr){
		CLR_BIT_N(val,5);
		SET_BIT_N(val,4);
		SET_BIT_N(val, 6);
		SET_BIT_N(val, 7);
	}else{
		SET_BIT_N(val,5);	
		CLR_BIT_N(val,4);
		CLR_BIT_N(val, 7);	
		CLR_BIT_N(val, 6);
	}

	// g131.reg14 bit[2] sd_len_mode 
	SET_BIT_N(val, 2);
	// g131.reg14 bit[1]  
	// set 0 (SDR mode)
	CLR_BIT_N(val,1);
	// bit[0]  set to PIO mode	
	SET_BIT_N(val,0);
	SD_SET_CONFIG0(val);

	// g131.reg17
	val = SD_CONFIG_GET();	
	SET_BIT_N(val, 17); // 1:4 bytes for each read
	SD_CONFIG_SET(val);
	return 0;
}
