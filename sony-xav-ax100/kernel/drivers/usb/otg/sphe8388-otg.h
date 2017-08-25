#ifndef __letb_8388_H_
#define __letb_8388_H_

#include <linux/usb/otg.h>
#include "otg_fsm.h"

//#define otg_debug(fmt, args...)               printk("#@#OTG: "fmt, ##args)
#define otg_debug(fmt, args...)

//#define       CONFIG_ADP_TIMER
#define	ADP_TIMER_FREQ	(5*HZ)

struct sphe8388_otg {
	struct usb_phy otg;

	struct sphe8388_regs __iomem *regs;
	int irq;

	struct work_struct work;
	struct workqueue_struct *qwork;

	struct notifier_block notifier;
	struct otg_fsm fsm;

#ifdef	CONFIG_ADP_TIMER
	struct timer_list adp_timer;
#endif
};

struct sphe8388_regs {
	u32 mode_select;
#define		WORD_MODE_MASK	(0x03)
#define		OTG_20			(0x0)
#define		OTG_12			(0x01)
#define		OTG_HOST		(0x02)
#define		OTG_DEVICE		(0x03)

#define		OTG_SRP		(1 << 2)
#define		OTG_ADP		(1 << 3)
#define		OTG_SIM     (1 << 4)

#define		ENA_SRP(reg)		((reg) |=  OTG_SRP)
#define		DIS_SRP(reg)		((reg) &= ~OTG_SRP)
#define		ENA_ADP(reg)		((reg) |=  OTG_ADP)
#define		DIS_ADP(reg)		((reg) &= ~OTG_ADP)
#define		ENA_SIM(reg)		((reg) |=  OTG_SIM)
#define		DIS_SIM(reg)		((reg) &= ~OTG_SIM)

	u32 otg_device_ctrl;
#define		B_HNP_EN_BIT			(1 << 5)
#define		A_CLE_ERR_BIT			(1 << 3)
#define		A_SET_B_HNP_EN_BIT	(1 << 2)
#define		A_BUS_REQ_BIT			(1 << 1)
#define		A_BUS_DROP_BIT		(1 << 0)

#define		B_HNP_EN(reg)			((reg)|= (1 << 5))
#define		B_BUS_REQ(reg)			((reg)|= (1 << 4))
#define		A_CLE_ERR(reg)			((reg)|= (1 << 3))
#define		A_SET_B_HNP_EN(reg)		((reg)|= (1 << 2))
#define		A_BUS_REQ(reg)			((reg)|= (1 << 1))
#define		A_BUS_DROP(reg)			((reg)|= (1 << 0))

	u32 otg_init_en;
	u32 otg_int_st;
#define		ID_PIN					(1 << 16)
#define		INT_MASK				(0x3ff)
#define		ADP_CHANGE_IF			(1 << 9)
#define		A_SRP_DET_IF			(1 << 8)
#define		B_AIDL_BDIS_IF			(1 << 7)
#define		A_BIDL_ADIS_IF			(1 << 6)
#define		A_AIDS_BDIS_TOUT_IF 	(1 << 5)
#define		B_SRP_FAIL_IF			(1 << 4)
#define		BDEV_CONNT_TOUT_IF 	(1 << 3)
#define		VBUS_RISE_TOUT_IF		(1 << 2)
#define		ID_CHAGE_IF			(1 << 1)
#define		OVERCURRENT_IF 		(1 << 0)

#define 		ENABLE_OTG_INT(x)		iowrite32(0x3fff, x)

	u32 a_wait_vrise_tmr;
	u32 b_send_srp_tmr;	//5
	u32 b_se0_srp_tmr;
	u32 b_data_pls_tmr;
	u32 b_srp_fail_tmr;
	u32 b_svld_bcon_tmr;	//9
	u32 b_aidl_bdis_tmr;
	u32 a_bdis_acon_tmr;
	u32 ldis_dschg_tmr;
	u32 a_bcon_sdb_tmr;
	u32 a_bcon_ldb_tmr;
	u32 a_adp_prb_tmr;	//15
	u32 b_adp_prb_tmr;
	u32 b_adp_detach_tmr;
	u32 adp_chng_precision;
	u32 a_wait_vfall_tmr;
	u32 a_wait_bcon_tmr;
	u32 a_aidl_bdis_tmr;	//21
	u32 b_ase0_brst_tmr;
	u32 a_bidl_adis_tmr;
	u32 adp_chrg_time;
	u32 vbus_pules_time;
	u32 a_bcon_sdb_win;	//26
	u32 otg_debug_reg;
	u32 adp_debug_reg;
};

void sphe_otg_update_transceiver(struct sphe8388_otg *);

#endif
