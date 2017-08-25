#ifndef _SP_UDC_REGS_H
#define _SP_UDC_REGS_H

#define USB_NAK_EP_EP0 		0x0
#define USB_NAK_EP1_BULK_IN 	0x1
#define USB_NAK_EP2_BULK_OUT 	0x2
#define USB_NAK_EP8_BULK_IN 	0x3
#define USB_NAK_EP9_BULK_OUT 	0x4
#define USB_NAK_EPB_BULK_OUT    0x5

#define USB_ACK_EP_EP0 		0x0
#define USB_ACK_EP1_BULK_IN 	0x1
#define USB_ACK_EP2_BULK_OUT 	0x2
#define USB_ACK_EP8_BULK_IN 	0x3
#define USB_ACK_EP9_BULK_OUT 	0x4
#define USB_ACK_EPB_BULK_OUT    0x5

#define USB_DMA_EP1_BULK_IN 	0x1
#define USB_DMA_EP2_BULK_OUT 	0x2
#define	USB_DMA_EP8_BULK_IN	0x3
#define USB_DMA_EP9_BULK_OUT	0x4
#define USB_DMA_EPB_BULK_OUT    0x5
#define USB_DMA_EPA_BULK_IN     0x6

#define  UDC_CS_OFST		0x080
#define  UDC_IE_OFST		0x084
#define  UDC_IF_OFST		0x088
#define  UDC_CIS_OFST		0x08C
#define  UDC_LLCSET0_OFST	0x03B0
#define  UDC_LLCSET1_OFST	0x03B4
#define  UDC_LLCS_OFST		0x03C0
#define  UDC_EP0SCS_OFST	0x0300
#define  UDC_EP0SDP_OFST	0x0304
#define  UDC_EP0CS_OFST		0x0308
#define  UDC_EP0DC_OFST		0x030C
#define  UDC_EP0DP_OFST		0x0310
#define  UDC_EP0DPVLD_OFST	0x0314
#define  UDC_EP12C_OFST		0x0330
#define  UDC_EP12PPC_OFST	0x0334
#define  UDC_LLCFS_OFST		0x0338
#define  UDC_EP12PINGFCL_OFST	0x033C
#define  UDC_EP12PINGFCH_OFST	0x0340
#define  UDC_EP12PONGFCL_OFST	0x0344
#define  UDC_EP12PONGFCH_OFST	0x0348
#define  UDC_EP12FDP_OFST	0x034C
#define  UDC_EP12FDPVLD_OFST	0x0350
#define  UDC_EP1SCS_OFST	0x0358
#define  UDC_LLCSET2_OFST	0x03C0
#define  UDC_LLCSTL_OFST	0x03C8

#define  UDC_EP3CS_OFST		0x0374


#define  UDC_LLCIF_OFST		0x0400
#define  UDC_LLCIE_OFST		0x0404
#define	 UDC_LLCIS_OFST		0x0408
#define  STD_REQ_IF 		0x410
#define	 STD_REQ_IE		0x414
#define	 STD_REQ_IS		0x418

#define  STD_SET_INTF	(1 << 9)


//remon add
#define	UDC_NBIF_OFFST		0x0420
#define UDC_NBIE_OFFST		0x0424
#define UDC_EP89C_OFST		0x0500
#define UDC_EP89PPC_OFST	0x0504
#define UDC_EP89FS_OFST		0x0508
#define UDC_EP89PINGFCL_OFST	0x050C
#define UDC_EP89PINGFCH_OFST	0x0510
#define UDC_EP89PONGFCL_OFST	0x0514
#define UDC_EP89PONGFCH_OFST	0x0518
#define UDC_EP89FDP_OFST	0x051C
#define UDC_EP89FDPVLD_OFST	0x0520
#define UDC_EP8INAKCN_OFST	0x0524

//endpoint AB register
#define UDC_EPABC_OFST		0x0550
#define UDC_EPABPPC_OFST	0x0554
#define UDC_EPABFS_OFST		0x0558
#define UDC_EPABPINGFCL_OFST	0x055C
#define UDC_EPABPINGFCH_OFST	0x0560
#define UDC_EPABPONGFCL_OFST	0x0564
#define UDC_EPABPONGFCH_OFST	0x0568
#define UDC_EPABFDP_OFST	0x056C
#define UDC_EPABFDPVLD_OFST	0x0570
#define UDC_EPBINAKCN_OFST	0x0574

