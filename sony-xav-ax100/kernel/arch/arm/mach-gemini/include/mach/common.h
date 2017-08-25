/**
 * @file common.h
 * @brief Basic datatype definition
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <mach/typedef.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#ifndef NULL
#define NULL  ((void *)0)
#endif

#define SUCCESS         0
#define FAIL            1

#define TRUE            1
#define FALSE           0

#define BOOL 	unsigned char

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

#define HAL_READ_UINT8( _register_, _value_ ) \
        ((_value_) = *((volatile UINT8 *)(_register_)))

#define HAL_WRITE_UINT8( _register_, _value_ ) \
        (*((volatile UINT8 *)(_register_)) = (_value_))

#define HAL_READ_UINT16( _register_, _value_ ) \
        ((_value_) = *((volatile UINT16 *)(_register_)))

#define HAL_WRITE_UINT16( _register_, _value_ ) \
        (*((volatile UINT16 *)(_register_)) = (_value_))

#define HAL_READ_UINT32( _register_, _value_ ) \
        ((_value_) = *((volatile UINT32 *)(_register_)))

#define HAL_WRITE_UINT32( _register_, _value_ ) \
        (*((volatile UINT32 *)(_register_)) = (_value_))

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/

/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/
/*workaround for <stdio.h> in which putchar is a macro*/
#ifndef WIN32
#undef putchar
int putchar(int ch);
#endif

#undef  sio_printf
#define sio_printf   printf
#undef  sio_vprintf
#define sio_vprintf  vsprintf
void usb_printf(const char *format,...);

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

#endif  /* __COMMON_H__ */

