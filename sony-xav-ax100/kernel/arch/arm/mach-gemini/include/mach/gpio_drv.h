#ifndef _SP_GPIO_H_
#define _SP_GPIO_H_


#ifdef  __KERNEL__
#include <linux/types.h>
#include <linux/ioctl.h>

typedef u32     u032;
typedef u16     u016;
typedef u8      u008;
#else   /* __KERNEL__ */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdint.h>

typedef  uint32_t u032;
typedef  uint16_t u016;
typedef  uint8_t  u008;
#endif  /* __KERNEL__ */


/*  GPIO  IOCTL code  */
#define GPIO_IOCTYPE                         'S'
#define GPIO_IOCNUM(n)                       (0x80 + n)

#define GPIO_IOC_FIRST_1    _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x10), u032)
#define GPIO_IOC_FIRST_0    _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x11), u032)
#define GPIO_IOC_MASTER_1   _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x12), u032)
#define GPIO_IOC_MASTER_0   _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x13), u032)
#define GPIO_IOC_SET_OE     _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x14), u032)
#define GPIO_IOC_CLR_OE     _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x15), u032)
#define GPIO_IOC_OUT_1      _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x16), u032)
#define GPIO_IOC_OUT_0      _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x17), u032)
#define GPIO_IOC_IN         _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x18), u032)

#define GPIO_IOC_PIN_MUX_SEL     _IOWR(GPIO_IOCTYPE, GPIO_IOCNUM(0x20), u032)

#define GPIO_FUNC_ORG           0
#define GPIO_FUNC_GPIO          1
#define GPIO_DIR_OUTPUT         1
#define GPIO_DIR_INPUT          0
#define GPIO_PULL_LOW           0
#define GPIO_PULL_HIGH          1
#define GPIO_PULL_FLOATING      2
#define	GPIO_NO_PULL			3
#define GPIO_IRQ_DISABLE        0
#define GPIO_IRQ_ENABLE         1