#define  DMACS_DMA_EN           0x80000000/*(1 << 31)*/
#define  DMACS_DMA_FLUSHEND     0x40000000/*(1 << 30)*/
#define  DMACS_DMA_FLUSH        0x20000000/*(1 << 29)*/

/* EP0CS */
#define EP0CS_OUT_EMPTY     	0x80/*(1 << 7)*/
#define EP0CS_OVLD              0x40/*(1 << 6)*/
#define EP0CS_CLR_EP0_OVLD  	0x20/*(1 << 5)*/
#define EP0CS_IVLD              0x10/*(1 << 4)*/
#define EP0CS_SET_EP0_IVLD    	0x08/*(1 << 3)*/
#define EP0CS_SFIFO_UPDATE   	0x04/*(1 << 2)*/
#define EP0CS_SFIFO_VALID      	0x02/*(1 << 1)*/
#define EP0CS_DIR_IN            0x01/*(1 << 0)*/

/* LCSET0 */
#define LCSET0_CLR_SUSP_CNT   	0x80/*(1 << 7)*/
#define LCSET0_SIM_MODE         0x40/*(1 << 6)*/
#define LCSET0_DISC_SUSP_EN   	0x20/*(1 << 5)*/
#define LCSET0_CPU_WKUP_EN    	0x10/*(1 << 4)*/
#define LCSET0_PWR_PART_N      	0x08/*(1 << 3)*/
#define LCSET0_PWR_SUSP_N      	0x04/*(1 << 2)*/
#define LCSET0_ISSUE_RESUME   	0x02/*(1 << 1)*/
#define LCSET0_SOFT_DISC        0x01/*(1 << 0)*/

/* LCSET1 */
#define LCSET0_NO_SET_SUSP_OPT         0x80/*(1 << 7)*/
#define LCSET0_NO_STOP_CHIRP           0x40/*(1 << 6)*/
#define LCSET0_INTER_PACKET_DLY        0x20/*(1 << 5)*/
#define LCSET0_FORCE_FULLSP            0x10/*(1 << 4)*/
#define LCSET0_VBUS_LOW_AUTO_DISC	0x08/*(1 << 3)*/
#define LCSET0_DISC_AUTO_DPDMPD        0x04/*(1 << 2)*/
#define LCSET0_SUPP_RWAKE              0x02/*(1 << 1)*/
#define LCSET0_SELF_POWER              0x01/*(1 << 0)*/

/* EP1SCS */
#define EP1SCS_FIFO_CNTR_MSK  	0xF0
#define EP1SCS_CLR_IVLD         0x08  /*(1 << 3)*/
#define EP1SCS_RESET_FIFO       0x04  /*(1 << 2)*/
#define EP1SCS_IVLD             0x02  /*(1 << 1)*/
#define EP1SCS_SET_IVLD     	0x01  /*(1 << 0)*/

/* EP12C */
#define EP12C_MSDC_CMD_VLD 	0x80/*(1 << 7)*/
#define EP12C_EP2_OVLD          0x40/*(1 << 6)*/
#define EP12C_EP1_IVLD          0x20/*(1 << 5)*/
#define EP12C_SET_EP1_IVLD   	0x10/*(1 << 4)*/
#define EP12C_CLR_EP2_OVLD   	0x08/*(1 << 3)*/
#define EP12C_RESET_PIPO       	0x04/*(1 << 2)*/
#define EP12C_ENABLE_BULK    	0x02/*(1 << 1)*/
#define EP12C_DIR_IN            0x01/*(1 << 0)*/

/* EP12PPC */
#define EP12C_CURRENT_BUFFER    0x04  /*(1 << 2)*/
#define EP12C_SWITCH_BUFFER    	0x02  /*(1 << 1)*/
#define EP12C_AUTO_SWITCH       0x01  /*(1 << 0)*/

/* EP7 */
#define UDC_EPADMACS_OFST	0x008
#define UDC_EPADMADA_OFST	0x00c
#define UDC_EP7CTR_OFST		0x1c0
#define UDC_EP7DAT_OFST		0x1cc
#define UDC_EP7VB_OFST		0x1d0
#define EP7_DIR			(1<<0)
#define EP7_VLD			(1<<3)
#define EP7_FLUSH		(1<<7)
#define EP7_DMA_FLUSH		(1<<30)
#define EP7_DMA_EN		(1<<31)


