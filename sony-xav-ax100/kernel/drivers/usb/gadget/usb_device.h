#ifndef _USB_DEVICE
#define _USB_DEVICE

/*G0:--DMA Control Registers */
#define  UEP12DMACS		0x000
#define  UEP12DMADA		0x004
#define  UDADMAS		0x008
#define  UDADMADA		0x00c
#define  UEP89DMACS		0x010
#define  UEP89DMADA		0x014
#define  UEPABDMACS		0x018
#define  UEPABDMADA		0x01c
#define  UDVDMACS		0x020
#define  UDVDMADA		0x024
#define  UDEPCDMACS		0x028
#define  UDEPCDMADA		0x02c
#define  UDEP2DMACS		0x030
#define  UDEP2DMADA		0x034
#define  UDEP9DMACS		0x038
#define  UDEP9DMADA		0x03c
#define  UDEPBDMACS		0x040
#define  UDEPBDMADA		0x044

/*G1:--System Domain Interrupt Registers */
#define  UDCCS			0x080
#define  UDCIE			0x084
#define  UDCIF			0x088
#define  UDADMA_CRCR		0x0c0
#define  UDADMA_ERBAR		0x0c4
#define  UDADMA_ERDPR		0x0c8
#define  UDADMA_RCSR		0x0cc
#define  UDADMA_RTR		0x0d0
#define  UDVDMA_CRCR		0x0d4
#define  UDVDMA_ERBAR		0x0d8
#define  UDVDMA_ERDPR		0x0dc
#define  UDVDMA_RCSR		0x0e0
#define  UDVDMA_RTR		0x0e4
#define  UDEPCDMA_CRCR		0x0e8
#define  UDEPCDMA_ERBAR		0x0ec
#define  UDEPCDMA_ERDPR		0x0f0
#define  UDEPCDMA_RCSR		0x0f4
#define  UDEPCDMA_RTR		0x0f8

/*G2:--EP0/1/2/3/4 Control Registers */
#define  UDLCSET		0x100
#define  UDLCSTL		0x104
#define  UDLCADDR		0x10c
#define  UDEP0SDP		0x110
#define  UDEP0CS		0x114
#define  UDEP0DC		0x118
#define  UDEP0DP		0x11c
#define  UDEP0VB		0x120
#define  UDEP0ONAKCN		0x124
#define  UDEP0INAKCN		0x128
#define  UDCMSTC		0x130
#define  UDEP12C		0x134
#define  UDEP12PPC		0x138
#define  UDEP12FS		0x13c
#define  UDEP12PIC		0x140
#define  UDEP12POC		0x144
#define  UDEP12FDP		0x148
#define  UDEP12VB		0x14c
#define  UDEP1SCS		0x150
#define  UDEP1SDP		0x154
#define  UDEP1INAKCN		0x158
#define  UDEP2ONAKCN		0x15c
#define  UDEP3VB		0x160
#define  UDEP3CTRL		0x164
#define  UDEP3PTR		0x168
#define  UDEP3DATA		0x16c
#define  UDEP4CTRL		0x170
#define  UDEP4PTR		0x174
#define  UDEP4DATA		0x178
#define  UDEP4VB		0x17c

/*G3:--EP5/6/7  Control Registers */
#define  UDEP5CTRL		0x180
#define  UDEP5HDLEN		0x184
#define  UDEP5FRAME		0x188
#define  UDEP5HDCTRL		0x18c
#define  UDEP5RPT		0x190
#define  UDEP5WPT		0x194
#define  UDEP5DATA		0x198
#define  UDSRE			0x19c
#define  UDFT			0x1a0
#define  UDEP5VB		0x1a4
#define  UDSTCDIV		0x1a8
#define  UDEP6CTRL		0x1c0
#define  UDEP6PTR		0x1c4
#define  UDEP6DATA		0x1c8
#define  UDEP6VB		0x1cc
#define  UDEP7CTRL		0x1e0
#define  UDEP7RPTR		0x1e4
#define  UDEP7WPTR		0x1e8
#define  UDEP7DATA		0x1ec
#define  UDEP7VB		0x1f0