typedef enum
{
	PMX_AADC_TEST     = 0x00000100,			// (0-1 0)
	PMX_CVBS_ADC_TEST = 0x00000101,			// (0-1 1)
	PMX_DDR_MPLL_TEST = 0x00000102,			// (0-1 2)
	PMX_THERMAL_TEST  = 0x00000103,			// (0-1 3)
	PMX_OTPRX_TEST    = 0x00000104,			// (0-1 4)
	PMX_DDRIO_TEST    = 0x00000105,			// (0-1 5)
	PMX_LVDSTX_TEST   = 0x00000106,			// (0-1 6)
	PMX_PLLDIS_TEST   = 0x00000107,			// (0-1 7)
	PMX_PLLA_H_TEST   = 0x00000108,			// (0-1 8)
	PMX_PLLSYS_TEST   = 0x00000109,			// (0-1 9)
	PMX_PLLC_SPDIF_TEST = 0x0000010a,		// (0-1 10)
	PMX_SARADC_TEST   = 0x0000010b,			// (0-1 11)
	PMX_ADAC_TEST     = 0x0000010c,			// (0-1 12)
	PMX_USB0_TEST     = 0x0000010d,			// (0-1 13)
	PMX_USB1_TEST     = 0x0000010e,			// (0-1 14)
	PMX_USB2_TEST     = 0x0000010f,			// (0-1 15)
	PMX_DAC_TEST      = 0x00000110,			// (0-1 16)
	PMX_SRV_ONLY1     = 0x00000111,			// (0-1 17)
	PMX_SRV_ONLY      = 0x00000112,			// (0-1 18)
	PMX_MBIST_TEST    = 0x00000113,			// (0-1 19)
	PMX_BIST_MODE     = 0x0000011c,			// (0-1 28)
	
	PMX_SPI_FLASH = 0x00010200,			// (0-2 0)
	PMX_SPI_NAND  = 0x00010302,         // (0-3 2)
	PMX_SPI_NAND_FLASH_HE = 0x00010104, // (0-1 4)
	PMX_SPI_NAND_FLASH_GP = 0x00010105, // (0-1 5)
	PMX_IR_IN     = 0x00010106, 		// (0-1 6)
	PMX_GPS_IN0   = 0x00010207,			// (0-2 7)
	PMX_GPS_IN1   = 0x00010209,			// (0-2 9)
	PMX_GPS_IN2   = 0x0001020b,		    // (0-2 11)
	PMX_UA0_TX    = 0x0001070c,		    // (0-7 12)
	PMX_UA0_RX    = 0x0001070f,		    // (0-7 15)
	PMX_UA5_TX    = 0x00010712,		    // (0-7 18)
	PMX_UA5_RX    = 0x00010715,		    // (0-7 21)
	PMX_UA5_CTS   = 0x00010718,		    // (0-7 24)
	PMX_UA5_RTS   = 0x0001071b,		    // (0-7 27)
	PMX_I2C_CBUS  = 0x0001031e,		    // (0-3 30)
	
	PMX_UA2_TX    = 0x00020700,		    // (0-7 0)
	PMX_UA2_RX    = 0x00020703,		    // (0-7 3)
	PMX_UA3_TX    = 0x00020706,		    // (0-7 6)
	PMX_UA3_RX    = 0x00020709,		    // (0-7 9)
	PMX_UA4_TX    = 0x0002030c,		    // (0-3 12)
	PMX_UA4_RX    = 0x0002030e,		    // (0-3 14)
	PMX_UA4_CTS   = 0x00020310,		    // (0-3 16)
	PMX_UA4_RTS   = 0x00020312,		    // (0-3 18)
	PMX_UA1_TX    = 0x00020714,		    // (0-7 20)
	PMX_UA1_RX    = 0x00020717,		    // (0-7 23)
	PMX_I2CM0     = 0x0002041a,			// (0-4 26)
	PMX_I2CM1     = 0x0002041d,			// (0-4 29)
	
	PMX_I2CDDC0_A    = 0x00030700,		// (0-7 0)
	PMX_SRV_SPDC_OUT = 0x00030103,	    // (0-1 3)
	PMX_SRV_SC_OUT   = 0x00030204,	    // (0-2 4)
	PMX_SRV_SC1_OUT  = 0x00030106,	    // (0-1 6)
	PMX_SRV_TRAY_OUT = 0x00030207,	    // (0-2 7)
	PMX_SRV_DMEA     = 0x00030109,	    // (0-1 9)
	PMX_SRV_FGIN     = 0x0003040a,	    // (0-4 10)
	PMX_SRV_PDM_VREF = 0x0003010d,	    // (0-1 13)
	PMX_SRV_PDM      = 0x0003010e,	    // (0-1 14)
	PMX_SRV_DFCT     = 0x0003010f,	    // (0-1 15)
	PMX_SRV_TTIO_LOW = 0x00030110,	    // (0-1 16)
	PMX_SRV_PLCK     = 0x00030111,	    // (0-1 17)
	PMX_SRV_SLRF     = 0x00030112,	    // (0-1 18)
	PMX_SRV_SLRFNS   = 0x00030113,	    // (0-1 19)
	PMX_SRV_GPIO0    = 0x00030714,	    // (0-7 20)
	PMX_SRV_GPIO1    = 0x00030717,	    // (0-7 23)
	PMX_SRV_GPIO2    = 0x0003071a,	    // (0-7 26)
	PMX_SRV_GPIO3    = 0x0003071d,	    // (0-7 29)
	
	PMX_SRV_GPIO4       = 0x00040700,   // (0-7 0)
	PMX_SRV_MD_SP       = 0x00040303,	// (0-3 3)
	PMX_SRV_MON_DVD     = 0x00040105,	// (0-1 5)
	PMX_SRV_DBG_SPI_DVD = 0x00040306,	// (0-3 6)
	PMX_SRV_DBG_P2S_DVD = 0x00040108,	// (0-1 8)
	PMX_SDIO_CTL_SD     = 0x00040309,	// (0-3 9)
	PMX_SDIO_CTL_HB     = 0x0004030b,	// (0-3 11)
	PMX_CARD0_SD        = 0x0004030d,	// (0-3 13)
	PMX_CARD0_HB        = 0x0004030f,	// (0-3 15)
	PMX_CARD0_eMMC      = 0x00040311,	// (0-3 17)
	PMX_CARD0_MS        = 0x00040313,	// (0-3 19)
	PMX_CARD0_MS_HB     = 0x00040315,	// (0-3 21)
	PMX_CARD1_SD        = 0x00040317,	// (0-3 23)
	PMX_CARD1_SD_HB     = 0x00040319,	// (0-3 25)
	PMX_CARD1_eMMC      = 0x0004031b,	// (0-3 27)	
	
	PMX_CARD1_MS_HB     = 0x00050300,   // (0-3 0)
	PMX_TOUCH_PANEL     = 0x00050202,	// (0-2 2)
	PMX_CCIR656IA_CTL   = 0x00050404,	// (0-4 4)
	PMX_CCIR656IB_CTL   = 0x00050407,	// (0-4 7)
	PMX_CCIR601_CTL     = 0x0005030a,	// (0-3 10)
	PMX_TCON_PWM1       = 0x0005050c,	// (0-5 12)
	PMX_TCON_PWM2       = 0x0005050f,	// (0-5 15)
	PMX_TCON_PWM3       = 0x00050512,	// (0-5 18)
	PMX_TCON_PWM4       = 0x00050515,	// (0-5 21)
	PMX_TCON_PWM5       = 0x00050518,	// (0-5 24)
	PMX_TCON_PWM6       = 0x0005051b,	// (0-5 27)
	
	PMX_TCON_PWM7       = 0x00060500,	// (0-5 0)
	PMX_TCON_PWM8       = 0x00060503,	// (0-5 3)
	PMX_TCON_PCLK       = 0x00060306,	// (0-3 6)		
	PMX_TCON_8_8_8      = 0x00060208,	// (0-2 8)		
	PMX_TCON_6_6_6      = 0x0006010a,	// (0-1 10)
	PMX_TCON_TTL        = 0x0006020b,	// (0-2 11)
	PMX_TCON_STP        = 0x0006020d,	// (0-2 13)
	PMX_SYNC_PC         = 0x0006020f,	// (0-2 15)
	PMX_AUD_BT_IFX      = 0x00060211,	// (0-2 17)
	PMX_AUD_EXT_ADC_IFX0= 0x00060313,	// (0-3 19)
	PMX_AUD_EXT_ADC_IFX1= 0x00060315,	// (0-3 21)
	PMX_AUD_EXT_ADC_IFX2= 0x00060317,	// (0-3 23)
	PMX_AUD_EXT_DAC_IFX0= 0x00060419,	// (0-4 25)
	PMX_AUD_EXT_DAC_IFX1= 0x0006041c,	// (0-4 28)
	PMX_UPHY0_EXT       = 0x0006011f,	// (0-1 31)
	
	PMX_AUD_EXT_DAC_IFX2= 0x00070400,	// (0-4 0)
	PMX_AUD_EXT_DAC_IFX3= 0x00070403,	// (0-4 3)
	PMX_AUD_TEST        = 0x00070106,	// (0-1 6)
	PMX_AUD_IEC_TX      = 0x00070107,	// (0-1 7)
	PMX_AUD_IEC_RX0     = 0x00070108,	// (0-1 8)
	PMX_AUD_IEC_RX1     = 0x00070309,	// (0-3 9)
	PMX_AUD_IEC_RX2     = 0x0007030b,	// (0-3 11)
	PMX_IEC_TX          = 0x0007070d,	// (0-7 13)
	PMX_GPIO_INT0       = 0x00070410,	// (0-4 16)
	PMX_GPIO_INT1       = 0x00070313,	// (0-3 19)
	PMX_GPIO_INT2       = 0x00070315,	// (0-3 21)
	PMX_GPIO_INT3       = 0x00070317,	// (0-3 23)
	PMX_GPIO_INT4       = 0x00070319,	// (0-3 25)
	PMX_GPIO_INT5       = 0x0007031b,	// (0-3 27)
	PMX_GPIO_INT6       = 0x0007031d,	// (0-3 29)
	PMX_SC2_JTAG        = 0x0007031f,	// (0-1 31)
	
	PMX_GPIO_INT7       = 0x00080300,	// (0-3 0)
	PMX_USBC0_OTG_EN    = 0x00080302,	// (0-1 2)
	PMX_USBC1_OTG_EN    = 0x00080303,	// (0-1 3)
	PMX_DFS_CA9_OUT     = 0x00080304,	// (0-1 4)
	PMX_CCIR656OA_CTL   = 0x00080405,	// (0-4 5)
	PMX_CCIR656OB_CTL   = 0x00080508,	// (0-5 8)
	PMX_RGB888I_CTL     = 0x0008010b,	// (0-1 11)
	PMX_SPI2M           = 0x0008010c,	// (0-1 12)
	PMX_SPI2S           = 0x0008010d,	// (0-1 13)
	PMX_SPI2TOUCH       = 0x0008010e,	// (0-1 14)
	PMX_I2CDDC0_PROBE   = 0x0008010f,	// (0-1 15)
	PMX_DISP_PROBE      = 0x00080110,	// (0-1 16)
	PMX_SPI_CB_DBG      = 0x00080111,	// (0-1 17)
	PMX_UPHY0_DEBUG     = 0x00080112,	// (0-1 18)
	PMX_UPHY1_DEBUG     = 0x00080113,	// (0-1 19)
	PMX_UPHY2_DEBUG     = 0x00080114,	// (0-1 20)
	PMX_UPHY0_IF        = 0x00080115,	// (0-1 21)
	PMX_UPHY1_IF        = 0x00080116,	// (0-1 22)
	PMX_UPHY2_IF        = 0x00080117,	// (0-1 23)
	PMX_TVD_PROBE       = 0x00080218,	// (0-2 24)
	PMX_CLKGENA         = 0x0008011a,	// (0-1 26)
	PMX_MO1_CLK27_O     = 0x0008011b,	// (0-1 27)
	PMX_MO1_PLLAFE_O    = 0x0008011c,	// (0-1 28)
	PMX_MO1_PLLA_135_147_O  = 0x0008011d,	// (0-1 29)
	PMX_XCK_OUT         = 0x0008011e,	// (0-1 30)
	PMX_CLK27_OUT       = 0x0008011f,	// (0-1 31)	
	
	PMX_DPLL0_CLK_O     = 0x00090100,	// (0-1 0)
	PMX_DPLL1_CLK_O     = 0x00090101,	// (0-1 1)
	PMX_PLLH_OUT        = 0x00090102,	// (0-1 2)
	PMX_HWCFG_PROB      = 0x00090103,	// (0-1 3)
	PMX_SRV_MON_1DIN    = 0x00090104,	// (0-1 4)
	PMX_SRV_DBG_GPIO_1DIN = 0x00090105,	// (0-1 5)
	PMX_SRV_DBG_SPI_1DIN  = 0x00090306,	// (0-3 6)
	PMX_SRV_MON_2DIN      = 0x00090108,	// (0-1 8)
	PMX_SRV_DBG_SPI_2DIN  = 0x00090309,	// (0-3 9)
	PMX_SRV_DBG_P2S_2DIN  = 0x0009020b,	// (0-2 11)
	PMX_SRV_DBG_P2S_3502  = 0x0009020d,	// (0-2 13)
	PMX_SRV_DBG_P2S_8700  = 0x0009010f,	// (0-1 15)
	PMX_SC2_CS          = 0x00090110,	// (0-1 16)
	PMX_ARM926_JTAG     = 0x00090111,	// (0-1 17)
	PMX_I2CM0_PROBE     = 0x00090212,	// (0-2 18)
	PMX_I2CM1_PROBE     = 0x00090214,	// (0-2 20)
	PMX_V656IN0_DEBUG   = 0x00090116,	// (0-1 22)
	PMX_V656IN1_DEBUG   = 0x00090117,	// (0-1 23)
	PMX_DSP_EJTAG       = 0x00090118,	// (0-1 24)
	PMX_TPI_DEBUG       = 0x00090119,	// (0-1 25)
	PMX_TPISUB_DEBUG    = 0x0009011a,	// (0-1 26)
	PMX_CVBS_ADC_PROB   = 0x0009021b,	// (0-2 27)
	PMX_SAR_ADC_PROBE   = 0x0009021d

}PMXSEL_ID;