/* EP89PPC */
#define EP89C_CURRENT_BUFFER    0x04/*(1 << 2)*/
#define EP89C_SWITCH_BUFFER    	0x02/*(1 << 1)*/
#define EP89C_AUTO_SWITCH       0x01/*(1 << 0)*/

/*EPABPPC*/
#define EPABC_CURRENT_BUFFER    0x04/*(1 << 2)*/
#define EPABC_SWITCH_BUFFER    	0x02/*(1 << 1)*/
#define EPABC_AUTO_SWITCH       0x01/*(1 << 0)*/


/* EP89C */
#define EP89C_MSDC_CMD_VLD 	0x80/*(1 << 7)*/
#define EP89C_EP9_OVLD          0x40/*(1 << 6)*/
#define EP89C_EP8_IVLD          0x20/*(1 << 5)*/
#define EP89C_SET_EP8_IVLD   	0x10/*(1 << 4)*/
#define EP89C_CLR_EP9_OVLD   	0x08/*(1 << 3)*/
#define EP89C_RESET_PIPO       	0x04/*(1 << 2)*/
#define EP89C_ENABLE_BULK    	0x02/*(1 << 1)*/
#define EP89C_DIR_IN            0x01/*(1 << 0)*/

/*EPABC*/
#define EPABC_EPB_OVLD          0x40 /*(1 << 6)*/
#define EPABC_EPA_IVLD          0x20 /*(1 << 5)*/
#define EPABC_SET_EPA_IVLD   	0x10 /*(1 << 4)*/
#define EPABC_CLR_EPB_OVLD   	0x08 /*(1 << 3)*/
#define EPABC_RESET_PIPO       	0x04 /*(1 << 2)*/
#define EPABC_ENABLE_BULK    	0x02 /*(1 << 1)*/
#define EPABC_DIR_IN            0x01 /*(1 << 0)*/


#define	EP12_DMA_COUNT ((1 << 22) - 1)
/* EP12FCH */
#define  EP12FCH_RESET_CNTR  		0x04/*(1 << 2)*/
#define  EP12FCH_FIFO_CNTRH_MASK  	0x03

/* EP89FCH */
#define  EP89FCH_RESET_CNTR  		0x04/*(1 << 2)*/
#define  EP89FCH_FIFO_CNTRH_MASK  	0x03

/* EPABFCH */
#define  EPABFCH_RESET_CNTR         0x04/*(1 << 2)*/
#define  EP89FCH_FIFO_CNTR_MASK     0x03

/* EP3CS */
#define EP3CS_IVLD        0x08/*(1 << 3)*/
#define EP3CS_CLR_IVLD    0x04/*(1 << 2)*/
#define EP3CS_SET_IVLD    0x02/*(1 << 1)*/
#define EP3CS_IN_EN       0x01/*(1 << 0)*/

/* LCSTL */
#define LCSTL_CLREPBSTL   0x8000/*(1 << 15)*/
#define LCSTL_CLREPASTL   0x4000/*(1 << 14)*/
#define LCSTL_CLREP9STL   0x2000/*(1 << 13)*/
#define LCSTL_CLREP8STL   0x1000/*(1 << 12)*/
#define LCSTL_CLREP3STL   0x800 /*(1 << 11)*/
#define LCSTL_CLREP2STL   0x400 /*(1 << 10)*/
#define LCSTL_CLREP1STL   0x200 /*(1 << 9)*/
#define LCSTL_CLREP0STL   0x100 /*(1 << 8)*/

#define LCSTL_SETEPBSTL   0x80/*(1 << 7)*/
#define LCSTL_SETEPASTL   0x40/*(1 << 6)*/
#define LCSTL_SETEP9STL   0x20/*(1 << 5)*/
#define LCSTL_SETEP8STL   0x10/*(1 << 4)*/
#define LCSTL_SETEP3STL   0x08/*(1 << 3)*/
#define LCSTL_SETEP2STL   0x04/*(1 << 2)*/
#define LCSTL_SETEP1STL   0x02/*(1 << 1)*/
#define LCSTL_SETEP0STL   0x01/*(1 << 0)*/