/*G4:--EP8/9/A/B  Control Registers */
#define  UDEP89C		0x200
#define  UDEP89PPC		0x204
#define  UDEP89FS		0x208
#define  UDEP89PIC		0x20c
#define  UDEP89C		0x200
#define  UDEP89PPC		0x204
#define  UDEP89FS		0x208
#define  UDEP89PIC		0x20c
#define  UDEP89POC		0x210
#define  UDEP89FDP		0x214
#define  UDEP89VB		0x218
#define  UDEP8INAKCN		0x21c
#define  UDEP9ONAKCN		0x220
#define  UDEP89S		0x224
#define  UDEPABC		0x240
#define  UDEPABPPC		0x244
#define  UDEPABFS		0x248
#define  UDEPABPIC		0x24c
#define  UDEPABPOC		0x250
#define  UDEPABFDP		0x254
#define  UDEPABVB		0x258
#define  UDEPAINAKCN		0x25c
#define  UDEPBONAKCN		0x260
#define  UDEPABS		0x264

/*G5:--EPC/D  Control Registers */
#define  UDEPCCTRL		0x280
#define  UDEPCRPT		0x284
#define  UDEPCWPT		0x288
#define  UDEPCDATA		0x28c
#define  UDEPCS			0x290
#define  UDEPCBDC		0x294
#define  UDEPDCTRL		0x298
#define  UDEPDST		0x29c
#define  UDEPDDATA		0x2a0

/*G6:--Phyclk Domain Interrupt Registers */
#define  UDLIF			0x300
#define  UDLIE			0x304
#define  UDNBIF			0x308
#define  UDNBIE			0x30c

#define  UDFRNUM		0x310
#define  IP_Name		0x378
#define  VERSION		0x37c

/*G7:--Bulk Out FIFO Control Registeres */
#define  UDEP2PPC		0x380
#define  UDEP2FS		0x384
#define  UDEP2PIC		0x388
#define  UDEP2POC		0x38c

#define  UDEP2FDP		0x390
#define  UDEP2VB		0x394

#define  UDEP9PPC		0x3a0
#define  UDEP9FS		0x3a4
#define  UDEP9PIC		0x3a8
#define  UDEP9POC		0x3ac

#define  UDEP9FDP		0x3b0
#define  UDEP9VB		0x3b4

#define  UDEPBPPC		0x3c0
#define  UDEPBFS		0x3c4
#define  UDEPBPIC		0x3c8
#define  UDEPBPOC		0x3cc

#define  UDEPBFDP		0x3d0
#define  UDEPBVB		0x3d4

/*DMA*/
#define  DMA_EN           	(1 << 31)
#define  DMA_FLUSHEND		(1 << 30)
#define  DMA_FLUSH		(1 << 29)
#define  DMA_WRITE		(1 << 28)
#define  DMA_COUNT_ALIGN	(1 << 27)
#define  DMA_COUNT_MASK		((1 << 22) - 1)/*21bit:0x1F FF FF*/
/*AUDIO COUNT MASK*/
#define  DMAA_COUNT_MASK	((1 << 16) - 1)/*15bit:0x1F FF*/
/*USB Device Controller Control Status (UDCCS)*/
#define  USB_CLK_EN           	(1 << 31)
#define  UPHY_CLK_CSS           (1 << 30)
#define  UPHY_SUSP           	(1 << 29)
#define  ISSUE_RESUME           (1 << 28)
#define  VBUS_PRE           	(1 << 25)
#define  VBUS           	(1 << 24)
#define  VBUS_SAMPLE_MASK	((1 << 16) - 1)/*15bit:0x1F FF*/
/*UDCIE UDCIF*/
#define EPC_TRB_IF 		(1 << 15)
#define VIDEO_TRB_IF		(1 << 14)
#define AUDIO_TRB_IF		(1 << 13)
#define EPC_ERF_IF		(1 << 12)
#define VIDEO_ERF_IF		(1 << 11)
#define AUDIO_ERF_IF		(1 << 10)
#define EPB_DMA_IF		(1 << 9)
#define EP9_DMA_IF		(1 << 8)
#define EP2_DMA_IF		(1 << 7)
#define EPC_DMA_IF		(1 << 6)
#define VIDEO_DMA_IF		(1 << 5)
#define EPAB_DMA_IF		(1 << 4)
#define EP89_DMA_IF		(1 << 3)
#define AUDIO_DMA_IF		(1 << 2)
#define EP12_DMA_IF		(1 << 1)
#define VBUS_IF			(1 << 0)
/*UDADMA_CRCR*/
#define  CRPTR_MASK		0xFFFFFFC0 /*0xFF FF FF C0*/
#define  CRR			(1 << 3)
#define  CS			(1 << 1)
#define  RCS			(1 << 0)

