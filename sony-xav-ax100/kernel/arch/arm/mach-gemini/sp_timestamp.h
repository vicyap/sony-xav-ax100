#ifndef __SUPPORT_SP_TIMESTAMP__
#define __SUPPORT_SP_TIMESTAMP__

#define SUPPORT_SP_TIMESTAMP
#define USE_PRINTK (0)
#define USE_EARLY_PRINTK (1)

extern void SP_TIMESTAMP( char *s, unsigned int th , unsigned char use_early_printk );

#endif // __SUPPORT_SP_TIMESTAMP__