/* CIS */
#define CIS_EPAB_DMA_IF         0x40000000/*(1 << 30)*/
#define CIS_EP89_DMA_IF         0x20000000/*(1 << 29)*/
#define CIS_FDISCONN_IF         0x08000000/*(1 << 27)*/
#define CIS_FCONN_IF            0x04000000/*(1 << 26)*/
#define CIS_DMA_IF              0x02000000/*(1 << 25)*/
#define CIS_VBUS_IF             0x01000000/*(1 << 24)*/
#define CIS_UDLC_IF             0x00020000/*(1 << 17)*/
#define CIS_VBUSPRE_IF          0x00020000/*(1 << 17)*/

/*UDCCS*/
#define UDCCS_FDISCONN_IF         0x08000000/*(1 << 27)*/
#define UDCCS_FCONN_IF            0x04000000/*(1 << 26)*/

/* CIE */
#define CIE_EPAB_DMA_IE       0x40000000/*(1 << 30)*/
#define CIE_EP89_DMA_IE  	  0x20000000/*(1 << 29)*/
#define CIE_FDISCONN_IE  	  0x08000000/*(1 << 27)*/
#define CIE_FCONN_IE          0x04000000/*(1 << 26)*/
#define CIE_DMA_IE            0x02000000/*(1 << 25)*/
#define CIE_VBUS_IE           0x01000000/*(1 << 24)*/

/* LCIE */
#define UDLC_RESETN_IE 		0x80000/*(1 << 19)*/
#define UDLC_SCONF_IE 		0x40000/*(1 << 18)*/
#define UDLC_RESUME_IE 		0x20000/*(1 << 17)*/
#define UDLC_SUSPEND_IE 	0x10000/*(1 << 16)*/
#define UDLC_DMA_IE 		0x08000/*(1 << 15)*/ /*DMA data finish Int*/
//#define UDLC_EP3I_IE		0x04000/*(1 << 14)*/
#define UDLC_PIPO_IE		0x02000/*(1 << 13)*/
#define UDLC_HCS_IE		0x01000/*(1 << 12)*/
#define UDLC_EP2N_IE		0x00800/*(1 << 11)*/
#define UDLC_EP1N_IE		0x00400/*(1 << 10)*/
#define UDLC_EP0N_IE		0x00200/*(1 << 9)*/
#define UDLC_HSS_IE		0x00100/*(1 << 8)*/
#define UDLC_EP3I_IE		(1 << 14)
#define UDLC_EP2O_IE		0x00080/*(1 << 7)*/
#define UDLC_EP1I_IE		0x00040/*(1 << 6)*/
#define UDLC_EP1SI_IE		0x00020/*(1 << 5)*/
#define UDLC_EP0I_IE		0x00010/*(1 << 4)*/
#define UDLC_EP0O_IE		0x00008/*(1 << 3)*/
#define UDLC_EP0S_IE		0x00004/*(1 << 2)*/
#define UDLC_ESUSP_IE		0x00002/*(1 << 1)*/
#define UDLC_RESET_IE		0x00001/*(1 << 0)*/

//add by remon
#define UDLC_EP8DMA_IE 		0x001/*(1 << 0)*/
#define UDLC_EP8DMA_IF 		0x001/*(1 << 0)*/
#define UDLC_EPADMA_IE 		(1<<6)
#define UDLC_EPADMA_IF 		(1<<6)

#define UDLC_EP9O_IE		0x020/*(1 << 5)*/
#define UDLC_EP8I_IF		0x010/*(1 << 4)*/
#define UDLC_EP8I_IE		0x010/*(1 << 4)*/
#define UDLC_EP8N_IE		0x004/*(1 << 2)*/
#define UDLC_EP9N_IE		0x008/*(1 << 3)*/
#define UDLC_EP8N_IF		0x004/*(1 << 2)*/
#define UDLC_EP9N_IF		0x008/*(1 << 3)*/

#define UDLC_EPBN_IE        0x200/*(1 << 9)*/
#define UDLC_EPBN_IF        0x200/*(1 << 9)*/
#define UDLC_EPBO_IE        0x800/*(1 << 11)*/
#define UDLC_EPBO_IF        0x800/*(1 << 11)*/
#define UDLC_EPAI_IF        (1 << 10)
#define UDLC_EPAN_IF        (1 << 8)
#define UDLC_EP9O_IF        (1 << 5)
//#define UDLC_EP9N_IF        (1 << 3)
#define EP9_OVLD	(1 << 6)
#define CLR_EP9_OVLD	(1 << 3)
#define EP89_ENA	(1 << 1)
#define EP89_DIR	(1 << 0)

