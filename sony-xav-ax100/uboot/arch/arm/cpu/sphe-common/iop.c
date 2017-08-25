
#include "iop.h"

#include <common.h>
#include <malloc.h>
#include "drivers/gpio/sp_gpio.h"
 
extern const unsigned char IOPcode_NEC [];

#define IOP_DATA0_CMD(type, cmd, param)	((type<<12)|(cmd<<8)|(param))

int Check_IOPData0Clear(void)
{
	unsigned int dTimeOutCnt = 100;
	
	while(((IOP_REG->iop_data[0] >> 12) & 0xF) != 0)
	{
		dTimeOutCnt--;
		if(dTimeOutCnt == 0)
		{	
			printf("[IOP]>>> Error(wait iop data0 clear timeout)................!!!! <<<\n");
			return -1;
		}		
		mdelay(1);
	}

	return 0;
}

void SendCmdToIOPData0(unsigned char bType, unsigned char bCmd ,unsigned char bParam)
{
	unsigned short wData0=0;
	
	wData0=IOP_DATA0_CMD(bType, bCmd, bParam);
	IOP_REG->iop_data[0] = wData0;
}

int IOP_BackupCam_SetGPIO(unsigned char bNum)
{	
	int dRet=0;
	
	dRet=Check_IOPData0Clear();
	if(dRet != 0)
		return dRet;
	
	SendCmdToIOPData0(IOP_CMD_TYPE_BACKUPCAM, BACKUPCAM2IOP_SET_GPIO, bNum);
	return dRet;
}

int IOP_BackupCam_SetPanelResolution(unsigned char bValue)
{	
	int dRet=0;
	
	dRet=Check_IOPData0Clear();
	if(dRet != 0)
		return dRet;

	SendCmdToIOPData0(IOP_CMD_TYPE_BACKUPCAM, BACKUPCAM2IOP_SET_PAN_RES, bValue);
	return dRet;
}

static int DRV_BackupCam_Init(void)
{
	if(IOP_BackupCam_SetPanelResolution(PAN_RES_0) != 0)
		return 1;
	
	GPIO_F_SET(1,1);
	GPIO_M_SET(1,0);
	if(IOP_BackupCam_SetGPIO(1) != 0)
		return 1;

	return 0;	
}

#ifdef SUPPORT_IOP_CHECKSUM
static void _DRV_IOP_Go(void)
{
    //Exit checksum flow if iop support
    IOP_REG->iop_data[0] = 0;
}
#endif

static int _DRV_IOP_Checksum(unsigned int StartAddr, unsigned int EndAddr, unsigned int iop_buffer_addr)
{
	int ret = 1;
	unsigned int sum = 0;
	unsigned char *dBaseAddr = (unsigned char *)iop_buffer_addr;
    int dTimeOutCnt = 100;
    int i;
	unsigned short IOPDataBackup[12];

    if (0 == dBaseAddr || StartAddr <= 0 || EndAddr <= 0 || StartAddr >= EndAddr)
        return 0;

    //Backup iop_data[1~11]
    memset(IOPDataBackup, 0x00, sizeof(IOPDataBackup));
    for (i = 1; i < 12; ++i)
        IOPDataBackup[i] = IOP_REG->iop_data[i];

    for(sum = 0, i = StartAddr; i < EndAddr; ++i)
        sum += dBaseAddr[i];

    //Enable iop 32-bit address mode
    IOP_ENABLE_32BIT_ADDR();

    //Inform IOP that RISC needs to checksum,IOP will run checksum flow later.
    IOP_REG->iop_data[0] = (IOP_REG->iop_data[0] | IOP_CHECKSUM_START);
    IOP_REG->iop_data[1] = StartAddr;
    IOP_REG->iop_data[2] = EndAddr;

    //Start IOP
    IOP_GO();                   /* release IOP */
    //GL_DelayUs(1000); //Kaney mark
    mdelay(1); //Kaney add

    IOP_REG->iop_regsel = 1;
    if (IOP_REG->iop_regout == 0x02)
    {
        IOP_REG->iop_regsel = 2;
        if (IOP_REG->iop_regout == 0x00)
        {
            printf("[IOP] IOP_GO Fail\n");
            return 0;
        }
    }

    while (((IOP_REG->iop_data[0] & IOP_CHECKSUM_READY) == 0) && dTimeOutCnt)
    {
        printf("[IOP] Wait checksum...!(%d) %x\n", dTimeOutCnt,IOP_REG->iop_data[0]);
        --dTimeOutCnt;
        mdelay(1); //Kaney add
    }
    if(dTimeOutCnt > 0)
    {
        unsigned int iop_sum = ((IOP_REG->iop_data[2]) << 16) | IOP_REG->iop_data[1];
        if(iop_sum != sum)
        {
            printf("[IOP] CheckSum[0x%x : 0x%x]Err Src 0x%x Dst 0x%x\n", StartAddr, EndAddr, sum, iop_sum);
            ret = 0;
        }
        else
        {
            printf("[IOP] IOP_Checksum = 0x%x\n", iop_sum);
            //Recover iop_data[1~11]
            for(i = 1; i < 12; ++i)
                IOP_REG->iop_data[i] = IOPDataBackup[i];
		IOP_REG->iop_data[0] = ((IOP_REG->iop_data[0]) & (~IOP_CHECKSUM_READY)); //  IOP_CHECKSUM_READY bit  must be  cleared 

        }
    }
    else
    {
        printf("[IOP] Init Standby (wait CheckSum) Error! \n");
        ret = 0;
    }

    return ret;
}

int DRV_IOP_InitByClk(unsigned short wSystemClock, unsigned int iop_buffer_addr)
{
	unsigned short wLen = IOP_CODE_SIZE;
	unsigned int dBaseAddr_phys = iop_buffer_addr;

    GPIO_F_SET(0,1);    //set gpio 0 to be controled by IOP
    GPIO_M_SET(0,0);

	IOP_REG->iop_control = ((IOP_REG->iop_control) &( ~(1 << 15)));
	IOP_REG->iop_control = ((IOP_REG->iop_control) &( ~(1 << 9)));

	mdelay(2);

	IOP_RESET();
	mdelay(1);
	IOP_RESET(); 
	MOON0_REG->reset[0] = ((MOON0_REG->reset[0]) | (1 << 4));
	mdelay(1);
	MOON0_REG->reset[0] = ((MOON0_REG->reset[0]) & (~(1 << 4)));

	//memcpy((unsigned char *)dBaseAddr_phys, IOPcode_NEC, wLen);

	IOP_REG->iop_base_adr_l = (dBaseAddr_phys & 0xFFFF);
	IOP_REG->iop_base_adr_h = (dBaseAddr_phys >> 16);

    //Setup IOP - IR/CEC timer
    IOP_REG->iop_data[2] = wSystemClock*100; //CEC
    IOP_REG->iop_data[3] = wSystemClock*112; //Nec IR

#ifdef SUPPORT_IOP_CHECKSUM
    if(_DRV_IOP_Checksum(0x0054, 0x1000, iop_buffer_addr) != 1)
        return 0;

    _DRV_IOP_Go();
#endif

	//DRV_BackupCam_Init();

	return 1;
}
