/******************************************************
 IOP

 This driver controls the IOP component.

 Copyright (c) 2006 Sunplus, Inc.
 All rights reserved.
*******************************************************/
//#include <string.h> //Kaney mark
#include <linux/string.h> //Kaney add
#include <iop_car/platform_define.h> //Kaney add
#include <iop_car/customize.h> //Kaney add
//#include "config_sys.h" //Kaney mark
#include <iop_car/plf_gpio_select.h>
//#include "drivers/drv_iop.h" //Kaney mark
#include <iop_car/drv_iop.h> //Kaney add
//#include "drivers/drv_ir.h" //Kaney mark
#include <iop_car/drv_ir.h> //Kaney add
//#include "platform/iop.h" //Kaney mark
#include <iop_car/iop.h> //Kaney add
#include <iop_car/regmap.h> //Kaney add
#include <linux/interrupt.h> //Kaney add
#include <linux/semaphore.h> //Kaney add
#include <linux/delay.h> //Kaney add
#include <linux/module.h> //Kaney add
#include <linux/slab.h> //Kaney add
#include <linux/mm_types.h> //Kaney add
#include <asm/cacheflush.h> //Kaney add
//#include <hal_interface.h> //Kaney mark
#include <iop_car/drv_cec_tx.h> //Kaney add
//#include <mach/gp_chunkmem.h> //Kaney add
#include <mach/sp_reservemem.h> //Kaney add
//#include <mach/gpio_drv.h> //Kaney add

//extern long gpio_alloc_tbl_set(u8 pin_num, u8 type); //Kaney add
//extern int sp_get_total_bank_num(); //Kaney add

//Control Debug Message
#define IOP_RISC_DATA_DBG
//#define IOP_RISC_MAP_PARSER_DBG

#define FIX_POWER_ON_ISSUE
#if defined(IC_SUPPORT_IOP_HW_RESET)||defined(IC_SUPPORT_IOP_PWR_CTRL)||(IC_VER == QCE483)||(PRODUCT_CAT == STB) //2010/12/16 lizhan modify for stb
#undef FIX_POWER_ON_ISSUE
#endif

#define SUPPORT_IOP_CHECKSUM	//2012/6/8 wenjing.yang added for mantis 0180853
#define IOP_CHECKSUM_START		(1 << 9)
#define IOP_CHECKSUM_READY		(1 << 10)


#if defined(SUPPORT_VFD_DRIVER) && defined(CONFIG_SUNPLUS_VFD_STB) //#ifdef SUPPORT_VFD_DRIVER  //chenzhao 2009-8-19 10:50 .
//#include "drivers/drv_vfd_panel.h" //chenzhao 2009-7-28 21:13 . //Kaney mark
#include <iop_stb/drv_vfd_panel.h> //Kaney add
#endif

#define REG_R(addr)			(*((volatile UINT16 *)(addr)))
#define REG_W(addr, value)	(*((volatile UINT16 *)(addr)) = ((UINT16)(value)))

#define	IOPHW_RESET			(1<<0)
#define	IOPHW_STALL			(1<<1)

//static UINT32 gIOPBufAddr=0;//cflu@20110308 mantis127184 set init value=0 for move iop init from cyg_user_start_real to cyg_user_start //Kaney mark
UINT32 gIOPBufAddr=0; //Kaney add
UINT32 gIOPBufAddr_phys = 0; //Kaney add
UINT32 g_u32IopBufAddr = 0;
#define IOP_RESET() 		HWREG_W(iop_control, HWREG_R(iop_control)|0x01)
#define IOP_GO()    		HWREG_W(iop_control, HWREG_R(iop_control)&~0x01)
#define IOP_ENABLE_32BIT_ADDR()	do {HWREG_W(cbus_dec_wdog_set, HWREG_R(cbus_dec_wdog_set) | (1 << 30));\
								HWREG_W(memory_bridge_control, HWREG_R(memory_bridge_control) | (1 << 0));}while(0)


//IOP ISR flag define
#define ISR_PORT			(11)
#define ISR_IR				(1 << 8)
#define ISR_VFD				(1 << 10)

#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER
extern  UINT32 gIopBufAddrVal;        //Iop Code Start Address
#define gpbWakeupTimer     		 ((UINT8 *)(gIopBufAddrVal + 0x0ff8)) //0x0ff5 ~ 0x0ff8
#endif

//GL_Mutex_t gIOPMutexID=NULL; //Kaney mark
struct semaphore semIOP; //Kaney add
static DRV_IopStandbyOnRequest_t stPwrOnTable;

//Debug Message Detail
#define iop_dmap_printf(f, a...) ((void) 0)
#ifdef IOP_RISC_MAP_PARSER_DBG
#undef iop_dmap_printf
#define iop_dmap_printf diag_printf
#endif

//Internal Function Declared
int WaitIopCacheRdy(void);

int _IOPStandby_DataMapParser(UINT8 *pbIopData, UINT32 dIopDataCount, IopDataMap_t *stRetMap)
{
	iop_dmap_printf("[IOP-RISC] <%s>\n", __FUNCTION__);

	UINT8 IOP_DATA_LEAD[] = {0x49,0x4F,0x50,0x5F,0x44,0x41,0x54,0x41}; //"IOP_DATA"
	UINT8 IOP_FUNCs_NAME[IOP_STBY_ON_FUNCs_MAX] = {
		IOP_FUNC_ID_0, IOP_FUNC_ID_1, IOP_FUNC_ID_2, IOP_FUNC_ID_3};

	UINT32 dOffset = 0;

	//Initialize IOP Data Map
	memset(stRetMap, 0x00, sizeof(IopDataMap_t));

	if(dIopDataCount > IOP_STBY_ON_DATA_MAX)
	{
		iop_dmap_printf("[IOP-RISC] > IOP_STBY_ON_DATA_MAX\n");
		stRetMap->fValid = 0;
		return -1;
	}

	//Check magic word
	if(memcmp(pbIopData, IOP_DATA_LEAD, sizeof(IOP_DATA_LEAD)) != 0)
	{
		iop_dmap_printf("[IOP-RISC] IOP_DATA_LEAD error\n");
		stRetMap->fValid = 0;
		return -1;
	}
	else
	{
		dOffset += sizeof(IOP_DATA_LEAD); //offset +8
	}

	//Parsing Map
	UINT8 fTerminated = 0;
	UINT8 bFuncName = 0;
	UINT8 bFuncSize = 0;
	UINT8 bFuncIdx = 0;
	while(!fTerminated)
	{
		//Asign local data;
		bFuncName = pbIopData[dOffset];
		bFuncSize = 2 + pbIopData[dOffset+1];
		iop_dmap_printf("----------------------------------\n");
		iop_dmap_printf("[IOP-RISC] bFuncName = %2x\n",bFuncName);
		iop_dmap_printf("[IOP-RISC] bFuncSize = %2x\n",bFuncSize);

		if(bFuncName == IOP_FUNCs_NAME[bFuncIdx])
		{
			if(bFuncSize > 2) //pbIopData[dOffset+1]>0
			{
				//Asign Returning Map.
				stRetMap->DataLoc[bFuncIdx].fValid = 1;
				stRetMap->DataLoc[bFuncIdx].dSize = pbIopData[dOffset+1];
				stRetMap->DataLoc[bFuncIdx].dOffset = dOffset+2;

				iop_dmap_printf("[IOP-RISC] Asign to Map(%d):\n",bFuncIdx);
/*
				iop_dmap_printf("[IOP-RISC] fValid = %d\n",stRetMap->DataLoc[bFuncIdx].fValid);
				iop_dmap_printf("[IOP-RISC] dSize = %d\n",stRetMap->DataLoc[bFuncIdx].dSize);
				iop_dmap_printf("[IOP-RISC] dOffset = %d\n",stRetMap->DataLoc[bFuncIdx].dOffset);
				iop_dmap_printf("[IOP-RISC] Data[0] = 0x%2x\n",pbIopData[dOffset+2]);
*/
			}
			else
			{
				iop_dmap_printf("[IOP-RISC][Warning] FuncSize 0\n");
			}
		}
		else
		{
			iop_dmap_printf("[IOP-RISC][Warning] bFuncIdx(%2x),bFuncName(%2x) miss match!\n",bFuncIdx, bFuncName);
		}

		bFuncIdx ++;
		dOffset += bFuncSize;

		//Terminating Condition.
		if((dOffset >= IOP_STBY_ON_DATA_MAX) || (bFuncIdx >= IOP_STBY_ON_FUNCs_MAX))
			fTerminated = 1;
	}

	UINT8 i;

	for(i=0; i<IOP_STBY_ON_FUNCs_MAX; i++)
	{
		if(stRetMap->DataLoc[i].fValid)
		{
			stRetMap->fValid = 1;
			break;
		}
	}

	return 0;

}

int WaitIopFunRdy(int iWait)//2010/5/4 lizhan modify.
{
	UINT16 tmp1;
	UINT16 tmp2;

	tmp1 = HWREG_R(iop_data[0]);
	tmp2 = HWREG_R(iop_data[0]);
	iWait = iWait / 50;
	while (iWait) {
		tmp1 = HWREG_R(iop_data[0]);
		if ((tmp1&FUN_MASK) != (tmp2&FUN_MASK)) {
			tmp2 = tmp1;
		}
		if (!(HWREG_R(iop_data[0])&FUN_MASK)) {
			return iWait;
		}
		--iWait;

		//GL_DelayUs(1); //Kaney mark
		udelay(1); //Kaney add
	}

#ifdef IOP_DEBUG_INFO
    //printf("wait iop function ready timeout!\n"); //Kaney mark
    printk("wait iop function ready timeout!\n"); //Kaney add
#endif

	return -1;
}

#ifdef SUPPORT_GPIO_MONITOR //2009/12/09,Haoye added for gpio monitor.
#define IS_IOP_READY()			(!(HWREG_R(iop_data[0])&FUN_MASK))
//#define IOP_MUTEX_INIT()		cyg_mutex_init(&iop_mutex) //Rex_091217: disable for never used