#define  ERBA_MASK		0xFFFFFFF0 /*0xFF FF FF F0*/
#define  ERBP_MASK		0xFFFFFFF0 /*0xFF FF FF F0*/

#define  CRT			(1 << 0)/*Command Ring Trig*/
/*USB Device Linker Layer Controller Setting (UDLCSET)*/
#define  CURR_LINSTATE_MASK	(0X3 << 18) /*bit18~19*/
#define  CURR_ALT_MASK		(0XF << 13) /*bit13~16*/
#define  CURR_INTF_MASK		(0XF << 9) /*bit9~12*/
#define  CURR_SPEED		(1 << 8)
#define SUPP_SYNCFRAME		(1 << 5)
#define SUPP_SETDESC		(1 << 4)
#define NEG_SAMPLE_EN		(1 << 3)
#define FORCE_FULLSP		(1 << 2)
#define SIM_MODE		(1 << 1)
#define SOFT_DISC		(1 << 0)
/*UDLCSTL */
#define CLREPDSTL		(1 << 26)
#define CLREPBSTL 		(1 << 25)
#define CLREPASTL		(1 << 24)
#define CLREP9STL		(1 << 23)
#define CLREP8STL		(1 << 22)
#define CLREP6STL		(1 << 21)
#define CLREP4STL		(1 << 20)
#define CLREP3STL		(1 << 19)
#define CLREP2STL		(1 << 18)
#define CLREP1STL		(1 << 17)
#define CLREP0STL		(1 << 16)

#define SETEPDSTL		(1 << 10)
#define SETEPBSTL		(1 << 9)
#define SETEPASTL		(1 << 8)
#define SETEP9STL		(1 << 7)
#define SETEP8STL		(1 << 6)
#define SETEP6STL		(1 << 5)
#define SETEP4STL		(1 << 4)
#define SETEP3STL		(1 << 3)
#define SETEP2STL		(1 << 2)
#define SETEP1STL		(1 << 1)
#define SETEP0STL		(1 << 0)
/*Device address register*/
#define ADDR_VLD		(1 << 7)
#define  DEV_ADDR_MASK		((1 << 7) - 1)/*6bit*/
/*UDEP0CS*/
#define EP0_OUT_EMPTY		(1 << 5)
#define EP0_OVLD		(1 << 4)
#define CLR_EP0_OUT_VLD		(1 << 3)
#define EP0_IVLD		(1 << 2)
#define SET_EP0_IN_VLD		(1 << 1)
#define EP0_DIR			(1 << 0) /*IN*/

#define  EP0_DATA_CNTR_MASK		((1 << 7) - 1)/*6bit*/
/*UDEP12C*/
#define SET_EP_IVLD		(1 << 4)
#define CLR_EP_OVLD		(1 << 3)
#define RESET_PIPO_FIFO		(1 << 2)
#define EP_ENA			(1 << 1)
#define EP_DIR			(1 << 0)

#define CURR_BUFF		(1 << 2)
#define SWITCH_BUFF		(1 << 1)
#define AUTO_SWITCH_EN		(1 << 0)
/*UDEP12FS*/
#define N_MSDC_CMD		(1 << 7)
#define A_FIFO_EMPTY		(1 << 6)
#define A_EP_OVLD		(1 << 5)
#define A_EP_IVLD		(1 << 4)
#define MSDC_CMD_VLD		(1 << 3)
#define FIFO_EMPTY		(1 << 2)
#define EP_OVLD		(1 << 1)
#define EP_IVLD		(1 << 0)

#define RESET_EP_PING_CNTR		(1 << 12)
#define RESET_EP_PONG_CNTR		(1 << 12)
/* EP3CS */
#define EP3_VLD        		(1 << 3)
#define EP3_EN       		(1 << 0)
#define EP4_VLD        		(1 << 3)
#define EP4_EN       		(1 << 0)

#define UDEP5FLUSH		(1 << 2)
#define UDEP5HDEN		(1 << 1)
#define UDEP5EN			(1 << 0)