/* LCIS */
#define UDLC_RESETN_IF 		0x80000/*(1 << 19)*/
#define UDLC_SCONF_IF 		0x40000/*(1 << 18)*/
#define UDLC_RESUME_IF 		0x20000/*(1 << 17)*/
#define UDLC_SUSPEND_IF 	0x10000/*(1 << 16)*/
#define UDLC_DMA_IF 		0x08000/*(1 << 15)*/
#define UDLC_EP3I_IF		0x04000/*(1 << 14)*/
#define UDLC_PIPO_IF		0x02000/*(1 << 13)*/
#define UDLC_HCS_IF		0x01000/*(1 << 12)*/
#define UDLC_EP2N_IF		0x00800/*(1 << 11)*/
#define UDLC_EP1N_IF		0x00400/*(1 << 10)*/
#define UDLC_EP0N_IF		0x00200/*(1 << 9)*/
#define UDLC_HSS_IF		0x00100/*(1 << 8)*/
#define UDLC_EP2O_IF		0x00080/*(1 << 7)*/
#define UDLC_EP1I_IF		0x00040/*(1 << 6)*/
#define UDLC_EP1SI_IF		0x00020/*(1 << 5)*/
#define UDLC_EP0I_IF		0x00010/*(1 << 4)*/
#define UDLC_EP0O_IF		0x00008/*(1 << 3)*/
#define UDLC_EP0S_IF		0x00004/*(1 << 2)*/
#define UDLC_ESUSP_IF		0x00002/*(1 << 1)*/
#define UDLC_RESET_IF		0x00001/*(1 << 0)*/

/*LCS*/
#define LCS_DISC_CONN_STATUS           0x80/*(1 << 7)*/
#define LCS_HOST_CONFIGED              0x40/*(1 << 6)*/
#define LCS_LNK_SUSP                   0x20/*(1 << 5)*/
#define LCS_HOST_ALLOW_RWAKE           0x10/*(1 << 4)*/
#define LCS_CURR_LINESTATE_SE0         0x00
#define LCS_CURR_LINESTATE_F_J         0x04/*(1 << 2)*/
#define LCS_CURR_LINESTATE_F_K         0x08/*(1 << 3)*/
#define LCS_CURR_LINESTATE_H_SQUELCH   0x04/*(1 << 2)*/
#define LCS_CURR_LINESTATE_MASK        0xC0/*(1 << 7)|(1 << 6)*/
#define LCS_CURR_SPEED_F               0x02/*(1 << 1)*/
#define LCS_VBUS_HIGH                  0x01/*(1 << 0)*/

/* LCFS */
#define CLR_EP2_OVLD      0x00020/*(1 << 5)*/
#define SET_EP1_IVLD      0x00010/*(1 << 4)*/
#define MSDC_CMD_VLD      0x00008/*(1 << 3)*/
#define CUR_FIFO_EMPTY 	  0x00004/*(1 << 2)*/
#define LCFS_EP2_OVLD     0x00002/*(1 << 1)*/
#define LCFS_EP1_IVLD     0x00001/*(1 << 0)*/

/* ep89 LCFS */
//#define CLR_EP9_OVLD      0x00020
#define A_EP8_IVLD	0x00010/*(1 << 4)*/
#define LCFS_EP9_OVLD     0x00002/*(1 << 1)*/
#define LCFS_EP8_IVLD     0x00001/*(1 << 0)*/

/*DMA */
/* ep12 */
#define  UDC_DMA_CS_OFST		0x000
#define  UDC_DMA_DA_OFST		0x004
#define  EP12_DMA_FIFO_FLUSH		(1 << 29)
#define  EP12_DMA_FLUSH_END		(1 << 30)

/* ep89 */
#define  UDC_EP89_DMA_CS_OFST		0x010
#define  UDC_EP89_DMA_DA_OFST		0x014

#define  DMACS_DMA_READ           0x00000000
#define  DMACS_DMA_WRITE          0x1c000000

/* epAB*/
#define  UDC_EPAB_DMA_CS_OFST		0x018
#define  UDC_EPAB_DMA_DA_OFST		0x01C



#endif //_SP_UDC_REGS_H