typedef struct PMXSEL_S {
	u032 id;
	u032 val;
}PMXSEL_T;
 int gpio_get_value(int handle, unsigned int *value);
 int gpio_set_function(int handle, unsigned int function);
 int gpio_set_direction(int handle, unsigned int direction);
 int gpio_set_input(int handle, int pull_level);
 int gpio_set_driving_current(int handle, unsigned int driving_current);
 int sp_gpio_release(int handle);
 int gpio_set_value(int handle, unsigned int value);
 int gpio_set_output(int handle, unsigned int value, int driving_current);
 long gpio_alloc_tbl_clear(u008 pin_num);
 int sp_gpio_request(unsigned int pin_index, char *name);
extern long gpio_alloc_tbl_set(u008 pin_num, u008 type);
extern void gpio_tbl_list(void);
extern long gpio_first_1(u032 bit);
extern long gpio_first_0(u032 bit);
extern long gpio_master_1(u032 bit);
extern long gpio_master_0(u032 bit);
extern long gpio_set_oe(u032 bit);
extern long gpio_clr_oe(u032 bit);
extern long gpio_out_1(u032 bit);
extern long gpio_out_0(u032 bit);
extern u032 gpio_in_val(u032 bit);

extern long gpio_pin_mux_sel(PMXSEL_ID id, u032 sel);
extern long gpio_pin_mux_get(PMXSEL_ID id, u032 *sel);

extern u032 gpio_in_unlocked(u032 bit);

#define GPIO_E_SET(a,d) do { \
                            if(d) { \
                                gpio_set_oe(a); \
                            } else { \
                                gpio_clr_oe(a); \
                            } \
                        } while(0)

#define GPIO_F_SET(a,d) do { \
                            if(d) { \
                                gpio_first_1(a); \
                            } else { \
                                gpio_first_0(a); \
                            } \
                        } while(0)

#define GPIO_M_SET(a,d) do { \
                            if(d) { \
                                gpio_master_1(a); \
                            } else { \
                                gpio_master_0(a); \
                            } \
                        } while(0)

#define GPIO_O_SET(a,d) do { \
                            if(d) { \
                                gpio_out_1(a); \
                            } else { \
                                gpio_out_0(a); \
                            } \
                        } while(0)

#define GPIO_I_GET(a)   gpio_in_val(a)

#define GPIO_PIN_MUX_SEL(a,d) gpio_pin_mux_sel(a, d)


#endif /* _SP_GPIO_H_ */