#define EP5CTRLEMPTY		(1 << 7)
#define EP5CTRLFULL		(1 << 6)
#define UDEP5FRAMEID		(1 << 0)

#define UDEP5ERROR		(1 << 4)
#define UDEP5FRAMEND		(1 << 3)
#define UDEP5FRAMESTILL		(1 << 2)
#define UDEP5FRAMESRC		(1 << 1)
#define UDEP5FRAMEPT		(1 << 0)

#define EP6_VLD        		(1 << 3)
#define EP6_EN       		(1 << 0)

/*UDEP89C*/
#define SET_EP8_IVLD		(1 << 4)
#define CLR_EP9_OVLD		(1 << 3)
#define EP89_RESET_PIPO_FIFO	(1 << 2)
#define EP89_ENA		(1 << 1)
#define EP89_DIR		(1 << 0)

#define EPAB_DIR		(1 << 3)

#define UDEPCEMPTY		(1 << 3)
#define UDEPCFULL		(1 << 2)
#define UDEPCFLUSH		(1 << 1)
#define UDEPCEN			(1 << 0)

#define EPC_SUCC_IF		(1 << 31)
#define EPC_FAIL_IF		(1 << 30)
#define EPC_OFLOW_IF		(1 << 29)
#define EP6_FAIL_IF		(1 << 27)
#define EP3_FAIL_IF		(1 << 26)
#define EP7_DMA_IF		(1 << 25)
#define EP5_DMA_IF		(1 << 24)
#define EP7I_IF			(1 << 23)
#define EP6I_IF			(1 << 22)
#define EP5I_IF			(1 << 21)
#define EP4I_IF			(1 << 20)
#define URES_IF			(1 << 29)
#define EP4_FAIL_IF		(1 << 18)
#define RESU_IF			(1 << 17)
#define SUS_IF			(1 << 16)
#define EP1_DMA_IF		(1 << 15)
#define EP3I_IF			(1 << 14)
#define PIPO_IF			(1 << 13)
#define TEST_IF			(1 << 12)
#define EP2N_IF			(1 << 11)
#define EP1N_IF			(1 << 10)
#define EP0N_IF			(1 << 9)
#define ADDR_IF			(1 << 8)
#define EP2O_IF			(1 << 7)
#define EP1I_IF			(1 << 6)
#define EP1SI_IF		(1 << 5)
#define EP0I_IF			(1 << 4)
#define EP0O_IF			(1 << 3)
#define EP0S_IF			(1 << 2)
#define SUSP_IF			(1 << 1)
#define RESET_IF		(1 << 0)

#define EPDOVLD_IF		(1 << 16)
#define SOF_IF			(1 << 15)
#define PIPOEPB_IF		(1 << 14)
#define PIPOEP9_IF		(1 << 13)
#define PIPOEP2_IF		(1 << 12)
#define EP11O_IF		(1 << 11)
#define EP10I_IF		(1 << 10)
#define EP11N_IF		(1 << 9)
#define EP10N_IF		(1 << 8)
#define PIPOEPAB_IF		(1 << 7)
#define EPADMA_IF		(1 << 6)
#define EP9O_IF			(1 << 5)
#define EP8I_IF			(1 << 4)
#define EP9N_IF			(1 << 3)
#define EP8N_IF			(1 << 2)
#define EP89PIPO_IF		(1 << 1)
#define EP8DMA_IF		(1 << 0)

/*Standard Request Code*/
#define USB_STDREQ_SYNCH_FRAME        0x0C
/*Descriptor Type*/
#define USB_DEVICE_DESCRIPTOR             0x0100
#define USB_CONFIGURATION_DESCRIPTOR      0x0200
#define USB_STRING_DESCRIPTOR             0x0300
#define USB_INTERFACE_DESCRIPTOR          0x0400
#define USB_ENDPOINT_DESCRIPTOR           0x0500
#define USB_QUALIFIER_DESCRIPTOR          0x0600
#define USB_OTHERSPEED_DESCRIPTOR         0x0700

#define USB_MSDC_CONFIGURATION_DESCRIPTOR_SIZE (0x0027-7)
#define USB_MSDC_INTERFACE_DESCRIPTOR_SIZE 0x0009
#define USB_MSDC_ENDPOINT_DESCRIPTOR_SIZE 0x0007

