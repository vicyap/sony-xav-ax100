
typedef unsigned char       BYTE    ;
typedef unsigned short      WORD    ;
typedef unsigned long       DWORD   ;

typedef unsigned long long  UINT64  ;
typedef unsigned int        UINT32  ;
typedef unsigned short      UINT16  ;
typedef unsigned char       UINT8   ;

typedef long long           INT64   ;
typedef int                 INT32   ;
typedef short               INT16   ;
typedef signed char         INT8    ;


#define PASS 				0
#define PASS2				1
#define FAIL				2

#define ROM_SUCCESS         PASS
#define ROM_FAIL           	FAIL

#define DEV_READ            2
#define DEV_WRITE           3

enum boot_code_parameter {
   VIRTUAL_PAGE_SIZE 						= 2048,
   VIRTUAL_PAGE_SHIFT 					= 11,
   VIRTUAL_PAGE_PER_BLOCK 			= 32,
   VIRTUAL_PAGE_PER_BLOCK_SHIFT = 5
};


#define MAX_ID_LEN 32
typedef struct SysInfo
{
	unsigned char IdBuf[32]; //[MAX_ID_LEN]; //####

	unsigned short	u16PageNoPerBlk; //####
	unsigned short	u16PageSize;
	
	unsigned short	u16PyldLen; //####
	unsigned short	u16ReduntLen;

	unsigned short	u16Redunt_Sector_Addr_Offset;
	unsigned short	u16Redunt_Sector_Len;

	unsigned int		u16TotalBlkNo; //####

	unsigned char		u8TotalBlkNoShift;
	unsigned char		ecc_mode;
	unsigned char		u8MultiChannel;//u8NFChannel;
	unsigned char		u8Support_Internal_Interleave;

	unsigned char		u8Support_External_Interleave;
	unsigned char		u8Internal_Chip_Number;
	unsigned short	u16InterruptMask;

	unsigned char		u8PagePerBlkShift;
	unsigned char		u8Support_TwoPlan;

	unsigned char		vendor_no;
	unsigned char		u8addrCycle;
	unsigned char		reserved;
	unsigned char		reserved1;
}psysinfo_t,*ppsysinfo_t;


typedef unsigned char (*predInitDriver_t)(void);
typedef void (*predEraseBlk_t)(unsigned char, unsigned int);
typedef int (*predReadWritePage_t)(unsigned char, unsigned int ,unsigned int* ,unsigned int* ,unsigned char );
typedef void (*predGetInfo_t)(void);

typedef struct SDev_s {

	unsigned char IsSmallblk;
	unsigned char IsSupportBCH;
	unsigned char DeviceID;
	unsigned char reserved;

		
	predInitDriver_t predInitDriver;
	predEraseBlk_t predEraseBlk ;
	predReadWritePage_t predReadWritePage ;
	predGetInfo_t predGetInfo;
}SDev_t;

typedef struct _BootInfo_t
{
	unsigned long pwrc_cfg;		//PWRON1(USB), PWRON0(button)
	unsigned long vic1_rawSts;		//RTC status
	unsigned long keyscan4;
	unsigned long iotraps;
	unsigned long version;
	unsigned long UsbNo;		//for redboot only
	unsigned long PLL2_D1_Clock;
}BootInfo_st;




