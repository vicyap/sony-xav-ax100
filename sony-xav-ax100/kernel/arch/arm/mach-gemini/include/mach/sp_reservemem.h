#ifndef _SP_RESERVEMEM_H_
#define _SP_RESERVEMEM_H_

/* variables of reserved address */
extern unsigned int res_arm926_addr;
extern unsigned int res_arm926_size;
extern unsigned int res_iop_addr;
extern unsigned int res_iop_size;
extern unsigned int res_display_addr;
extern unsigned int res_display_size;
extern unsigned int res_rtcfg_addr;
extern unsigned int res_rtcfg_size;
extern unsigned int res_chunkmem_addr;
extern unsigned int res_chunkmem_size;

/* iop start address */
extern volatile u32* sp_get_iop_addr_v(void);
extern u32 sp_get_iop_addr_p(void);

/* reserve default memory map */
extern void __init default_memory_reservation(void);

#endif // _SP_RESERVEMEM_H_