int (*WriteToGpMon)(UINT16 wCmd,UINT8 *pData, UINT8 bLen);
int (*ReadFromGpMon)(UINT16 wCmd,UINT8 *pData, UINT8 bLen);

int ReadFromGpMon_Normal(UINT16 wCmd,UINT8 *pData, UINT8 bLen);
int WriteToGpMon_Normal(UINT16 wCmd,UINT8 *pData, UINT8 bLen);
int WriteToGpMon_Normal(UINT16 wCmd,UINT8 *pData, UINT8 bLen)
{
    int i,iRet=1;
    UINT8 bDataBuff[6];

    if(bLen>6)
        return -1;

    for(i=0;i<6;i++){
        if(i<bLen)
            bDataBuff[i]=pData[i];
        else
            bDataBuff[i]=0;
    }
	//GL_MutexLock(gIOPMutexID);//2010/8/2 lizhan fix 0088608 for avoid Multiple modules simultaneously read and write data on the IOP //Kaney mark
	down_interruptible(&semIOP); //Kaney add
    if (WaitIopFunRdy(IOP_RDY_WAIT) < 0){
        iRet = -1;
        goto END_OF_GpMon_WRITE;
    }

    HWREG_W(iop_data[IOP_CMD], wCmd);
    HWREG_W(iop_data[IOP_DAT0], (bDataBuff[1]<<8)|bDataBuff[0]);
    HWREG_W(iop_data[IOP_DAT1], (bDataBuff[3]<<8)|bDataBuff[2]);
    HWREG_W(iop_data[IOP_DAT2], (bDataBuff[5]<<8)|bDataBuff[4]);
    HWREG_W(iop_data[0], (HWREG_R(iop_data[0]) & IOP_DATLEN_MASK) | (FUN_GPMON_WRITE | bLen));

    if (WaitIopFunRdy(IOP_RDY_WAIT) < 0){
        iRet = -1;
    }
END_OF_GpMon_WRITE:
	//GL_MutexUnlock(gIOPMutexID);//2010/8/2 lizhan fix 0088608 for avoid Multiple modules simultaneously read and write data on the IOP //Kaney mark
    up(&semIOP); //Kaney add
	return iRet;
}

int ReadFromGpMon_Normal(UINT16 wCmd,UINT8 *pData, UINT8 bLen)
{
    int iRet=1,i;
    UINT8 bDataBuff[6];

    if(bLen>6)
        return -1;

    if (WaitIopFunRdy(IOP_RDY_WAIT) < 0){
        iRet = -1;
        goto END_OF_GpMon_READ;
    }

    HWREG_W(iop_data[IOP_CMD], wCmd);
    HWREG_W(iop_data[0], (HWREG_R(iop_data[0]) & IOP_DATLEN_MASK) | (FUN_GPMON_READ | bLen));

    if (WaitIopFunRdy(IOP_RDY_WAIT) < 0){
        iRet = -1;
        goto END_OF_GpMon_READ;
    }

    bDataBuff[0]= (HWREG_R(iop_data[IOP_DAT0]))&0xff;
    bDataBuff[1]= (HWREG_R(iop_data[IOP_DAT0])>>8)&0xff;
    bDataBuff[2]= (HWREG_R(iop_data[IOP_DAT1]))&0xff;//20110421 lizhan fix 0127560 for use gpio number more than 64 ,Passed into the function parameter error.
    bDataBuff[3]= (HWREG_R(iop_data[IOP_DAT1])>>8)&0xff;
    bDataBuff[4]= (HWREG_R(iop_data[IOP_DAT2]))&0xff;
    bDataBuff[5]= (HWREG_R(iop_data[IOP_DAT2])>>8)&0xff;

    for(i=0;i<bLen;i++)
        pData[i]=bDataBuff[i];

END_OF_GpMon_READ:
    return iRet;
}
#endif //SUPPORT_GPIO_MONITOR

int WaitIopCacheRdy(void)
{
	UINT32 dTimeOutCnt = 100;

	printk("[IOP]>>> start wait cache ------- start <<<\n");
	while (((HWREG_R(iop_data[0])&(1<<10))==0) && dTimeOutCnt--)
	{
		printk("[IOP] Wait cache...!\n");
		//GL_DelayUs(1000); //Kaney mark
		mdelay(1); //Kaney add, denzel: replace msleep witg mdelay due to power off have atomic
	}
	printk("[IOP]>>> start wait cache ------- end <<<\n");

	if(dTimeOutCnt == 0)
	{
		printk("[IOP]>>> Error(wait cache timeout)................!!!! <<<\n");
		return -1;
	}

	return 0;
}
#if 0 //Kaney mark isr and move to sphe_iop.c
// IOP ISR&DSR Moved here.
//garylin. 2009.8.27

GL_IsrReturn_t isr_iop(void *pParam)
{
	return (GL_ISR_HANDLED| GL_ISR_CALL_DSR);
	//return CYG_ISR_HANDLED;
}

extern DRV_Status_t DRV_IOPIrIsInited(void);
extern void DRV_IOPIrDSR(void);
#ifdef SUPPORT_GPIO_MONITOR
extern void DRV_GpMon_IopDsr(void);
#endif

GL_DsrReturn_t dsr_iop(void *pParam)
{
	#ifdef SUPPORT_VFD_DRIVER //chenzhao 2009-8-18 16:45 .
	if(HWREG_R(iop_data[ISR_PORT]) & ISR_VFD)
	{
		DRV_VfdDsr();
		HWREG_W(iop_data[ISR_PORT] ,HWREG_R(iop_data[ISR_PORT])&(~ISR_VFD));
	}
	#endif
#ifdef SUPPORT_GPIO_MONITOR
	if((HWREG_R(iop_data[ISR_PORT]))&IOP_INTR_GPMON)
	{
		DRV_GpMon_IopDsr();
		HWREG_W(iop_data[ISR_PORT] ,HWREG_R(iop_data[ISR_PORT])&(~IOP_INTR_GPMON));
	}

#endif
	if((HWREG_R(iop_data[ISR_PORT]))&ISR_IR)
	{
		if(DRV_IOPIrIsInited()==DRV_SUCCESS)
			DRV_IOPIrDSR();
		HWREG_W(iop_data[ISR_PORT] ,HWREG_R(iop_data[ISR_PORT])&(~ISR_IR));
	}

	return GL_DSR_HANDLED;
}
#endif
//2012/6/11 wenjing.yang added for mantis 0180853 	START {{
#ifdef SUPPORT_IOP_CHECKSUM
void _DRV_IOP_Go(void)
{
	//Exit checksum flow if iop support
	HWREG_W(iop_data[0], 0);
}

DRV_Status_t _DRV_IOP_Checksum(UINT32 StartAddr, UINT32 EndAddr)
{
	DRV_Status_t ret = DRV_SUCCESS;
	UINT32 sum = 0;
	UINT8 *dBaseAddr = (unsigned char *)gIOPBufAddr;
	int dTimeOutCnt = 100;
	int i;
	UINT16 IOPDataBackup[12];

	if (NULL == dBaseAddr || StartAddr <= 0 || EndAddr <= 0 || StartAddr >= EndAddr)
		return DRV_ERR_FAILURE;

	//Backup iop_data[1~11]
	memset(IOPDataBackup, 0x00, sizeof(IOPDataBackup));
	for (i = 1; i < 12; ++i)
		IOPDataBackup[i] = HWREG_R(iop_data[i]);

	for(sum = 0, i = StartAddr; i < EndAddr; ++i)
		sum += dBaseAddr[i];

	//Enable iop 32-bit address mode
	IOP_ENABLE_32BIT_ADDR();

	//Inform IOP that RISC needs to checksum,IOP will run checksum flow later.
	HWREG_W(iop_data[0], HWREG_R(iop_data[0]) | IOP_CHECKSUM_START);
	HWREG_W(iop_data[1], StartAddr);
	HWREG_W(iop_data[2], EndAddr);

	//Start IOP
	IOP_GO();					/* release IOP */
	//GL_DelayUs(1000); //Kaney mark
	mdelay(1); //Kaney add, denzel: replace msleep witg mdelay due to power off have atomic
	HWREG_W(iop_regsel, 1);
	if (HWREG_R(iop_regout) == 0x02)
	{
		HWREG_W(iop_regsel, 2);
		if (HWREG_R(iop_regout) == 0x00)
		{
			printk("[IOP] IOP_GO Fail\n");
			return DRV_ERR_FAILURE;
		}
	}

	while (((HWREG_R(iop_data[0]) & IOP_CHECKSUM_READY) == 0) && dTimeOutCnt)
	{
		printk("[IOP] Wait checksum...!(%d) %x\n", dTimeOutCnt,HWREG_R(iop_data[0]));
		--dTimeOutCnt;
		//GL_DelayUs(1000); //Kaney mark
		mdelay(1); //Kaney add, denzel: replace msleep witg mdelay due to power off have atomic
	}
	if(dTimeOutCnt > 0)
	{
		UINT32 iop_sum = (HWREG_R(iop_data[2]) << 16) | HWREG_R(iop_data[1]);
		if(iop_sum != sum)
		{
			printk("[IOP] CheckSum[0x%x : 0x%x]Err Src 0x%x Dst 0x%x\n", StartAddr, EndAddr, sum, iop_sum);
			ret = DRV_ERR_FAILURE;
		}
		else
		{
			printk("[IOP] IOP_Checksum = 0x%x\n", iop_sum);
			//Recover iop_data[1~11]
			for(i = 1; i < 12; ++i)
				HWREG_W(iop_data[i], IOPDataBackup[i]);
			HWREG_W(iop_data[0],(HWREG_R(iop_data[0]) & (~IOP_CHECKSUM_READY)));// IOP_CHECKSUM_READY bit  must be  cleared

		}
	}
	else
	{
		printk("[IOP] Init Standby (wait CheckSum) Error! \n");
		ret = DRV_ERR_FAILURE;
	}

	return ret;
}
#endif