#define USB_MSDC_IN_EP 1
#define USB_MSDC_OUT_EP 2
#define USB_LANGUAGE_ID_STRING_SIZE       0x0004              
#define USB_PRODUCT_STRING_SIZE           0x0030
#define USB_MANUFACTURER_STRING_SIZE      0x0020
#define USB_SERIAL_NUMBER_STRING_SIZE     0x0012

#define SCSI_BLANK							0xA1	//
#define SCSI_CLOSE_TRACK_RZONE_SESSION_BORDER				0x5B	//
#define SCSI_FORMAT_UNIT						0x04	//
#define SCSI_GET_CONFIGURATION						0x46	//
#define SCSI_INQUIRY							0x12	//
#define SCSI_LOAD_UNLOAD_MEDIUM						0xA6
#define SCSI_MODE_SELECT_10						0x55	//
#define SCSI_MODE_SELECT_6						0x15	//
#define SCSI_MODE_SENSE_10						0x5A	//
#define SCSI_MODE_SENSE_6						0x1A	//
#define SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL				0x1E	//
#define SCSI_READ_6							0x08
#define SCSI_READ_10							0x28	//
#define SCSI_READ_12							0xA8
#define SCSI_READ_BUFFER_CAPACITY					0x5C
#define SCSI_READ_CAPACITY_10						0x25
#define SCSI_READ_CAPACITY_16						0x9E
#define SCSI_READ_CD							0xBE	//
#define SCSI_READ_DISC_INFORMATION					0x51
#define SCSI_READ_DVD_STRUCTURE						0xAD
#define SCSI_READ_FORMAT_CAPACITIES					0x23
#define SCSI_READ_TOC_PMA_ATIP						0x43	//
#define SCSI_READ_TRACK_RZONE_INFORMATION				0x52
#define SCSI_REPORT_KEY							0xA4
#define SCSI_REQUEST_SENSE						0x03
#define SCSI_RESERVE_TRACK_RZONE					0x53
#define SCSI_SEND_CUE_SHEET						0x5D
#define SCSI_SEND_DVD_STRUCTURE						0xBF
#define SCSI_SEND_KEY							0xA3
#define SCSI_SEND_OPC_INFORMATION					0x54
#define SCSI_SET_CD_SPEED						0xBB
#define SCSI_START_STOP_UNIT						0x1B
#define SCSI_SYNCHRONIZE_CACHE						0x35	//
#define SCSI_TEST_UNIT_READY						0x00	//
#define SCSI_VERIFY_10							0x2F
#define SCSI_VERIFY_12							0xAF
#define SCSI_VERIFY_16							0x8F
#define SCSI_WRITE_6							0x0A
#define SCSI_WRITE_10							0x2A	
#define SCSI_WRITE_12							0xAA

#define READ_WRITE_ERROR_RECOVERY_PAGE				0x01
#define FLEXIBLE_DISK_PAGE					0x05
#define REMOVABLE_BLOCK_ACCESS_CAPABILITIES_PAGE		0x1B
#define TIMER_AND_PROTECT_PAGE					0x1C
#define ALL_PAGES						0x3F

#define LEN_01										0x0C
#define LEN_1C										0x08
#define LEN_05										0x20
#define LEN_3F										0x34	// 0x0C + 0x08 + 0x20

#define LEN_INQUIRY	0x24

#define MIN(x, y)					(((x) > (y)) ? (y) : (x))
#define MAX(x, y)					(((x) < (y)) ? (y) : (x))
#define GET_UINT32_B(p)				(((u32)((p)[3])) << 0)  |	\
                                            (((u32)((p)[2])) << 8)  |  \
                                            (((u32)((p)[1])) << 16) |  \
                                            (((u32)((p)[0])) << 24)
#define GET_UINT16_B(p)				(((u16)((p)[1])) << 0)  |	\
                                            (((u16)((p)[0])) << 8)
typedef struct _USBEp0Container
{
	u8   bmRequestType;
	u8   bRequest;
	u16  wValue;
	u16  wIndex;
	u16  wLength;
} USBEp0Container,*PUSBEp0Container;

typedef void (*pfStdRequest)(PUSBEp0Container);

int letb123_udc_probe(void);

#endif/*_USB_DEVICE*/