extern irqreturn_t iop_interrupt(int irq, void *dev_id); //Kaney add
extern irqreturn_t iop_interrupt1(int irq, void *dev_id); //Kaney add

//2012/6/11 wenjing.yang added for mantis 0180853 	END }}
/***************************************************
Prototype:
	DRV_Status_t DRV_IOPMemoryInit(void)
Parameters:
	none
Returns:
	DRV_SUCCESS if successful
Description:
	Initialize the IOP memory config.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPMemoryInit(void)
{
	//gIOPBufAddr = MCF_BufAlloc(MEM_ANCHOR_IOP); //Kaney mark
 #if 0 //Kaney add
	struct page * pages = NULL;
	pages = alloc_pages(GFP_KERNEL,1);
    gIOPBufAddr = (UINT32)(__va(PFN_PHYS(page_to_pfn(pages))));
	gIOPBufAddr_phys = PFN_PHYS(page_to_pfn(pages));
 #elif 0
	gIOPBufAddr = gp_chunk_malloc_nocache(0, 0, IOP_CODE_SIZE);
	if(!gIOPBufAddr)
	{
		printk("chunk memory allocation failed\n");
		return DRV_ERR_FAILURE;
	}
	gIOPBufAddr_phys = (UINT32)gp_chunk_pa((void *)gIOPBufAddr);
 #elif 0
    gIOPBufAddr = (unsigned char*)kmalloc(IOP_CODE_SIZE, GFP_KERNEL);
    if(!gIOPBufAddr)
    {
    	printk("IOP kmalloc fail!\n");
		return DRV_ERR_FAILURE;
    }
    gIOPBufAddr_phys = virt_to_phys((unsigned char*)gIOPBufAddr);
 #else
    gIOPBufAddr = sp_get_iop_addr_v();
    if(!gIOPBufAddr)
	{
		printk("IOP get reserved memory address failed\n");
		return DRV_ERR_FAILURE;
	}
	gIOPBufAddr_phys = sp_get_iop_addr_p();
 #endif
	g_u32IopBufAddr = gIOPBufAddr;

	printk(" %s  %s  gIOPBufAddr = %x , gIOPBufAddr_phys = %x\n",__FILE__,__FUNCTION__,gIOPBufAddr, gIOPBufAddr_phys); //Kaney add

	return DRV_SUCCESS;
}

#if 1 //Kaney add for Load IOP firmware move to boot code
DRV_Status_t DRV_IOPInit_without_fw(void)
{
	if(gIOPBufAddr==0)
	{
		if(DRV_IOPMemoryInit() != DRV_SUCCESS)
		{
			printk("[IOP] Init Normal (Creat Memory) Error!\n");
			//return DRV_ERR_FAILURE; //Kaney mark
		}
	}

	GPIO_F_SET(0,1);	//set gpio 0 to be controled by IOP
	GPIO_M_SET(0,0);
	//gpio_alloc_tbl_set(0, 0); //Kaney add

	/*if(gIOPMutexID==NULL) //Kaney mark
	{
		if(GL_SUCCESS!=GL_MutexCreate("IOP Mutex", &gIOPMutexID))
			return DRV_ERR_FAILURE;
	}*/
	sema_init(&semIOP, 1); //Kaney add

	//GL_IntrAttach(iop_hnd); //Kaney mark
	//GL_IntrEnable(iop_hnd); //Kaney mark
	if (request_irq(SP_IRQ_IOP_2_MIPZ_INT0, iop_interrupt, IRQF_DISABLED, "iop", NULL)) //Kaney add
	{
		printk(KERN_INFO"zhaiyong: iop: cant get interrupt num." );
	}

	//Set Repeat Rate // 2009/08/25 lizhan	mantis fix 0076348.
	//HWREG_W(iop_data[5], HWREG_R(iop_data[5]) | (IR_FIRST_KEY_INTERVAL << 8));
	DRV_IOPIrSetRateDefault(); //spike.yang 2010-01-20

#ifdef SUPPORT_GPIO_MONITOR //2009/12/09,haoye added for gpio monitor.
		WriteToGpMon = WriteToGpMon_Normal;
		ReadFromGpMon = ReadFromGpMon_Normal;
#endif

	return DRV_SUCCESS;
}
#endif

//spike.yang 2010-05-03 Add for Variable System Clock, if any problem, plz call me.
DRV_Status_t DRV_IOP_InitByClk(const UINT8 * pbIopcode, UINT16 wSystemClock)
{
	//cflu@20110308 mantis12718 add for move iop init from cyg_user_start_real to cyg_user_start  +++
	if(gIOPBufAddr==0)
	{
		if(DRV_IOPMemoryInit() != DRV_SUCCESS)
		{
			printk("[IOP] Init Normal (Creat Memory) Error!\n");
			return DRV_ERR_FAILURE;
		}
	}
	//cflu@20110308 mantis12718 add for move iop init from cyg_user_start_real to cyg_user_start  ---
	UINT32 dBaseAddr = gIOPBufAddr;
	UINT16 wLen = IOP_CODE_SIZE;
	UINT32 dBaseAddr_phys = gIOPBufAddr_phys; //Kaney add

	GPIO_F_SET(0,1);	//set gpio 0 to be controled by IOP
	GPIO_M_SET(0,0);
	//gpio_alloc_tbl_set(0, 0); //Kaney add
	HWREG_W(iop_control,(HWREG_R(iop_control) &( ~(1 << 15)))); //Kaney add
	#if (IC_VER == QAF561)
	HWREG_W(iop_control,(HWREG_R(iop_control) &( ~(1 << 9))));
	//GL_DelayUs(2000); //Kaney mark
	msleep(2); //Kaney add
	#endif
	IOP_RESET();				/* reset IOP before we change IOP code */
	//2012/6/8 wenjing.yang modified for mantis 0180853  Start {{
	//GL_DelayUs(1000); //Kaney mark
	msleep(1); //Kaney add
	IOP_RESET();
	HWREG_W(reset[0], (HWREG_R(reset[0]) | (1 << 4))); 	// HW reset IOP
	//GL_DelayUs(1000);	//Delay a moment //Kaney mark
	msleep(1); //Kaney add
	HWREG_W(reset[0], (HWREG_R(reset[0]) & (~(1 << 4))));
	//2012/6/8 wenjing.yang modified for mantis 0180853  End }}

	memcpy((unsigned char *)dBaseAddr, pbIopcode, wLen);
	/* flush L1 cache by all */
	flush_cache_all(); //Kaney add
	/* flush L2 cache by all */
	outer_flush_all(); //Kaney add
#if 0 //Kaney mark
	// Translate to IOP view
	if (0xA0000000 == (dBaseAddr & 0xF0000000)) {
		// DRAM A 0x00000000 ~ 0x3FFFFFFF
		dBaseAddr &= 0x0FFFFFFF;
	} else {
		// DRAM B 0x10000000 ~ 0x17FFFFFF
		dBaseAddr &= 0x0FFFFFFF;
		dBaseAddr |= 0x10000000;
	}

	HWREG_W(iop_base_adr_l, dBaseAddr & 0xFFFF);
	HWREG_W(iop_base_adr_h, dBaseAddr >> 16);
#endif
	HWREG_W(iop_base_adr_l, (dBaseAddr_phys & 0xFFFF)); //Kaney add
	HWREG_W(iop_base_adr_h, (dBaseAddr_phys >> 16)); //Kaney add

	//Setup IOP - IR/CEC timer
	HWREG_W(iop_data[2], wSystemClock*100); //CEC
	HWREG_W(iop_data[3], wSystemClock*112); //Nec IR

	//2012/6/8 wenjing.yang modified for mantis 0180853  Start {{
	#ifdef SUPPORT_IOP_CHECKSUM
	if(_DRV_IOP_Checksum(0x0054, 0x1000) != DRV_SUCCESS)
		return DRV_ERR_FAILURE;

	_DRV_IOP_Go();
	#else
	//Enable iop 32-bit address mode
	IOP_ENABLE_32BIT_ADDR();

	IOP_GO();					/* release IOP */
	//delay_1us(10000);         //prevent input IR key in the beginning //Kaney mark
	msleep(10); //Kaney add
	#endif
	//2012/6/8 wenjing.yang modified for mantis 0180853  End }}

	/*if(gIOPMutexID==NULL) //Kaney mark
	{
		if(GL_SUCCESS!=GL_MutexCreate("IOP Mutex", &gIOPMutexID))
			return DRV_ERR_FAILURE;
	}*/
	sema_init(&semIOP, 1); //Kaney add

	return DRV_SUCCESS;
}


//spike.yang 2010-05-03 Add for Variable System Clock, if any problem, plz call me.
DRV_Status_t DRV_IOPInitByClk(const UINT8 * pbIopcode, UINT16 wSystemClock)
{
	//2012/6/8 wenjing.yang added for mantis 0180853 	START {{
	#ifdef SUPPORT_IOP_CHECKSUM
	UINT32 u32Count = 0;
	while ((DRV_SUCCESS != DRV_IOP_InitByClk(pbIopcode, wSystemClock)) && (u32Count++ < 10));
	/*if (u32Count >= 10) //Kaney mark
	{
		HWREG_W(reset[0], 1);
		while (1);
	}*/
	#else
	DRV_IOP_InitByClk(pbIopcode, wSystemClock);
	DRV_IOP_InitByClk(pbIopcode, wSystemClock); //for IOP stable run up.
	#endif
	//2012/6/8 wenjing.yang added for mantis 0180853 	END }}

	//GL_IntrAttach(iop_hnd); //Kaney mark
	//GL_IntrEnable(iop_hnd); //Kaney mark
	if (request_irq(SP_IRQ_IOP_2_MIPZ_INT0, iop_interrupt, IRQF_DISABLED, "iop", NULL)) //Kaney add
	{
		printk(KERN_INFO"zhaiyong: iop: cant get interrupt num." );
	}

	//Set Repeat Rate // 2009/08/25 lizhan  mantis fix 0076348.
	//HWREG_W(iop_data[5], HWREG_R(iop_data[5]) | (IR_FIRST_KEY_INTERVAL << 8));
	DRV_IOPIrSetRateDefault(); //spike.yang 2010-01-20

#ifdef SUPPORT_GPIO_MONITOR //2009/12/09,haoye added for gpio monitor.
	WriteToGpMon = WriteToGpMon_Normal;
	ReadFromGpMon = ReadFromGpMon_Normal;
#endif

	return DRV_SUCCESS;
}

/***************************************************
Prototype:
	DRV_IOPStatus_t DRV_IOPUninit(void)
Parameters:
Returns:
	DRV_SUCCESS on success
Description:
	Stop IOP.
***************************************************/
void iop_enter_idle_state(UINT32 dwCounter)
{
	HWREG_W(iop_data[11], (HWREG_R(iop_data[11]) | (1 << 7)));	// Talk to IOP to enter idle use mailbox 0 bit12
	do
	{
		iop_dmap_printf("\n wait normal IOP enter idle state \n");
		dwCounter--;
		if((HWREG_R(iop_data[11]) & (1 << 6)) || (dwCounter == 0))
		{
			iop_dmap_printf("\n Normal IOP enter idle state or time out \n");
			break;
		}
		//GL_DelayUs(3000); //Kaney mark
		msleep(3); //Kaney add
	} while (1);
	if(dwCounter == 0)
		iop_dmap_printf(" Eorror : time out\n");
	else
	    iop_dmap_printf(" Normal IOP already enter idle state\n");
}
DRV_Status_t DRV_IOPUninit(void)
{
	//GL_IntrDisable(iop_hnd); //Kaney mark
	//GL_IntrDetach(iop_hnd); //Kaney mark
	free_irq(SP_IRQ_IOP_2_MIPZ_INT0, NULL); //Kaney add
	#if (IC_VER == QAF561)
	HWREG_W(iop_control,(HWREG_R(iop_control) | (1 << 9)));
	//GL_DelayUs(2000); //Kaney mark
	msleep(2); //Kaney add
	#endif
	iop_enter_idle_state(10000);
	//GL_DelayUs(5000); //Kaney mark
	msleep(5); //Kaney add
	IOP_RESET();				/* reset IOP before we change IOP code */
	//REG_W(IOP_CONTROL, 1);		/* reset IOP */
	//2012/6/8 wenjing.yang modified for mantis 0180853  Start {{
	//GL_DelayUs(1000); //Kaney mark
	msleep(1); //Kaney add
	IOP_RESET();
	HWREG_W(reset[0], (HWREG_R(reset[0]) | (1 << 4))); 	// HW reset IOP
	//GL_DelayUs(1000);	//Delay a moment //Kaney mark
	msleep(1); //Kaney add
	HWREG_W(reset[0], (HWREG_R(reset[0]) & (~(1 << 4))));
	//2012/6/8 wenjing.yang modified for mantis 0180853  End }}

	return DRV_SUCCESS;
}

/***************************************************
Prototype:
	DRV_IOPSetSysClk(DRV_IOPSysClk_t clkFreq)
Parameters:
	clkFreq: IOP system clock frequency
Returns:
	DRV_SUCCESS: if successful
	DRV_ERR_INVALID_PARAM: if any of the parameters is invalid
Description:
	Set the IOP system clock.
Restrictions:
***************************************************/
DRV_Status_t DRV_IOPSetSysClk(DRV_IOPSysClk_t clkFreq)
{
	return DRV_SUCCESS;
}



#ifdef FIX_POWER_ON_ISSUE

void StandbyInRiscOn(void)
{
	UINT32 BUILD_ATTR_UNUSED dummy = GL_GlobalIntrDisable();

	//#define PWR_STEP_EN_0	1
	#define PWR_STEP_EN_1	1
	#define PWR_STEP_EN_2	1
	#define PWR_STEP_EN_3	1
	#define PWR_STEP_EN_4	1
	#define PWR_STEP_EN_5	1
	#define PWR_STEP_EN_6	1
	#define PWR_STEP_EN_7	1

	//PWR_STEP_EN_2 sub-routine.
	#define PWR_HDMI_EN		1
	#define PWR_VDAC_EN		1
	#define PWR_ADAC_EN		1
	#define PWR_UPHY_EN		1
	#define PWR_PLLTV_EN	1
	#define PWR_PLLH_EN		1

	//Power Test ====================================
	printk("================== %s ================== start\n", __FUNCTION__);

	UINT32 *base, group, index;
	UINT8 bStep = 0;

#if (PWR_STEP_EN_2 == 1)
	bStep = 2;
	printk("================== %s ================== step %d\n", __FUNCTION__,bStep);

	#if (PWR_HDMI_EN == 1)
	//HDMI...
	UINT8 bTemp;
	extern UINT8 HDRV_ReadReg(UINT8 bDev, UINT8 bAddr);
	extern void HDRV_WriteReg(UINT8 bDev, UINT8 bAddr, UINT8 bValue);
	bTemp = HDRV_ReadReg(0x72,0x08);
	bTemp &= ~(1<<0);
	HDRV_WriteReg(0x72,0x08,bTemp);

	bTemp = HDRV_ReadReg(0x7a,0x3d);
	bTemp &= ~((1<<0)|(1<<1)|(1<<2));
	HDRV_WriteReg(0x7a,0x3d,bTemp);

	printk("(0x72,0x08) = %2x, (0x7a,0x3d) = %2x\n", HDRV_ReadReg(0x72,0x08), HDRV_ReadReg(0x7a,0x3d));
	#endif

	#if (PWR_VDAC_EN == 1)
	//VDAC...
	group = 89;
	index = 19;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base &= ~(1<<0);
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);

	group = 87;
	index = 5;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base |= (1<<15);
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);
	#endif

	#if (PWR_UPHY_EN == 1)
	//UPHY0/UPHY1
	group = 144;
	index = 0;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base |= (1<<30);
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);
	#endif

	#if (PWR_PLLTV_EN == 1)
	//PLLTV...
	group = 1;
	index = 23;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base |= (1<<2);
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);
	#endif

	#if (PWR_PLLH_EN == 1)
	//PLLH...
	group = 1;
	index = 24;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base &= ~(1<<14);
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);
	#endif
#endif

#if (PWR_STEP_EN_3 == 1)
	bStep = 3;
	printk("================== %s ================== step %d\n", __FUNCTION__,bStep);

	//Set GPIO 24, Analog HW Reset.
	const UINT8 gpio_pin = 24;
	UINT8 x = gpio_pin;
	HWREG_W(gpio_first[x/32], HWREG_R(gpio_first[x/32]) | (1<<(x%32)));
	HWREG_W(gpio_master[x/32], HWREG_R(gpio_master[x/32]) | (1<<(x%32)));
	HWREG_W(gpio_oe[x/32], HWREG_R(gpio_oe[x/32]) | (1<<(x%32)));

	//toggle 0->1
	HWREG_W(gpio_out[x/32], HWREG_R(gpio_out[x/32]) & ~(1<<(x%32)));
	GL_DelayUs(10000); //just test, maybe it can be removed.
	HWREG_W(gpio_out[x/32], HWREG_R(gpio_out[x/32]) | (1<<(x%32)));

#endif

#if (PWR_STEP_EN_4 == 1)
	bStep = 4;
	printk("================== %s ================== step %d\n", __FUNCTION__,bStep);

	group = 1;
	index = 1;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base &= ~(0x1f << 3);
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);


#endif

#if (PWR_STEP_EN_5 == 1)
	bStep = 5;
	printk("================== %s ================== step %d\n", __FUNCTION__,bStep);

	group = 0;
	index = 16;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
//	*base = 0xffffffec & ~(1<<17); //keep uart
//	*base = 0xffffffec;
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);

	group = 0;
	index = 17;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base = 0xffffffff;
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);

	group = 0;
	index = 18;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base = 0xffffffff;
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);

	group = 0;
	index = 19;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base = 0xffffffff;
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);
#endif

#if (PWR_STEP_EN_6 == 1)
	bStep = 6;
	printk("================== %s ================== step %d\n", __FUNCTION__,bStep);

	//Wait IOP Loading Cache
	WaitIopCacheRdy();

	//wait IOP ready
	delay_1us(100000);

	//Inform IOP that RISC Ready, IOP will turn off the RISC Later.
	HWREG_W(iop_data[0], HWREG_R(iop_data[0]) |(1<<11));


#endif

#if (PWR_STEP_EN_7 == 1)
	bStep = 7;
	printk("================== %s ================== step %d\n", __FUNCTION__,bStep);

	group = 0;
	index = 11;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base = 0x00000000;
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);

	group = 0;
	index = 10;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base = 0x00000000;
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);

	group = 0;
	index = 9;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base = 0x00000000;
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);

#if (PWR_STEP_EN_1 == 1)
	bStep = 1;
	printk("================== %s ================== step %d\n", __FUNCTION__,bStep);

	group = 0;
	index = 6;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base |= ((1<<18)|(1<<31));
	//*base |= ((1<<31));

	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);
#endif


	group = 0;
	index = 8;
	base = (UINT32 *)(&HWREG_R(stamp) + group * 32 + index);
	*base = 0x00000010;
	printk("[%d] g%da%d=%8x\n",bStep, group, index,*base);


	printk("================== %s ================== end\n", __FUNCTION__);
#endif

}
#endif
#if(PRODUCT_CAT == STB)
void DRV_IOPSetICVersion(void){
	//2012/8/15 wenjing.yang modified for mantis 0190578 	START {{{
	//IC_VERSION Format:
	//bit 4 ~ 7: IC Version
	//bit 0 ~ 3: IC Sub Version
	//IC Version: 1500: 0    1502: 2    1503: 3
	//Sub Version: 1500_A: 0    1500_C: 1

	#if (IC_VER == QAE377)	//for 1500 A&C
	*IC_VERSION_ADDR = (0 << 4) | (HWREG_R(stamp) & 0xf);
	#elif (IC_VER == QCE491)	//for 1502
	*IC_VERSION_ADDR = (2 << 4) | 0x00;
	#elif (IC_VER == QAF536)	//for 1503
	*IC_VERSION_ADDR = (3 << 4) | 0x00;
	#elif (IC_VER == QAF561)	//for 1506
	*IC_VERSION_ADDR = (4 << 4) | 0x00;
	#endif
	//2012/8/15 wenjing.yang modified for mantis 0190578 	END }}}
}
#endif

#if 0
//Kaney add for inform IOP that DRAM1 is used or not
static void inform_IOP_dram1_is_used(void)
{
    unsigned int dram_num;

    dram_num = sp_get_total_bank_num();
	if (dram_num == 1) //DRAM0 is used, DRAM1 is not used
    {
		HWREG_W(iop_data[11], (HWREG_R(iop_data[11]) & ~(1 << 2)));
		printk("inform_IOP_dram1_is_not_used\n");
    }
	else if (dram_num == 2) //DRAM0 and DRAM1 are used
	{
		HWREG_W(iop_data[11], (HWREG_R(iop_data[11]) | (1 << 2)));
		printk("inform_IOP_dram1_is_used\n");
	}
}
#endif

//spike.yang 2010-01-11 Add for Standby Mode, if any problem, plz call me.
DRV_Status_t DRV_IOPStandby_Initialize(DRV_IopStandbyCfg_t *pstConfig)
{
	printk("[IOP]>>>>>>>>>>>>>>> %s <<<<<<<<<<<<<<<<<<<<<\n",__FUNCTION__);

	UINT32 i;
	UINT32 dBaseAddr = gIOPBufAddr;
	UINT32 dBaseAddr_phys = gIOPBufAddr_phys; //Kaney add
#ifdef IC_SUPPORT_IOP_HW_RESET
	UINT16 wLen = pstConfig->dIopCodeSize;//for mantis 110569,chenzhao  //zhangjihua add for mantis 149050
	printk("IOP SIZE %d\n",wLen);
	if(wLen>IOP_CODE_SIZE)
		wLen = IOP_CODE_SIZE;
#else
	UINT16 wLen = IOP_CODE_SIZE;
#endif

	IOP_RESET();				/* reset IOP before we change IOP code */
	//2012/6/8 wenjing.yang modified for mantis 0180853  Start {{
	//GL_DelayUs(1000); //Kaney mark
	mdelay(1); //Kaney add, denzel: replace msleep witg mdelay due to power off have atomic
	IOP_RESET();
	HWREG_W(reset[0], (HWREG_R(reset[0]) | (1 << 4))); 	// HW reset IOP
	//GL_DelayUs(1000);	//Delay a moment //Kaney mark
	mdelay(1); //Kaney add, denzel: replace msleep witg mdelay due to power off have atomic
	HWREG_W(reset[0], (HWREG_R(reset[0]) & (~(1 << 4))));
	//2012/6/8 wenjing.yang modified for mantis 0180853  End }}

	GPIO_F_SET(0,1);	//set gpio 0 to be controled by IOP
	GPIO_M_SET(0,0);
    //gpio_alloc_tbl_set(0, 0); //Kaney add
	//Download Standby IOP Code. (by memory copy)
	memset((unsigned char *)dBaseAddr,0, IOP_CODE_SIZE);//for mantis 110569,chenzhao add padding. //zhangjihua add for mantis 149050
	memcpy((unsigned char *)dBaseAddr, pstConfig->pbIopCode, wLen);
	/* flush L1 cache by all */
	flush_cache_all(); //Kaney add
	/* flush L2 cache by all */
	outer_flush_all(); //Kaney add
  #if 0 //Kaney mark
	// Translate to IOP view
	if (0xA0000000 == (dBaseAddr & 0xF0000000)) {
		// DRAM A 0x00000000 ~ 0x3FFFFFFF
		dBaseAddr &= 0x0FFFFFFF;
	} else {
		// DRAM B 0x10000000 ~ 0x17FFFFFF
		dBaseAddr &= 0x0FFFFFFF;
		dBaseAddr |= 0x10000000;
	}

	HWREG_W(iop_base_adr_l, dBaseAddr & 0xFFFF);
	HWREG_W(iop_base_adr_h, dBaseAddr >> 16);
  #endif
    HWREG_W(iop_base_adr_l, (dBaseAddr_phys & 0xFFFF)); //Kaney add
	HWREG_W(iop_base_adr_h, (dBaseAddr_phys >> 16)); //Kaney add

	//Clear IOP Data
	HWREG_W(iop_data[0], 0);

	//inform_IOP_dram1_is_used(); //Kaney add for inform IOP that DRAM1 is used or not

	//Setup for Standby IR/VFD/CEC etc, almost by Mail-Box.
	if(pstConfig->pfSetupIrIop != NULL)
	{
		//Setup IOP-IR Interface
		pstConfig->pfSetupIrIop();
	}
	if(pstConfig->pfSetupVfdIop != NULL)
	{
		//Setup IOP-VFD Interface
		pstConfig->pfSetupVfdIop();
	}
	if(pstConfig->pfSetupCecIop != NULL)
	{
		//Setup IOP-CEC Interface
		pstConfig->pfSetupCecIop();
	}

	#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER //lizhan add 2010-3-23
	if(pstConfig->pfWriteDateTimeIop != NULL)
	{
		//Setup IOP-WAKEUPTIMER Interface
		pstConfig->pfWriteDateTimeIop();
	}
	#endif
	#if(PRODUCT_CAT == STB)
	DRV_IOPSetICVersion();
	#endif

	//2011/8/3 wenjing.yang modified for mantis 0145407 	START {{{
	#ifdef IOP_GPIO_CONTROL_TEST
	DRV_IOPStandby_TestGpio();
	#endif
	//2011/8/3 wenjing.yang modified for mantis 0145407 	END }}}

#if (IC_VER == QCE483) //Maybe update later for QCE483+
	// Should let IOP control RISC's Register.
	HWREG_W(cbus_dec_wdog_set, HWREG_R(cbus_dec_wdog_set) |(1<<30));
	HWREG_W(memory_bridge_control, HWREG_R(memory_bridge_control)|(1<<0));
	//printk(" CBUS:%08x \n MEMBr:%08x \n", HWREG_R(cbus_dec_wdog_set), HWREG_R(memory_bridge_control));
	printk("[IOP] Release RISC Register to IOP...\n");
#endif

#ifdef SUPPORT_IOP_CHECKSUM
	if(_DRV_IOP_Checksum(0x0054, 0x1000) != DRV_SUCCESS)
		return DRV_ERR_FAILURE;

	_DRV_IOP_Go();
#else
	//Enable iop 32-bit address mode
	IOP_ENABLE_32BIT_ADDR();

	//Start IOP
	IOP_GO();					/* release IOP */
#endif

	//RISC-IOP cowork Start
	//-----------------------------------------------------------------
	printk("[IOP]============ RISC-IOP Standby Cowork ============\n");
	//check iop_data
	for(i=0; i<=11; i++)
	{
		printk("[IOP]iop_data[%d] = %4x\n", i, HWREG_R(iop_data[i]));
	}
	//-----------------------------------------------------------------
	#ifdef SUPPORT_USB_DEBUG
	extern  int dvb_close_debugmessage_file(void);
	dvb_close_debugmessage_file();
	#endif

	//
#ifdef FIX_POWER_ON_ISSUE

	StandbyInRiscOn();

#else

	//Wait IOP Loading Cache
	WaitIopCacheRdy();

	//Maybe it can be removed, if system is ok.
	//delay_1us(100000); //Kaney mark
	mdelay(100); //Kaney add, denzel: replace msleep witg mdelay due to power off have atomic

	//move the C line to function:DRV_IOPStandby_RISC_PowerOff
	//Inform IOP that RISC Ready, IOP will turn off the RISC Later.
	//HWREG_W(iop_data[0], HWREG_R(iop_data[0]) |(1<<11));

#if (IC_VER == QAE377)				// tempory fixed for 491
	//System to Bypass Mode, change
	HWREG_W(sspll_cfg, HWREG_R(sspll_cfg) |(0x1<<31));
#endif	// ~IC_VER

#endif

	return DRV_SUCCESS;
}

//2012/8/15 wenjing.yang modified for mantis 0190578	START {{{
void DRV_IOPStandby_RISC_PowerOff(void)
{
	//Inform IOP that RISC Ready, IOP will turn off the RISC Later.
	HWREG_W(iop_data[0], HWREG_R(iop_data[0]) |(1<<11));
}
//2012/8/15 wenjing.yang modified for mantis 0190578	END }}}

#if 0 //Kaney mark
/***************************************************
Prototype:
	DRV_IOPStatus_t DRV_IOPStandby_POTGet(DRV_IopStandbyOnRequest_t *pstRet)
Parameters:
	pstRet: Get Power On Table(from standby)
Returns:
	DRV_SUCCESS if successful.
	DRV_ERR_INVALID_OP if DRV_IOPStandby_POTSave is un-done.
Description:
	Get POT to check which command is request from standby to on.
Restrictions:
	DRV_IOPStandby_POTSave should be done after booting.
***************************************************/
DRV_Status_t DRV_IOPStandby_POTGet(DRV_IopStandbyOnRequest_t *pstRet)
{
	if(!stPwrOnTable.fSaved)
	{
		return DRV_ERR_INVALID_OP;
	}

	*pstRet = stPwrOnTable;

	return DRV_SUCCESS;
}
#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER //added for wake up from IOP
INT32 DRV_IopStby_WkTimeInit(void)
{
	//stPwrOnTable.stWkTimingInfo.pbIopWkTimerCount = (UINT8*)GL_MemAlloc( sizeof(UINT8)*IOP_MANUALWKTIME_COUNT) ; //Kaney mark
	stPwrOnTable.stWkTimingInfo.pbIopWkTimerCount = kzalloc(IOP_MANUALWKTIME_COUNT, GFP_KERNEL); //Kaney add
	if (!stPwrOnTable.stWkTimingInfo.pbIopWkTimerCount)
	{
		iop_dmap_printf("%s pbIopWkTimerCount is NULL.\n", __FUNCTION__);
		return DRV_ERR_FAILURE;
	}
	//stPwrOnTable.stWkTimingInfo.pbAutoWkTimerCount = (UINT8*)GL_MemAlloc( sizeof(UINT8)*IOP_AUTOWKTIME_COUNT); //Kaney mark
	stPwrOnTable.stWkTimingInfo.pbAutoWkTimerCount = kzalloc(IOP_AUTOWKTIME_COUNT, GFP_KERNEL); //Kaney add
	if (!stPwrOnTable.stWkTimingInfo.pbAutoWkTimerCount)
	{
		iop_dmap_printf("%s pbAutoWkTimerCount is NULL.\n", __FUNCTION__);
		return DRV_ERR_FAILURE;
	}
	//stPwrOnTable.stWkTimingInfo.pbRiscIntoStbyTimingInfo =(UINT8*)GL_MemAlloc( sizeof(UINT8)*STANDBY_TIMEINFO); //Kaney mark
	stPwrOnTable.stWkTimingInfo.pbRiscIntoStbyTimingInfo = kzalloc(STANDBY_TIMEINFO, GFP_KERNEL); //Kney add
	if (!stPwrOnTable.stWkTimingInfo.pbRiscIntoStbyTimingInfo)
	{
		iop_dmap_printf("%s pbRiscIntoStbyTimingInfo is NULL.\n", __FUNCTION__);
		return DRV_ERR_FAILURE;
	}

	return DRV_SUCCESS;
}
INT32 DRV_IopStby_WkTimeUinit(void)
{
	if (stPwrOnTable.stWkTimingInfo.pbIopWkTimerCount)
	{
		//GL_MemFree(stPwrOnTable.stWkTimingInfo.pbIopWkTimerCount); //Kaney mark
		kfree(stPwrOnTable.stWkTimingInfo.pbIopWkTimerCount); //Kaney add
		stPwrOnTable.stWkTimingInfo.pbIopWkTimerCount = NULL;
	}
	if (stPwrOnTable.stWkTimingInfo.pbAutoWkTimerCount)
	{
		//GL_MemFree(stPwrOnTable.stWkTimingInfo.pbAutoWkTimerCount);  //Kaney mark
		kfree(stPwrOnTable.stWkTimingInfo.pbAutoWkTimerCount); //Kaney add
		stPwrOnTable.stWkTimingInfo.pbAutoWkTimerCount = NULL;
	}
	if (stPwrOnTable.stWkTimingInfo.pbRiscIntoStbyTimingInfo)
	{
		//GL_MemFree(stPwrOnTable.stWkTimingInfo.pbRiscIntoStbyTimingInfo); //Kaney mark
		kfree(stPwrOnTable.stWkTimingInfo.pbRiscIntoStbyTimingInfo); //Kaney add
		stPwrOnTable.stWkTimingInfo.pbRiscIntoStbyTimingInfo = NULL;
	}
	return DRV_SUCCESS;
}
#endif
/***************************************************
Prototype:
	DRV_IOPStatus_t DRV_IOPStandby_POTSave(void)
Parameters:
Returns:
	DRV_SUCCESS if successful.
Description:
	Save POT(Power On Table) to check which command is
	request from standby to on.
Restrictions:
	DRV_IOPStandby_POTSave should be done after booting,
	and before iop data cleared.
***************************************************/
//2011/5/10 wenjing.yang modified for mantis 0130994 	START {{{
UINT8 IopData[IOP_STBY_ON_DATA_MAX] = {0};
IopDataMap_t stIopMap = {0};

void DRV_IOP_Get_Standby_POT(UINT8 **ppIopData, IopDataMap_t **ppstIopMap)
{
	*ppIopData = IopData;
	*ppstIopMap = &stIopMap;
}
//2011/5/10 wenjing.yang modified for mantis 0130994 	END }}}
#if 1 //Kaney add
#define MAX_BUF_SIZE 512
static unsigned char iop_data[MAX_BUF_SIZE];

void cyg_get_iop_data(void)
{
	unsigned char *src;

	//printk("enter %s\n", __FUNCTION__);

	// get source address
	src = (unsigned char *)SHARE_RAM_BASE;

	// copy iop_data from SRAM to local buffer
	memset(iop_data, 0x00, MAX_BUF_SIZE);
	memcpy(iop_data, src, MAX_BUF_SIZE);
}

int PLF_GetIOPData(unsigned char *buf, size_t size)
{
	int retVal;

	retVal = 0;

	if ((NULL != buf) && (MAX_BUF_SIZE >= size)) {
		// reset buffer
		memset(buf, 0x00, size);

		// copy iop_data from source address
		memcpy(buf, iop_data, size);

		retVal = 0;
	} else {
		printk("[Get IOP Data] Fail: Buffer addr. is %x, size is %d\n",
			(unsigned int)buf, size);

		retVal = -1;
	}

	return retVal;
}
#endif
DRV_Status_t DRV_IOPStandby_POTSave(void)
{
	INT32 dRet=0;

	//Get IopData from First Boot
	//extern int PLF_GetIOPData(unsigned char *buf, size_t size); //Kaney mark
	dRet = PLF_GetIOPData((unsigned char *)&IopData[0], IOP_STBY_ON_DATA_MAX);
	if(dRet != 0)
	{
		return DRV_ERR_INVALID_PARAM;
	}

	//debug IOP-RISC Data
#ifdef IOP_RISC_DATA_DBG
	printk("[IOP-RISC] IOP_RISC Data:");
	UINT8 i;
	for(i=0; i<64; i++)
	{
		if((i%16) == 0)
			printk("\n");
		printk("[%2x]",IopData[i]);
	}
	printk("\n\n");
#endif

	//Get func's offset and size etc.
	dRet = _IOPStandby_DataMapParser(&IopData[0], IOP_STBY_ON_DATA_MAX, &stIopMap);
#if (PRODUCT_CAT != STB)	//2011/5/10 wenjing.yang modified for mantis 0130994
#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER // added for wake up from IOP
	UINT8 *IopDataOffset;
	if((stIopMap.DataLoc[0].fValid))
	{
		printk("Iop Func0 offset: %d\n", stIopMap.DataLoc[0].dOffset);
		printk("Iop Func0 size: %d\n", stIopMap.DataLoc[0].dSize);
		if(DRV_IopStby_WkTimeInit() == DRV_SUCCESS)
		{
			IopDataOffset = &IopData[stIopMap.DataLoc[0].dOffset];
			memcpy(stPwrOnTable.stWkTimingInfo.pbIopWkTimerCount, &IopDataOffset[0], IOP_MANUALWKTIME_COUNT);
			memcpy(stPwrOnTable.stWkTimingInfo.pbAutoWkTimerCount, &IopDataOffset[IOP_MANUALWKTIME_COUNT], IOP_AUTOWKTIME_COUNT);
			memcpy(stPwrOnTable.stWkTimingInfo.pbRiscIntoStbyTimingInfo, &IopDataOffset[IOP_MANUALWKTIME_COUNT + IOP_AUTOWKTIME_COUNT], STANDBY_TIMEINFO);
		}
	}
#endif
	//Asign parameter for module (func 1)
	//IR
	if((stIopMap.DataLoc[1].fValid))
	{
		//printk("Iop Func1 offset: %d\n", stIopMap.DataLoc[1].dOffset);

		UINT8 PwrCmdFlag = IopData[stIopMap.DataLoc[1].dOffset];

		if(PwrCmdFlag & IOP_STBY_ON_CMD_EJECT_BIT)
		{
			//printk("[IOP-RISC][IR] Get EJECT_BIT !\n");
			stPwrOnTable.eCmd = IOP_STBY_ON_CMD_Eject;
		}
			else if(PwrCmdFlag & IOP_STBY_ON_CMD_PLAY_BIT)
		{
			//printk("[IOP-RISC][IR] Get PLAY_BIT !\n");
			stPwrOnTable.eCmd = IOP_STBY_ON_CMD_Play;
		}
		else
		{
			//printk("[IOP-RISC][IR] Get POWER_BIT !\n");
			stPwrOnTable.eCmd = IOP_STBY_ON_CMD_Power;
		}
	}
	//

	//Others
	//...

	stPwrOnTable.fSaved = 1;
#endif

	return DRV_SUCCESS;
}
#endif

//spike.yang 2009-12-22 Add for CEC, if any problem, plz call me.
DRV_Status_t DRV_IOP_GetIopBufAddrValue(UINT32 *pIopBufVal)
{
	printk("[IOP] DRV_IOP_GetIopBufAddrValue(gIOPBufAddr = %8x)\n",gIOPBufAddr);
	*pIopBufVal = gIOPBufAddr;
	return DRV_SUCCESS;
}

//huazhi.cui 2009-12-31 add for gamepad,if any problem, please call me.
int ReadGamePad(void)
{
	if (HWREG_R(iop_data[0])&FUN_MASK)
		return -1;

	//read game pad
	HWREG_W(iop_data[0], (HWREG_R(iop_data[0])&VFD_DATLEN_MASK)|FUN_GPAD_READ);

	//wait read end
	while(HWREG_R(iop_data[0])&FUN_MASK);

	if ((HWREG_R(iop_data[4]) != 0xffff)&&(HWREG_R(iop_data[4]) != 0)
		&&(HWREG_R(iop_data[4]) != 0x00ff)&&(HWREG_R(iop_data[4]) != 0xff00)){
		//printk("game pad= %04x\n",HWREG_R(iop_data[4]));
	}

	return 1;
}

//2010/5/4 lizhan added for mantis bug 0085542 start.........{{{
#ifdef RISC_CFG_IOP_USE_GPAD_GPIO
int NESGamePad_UsePinIopInit(UINT8 bClkGpioNum,UINT8 bLatchGpioNum,UINT8 bD0GpioNum,UINT8 bD1GpioNum)
{
	int iRet=1;
	//GL_MutexLock(gIOPMutexID); //Kaney mark
	down_interruptible(&semIOP); //Kaney add
	if (WaitIopFunRdy(IOP_RDY_WAIT) < 0){
	     printk("@NESGamePad_UsePinInit :ERROR-1 \n");
	    iRet = -1;
	    goto END_OF_GAMEPAD_PIN_INIT;
	}
	HWREG_W(iop_data[2], ((bClkGpioNum/8)<<8)|(1<<(bClkGpioNum%8)));
	HWREG_W(iop_data[3],((bLatchGpioNum/8)<<8)|(1<<(bLatchGpioNum%8)));
	HWREG_W(iop_data[4],((bD0GpioNum/8)<<8)|(1<<(bD0GpioNum%8)));
	HWREG_W(iop_data[5],((bD1GpioNum/8)<<8)|(1<<(bD1GpioNum%8)));
	HWREG_W(iop_data[6],((bD1GpioNum%8)<<8)|(bD0GpioNum%8));

	HWREG_W(iop_data[0],(HWREG_R(iop_data[0])|FUN_GPAD_CFG_USE_PIN));

	printk("@@@@bClkGpioNum = %d ; bLatchGpioNum = %d; bD0GpioNum = %d; bD1GpioNum = %d@@\n",\
		bClkGpioNum,bLatchGpioNum,bD0GpioNum,bD1GpioNum);

	if (WaitIopFunRdy(IOP_RDY_WAIT) < 0){
	    printk("@NESGamePad_UsePinInit :ERROR-2 \n");
	    iRet = -1;
	}

END_OF_GAMEPAD_PIN_INIT:
    //GL_MutexUnlock(gIOPMutexID); //Kaney mark
    up(&semIOP); //Kaney add
    return iRet;

}
#endif
//2010/5/4 lizhan added for mantis bug 0085542 ended.......}}}


 //2010/11/5  lijin added for MD Game.........{{{
#ifdef RISC_CFG_IOP_USE_GPAD_GPIO
int MDGamePad_UsePinIopInit(UINT8 bClkGpioNum,UINT8 bLatchGpioNum,UINT8 bD0GpioNum,UINT8 bD1GpioNum)
{
	int iRet=1;
	//GL_MutexLock(gIOPMutexID); //Kaney mark
	down_interruptible(&semIOP); //Kaney add
	if (WaitIopFunRdy(IOP_RDY_WAIT) < 0){
	     printk("@NESGamePad_UsePinInit :ERROR-1 \n");
	    iRet = -1;
	    goto END_OF_GAMEPAD_PIN_INIT;
	}
	HWREG_W(iop_data[2], ((bClkGpioNum/8)<<8)|(1<<(bClkGpioNum%8)));
	HWREG_W(iop_data[3],((bLatchGpioNum/8)<<8)|(1<<(bLatchGpioNum%8)));
	HWREG_W(iop_data[4],((bD0GpioNum/8)<<8)|(1<<(bD0GpioNum%8)));
	HWREG_W(iop_data[5],((bD1GpioNum/8)<<8)|(1<<(bD1GpioNum%8)));
	HWREG_W(iop_data[6],((bD1GpioNum%8)<<8)|(bD0GpioNum%8));
	HWREG_W(iop_data[0],(HWREG_R(iop_data[0])|FUN_GPAD_CFG_USE_PIN));

	printk("@@@@bClkGpioNum = %d ; bLatchGpioNum = %d; bD0GpioNum = %d; bD1GpioNum = %d@@\n",\
		bClkGpioNum,bLatchGpioNum,bD0GpioNum,bD1GpioNum);

	if (WaitIopFunRdy(IOP_RDY_WAIT) < 0){
	    printk("@NESGamePad_UsePinInit :ERROR-2 \n");
	    iRet = -1;
	}

END_OF_GAMEPAD_PIN_INIT:
    //GL_MutexUnlock(gIOPMutexID); //Kaney mark
    up(&semIOP); //Kaney add
    return iRet;

}
#endif
//2010/11/5  lijin added for MD Game................}}}

#ifdef IOP_GPIO_CONTROL_TEST
typedef enum
{
	IOP_GPIO_OUTPUT_LOW,
	IOP_GPIO_OUTPUT_HIGH,
	IOP_GPIO_INPUT,
	IOP_GPIO_MAX
} Gpio_Status_t;

UINT8 g_u8TestGpio = 1;
UINT8 g_u8TestTargetIC = 1; //0: 1500, 1:1502

static int DRV_IOP_TestGpio_Data_Ready(int iWait)
{
	while (iWait) {
		if (HWREG_R(iop_data[0])& GPIO_TEST_GPIO_INPUT_READY) {
			return iWait;
		}
		--iWait;

		//GL_DelayUs(1); //Kaney mark
		udelay(1); //Kaney add
	}

	return -1;
}

UINT8 DRV_IOP_TestGpio(UINT32 dGpioNum, UINT8 eGpioState)
{
	UINT8 ret = -1;
	UINT8 bGpioNum = dGpioNum;

	GPIO_F_SET(bGpioNum,1);
	GPIO_M_SET(bGpioNum,0);

	printk("%s: Pin=%d, state=%d\n", __FUNCTION__, dGpioNum, eGpioState);
	//GL_MutexLock(gIOPMutexID); //Kaney mark
	down_interruptible(&semIOP); //Kaney add
	if (WaitIopFunRdy(IOP_RDY_WAIT) < 0)
	{
		ret = -1;
		goto END_OF_IOP_TestGpio;
	}
	HWREG_W(iop_data[2], ((dGpioNum/8)<<8)|(1<<(dGpioNum%8)));
	if(eGpioState ==IOP_GPIO_INPUT)
	{
		HWREG_W(iop_data[0],FUN_GPIO_CONTROL|GPIO_TEST_CMD_INPUT);
		if(DRV_IOP_TestGpio_Data_Ready(IOP_RDY_WAIT) < 0)
		{
			ret = -1;
			goto END_OF_IOP_TestGpio;
		}

		if(HWREG_R(iop_data[0])&GPIO_TEST_GPIO_LENEL)
		{
			ret=1;
		}
		else
		{
			ret=0;
		}
		HWREG_W(iop_data[0], (HWREG_R(iop_data[0])&(~GPIO_TEST_GPIO_INPUT_READY)));
	}
	else
	{
		if(eGpioState ==IOP_GPIO_OUTPUT_LOW)
		{
			HWREG_W(iop_data[0],FUN_GPIO_CONTROL|GPIO_TEST_CMD_OUTPUT);
		}
		else
		{
			HWREG_W(iop_data[0],((FUN_GPIO_CONTROL|GPIO_TEST_CMD_OUTPUT)|GPIO_TEST_GPIO_LENEL));
		}
	}

	if (WaitIopFunRdy(IOP_RDY_WAIT) < 0)
	{
		ret = -1;
	}

END_OF_IOP_TestGpio:

	//GL_MutexUnlock(gIOPMutexID); //Kaney mark
    up(&semIOP); //Kaney add

	return ret;
}


//2011/8/3 wenjing.yang modified for mantis 0145407 	START {{{
/*************************************************************
 *u8GPIONum:(important: All of the GPIO numbers musn't be shifted!!!)
 *	bit0~3: Test GPIO Num
 *	0~9(single test)	only test the given GPIO number
 *	0x0f(all test)	test GPIO 0~9
 *
 *	bit4~7: HW Reset GPIO Num
 *
 *u8ICVer:
 *	0(1500)
 *	1(1502)
 ************************************************************/
DRV_Status_t DRV_IOPStandby_ConfigTestGpio(UINT8 u8GPIONum, UINT8 u8ICVer)
{
	g_u8TestGpio = u8GPIONum;
	g_u8TestTargetIC = u8ICVer;
	if(u8ICVer==0)
	{
		printk("Test 1500 Standby GPIO %d\n", u8GPIONum);
	}
	else
	{
		printk("Test 1502 Standby GPIO %d\n", u8GPIONum);
		g_u8TestTargetIC = 1;
	}
	return DRV_SUCCESS;
}

DRV_Status_t DRV_IOPStandby_TestGpio(void)
{
	UINT8 i;

	for (i = 0; i < 10; ++i)
	{
		GPIO_M_SET(i, 0);
	}

	HWREG_W(iop_data[2], (((6 << 4) |g_u8TestGpio) << 8) | g_u8TestTargetIC);

	return DRV_SUCCESS;
}
//2011/8/3 wenjing.yang modified for mantis 0145407 	END }}}
#endif

//2010/4/27 lizhan add for standby wakeup system by gpio control hardware reset.START {{{
/***************************************************
Name		GPIO in Standby		6502-Port		6502-Bit
IR				61			  	   Port 7	                 Bit 5
VFD_CLK			62				   Port 7			   Bit 6
VFD_STB			63				   Port 7			   Bit 7
VFD_DAT			64				   Port 8			   Bit 0
CEC				65				   Port 8			   Bit 1
HPD				66				   Port 8			   Bit 2
HW_CFG2		67				   Port 8			   Bit 3
HW_CFG3		68				   Port 8			   Bit 4
***************************************************/
DRV_Status_t DRV_IOPStandby_Ctrl_Hw_Reset_Gpio(UINT8 bGpioNum)

{
	HWREG_W(iop_data[10], ((bGpioNum/8)<<8)|(1<<(bGpioNum%8)));
	return DRV_SUCCESS;
}
//2010/4/27 lizhan add for standby wakeup system by gpio control hardware reset. END }}}

#if 1 //Kaney add
iop_standby_timer_t iop_standby_timer;

void get_time_from_IOP_standby(void)
{
    iop_standby_timer.bCurSec = HWREG_R(iop_data[1]) >> 8;
	iop_standby_timer.bCurMin = HWREG_R(iop_data[2]) & 0xff;
	iop_standby_timer.bCurHour = HWREG_R(iop_data[2]) >> 8;
	iop_standby_timer.bCurDay = HWREG_R(iop_data[3]) & 0xff;
	iop_standby_timer.bCurMon = HWREG_R(iop_data[3]) >> 8;
	iop_standby_timer.bCurYear = HWREG_R(iop_data[4]);

	printk("iop_standby_timer.bCurSec = %d\n", iop_standby_timer.bCurSec);
	printk("iop_standby_timer.bCurMin = %d\n", iop_standby_timer.bCurMin);
	printk("iop_standby_timer.bCurHour = %d\n", iop_standby_timer.bCurHour);
	printk("iop_standby_timer.bCurDay = %d\n", iop_standby_timer.bCurDay);
	printk("iop_standby_timer.bCurMon = %d\n", iop_standby_timer.bCurMon);
	printk("iop_standby_timer.bCurYear = %d\n", iop_standby_timer.bCurYear);
}

void get_time_from_IOP(iop_standby_timer_t *time)
{
    time->bCurSec = iop_standby_timer.bCurSec;
	time->bCurMin = iop_standby_timer.bCurMin;
	time->bCurHour = iop_standby_timer.bCurHour;
	time->bCurDay = iop_standby_timer.bCurDay;
	time->bCurMon = iop_standby_timer.bCurMon;
	time->bCurYear = iop_standby_timer.bCurYear;
}

#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER
wakeup_timer_t wakeup_timer;

void DVB_Timer_Standby_WriteNeedDateTime(void)
{
	if(wakeup_timer.bWakeupYear == 0 && wakeup_timer.bWakeupMon ==0 && wakeup_timer.bWakeupDay ==0
	     && wakeup_timer.bWakeupHour ==0 && wakeup_timer.bWakeupMin ==0)
	{
	    printk("wakeup_timer is not set.\n");
		wakeup_timer.bWakeupMin = 0xff;
		wakeup_timer.bWakeupHour = 0xff;
		wakeup_timer.bWakeupDay = 0xff;
		wakeup_timer.bWakeupMon = 0xff;
		wakeup_timer.bWakeupYear = 0xff;
		wakeup_timer.bWakeupYear = 0xff;
	}

    WAKEUP_TIMER_MIN_REG			=		wakeup_timer.bWakeupMin;
	WAKEUP_TIMER_HOUR_REG			=		wakeup_timer.bWakeupHour;
	WAKEUP_TIMER_DAY_REG			=		wakeup_timer.bWakeupDay;
	WAKEUP_TIMER_MON_REG			=		wakeup_timer.bWakeupMon;
	WAKEUP_TIMER_YEAR_LOW_REG		=		wakeup_timer.bWakeupYear & 0xff;
	WAKEUP_TIMER_YEAR_HIGH_REG		=		(wakeup_timer.bWakeupYear >> 8) & 0xff;

    CUR_TS_SEC_REG					=		wakeup_timer.bCurSec;
	CUR_TS_MIN_REG					=		wakeup_timer.bCurMin;
	CUR_TS_HOUR_REG					=		wakeup_timer.bCurHour;
	CUR_TS_DAY_REG					=		wakeup_timer.bCurDay;
	CUR_TS_MON_REG					=		wakeup_timer.bCurMon;
	CUR_TS_YEAR_LOW_REG				=		wakeup_timer.bCurYear & 0xff;
	CUR_TS_YEAR_HIGH_REG			=		(wakeup_timer.bCurYear >> 8) & 0xff;

	UINT8 u8LeapYearFlag = 0;
	if((!((wakeup_timer.bCurYear)%4) && ((wakeup_timer.bCurYear)%100)) || (!((wakeup_timer.bCurYear)%400)))
	{
		u8LeapYearFlag = 1<<0;	//is leap year.
	}
	if((!((wakeup_timer.bCurYear+1)%4) && ((wakeup_timer.bCurYear+1)%100)) || (!((wakeup_timer.bCurYear+1)%400)))
	{
		u8LeapYearFlag = 1<<1;	//the next year is leap year.
	}

	CUR_NEXT_TS_YEAR_IS_LEAP_REG	=			u8LeapYearFlag;

	/*struct tm result;
    	struct timeval tv;
    	do_gettimeofday(&tv);
    	time_to_tm(tv.tv_sec, 0, &result);
	result.tm_mon = result.tm_mon + 1;
	result.tm_year = result.tm_year + 1900;
   	printk("result.tm_sec=%d\n",result.tm_sec);
	printk("result.tm_min=%d\n",result.tm_min);
	printk("result.tm_hour=%d\n",result.tm_hour);
	printk("result.tm_mday=%d\n",result.tm_mday);
	printk("result.tm_mon=%d\n",result.tm_mon);
	printk("result.tm_year=%d\n",result.tm_year);*/

	/*CUR_TS_SEC_REG					=			result.tm_sec;
	CUR_TS_MIN_REG					=			result.tm_min;
	CUR_TS_HOUR_REG 				=			result.tm_hour;
	CUR_TS_DAY_REG					=			result.tm_mday;
	CUR_TS_MON_REG					=			result.tm_mon;
	CUR_TS_YEAR_LOW_REG 			=			result.tm_year & 0xff;
	CUR_TS_YEAR_HIGH_REG			=			(result.tm_year >> 8) & 0xff;

	UINT8 u8LeapYearFlag = 0;
	if((!((result.tm_year)%4) && ((result.tm_year)%100)) || (!((result.tm_year)%400)))
	{
		u8LeapYearFlag = 1<<0;	//is leap year.
	}
	if((!((result.tm_year+1)%4) && ((result.tm_year+1)%100)) || (!((result.tm_year+1)%400)))
	{
		u8LeapYearFlag = 1<<1;	//the next year is leap year.
	}

	CUR_NEXT_TS_YEAR_IS_LEAP_REG	=			u8LeapYearFlag;*/
}

vfunc DVB_Timer_Standby_WriteDateTimeIop2(void)
{
	return DVB_Timer_Standby_WriteNeedDateTime;
}
#endif

int MAINAPP_FinalizePlatform(void) //Kaney add
{
#if defined(SUPPORT_CEC) && defined(CONFIG_SUNPLUS_CEC_STB) //#ifdef SUPPORT_CEC
	//DRV_CEC_Finalize(); //Kaney mark
#endif

#if defined(SUPPORT_VFD_DRIVER) && defined(CONFIG_SUNPLUS_VFD_STB) //#ifdef SUPPORT_VFD_DRIVER //lizhan 2010-3-12 13:50 .
	//VFD_Finalize(); //Kaney mark
#endif

    //Initialize Standby IR
	extern const UINT32 g_u32CustomCode;
	extern const DRV_IRStandbySupCmd_t g_u8IrStandbyTable[];
	extern UINT8 IrStandby_Get_CmdCount(void);
	ir_Standby_Initialize((UINT8 *)g_u8IrStandbyTable, IrStandby_Get_CmdCount(), g_u32CustomCode);

#if defined(SUPPORT_CEC) && defined(CONFIG_SUNPLUS_CEC_STB) //#ifdef SUPPORT_CEC
	cec_Standby_Initialize();
#endif

    //Un-Init Normal IOP
	//DRV_IOPUninit(); //Kaney mark

    //Initialize Standby IOP
#ifdef IR_PROTOCOL_PHILIPS
	extern const unsigned char IOPcode_PHILIPS_Standby[];
#else
	extern const unsigned char IOPcode_NEC_Standby[];
#endif
	DRV_IopStandbyCfg_t stIopStandbyCfg = {0};
#ifdef IR_PROTOCOL_PHILIPS
	stIopStandbyCfg.pbIopCode = IOPcode_PHILIPS_Standby;
#else
	stIopStandbyCfg.pbIopCode = IOPcode_NEC_Standby;
#endif
	stIopStandbyCfg.pfSetupIrIop	= ir_Standby_Get_SetupIopFunct();

#if defined(SUPPORT_VFD_DRIVER) && defined(CONFIG_SUNPLUS_VFD_STB) //#if defined( SUPPORT_VFD_DRIVER )
	stIopStandbyCfg.pfSetupVfdIop	= vfd_Standby_Get_SetupIopFunct();
#else
	stIopStandbyCfg.pfSetupVfdIop	= NULL;
#endif

#if defined(SUPPORT_CEC) && defined(CONFIG_SUNPLUS_CEC_STB) //#ifdef SUPPORT_CEC
	stIopStandbyCfg.pfSetupCecIop	= cec_Standby_Get_SetupIopFunct();
#else
	stIopStandbyCfg.pfSetupCecIop	= NULL;
#endif

#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER //lizhan add 2010-3-23
	stIopStandbyCfg.pfWriteDateTimeIop=DVB_Timer_Standby_WriteDateTimeIop2();
#endif
	//2010/4/27 lizhan add for standby wakeup system by gpio control hardware reset.START {{{
	DRV_IOPStandby_Ctrl_Hw_Reset_Gpio(67);//default use HW_CFG2(GPIO 67)
	//2010/4/27 lizhan add for standby wakeup system by gpio control hardware reset. END }}}

    while (DRV_IOPStandby_Initialize(&stIopStandbyCfg) != DRV_SUCCESS);	//2012/6/11 wenjing.yang modified for mantis 0180853

	//DRV_IOPStandby_RISC_PowerOff();		//2012/8/15 wenjing.yang modified for mantis 0190578 //Kaney mark for move to MAINAPP_FinalizePlatform2()

	return 0;
}

int MAINAPP_FinalizePlatform2(void) //Kaney add
{
	DRV_IOPStandby_RISC_PowerOff();
	
	return 0;
}

#endif
