
#include <linux/delay.h>
#include <mach/kernel.h>
#include <mach/hal/hal_imgdecsnd.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

#define MJMODE_DONE_DEFAULT_TIMEOUT   1000 // (ms) // MJmode uses interrupt to replace polling

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/

JR_FILTER_COEF g_imgdecsnd_jr_coef_tbl[18] = {
// emu_odd_enlarge_linear
// group 1
{
{
0x0000, 0x003c,
0x0400, 0x0000,
0x0000, 0x0038,
0x0800, 0x0000,
0x0000, 0x0034,
0x0c00, 0x0000,
0x0000, 0x0030,
0x1000, 0x0000,
0x0000, 0x002c,
0x1400, 0x0000,
0x0000, 0x0028,
0x1800, 0x0000,
0x0000, 0x0024,
0x1c00, 0x0000,
0x0000, 0x0020,
0x2000, 0x0000
},
{
0x0000, 0x0000,
0x4000, 0x0000
},
{
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000
},
{
0x0000, 0x0000,
0x0000, 0x0000
}
},
// group 2
{
{
0x0000, 0x0040,
0x0000, 0x0000,
0x0000, 0x0040,
0x0000, 0x0000,
0x0000, 0x0040,
0x0000, 0x0000,
0x0000, 0x0040,
0x0000, 0x0000,
0x0000, 0x0040,
0x0000, 0x0000,
0x0000, 0x0040,
0x0000, 0x0000,
0x0000, 0x0040,
0x0000, 0x0000,
0x0000, 0x0040,
0x0000, 0x0000
},
{
0x0000, 0x0040,
0x0000, 0x0000
},
{
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000,
0x0000, 0x0000
},
{
0x0000, 0x0000,
0x0000, 0x0000
}
},
// group 3
{
{
0x01fe, 0x003b,
0x07fc, 0x02ff,
0x00ff, 0xfd3a,
0x0bfa, 0x02ff,
0x0001, 0xfa38,
0x10f9, 0x02ff,
0xff01, 0xf836,
0x14f8, 0x03ff,
0xff02, 0xf733,
0x19f7, 0x03ff,
0xff03, 0xf630,
0x1ef6, 0x03ff,
0xff03, 0xf52c,
0x23f5, 0x03ff,
0xff03, 0xf527,
0x27f5, 0x03ff
},
{
0x0001, 0xfd03,
0x3b03, 0xfd01
},
{
0x0003, 0x0101,
0x0100, 0x0002,
0x0203, 0x0102,
0x0203, 0x0201,
0x0000, 0x0303,
0x0002, 0x0301,
0x0303, 0x0302,
0x0301, 0x0100,
0x0202, 0x0102,
0x0201, 0x0200,
0x0101, 0x0100,
0x0101, 0x0300,
0x0002, 0x0300,
0x0103, 0x0300,
0x0003, 0x0203,
0x0302, 0x0300
},
{
0x0002, 0x0202,
0x0002, 0x0202
}
},
// group 4
{
{
0x02fc, 0x0337,
0x0afa, 0x02ff,
0x01fd, 0x0036,
0x0ef9, 0x03ff,
0x01fe, 0xfe35,
0x12f8, 0x03ff,
0x00ff, 0xfb33,
0x16f7, 0x03ff,
0x0000, 0xfa30,
0x1af7, 0x03ff,
0x0001, 0xf82e,
0x1ff6, 0x03ff,
0x0002, 0xf72a,
0x23f6, 0x02ff,
0xff02, 0xf627,
0x27f6, 0x02ff
},
{
0x0002, 0xfb06,
0x3706, 0xfb02
},
{
0x0002, 0x0202,
0x0201, 0x0201,
0x0202, 0x0203,
0x0101, 0x0001,
0x0103, 0x0001,
0x0101, 0x0001,
0x0302, 0x0302,
0x0202, 0x0101,
0x0202, 0x0003,
0x0300, 0x0101,
0x0101, 0x0201,
0x0002, 0x0001,
0x0000, 0x0203,
0x0002, 0x0302,
0x0302, 0x0300,
0x0003, 0x0203
},
{
0x0001, 0x0203,
0x0003, 0x0201
}
},
// group 5
{
{
0x02fb, 0x0634,
0x0cf9, 0x02ff,
0x02fc, 0x0333,
0x10f8, 0x02ff,
0x01fd, 0x0032,
0x14f8, 0x02ff,
0x01fd, 0xfe31,
0x17f7, 0x02ff,
0x01fe, 0xfc2e,
0x1bf7, 0x02ff,
0x01ff, 0xfa2c,
0x1ff7, 0x0200,
0x0000, 0xf929,
0x22f7, 0x0100,
0x0001, 0xf826,
0x26f8, 0x0100
},
{
0x0002, 0xfa09,
0x3409, 0xfa02
},
{
0x0100, 0x0101,
0x0301, 0x0302,
0x0000, 0x0103,
0x0102, 0x0302,
0x0300, 0x0301,
0x0000, 0x0302,
0x0203, 0x0100,
0x0302, 0x0203,
0x0103, 0x0202,
0x0202, 0x0103,
0x0002, 0x0301,
0x0002, 0x0000,
0x0301, 0x0201,
0x0303, 0x0201,
0x0200, 0x0200,
0x0002, 0x0002
},
{
0x0002, 0x0101,
0x0001, 0x0102
}
},
// group 6
{
{
0x03f9, 0x0832,
0x0ef8, 0x03ff,
0x02fa, 0x0532,
0x11f7, 0x02ff,
0x02fb, 0x0230,
0x15f7, 0x02ff,
0x02fc, 0x002f,
0x18f7, 0x0200,
0x02fd, 0xfe2d,
0x1bf7, 0x0100,
0x01fe, 0xfc2b,
0x1ff7, 0x0100,
0x01ff, 0xfa28,
0x22f8, 0x0000,
0x0100, 0xf925,
0x25f9, 0x0001
},
{
0x0003, 0xf90b,
0x320b, 0xf903
},
{
0x0003, 0x0001,
0x0101, 0x0002,
0x0302, 0x0000,
0x0303, 0x0302,
0x0202, 0x0203,
0x0002, 0x0203,
0x0102, 0x0000,
0x0202, 0x0100,
0x0002, 0x0001,
0x0302, 0x0301,
0x0301, 0x0000,
0x0203, 0x0102,
0x0201, 0x0201,
0x0202, 0x0303,
0x0000, 0x0202,
0x0202, 0x0000
},
{
0x0000, 0x0000,
0x0000, 0x0000
}
},
// group 7
{
{
0x02f9, 0x0a2f,
0x10f8, 0x0200,
0x02f9, 0x072e,
0x13f8, 0x0100,
0x02fa, 0x052d,
0x16f8, 0x0100,
0x02fb, 0x022c,
0x19f8, 0x0000,
0x02fb, 0x002a,
0x1cf9, 0x0001,
0x02fc, 0xfe29,
0x1ff9, 0xff01,
0x02fd, 0xfd26,
0x21fa, 0xff01,
0x01fe, 0xfb24,
0x24fb, 0xfe01
},
{
0x0002, 0xf80d,
0x300d, 0xf802
},
{
0x0100, 0x0202,
0x0101, 0x0001,
0x0203, 0x0303,
0x0001, 0x0301,
0x0201, 0x0103,
0x0101, 0x0102,
0x0200, 0x0302,
0x0102, 0x0303,
0x0103, 0x0303,
0x0100, 0x0100,
0x0102, 0x0300,
0x0102, 0x0201,
0x0002, 0x0003,
0x0302, 0x0002,
0x0301, 0x0301,
0x0103, 0x0103
},
{
0x0001, 0x0201,
0x0001, 0x0201
}
},
// group 8
{
{
0x01f9, 0x0c2d,
0x11f9, 0x0000,
0x01f9, 0x092c,
0x14f9, 0x0000,
0x01fa, 0x072b,
0x17f9, 0x0000,
0x01fa, 0x052a,
0x19fa, 0xff01,
0x01fb, 0x0329,
0x1cfa, 0xfe01,
0x01fb, 0x0127,
0x1ffb, 0xfe01,
0x01fc, 0xff25,
0x21fc, 0xfd01,
0x01fd, 0xfd23,
0x23fd, 0xfd01
},
{
0x0001, 0xf90f,
0x2e0f, 0xf901
},
{
0x0101, 0x0101,
0x0300, 0x0302,
0x0203, 0x0302,
0x0101, 0x0103,
0x0300, 0x0103,
0x0002, 0x0003,
0x0302, 0x0002,
0x0300, 0x0200,
0x0300, 0x0001,
0x0103, 0x0301,
0x0303, 0x0002,
0x0002, 0x0101,
0x0301, 0x0103,
0x0102, 0x0302,
0x0200, 0x0303,
0x0303, 0x0002
},
{
0x0000, 0x0000,
0x0000, 0x0000
}
},
// group 9
{
{
0xfffb, 0x0e28,
0x13fb, 0xfe00,
0xfffb, 0x0c27,
0x15fc, 0xfe00,
0x00fb, 0x0a27,
0x17fd, 0xfe00,
0x00fb, 0x0826,
0x19fd, 0xfd00,
0x00fb, 0x0625,
0x1bfe, 0xfd00,
0x00fb, 0x0524,
0x1dff, 0xfd00,
0x00fc, 0x0322,
0x1f00, 0xfc00,
0x00fc, 0x0221,
0x2102, 0xfc00
},
{
0x00ff, 0xfb11,
0x2811, 0xfbff
},
{
0x0201, 0x0301,
0x0103, 0x0302,
0x0301, 0x0303,
0x0101, 0x0202,
0x0001, 0x0300,
0x0100, 0x0102,
0x0101, 0x0302,
0x0202, 0x0302,
0x0102, 0x0301,
0x0202, 0x0203,
0x0203, 0x0000,
0x0202, 0x0003,
0x0200, 0x0203,
0x0103, 0x0302,
0x0201, 0x0001,
0x0100, 0x0102
},
{
0x0001, 0x0200,
0x0200, 0x0201
}
},
// group 10
{
{
0xfefc, 0x1025,
0x14fd, 0xfd00,
0xfefc, 0x0e24,
0x16fe, 0xfd00,
0xfefb, 0x0d24,
0x18ff, 0xfc00,
0xfffb, 0x0b23,
0x1900, 0xfc00,
0xfffb, 0x0922,
0x1b01, 0xfc00,
0xfffb, 0x0821,
0x1c02, 0xfc00,
0xfffb, 0x0620,
0x1e03, 0xfb00,
0x00fb, 0x041f,
0x1f04, 0xfb00
},
{
0x00fd, 0xfd12,
0x2512, 0xfdfd
},
{
0x0102, 0x0200,
0x0102, 0x0202,
0x0200, 0x0303,
0x0001, 0x0102,
0x0303, 0x0001,
0x0000, 0x0302,
0x0002, 0x0103,
0x0200, 0x0202,
0x0102, 0x0203,
0x0100, 0x0102,
0x0202, 0x0003,
0x0301, 0x0001,
0x0302, 0x0103,
0x0102, 0x0301,
0x0003, 0x0302,
0x0203, 0x0300
},
{
0x0003, 0x0002,
0x0202, 0x0003
}
},
// group 11
{
{
0xfdff, 0x1120,
0x1400, 0xfc00,
0xfdfe, 0x1020,
0x1501, 0xfcff,
0xfdfe, 0x0f1f,
0x1702, 0xfcff,
0xfdfd, 0x0d1f,
0x1803, 0xfcff,
0xfefd, 0x0c1e,
0x1904, 0xfcff,
0xfefd, 0x0a1e,
0x1a05, 0xfcff,
0xfefc, 0x091d,
0x1b07, 0xfcff,
0xfefc, 0x081c,
0x1c08, 0xfcfe
},
{
0x00fd, 0x0013,
0x2013, 0x00fd
},
{
0x0001, 0x0301,
0x0103, 0x0300,
0x0103, 0x0100,
0x0303, 0x0203,
0x0201, 0x0003,
0x0002, 0x0202,
0x0303, 0x0201,
0x0102, 0x0202,
0x0001, 0x0003,
0x0203, 0x0201,
0x0100, 0x0301,
0x0203, 0x0200,
0x0203, 0x0201,
0x0200, 0x0200,
0x0302, 0x0102,
0x0201, 0x0203
},
{
0x0000, 0x0000,
0x0000, 0x0000
}
},
// group 12
{
{
0xfd01, 0x111c,
0x1303, 0xfdff,
0xfd01, 0x101c,
0x1404, 0xfdff,
0xfd00, 0x0f1b,
0x1504, 0xfdff,
0xfe00, 0x0d1b,
0x1605, 0xfdff,
0xfeff, 0x0c1b,
0x1706, 0xfdff,
0xfeff, 0x0b1a,
0x1807, 0xfefe,
0xfefe, 0x0a1a,
0x1808, 0xfefe,
0xfefe, 0x0919,
0x1909, 0xfefe
},
{
0x00fd, 0x0212,
0x1c12, 0x02fd
},
{
0x0303, 0x0001,
0x0100, 0x0202,
0x0300, 0x0000,
0x0100, 0x0301,
0x0302, 0x0003,
0x0103, 0x0301,
0x0000, 0x0302,
0x0103, 0x0300,
0x0002, 0x0301,
0x0102, 0x0300,
0x0101, 0x0302,
0x0002, 0x0003,
0x0103, 0x0301,
0x0302, 0x0102,
0x0202, 0x0202,
0x0202, 0x0202
},
{
0x0003, 0x0100,
0x0000, 0x0103
}
},
// group 13
{
{
0xff04, 0x0f17,
0x1105, 0xffff,
0xff03, 0x0f16,
0x1106, 0xffff,
0xff03, 0x0e17,
0x1206, 0xffff,
0xff02, 0x0d16,
0x1307, 0x00ff,
0xff02, 0x0c16,
0x1308, 0x00ff,
0xff02, 0x0c16,
0x1409, 0x00ff,
0xff01, 0x0b15,
0x1409, 0x00ff,
0xff01, 0x0a15,
0x150a, 0x01ff
},
{
0x00ff, 0x0410,
0x1710, 0x04ff
},
{
0x0101, 0x0300,
0x0102, 0x0202,
0x0103, 0x0003,
0x0301, 0x0302,
0x0101, 0x0100,
0x0203, 0x0301,
0x0003, 0x0203,
0x0102, 0x0001,
0x0001, 0x0302,
0x0301, 0x0101,
0x0000, 0x0000,
0x0100, 0x0201,
0x0002, 0x0103,
0x0303, 0x0301,
0x0001, 0x0201,
0x0102, 0x0100
},
{
0x0002, 0x0301,
0x0001, 0x0302
}
},
// group 14
{
{
0x0302, 0x0f14,
0x1006, 0x00ff,
0x0004, 0x0e14,
0x1007, 0x00ff,
0x0004, 0x0d14,
0x1107, 0x01ff,
0x0004, 0x0d14,
0x1108, 0x01ff,
0x0003, 0x0c14,
0x1208, 0x01ff,
0x0003, 0x0b14,
0x1209, 0x01ff,
0xff02, 0x0b13,
0x130a, 0x02ff,
0xff02, 0x0a13,
0x130a, 0x02ff
},
{
0x0000, 0x050f,
0x140f, 0x0500
},
{
0x0101, 0x0003,
0x0002, 0x0203,
0x0103, 0x0103,
0x0200, 0x0303,
0x0001, 0x0302,
0x0102, 0x0003,
0x0000, 0x0001,
0x0300, 0x0103,
0x0002, 0x0200,
0x0003, 0x0203,
0x0000, 0x0300,
0x0201, 0x0303,
0x0303, 0x0102,
0x0000, 0x0003,
0x0301, 0x0202,
0x0202, 0x0103
},
{
0x0002, 0x0302,
0x0202, 0x0302
}
},
// group 15
{
{
0x0207, 0x0d10,
0x0e08, 0x0200,
0x0106, 0x0d10,
0x0e08, 0x0200,
0x0106, 0x0c10,
0x0e08, 0x0300,
0x0106, 0x0c10,
0x0f09, 0x0300,
0x0105, 0x0c10,
0x0f09, 0x0300,
0x0105, 0x0b10,
0x0f0a, 0x0300,
0x0005, 0x0b10,
0x0f0a, 0x0400,
0x0004, 0x0a10,
0x100a, 0x0400
},
{
0x0002, 0x070d,
0x110d, 0x0702
},
{
0x0001, 0x0203,
0x0000, 0x0200,
0x0303, 0x0103,
0x0201, 0x0300,
0x0202, 0x0302,
0x0303, 0x0001,
0x0100, 0x0202,
0x0001, 0x0101,
0x0102, 0x0002,
0x0103, 0x0201,
0x0001, 0x0301,
0x0200, 0x0302,
0x0300, 0x0100,
0x0302, 0x0102,
0x0302, 0x0300,
0x0003, 0x0203
},
{
0x0001, 0x0203,
0x0003, 0x0201
}
},
// group 16
{
{
0x0307, 0x0c0f,
0x0d08, 0x0300,
0x0207, 0x0c0f,
0x0d08, 0x0300,
0x0207, 0x0c0f,
0x0d09, 0x0300,
0x0206, 0x0c0f,
0x0e09, 0x0401,
0x0206, 0x0b0f,
0x0e09, 0x0401,
0x0106, 0x0b0e,
0x0e0a, 0x0401,
0x0105, 0x0b0e,
0x0e0a, 0x0501,
0x0105, 0x0a0e,
0x0e0a, 0x0501
},
{
0x0003, 0x080d,
0x0f0d, 0x0803
},
{
0x0003, 0x0301,
0x0101, 0x0102,
0x0301, 0x0201,
0x0202, 0x0203,
0x0200, 0x0100,
0x0300, 0x0303,
0x0102, 0x0000,
0x0001, 0x0000,
0x0001, 0x0200,
0x0003, 0x0200,
0x0300, 0x0103,
0x0100, 0x0301,
0x0302, 0x0003,
0x0201, 0x0001,
0x0201, 0x0302,
0x0203, 0x0102
},
{
0x0000, 0x0001,
0x0201, 0x0000
}
},
// group 17
{
{
0x0307, 0x0c0e,
0x0c08, 0x0401,
0x0307, 0x0c0e,
0x0d08, 0x0401,
0x0307, 0x0b0e,
0x0d09, 0x0401,
0x0207, 0x0b0e,
0x0d09, 0x0401,
0x0206, 0x0b0e,
0x0d09, 0x0501,
0x0206, 0x0b0e,
0x0d0a, 0x0501,
0x0206, 0x0a0e,
0x0d0a, 0x0501,
0x0205, 0x0a0d,
0x0d0a, 0x0502
},
{
0x0003, 0x080c,
0x0e0c, 0x0803
},
{
0x0203, 0x0101,
0x0302, 0x0000,
0x0102, 0x0001,
0x0003, 0x0100,
0x0001, 0x0301,
0x0000, 0x0201,
0x0300, 0x0201,
0x0101, 0x0301,
0x0203, 0x0100,
0x0202, 0x0002,
0x0101, 0x0000,
0x0300, 0x0102,
0x0000, 0x0300,
0x0301, 0x0203,
0x0003, 0x0203,
0x0302, 0x0300
},
{
0x0003, 0x0103,
0x0203, 0x0103
}
},
// group 18
{
{
0x0308, 0x0c0d,
0x0c08, 0x0401,
0x0307, 0x0b0d,
0x0c09, 0x0401,
0x0307, 0x0b0d,
0x0c09, 0x0401,
0x0307, 0x0b0d,
0x0c09, 0x0501,
0x0207, 0x0b0d,
0x0d09, 0x0501,
0x0206, 0x0a0d,
0x0d09, 0x0502,
0x0206, 0x0a0d,
0x0d0a, 0x0502,
0x0206, 0x0a0d,
0x0d0a, 0x0602
},
{
0x0004, 0x080c,
0x0e0c, 0x0804
},
{
0x0300, 0x0003,
0x0203, 0x0100,
0x0203, 0x0303,
0x0200, 0x0201,
0x0102, 0x0202,
0x0301, 0x0302,
0x0001, 0x0103,
0x0302, 0x0002,
0x0300, 0x0002,
0x0003, 0x0103,
0x0303, 0x0302,
0x0003, 0x0200,
0x0201, 0x0301,
0x0101, 0x0300,
0x0100, 0x0201,
0x0102, 0x0001
},
{
0x0000, 0x0202,
0x0002, 0x0200
}
},
};

static const unsigned char de_zig_zag_table[64] =
{ // de_zig_zag_table[zig_zag_order] = 8x8_array_order
  0,  1,  8, 16,  9,  2,  3, 10,
 17, 24, 32, 25, 18, 11,  4,  5,
 12, 19, 26, 33, 40, 48, 41, 34,
 27, 20, 13,  6,  7, 14, 21, 28,
 35, 42, 49, 56, 57, 50, 43, 36,
 29, 22, 15, 23, 30, 37, 44, 51,
 58, 59, 52, 45, 38, 31, 39, 46,
 53, 60, 61, 54, 47, 55, 62, 63
};


void *g_imgdecsnd_pHWRegBase = NULL;
IMG_DEC_SND_REG *g_pImgDecSndReg = NULL;
JR_REG_INFO g_imgdecsnd_jr_reg_info;

wait_queue_head_t g_imgdecsnd_MJmode_interrupt_wait; // MJmode uses interrupt to replace polling
unsigned char g_imgdecsnd_MJmode_interrupt_done; // MJmode uses interrupt to replace polling

/**************************************************************************
 *             F U N C T I O N    I M P L E M E N T A T I O N S           *
 **************************************************************************/

void IMGDECSND_jr_set_port(
	UINT8 addr, 
	UINT32 data)
{
	g_pImgDecSndReg->huf.jr_reg_data = data & 0xffff;
	g_pImgDecSndReg->huf.jr_reg_num = (UINT32)addr;
	g_pImgDecSndReg->huf.jr_reg_num = g_pImgDecSndReg->huf.jr_reg_num | 0x8000;
	g_pImgDecSndReg->huf.jr_reg_num = 0;
}

UINT16 IMGDECSND_jr_get_port(
	UINT8 addr)
{
	UINT16 value;
	g_pImgDecSndReg->huf.jr_reg_num = (UINT32)addr;
	g_pImgDecSndReg->huf.jr_reg_num = g_pImgDecSndReg->huf.jr_reg_num | 0x4000;
	value = (UINT16)(g_pImgDecSndReg->huf.jr_reg_data & 0xffff);
	g_pImgDecSndReg->huf.jr_reg_num = 0;
	return value;
}

void IMGDECSND_jr_load_coef_table(
	UINT32 downsample_rate_w, 
	UINT32 downsample_rate_h)
{
	// horizontal
	if(downsample_rate_w > 1024)
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[0];
	}
	else if(downsample_rate_w == 1024)
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[1];
	}
	else if((downsample_rate_w < 1024) && (downsample_rate_w >= 960))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[2];
	}
	else if((downsample_rate_w < 960) && (downsample_rate_w >= 900))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[3];
	}
	else if((downsample_rate_w < 900) && (downsample_rate_w >= 850))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[4];
	}
	else if((downsample_rate_w < 850) && (downsample_rate_w >= 819))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[5];
	}
	else if((downsample_rate_w < 819) && (downsample_rate_w >= 768))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[6];
	}
	else if((downsample_rate_w < 768) && (downsample_rate_w >= 725))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[7];
	}
	else if((downsample_rate_w < 725) && (downsample_rate_w >= 640))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[8];
	}
	else if((downsample_rate_w < 640) && (downsample_rate_w >= 585))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[9];
	}
	else if((downsample_rate_w < 585) && (downsample_rate_w >= 511))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[10];
	}
	else if((downsample_rate_w < 511) && (downsample_rate_w >= 455))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[11];
	}
	else if((downsample_rate_w < 455) && (downsample_rate_w >= 372))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[12];
	}
	else if((downsample_rate_w < 372) && (downsample_rate_w >= 315))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[13];
	}
	else if((downsample_rate_w < 315) && (downsample_rate_w >= 255))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[14];
	}
	else if((downsample_rate_w < 255) && (downsample_rate_w >= 195))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[15];
	}
	else if((downsample_rate_w < 195) && (downsample_rate_w >= 128))
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[16];
	}
	else
	{
		g_imgdecsnd_jr_reg_info.h_coef = &g_imgdecsnd_jr_coef_tbl[17];
	}

	// vertical
	if(downsample_rate_h > 1024)
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[0];
	}
	else if(downsample_rate_h == 1024)
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[1];
	}
	else if((downsample_rate_h < 1024) && (downsample_rate_h >= 960))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[2];
	}
	else if((downsample_rate_h < 960) && (downsample_rate_h >= 900))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[3];
	}
	else if((downsample_rate_h < 900) && (downsample_rate_h >= 850))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[4];
	}
	else if((downsample_rate_h < 850) && (downsample_rate_h >= 819))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[5];
	}
	else if((downsample_rate_h < 819) && (downsample_rate_h >= 768))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[6];
	}
	else if((downsample_rate_h < 768) && (downsample_rate_h >= 725))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[7];
	}
	else if((downsample_rate_h < 725) && (downsample_rate_h >= 640))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[8];
	}
	else if((downsample_rate_h < 640) && (downsample_rate_h >= 585))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[9];
	}
	else if((downsample_rate_h < 585) && (downsample_rate_h >= 511))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[10];
	}
	else if((downsample_rate_h < 511) && (downsample_rate_h >= 455))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[11];
	}
	else if((downsample_rate_h < 455) && (downsample_rate_h >= 372))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[12];
	}
	else if((downsample_rate_h < 372) && (downsample_rate_h >= 315))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[13];
	}
	else if((downsample_rate_h < 315) && (downsample_rate_h >= 255))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[14];
	}
	else if((downsample_rate_h < 255) && (downsample_rate_h >= 195))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[15];
	}
	else if((downsample_rate_h < 195) && (downsample_rate_h >= 128))
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[16];
	}
	else
	{
		g_imgdecsnd_jr_reg_info.v_coef = &g_imgdecsnd_jr_coef_tbl[17];
	}
}

void IMGDECSND_jr_pre_setup(
	JPEG_INFO *jpeg_info_data, 
	DECODE_INFO *decode_info_data, 
	UINT32 working_buf_phyaddr)
{
	UINT8 output_run_time;
	UINT8 src_type = 0;
	UINT32 outbuf_phyaddr = decode_info_data->outbuf_phyaddr;
	UINT32 outUVbuf_phyaddr = decode_info_data->outUVbuf_phyaddr;
	UINT32 ratio, w_align, h_align;
	UINT32 downsample_rate_w = 0, downsample_rate_h = 0;
	UINT32 h_output_len, v_output_len;
	UINT32 hw_info_pre_scaler;

	output_run_time = jpeg_info_data->jpegDecode.output_run_time;

	g_imgdecsnd_jr_reg_info.temp_buf_onLuma = 1;
	g_imgdecsnd_jr_reg_info.rgb_output_onChroma = 0;
	g_imgdecsnd_jr_reg_info.odd_symm_filter = 0;
	g_imgdecsnd_jr_reg_info.rgb_input_onChroma = 0;
	g_imgdecsnd_jr_reg_info.bw_file = 0;
	g_imgdecsnd_jr_reg_info.yuv2RGB_flag = 0;

	if((output_run_time & 0x0f) > 1) // bit[3:0] = total run times
		g_imgdecsnd_jr_reg_info.pd_enable = 1;
	else
		g_imgdecsnd_jr_reg_info.pd_enable = 0;

	if((output_run_time & 0x0f) > 1) // bit[3:0] = total run times
	{
		switch((output_run_time & 0xf0))
		{ // bit[7] = vertical run 2 times, bit[6] = horizontal run 2 times, bit[5:4] = current run time
			case 0x40: // 0100-0000
				// outbuf_phyaddr += 0;
				// outUVbuf_phyaddr += 0;
				break;
			case 0x50: // 0101-0000
				if((decode_info_data->render_type & 0xf) == RENDER_YUV420) // 0xf for orientation
				{
					outbuf_phyaddr += 4096;
					outUVbuf_phyaddr += 4096;
				}
				else if((decode_info_data->render_type & 0xf) == RENDER_RGB565) // 0xf for orientation
					outbuf_phyaddr += 4096 * 2;
				else // RENDER_RGBA8888 and RENDER_ARGB8888
					outbuf_phyaddr += 4096 * 4;
				break;
			case 0x80: // 1000-0000
				// outbuf_phyaddr += 0;
				// outUVbuf_phyaddr += 0;
				break;
			case 0x90: // 1001-0000
				if((decode_info_data->render_type & 0xf) == RENDER_YUV420) // 0xf for orientation
				{
					outbuf_phyaddr += 4096 * decode_info_data->outbuf_width;
					outUVbuf_phyaddr += 4096 * decode_info_data->outbuf_width / 2;
				}
				else if((decode_info_data->render_type & 0xf) == RENDER_RGB565) // 0xf for orientation
					outbuf_phyaddr += 4096 * decode_info_data->outbuf_width;
				else // RENDER_RGBA8888 and RENDER_ARGB8888
					outbuf_phyaddr += 4096 * decode_info_data->outbuf_width;
				break;
			case 0xc0: // 1100-0000
				// outbuf_phyaddr += 0;
				// outUVbuf_phyaddr += 0;
				break;
			case 0xd0: // 1101-0000
				if((decode_info_data->render_type & 0xf) == RENDER_YUV420) // 0xf for orientation
				{
					outbuf_phyaddr += 4096;
					outUVbuf_phyaddr += 4096;
				}
				else if((decode_info_data->render_type & 0xf) == RENDER_RGB565) // 0xf for orientation
					outbuf_phyaddr += 4096 * 2;
				else // RENDER_RGBA8888 and RENDER_ARGB8888
					outbuf_phyaddr += 4096 * 4;
				break;
			case 0xe0: // 1110-0000
				if((decode_info_data->render_type & 0xf) == RENDER_YUV420) // 0xf for orientation
				{
					outbuf_phyaddr += 4096 * decode_info_data->outbuf_width;
					outUVbuf_phyaddr += 4096 * decode_info_data->outbuf_width / 2;
				}
				else if((decode_info_data->render_type & 0xf) == RENDER_RGB565) // 0xf for orientation
					outbuf_phyaddr += 4096 * decode_info_data->outbuf_width;
				else // RENDER_RGBA8888 and RENDER_ARGB8888
					outbuf_phyaddr += 4096 * decode_info_data->outbuf_width;
				break;
			case 0xf0: // 1111-0000
				if((decode_info_data->render_type & 0xf) == RENDER_YUV420) // 0xf for orientation
				{
					outbuf_phyaddr += 4096 * decode_info_data->outbuf_width + 4096;
					outUVbuf_phyaddr += 4096 * decode_info_data->outbuf_width / 2 + 4096;
				}
				else if((decode_info_data->render_type & 0xf) == RENDER_RGB565) // 0xf for orientation
					outbuf_phyaddr += 4096 * decode_info_data->outbuf_width + (4096 * 2);
				else // RENDER_RGBA8888 and RENDER_ARGB8888
					outbuf_phyaddr += (4096 * decode_info_data->outbuf_width) + (4096 * 4);
				break;
			default:
				// outbuf_phyaddr += 0;
				// outUVbuf_phyaddr += 0;
				break;
		}
	}

	g_imgdecsnd_jr_reg_info.luma_start = outbuf_phyaddr;
	g_imgdecsnd_jr_reg_info.chroma_start = outUVbuf_phyaddr;
	g_imgdecsnd_jr_reg_info.temp_buffer = working_buf_phyaddr;

	g_imgdecsnd_jr_reg_info.render_input_offset = JR_DMA_BUF_BASE;
	g_imgdecsnd_jr_reg_info.orientation = ((decode_info_data->render_type & 0xe000) >> 13); // for orthogonal orientation (EXIF) // 0xe000 for orientation
	g_imgdecsnd_jr_reg_info.rgb_swap = decode_info_data->render_endian; // 0(Big Endian) or 1(Little Endian) only for RGB domain
	g_imgdecsnd_jr_reg_info.alpha_part = 0xff; // 0~0xff for RGBA/ARGB
	g_imgdecsnd_jr_reg_info.frame_width  = decode_info_data->outbuf_width;
	g_imgdecsnd_jr_reg_info.frame_height = decode_info_data->outbuf_height;
	if((decode_info_data->render_type & 0xf) == RENDER_YUV420) // 0xf for orientation
		g_imgdecsnd_jr_reg_info.dis_color_mode = DIS_YUV420;
	else if((decode_info_data->render_type & 0xf) == RENDER_RGB565) // 0xf for orientation
		g_imgdecsnd_jr_reg_info.dis_color_mode = DIS_RGB565;
	else if((decode_info_data->render_type & 0xf) == RENDER_RGBA8888) // 0xf for orientation
		g_imgdecsnd_jr_reg_info.dis_color_mode = DIS_RGBA8888;
	else if((decode_info_data->render_type & 0xf) == RENDER_ARGB8888) // 0xf for orientation
		g_imgdecsnd_jr_reg_info.dis_color_mode = DIS_ARGB8888;
	else
		g_imgdecsnd_jr_reg_info.dis_color_mode = DIS_RGBA8888;

	if(jpeg_info_data->jpegSOF.components == 1)
		g_imgdecsnd_jr_reg_info.bw_file = 1;

	// set source size and color mode
	ratio = jpeg_info_data->jpegSOF.Hfactor[0] * jpeg_info_data->jpegSOF.Vfactor[0];
	if(ratio == 4)
	{
		src_type = SRC_YUV411;  // 411
		w_align = ((UINT32)jpeg_info_data->jpegSOF.width + 15) & 0xfffffff0;
		h_align = ((UINT32)jpeg_info_data->jpegSOF.height + 15) & 0xfffffff0;
	}
	else if(ratio == 2)
	{
		if(jpeg_info_data->jpegSOF.Hfactor[0] == 2)
		{
			src_type = SRC_YUV422_2X1;  // 422(2x1)
			w_align = ((UINT32)jpeg_info_data->jpegSOF.width + 15) & 0xfffffff0;
			h_align = ((UINT32)jpeg_info_data->jpegSOF.height + 7) & 0xfffffff8;
		}
		else
		{
			src_type = SRC_YUV422_1X2;  // 422(1x2)
			w_align = ((UINT32)jpeg_info_data->jpegSOF.width + 7) & 0xfffffff8;
			h_align = ((UINT32)jpeg_info_data->jpegSOF.height + 15) & 0xfffffff0;
		}
	}
	else
	{
		src_type = SRC_YUV444;  // 444
		w_align = ((UINT32)jpeg_info_data->jpegSOF.width + 7) & 0xfffffff8;
		h_align = ((UINT32)jpeg_info_data->jpegSOF.height + 7) & 0xfffffff8;
	}
	g_imgdecsnd_jr_reg_info.src_width  = w_align;
	g_imgdecsnd_jr_reg_info.src_height = h_align;
	g_imgdecsnd_jr_reg_info.src_color_mode = src_type;
	g_imgdecsnd_jr_reg_info.alpha_scaling = 0;

	// set pre_scaler
	if(jpeg_info_data->jpegSOF.Hfactor[1] == 2)			hw_info_pre_scaler = PSCTRL_EN & CHROMA_PS_H;
	else if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)	hw_info_pre_scaler = PSCTRL_EN & CHROMA_PS_V;
	else if(jpeg_info_data->jpegSOF.Hfactor[0] == 4)	hw_info_pre_scaler = PSCTRL_EN & LUMA_PS_H;
	else if(jpeg_info_data->jpegSOF.Vfactor[0] == 4)	hw_info_pre_scaler = PSCTRL_EN & LUMA_PS_V;
	else hw_info_pre_scaler = 0;
	g_imgdecsnd_jr_reg_info.pre_scaler = hw_info_pre_scaler;

	// set auto_fetch
	g_imgdecsnd_jr_reg_info.auto_fetch_enable = 0;

	// set down_sample_rate: 1024*1024 = 1x, 512*1024 = 0.5x, 2048*1024 = 2x (*1024 is for accuracy in scaler_index)
	// render_width and render_height must < 2^12=4096 for total 32 bits in UINT32
	if((((decode_info_data->render_type & 0xe000) >> 13) == RIGHT_ROTATE_90) | (((decode_info_data->render_type & 0xe000) >> 13) == LEFT_ROTATE_90)) // for orientation
	{
		downsample_rate_w = (decode_info_data->render_height << 20)/jpeg_info_data->jpegSOF.width;
		downsample_rate_h = (decode_info_data->render_width << 20)/jpeg_info_data->jpegSOF.height;
	}
	else
	{
		downsample_rate_w = (decode_info_data->render_width << 20)/jpeg_info_data->jpegSOF.width;
		downsample_rate_h = (decode_info_data->render_height << 20)/jpeg_info_data->jpegSOF.height;
	}

	// JR scaler only supports the scaling range from x1/1024 to x1024 (0x1<<10 ~ 0x1<<30).
	if(downsample_rate_w == 0)	downsample_rate_w = 1024;
	if(downsample_rate_w > (0x1<<30))	downsample_rate_w = (0x1<<30);
	if(downsample_rate_h == 0)		downsample_rate_h = 1024;
	if(downsample_rate_h > (0x1<<30))	downsample_rate_h = (0x1<<30);

	g_imgdecsnd_jr_reg_info.scaler_index_h = (0x1<<30)/downsample_rate_w; // 1024 = 1x, 512 = 2x, 2048 = 0.5x
	g_imgdecsnd_jr_reg_info.scaler_index_v = (0x1<<30)/downsample_rate_h; // 1024 = 1x, 512 = 2x, 2048 = 0.5x

	downsample_rate_w >>= 10; // 1 <= downsample_rate_w <= 1024*1024(=0x1<<20)
	downsample_rate_h >>= 10; // 1 <= downsample_rate_h <= 1024*1024(=0x1<<20)

	if((downsample_rate_w > 1024) || (downsample_rate_h > 1024))
		g_imgdecsnd_jr_reg_info.hd_color_space = 1;
	else
		g_imgdecsnd_jr_reg_info.hd_color_space = 0;

	if((g_imgdecsnd_jr_reg_info.scaler_index_h == 1024) && (g_imgdecsnd_jr_reg_info.scaler_index_v == 1024))
		g_imgdecsnd_jr_reg_info.bypass_enable = 1;
	else
		g_imgdecsnd_jr_reg_info.bypass_enable = 0;
	if((decode_info_data->render_type & 0xe000) >> 13) // for orientation
		g_imgdecsnd_jr_reg_info.bypass_enable = 0;

	g_imgdecsnd_jr_reg_info.disp_box_h_bound = ((decode_info_data->render_width + 0xf) & 0xfffffff0) >> 1; // (>> 1) for unit in 2 pixels
	g_imgdecsnd_jr_reg_info.disp_box_v_bound = ((decode_info_data->render_height + 0xf) & 0xfffffff0) >> 1; // (>> 1) for unit in 2 pixels

	if((((decode_info_data->render_type & 0xe000) >> 13) == RIGHT_ROTATE_90) | (((decode_info_data->render_type & 0xe000) >> 13) == LEFT_ROTATE_90)) // for orientation
	{
		h_output_len = ((1024 * (g_imgdecsnd_jr_reg_info.src_height - 1))/g_imgdecsnd_jr_reg_info.scaler_index_h) + 1;
		v_output_len = ((1024 * (g_imgdecsnd_jr_reg_info.src_width - 1))/g_imgdecsnd_jr_reg_info.scaler_index_v) + 1;
	}
	else
	{
		h_output_len = ((1024 * (g_imgdecsnd_jr_reg_info.src_width - 1))/g_imgdecsnd_jr_reg_info.scaler_index_h) + 1;
		v_output_len = ((1024 * (g_imgdecsnd_jr_reg_info.src_height - 1))/g_imgdecsnd_jr_reg_info.scaler_index_v) + 1;
	}

	if(g_imgdecsnd_jr_reg_info.scaler_index_h < 1024)
		g_imgdecsnd_jr_reg_info.h_filter_phase = (~((((g_imgdecsnd_jr_reg_info.disp_box_h_bound << 1) - h_output_len) * g_imgdecsnd_jr_reg_info.scaler_index_h)/2) + 1) & 0x7ff;
	else if(g_imgdecsnd_jr_reg_info.scaler_index_h == 1024)
		g_imgdecsnd_jr_reg_info.h_filter_phase = 1;
	else
		g_imgdecsnd_jr_reg_info.h_filter_phase = 715;

	if(g_imgdecsnd_jr_reg_info.scaler_index_v < 1024)
		g_imgdecsnd_jr_reg_info.v_filter_phase = (~((((g_imgdecsnd_jr_reg_info.disp_box_v_bound << 1) - v_output_len) * g_imgdecsnd_jr_reg_info.scaler_index_v)/2) + 1) & 0x7ff;
	else if(g_imgdecsnd_jr_reg_info.scaler_index_v == 1024)
		g_imgdecsnd_jr_reg_info.v_filter_phase = 1;
	else
		g_imgdecsnd_jr_reg_info.v_filter_phase = 711;

	// tuning JR coefficient for quality
	IMGDECSND_jr_load_coef_table(downsample_rate_w, downsample_rate_h);

}

void IMGDECSND_jr_write_h_coef(
	void)
{
    UINT32 start_maddr;
    UINT32 tab_offset;

    tab_offset = 0;
    for(start_maddr=JR_COE_H_SET_1_START; start_maddr<JR_COE_H_SET_1_END; start_maddr++)
    {
        IMGDECSND_jr_set_port(start_maddr, g_imgdecsnd_jr_reg_info.h_coef->coef_set_1[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_H_SET_2_START; start_maddr<JR_COE_H_SET_2_END; start_maddr++)
    {
        IMGDECSND_jr_set_port(start_maddr, g_imgdecsnd_jr_reg_info.h_coef->coef_set_2[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_H_SET_3_START; start_maddr<JR_COE_H_SET_3_END; start_maddr++)
    {
        IMGDECSND_jr_set_port(start_maddr, g_imgdecsnd_jr_reg_info.h_coef->coef_set_3[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_H_SET_4_START; start_maddr<JR_COE_H_SET_4_END; start_maddr++)
    {
        IMGDECSND_jr_set_port(start_maddr, g_imgdecsnd_jr_reg_info.h_coef->coef_set_4[tab_offset]);
        tab_offset ++;
    }
}


void IMGDECSND_jr_write_v_coef(
	void)
{
    UINT32 start_maddr;
    UINT32 tab_offset;

    tab_offset = 0;
    for(start_maddr=JR_COE_V_SET_1_START; start_maddr<JR_COE_V_SET_1_END; start_maddr++)
    {
        IMGDECSND_jr_set_port(start_maddr, g_imgdecsnd_jr_reg_info.v_coef->coef_set_1[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_V_SET_2_START; start_maddr<JR_COE_V_SET_2_END; start_maddr++)
    {
        IMGDECSND_jr_set_port(start_maddr, g_imgdecsnd_jr_reg_info.v_coef->coef_set_2[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_V_SET_3_START; start_maddr<JR_COE_V_SET_3_END; start_maddr++)
    {
        IMGDECSND_jr_set_port(start_maddr, g_imgdecsnd_jr_reg_info.v_coef->coef_set_3[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_V_SET_4_START; start_maddr<JR_COE_V_SET_4_END; start_maddr++)
    {
        IMGDECSND_jr_set_port(start_maddr, g_imgdecsnd_jr_reg_info.v_coef->coef_set_4[tab_offset]);
        tab_offset ++;
    }
}

void IMGDECSND_jr_write_coef_table( // tuning JR coefficient for quality
	void)
{
	IMGDECSND_jr_set_port(JR_WRITE_COE_CHROMA, 0x0000); // JR_WRITE_COE_CHROMA = 0: Write luma filter coefficients
	IMGDECSND_jr_write_v_coef();
	IMGDECSND_jr_write_h_coef();

	IMGDECSND_jr_set_port(JR_WRITE_COE_CHROMA, 0x0001); // JR_WRITE_COE_CHROMA = 1: Write chroma filter coefficients
	IMGDECSND_jr_write_v_coef();
	IMGDECSND_jr_write_h_coef();

	IMGDECSND_jr_set_port(JR_WRITE_COE_CHROMA, 0x0000);
}

void IMGDECSND_DMA_load(
	UINT32 AddrMode, 
	UINT32 VAddr, 
	UINT32 HAddr, 
	UINT32 DAddr, 
	UINT32 Len)
{
	g_pImgDecSndReg->jdm.jdm_dma1_mode = (Len << 8) | (AddrMode & 0x0f);
	g_pImgDecSndReg->jdm.jdm_dma1_start_addr = (DAddr >> 2) & 0x7ff;
	g_pImgDecSndReg->jdm.jdm_dma1_xaddr = ((VAddr & 0xffff0000) >> 6) | HAddr;
	g_pImgDecSndReg->jdm.jdm_dma1_yaddr = VAddr & 0xffff;
}

void IMGDECSND_DMA_save(
	UINT32 AddrMode, 
	UINT32 VAddr, 
	UINT32 HAddr, 
	UINT32 DAddr, 
	UINT32 Len)
{
	g_pImgDecSndReg->jdm.jdm_dma1_mode = (Len << 8) | (1 << 5) | (AddrMode & 0x0f);
	g_pImgDecSndReg->jdm.jdm_dma1_start_addr = (DAddr >> 2) & 0x7ff;
	g_pImgDecSndReg->jdm.jdm_dma1_xaddr = ((VAddr & 0xffff0000) >> 6) | HAddr;
	g_pImgDecSndReg->jdm.jdm_dma1_yaddr = VAddr & 0xffff;
}

void IMGDECSND_DMA_clear(
	UINT32 AddrMode, 
	UINT32 DAddr, 
	UINT32 Len)
{
	g_pImgDecSndReg->jdm.jdm_dma1_mode = (Len << 8) | (AddrMode & 0x0f);
	g_pImgDecSndReg->jdm.jdm_dma1_start_addr = (DAddr >> 2) & 0x7ff;
	g_pImgDecSndReg->jdm.jdm_dma1_xaddr = 0;
	g_pImgDecSndReg->jdm.jdm_dma1_yaddr = 0;
}

void IMGDECSND_DMA_wait(
	void)
{
	UINT32 jdm_dma1_done;
	UINT32 time_out = 1000; // 1000 us = 1 ms

	do
	{
		if(time_out == 0)
		{
			WARNING("DMA wait: time out!");
			break;
		}

		udelay(1);

		jdm_dma1_done = g_pImgDecSndReg->jdm.jdm_dma1_done;
		//DEBUG("jdm_dma1_done = 0x%08x\n", jdm_dma1_done);

		time_out--;
	}while(!(jdm_dma1_done & 0x1));
}

void IMGDECSND_JPEG_fill_DQT_reg(
	JPEG_INFO *jpeg_info_data)
{
	UINT32 i, j;
	UINT32 base, addr;

	for(i=0; i<jpeg_info_data->jpegDQT.count; i++)
	{
		base = (UINT32)jpeg_info_data->jpegDQT.Tq[i] << 6;
		for(j=0; j<64; j++)
		{
			addr = base + (UINT32)de_zig_zag_table[j];
			if(jpeg_info_data->jpegDQT.Pq[i] == 0) // for 8bit DQT
				g_pImgDecSndReg->invq.jpeg_invq_qmx_par = (addr << 8) | (UINT32)jpeg_info_data->jpegDQT.Qs[i][j];
			else if(jpeg_info_data->jpegDQT.Pq[i]  == 1) // for 16bit DQT, remove high byte
				g_pImgDecSndReg->invq.jpeg_invq_qmx_par = (addr << 8) | (UINT32)(jpeg_info_data->jpegDQT.Qs[i][j] & 0x00ff);
		}
	}
}

void IMGDECSND_JPEG_fill_DHT_reg(
	JPEG_INFO *jpeg_info_data, 
	UINT8 table_id, 
	DHT_TYPE DHT_type)
{
	UINT32 i;
	UINT32 base_address;
	UINT32 DMA_address;

	if(table_id > 3)
	{
		WARNING("table_id > 3: table_id = %d\n", table_id);
		return;
	}

	switch(DHT_type)
	{
		case DHT_DC0:
			base_address = 0x0;
			// min codeword
			for(i=0; i<24; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_mincode[table_id][i];
				base_address += 0x100;
			}
			// offset
			for(i=0; i<8; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_offset[table_id][i];
				base_address += 0x100;
			}
			// value
			for(i=0; i<8; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_value[table_id][i];
				base_address += 0x100;
			}
			break;
		case DHT_DC1:
			base_address = 0x30 << 8;
			// min codeword
			for(i=0; i<24; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_mincode[table_id][i];
				base_address += 0x100;
			}
			// offset
			for(i=0; i<8; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_offset[table_id][i];
				base_address += 0x100;
			}
			// value
			for(i=0; i<8; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_value[table_id][i];
				base_address += 0x100;
			}
			break;
		case DHT_AC0:
			base_address = 0x80 << 8;
			// min codeword
			for(i=0; i<24; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.AC_mincode[table_id][i];
				base_address += 0x100;
			}
			// offset
			for(i=0; i<16; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.AC_offset[table_id][i];
				base_address += 0x100;
			}
			DMA_address = 0;
			// value (write to DMA)
			for(i=0; i<jpeg_info_data->jpegDHT.huff_ac_size; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_acval_0 = DMA_address | (UINT32)jpeg_info_data->jpegDHT.AC_value[table_id][i];
				DMA_address += 0x100;
			}
			break;
		case DHT_AC1:
			base_address = 0xB0 << 8;
			// min codeword
			for(i=0; i<24; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.AC_mincode[table_id][i];
				base_address += 0x100;
			}
			// offset
			for(i=0; i<16; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.AC_offset[table_id][i];
				base_address += 0x100;
			}
			DMA_address = 0;
			// value (write to DMA)
			for(i=0; i<jpeg_info_data->jpegDHT.huff_ac_size; i++)
			{
				g_pImgDecSndReg->huf.jpeg_huff_wr_acval_1 = DMA_address | (UINT32)jpeg_info_data->jpegDHT.AC_value[table_id][i];
				DMA_address += 0x100;
			}
			break;
		default:
			break;
	}
}

void IMGDECSND_JPEG_graph_idct_8x8(
	JPEG_INFO *jpeg_info_data, 
	UINT32 component)
{
	UINT32 jdm_mode;
	UINT32 time_out = 1000; // 1000 us = 1 ms

	g_pImgDecSndReg->invq.jpeg_invq_mode = ((UINT32)jpeg_info_data->jpegSOF.Qtable[component] << 2) | INVQ_JPEG_MODE | INVQ_JPEG_NOBYPASS;

	// start to decode
	if(jpeg_info_data->jpegSOF.sof_n == 2) // progressive JPEG
	{
		g_pImgDecSndReg->jdm.jdm_mode = GRAPH_JPEG | GRAPH_JPEG_SAT | GRAPH_JPEG_OUT_8;
	}
	else // baseline JPEG
	{
		g_pImgDecSndReg->jdm.jdm_mode = GRAPH_JPEG | GRAPH_JPEG_SAT | GRAPH_JPEG_OUT_8 | GRAPH_JPEG_32;
	}

	// wait until the previous block is completely decoded.
	do
	{
		if(time_out == 0)
		{
			WARNING("DMA wait: time out!");
			break;
		}

		udelay(1);

		jdm_mode = g_pImgDecSndReg->jdm.jdm_mode;
		//DEBUG("jdm_mode = 0x%08x\n", jdm_mode);

		time_out--;
	}while(jdm_mode & (1<<4));

	g_pImgDecSndReg->invq.jpeg_invq_mode = 0;
}//graph_idct_8x8

void IMGDECSND_JPEG_scaler_1_mcu(
	INT32 mcu_x, 
	INT32 mcu_y)
{
	//DEBUG("mcu_x = %d, mcu_y = %d\n", mcu_x, mcu_y);

	IMGDECSND_jr_set_port(JR_MCU_X, mcu_x);
	IMGDECSND_jr_set_port(JR_MCU_Y, mcu_y);

	g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control & 0xffef;
	g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control | 0x10;
	g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control & 0xffef;
}

//---------------------------------------------------------------

void imgdecsnd_clock_enable(void)
{
	moonReg_t *pMoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	pMoonReg->reset[1] &= ~(MOON_SND_IMG_MISC_RESET & 
							MOON_SND_HUFF_RESET & 
							MOON_SND_JPG_IQT_RESET & 
							MOON_SND_JR_RESET);

	pMoonReg->clken[1] |= (MOON_SND_IMG_MISC_CLKEN & 
							MOON_SND_HUFF_CLKEN & 
							MOON_SND_JPG_IQT_CLKEN & 
							MOON_SND_JR_CLKEN);

	pMoonReg->gclken[1] &= ~(MOON_SND_IMG_MISC_GCLKEN & 
							MOON_SND_HUFF_GCLKEN & 
							MOON_SND_JPG_IQT_GCLKEN & 
							MOON_SND_JR_GCLKEN);

	//DEBUG("imgdecsnd_clock_enable: pMoonReg->reset[1] = 0x%08x, pMoonReg->clken[1] = 0x%08x, pMoonReg->gclken[1] = 0x%08x\n", 
	//														pMoonReg->reset[1], pMoonReg->clken[1], pMoonReg->gclken[1]);
}	
EXPORT_SYMBOL(imgdecsnd_clock_enable);

void imgdecsnd_clock_disable(void)
{
}	
EXPORT_SYMBOL(imgdecsnd_clock_disable);

int imgdecsnd_hal_reg_mmap(
	void)
{
#if 0
	request_mem_region(RGST_BASE, RGST_SIZE, "Register_Map");
	g_imgdecsnd_pHWRegBase = ioremap(RGST_BASE, RGST_SIZE);
	if(g_imgdecsnd_pHWRegBase == NULL)
	{
		ERR("ioremap for HW register failed\n");
		return -1;
	}
	g_pImgDecSndReg = (IMG_DEC_SND_REG *)(g_imgdecsnd_pHWRegBase + RGST_IMG_DEC_SND_HUF_OFFSET);
#else // Hardware register memory address by hardware.h
	g_pImgDecSndReg = (IMG_DEC_SND_REG *)LOGI_ADDR_IMG_DEC_SND_HUF_REG;
#endif

	return 0;
}
EXPORT_SYMBOL(imgdecsnd_hal_reg_mmap);

void imgdecsnd_hal_reg_unmmap(
	void)
{
#if 0
	iounmap(g_imgdecsnd_pHWRegBase);
	g_imgdecsnd_pHWRegBase = NULL;
	g_pImgDecSndReg = NULL;
#else // Hardware register memory address by hardware.h
	g_pImgDecSndReg = NULL;
#endif
}
EXPORT_SYMBOL(imgdecsnd_hal_reg_unmmap);

void imgdecsnd_hal_print_all_registers( // only for test!!! (waiting to be removed as release)
	void)
{
	printk("\n");
	printk("g_pImgDecSndReg->huf.jpeg_huff_mode = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_mode);
	printk("g_pImgDecSndReg->huf.jpeg_huff_ctrl = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_ctrl);
	printk("g_pImgDecSndReg->huf.jpeg_huff_status = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_status);
	printk("g_pImgDecSndReg->huf.jpeg_huff_scan_ctrl = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_scan_ctrl);
	printk("g_pImgDecSndReg->huf.jpeg_huff_table = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_table);
	printk("g_pImgDecSndReg->huf.jpeg_huff_mcuno = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_mcuno);
	printk("g_pImgDecSndReg->huf.jpeg_huff_jpeghsize = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_jpeghsize);
	printk("g_pImgDecSndReg->huf.jpeg_huff_jpegvsize = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_jpegvsize);
	printk("g_pImgDecSndReg->huf.jpeg_huff_bitstream = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_bitstream);
	printk("g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset);
	printk("g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info);
	printk("g_pImgDecSndReg->huf.jpeg_huff_bits_info = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_bits_info);
	printk("g_pImgDecSndReg->huf.jpeg_huff_rd_huff_info = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_rd_huff_info);
	printk("g_pImgDecSndReg->huf.jpeg_huff_dma_offset_0 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_dma_offset_0);
	printk("g_pImgDecSndReg->huf.jpeg_huff_dma_offset_1 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_dma_offset_1);
	printk("g_pImgDecSndReg->huf.jpeg_huff_acval_offset_0 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_acval_offset_0);
	printk("g_pImgDecSndReg->huf.jpeg_huff_acval_offset_1 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_acval_offset_1);
	printk("g_pImgDecSndReg->huf.jpeg_huff_acval_offset_2 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_acval_offset_2);
	printk("g_pImgDecSndReg->huf.jpeg_huff_acval_offset_3 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_acval_offset_3);
	printk("g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_wr_dc_01);
	printk("g_pImgDecSndReg->huf.jpeg_huff_wr_dc_23 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_wr_dc_23);
	printk("g_pImgDecSndReg->huf.jpeg_huff_wr_acval_0 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_wr_acval_0);
	printk("g_pImgDecSndReg->huf.jpeg_huff_wr_acval_1 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_wr_acval_1);
	printk("g_pImgDecSndReg->huf.jpeg_huff_wr_acval_2 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_wr_acval_2);
	printk("g_pImgDecSndReg->huf.jpeg_huff_wr_acval_3 = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_wr_acval_3);
	printk("g_pImgDecSndReg->huf.jpeg_huff_rd_addr = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_rd_addr);
	printk("g_pImgDecSndReg->huf.jpeg_huff_rd_val = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_rd_val);
	printk("g_pImgDecSndReg->huf.jpeg_huff_probe = 0x%08x\n", g_pImgDecSndReg->huf.jpeg_huff_probe);
	printk("g_pImgDecSndReg->huf.jr_control = 0x%08x\n", g_pImgDecSndReg->huf.jr_control);
	printk("g_pImgDecSndReg->huf.jr_status = 0x%08x\n", g_pImgDecSndReg->huf.jr_status);
	printk("g_pImgDecSndReg->huf.jr_reg_num = 0x%08x\n", g_pImgDecSndReg->huf.jr_reg_num);
	printk("g_pImgDecSndReg->huf.jr_reg_data = 0x%08x\n", g_pImgDecSndReg->huf.jr_reg_data);
	printk("\n");
	printk("g_pImgDecSndReg->jdm.jdm_mode = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_mode);
	printk("g_pImgDecSndReg->jdm.jdm_status = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_status);
	printk("g_pImgDecSndReg->jdm.jdm_dma1_mode = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_dma1_mode);
	printk("g_pImgDecSndReg->jdm.jdm_dma1_xaddr = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_dma1_xaddr);
	printk("g_pImgDecSndReg->jdm.jdm_dma1_yaddr = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_dma1_yaddr);
	printk("g_pImgDecSndReg->jdm.jdm_dma1_done = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_dma1_done);
	printk("g_pImgDecSndReg->jdm.jdm_dma1_start_addr = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_dma1_start_addr);
	printk("g_pImgDecSndReg->jdm.jdm_dma1_block_num = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_dma1_block_num);
	printk("g_pImgDecSndReg->jdm.jdm_jpeg_offset = 0x%08x\n", g_pImgDecSndReg->jdm.jdm_jpeg_offset);
	printk("\n");
	printk("g_pImgDecSndReg->invq.jpeg_invq_qmx_par = 0x%08x\n", g_pImgDecSndReg->invq.jpeg_invq_qmx_par);
	printk("g_pImgDecSndReg->invq.jpeg_invq_mode = 0x%08x\n", g_pImgDecSndReg->invq.jpeg_invq_mode);
	printk("g_pImgDecSndReg->invq.jpeg_invq_chksum = 0x%08x\n", g_pImgDecSndReg->invq.jpeg_invq_chksum);
	printk("\n");
}
EXPORT_SYMBOL(imgdecsnd_hal_print_all_registers);

void imgdecsnd_hal_jr_setup(
	JPEG_INFO *jpeg_info_data, 
	DECODE_INFO *decode_info_data, 
	UINT32 working_buf_phyaddr)
{
	UINT8 output_run_time;
	UINT32 mcu_num_h, mcu_num_v;
	UINT32 mcu_num_4096_h, mcu_num_4096_v;

	output_run_time = jpeg_info_data->jpegDecode.output_run_time;

	IMGDECSND_jr_pre_setup(jpeg_info_data, decode_info_data, working_buf_phyaddr);

	//enable control commond
	g_pImgDecSndReg->huf.jr_control = 1;

	IMGDECSND_jr_write_coef_table();

	//Output buffer address
	IMGDECSND_jr_set_port(JR_LUMA_BASE, (g_imgdecsnd_jr_reg_info.luma_start >> 2) & 0x0000ffff); // 4-byte unit
	IMGDECSND_jr_set_port(JR_BASE_EXTEND0, (g_imgdecsnd_jr_reg_info.luma_start >> 18) & 0x00003fff);
	IMGDECSND_jr_set_port(JR_CROMA_BASE, (g_imgdecsnd_jr_reg_info.chroma_start >> 2) & 0x0000ffff); // 4-byte unit
	IMGDECSND_jr_set_port(JR_BASE_EXTEND1, (g_imgdecsnd_jr_reg_info.chroma_start >> 18) & 0x00003fff);

	//Temp buffer of JR coefficient table setting: 4-byte unit & 8-byte_align
	IMGDECSND_jr_set_port(JR_TEMP_BUF_BASE, ((g_imgdecsnd_jr_reg_info.temp_buffer + 4) >> 2) & 0x0000fffe);
	IMGDECSND_jr_set_port(JR_BASE_EXTEND2, ((g_imgdecsnd_jr_reg_info.temp_buffer + 4) >> 18) & 0x00003fff);

	// DMA address (Debug progressive JPEG output error, after MJ-baseline JPEG decoding.)
	IMGDECSND_jr_set_port(JR_DMA_CP_BASE, g_imgdecsnd_jr_reg_info.render_input_offset); // [10:0]: Indicates which ping-pong DMA buffer for JR using.

	// set source/display color mode
	IMGDECSND_jr_set_port(JR_SRC_COLOR_MODE, g_imgdecsnd_jr_reg_info.src_color_mode);
	IMGDECSND_jr_set_port(JR_DISP_COLOR_MODE, (g_imgdecsnd_jr_reg_info.dis_color_mode & 0x0f)
									|(g_imgdecsnd_jr_reg_info.alpha_part << 4)
									|(g_imgdecsnd_jr_reg_info.rgb_swap << 12)
									|(g_imgdecsnd_jr_reg_info.orientation << 13));

	// set size of source picture
	IMGDECSND_jr_set_port(JR_SRC_WIDTH, g_imgdecsnd_jr_reg_info.src_width);
	IMGDECSND_jr_set_port(JR_SRC_HEIGHT, g_imgdecsnd_jr_reg_info.src_height);

	//set filter initial phase
	IMGDECSND_jr_set_port(JR_V_PHASE_OFFSET, g_imgdecsnd_jr_reg_info.v_filter_phase);
	IMGDECSND_jr_set_port(JR_H_PHASE_OFFSET, g_imgdecsnd_jr_reg_info.h_filter_phase);

	//scaler index
	IMGDECSND_jr_set_port(JR_SCALER_INDEX_V, (g_imgdecsnd_jr_reg_info.scaler_index_v & 0xffff));
	IMGDECSND_jr_set_port(JR_SCALER_INDEX_H, (g_imgdecsnd_jr_reg_info.scaler_index_h & 0xffff));
	IMGDECSND_jr_set_port(JR_SCALER_INDEX_EXT, ((g_imgdecsnd_jr_reg_info.scaler_index_h >> 16) & 0xf)|(((g_imgdecsnd_jr_reg_info.scaler_index_v >> 16) & 0xf) << 8));

	// set frame buffer size
	IMGDECSND_jr_set_port(JR_FRAME_WIDTH, g_imgdecsnd_jr_reg_info.frame_width & 0xffff);
	IMGDECSND_jr_set_port(JR_FRAME_HEIGHT, g_imgdecsnd_jr_reg_info.frame_height & 0xffff);

	// offset from display point to original point
	IMGDECSND_jr_set_port(JR_RENDER_V_INIT, 0); // [9:0] unit in 2 pixels
	IMGDECSND_jr_set_port(JR_RENDER_H_INIT, 0); // [9:0] unit in 2 pixels

	// set size of display pic
	IMGDECSND_jr_set_port(JR_RENDER_H_BOUND, g_imgdecsnd_jr_reg_info.disp_box_h_bound & 0x0fff); // [11:0] unit in 2 pixels
	IMGDECSND_jr_set_port(JR_RENDER_V_BOUND, g_imgdecsnd_jr_reg_info.disp_box_v_bound & 0x0fff); // [11:0] unit in 2 pixels

	//Pre-scaling asetting
	IMGDECSND_jr_set_port(JR_SRCPP_CTRL, g_imgdecsnd_jr_reg_info.pre_scaler & 0x001f);

	//JR_MISC[0] temp buffer load in luma DRAM
	//JR_MISC[1] Enable drop mode
	//JR_MISC[2] RGB output in chroma DRAM
	//JR_MISC[3] Enable odd-symmetric filter coefficient
	//JR_MISC[4] HD-color space
	//JR_MISC[5] RGB input in Chroma DRAM
	//JR_MISC[6] Bypass mode
	//JR_MISC[7] BW file(luma only file)
	//JR_MISC[9:8]: YUV color space conversion
	//                      00:BYPASS
	//                      01:JPEG_YUV ->BT.601_YUV
	//                      10:JPEG_YUV ->BT.709_YUV
	//JR_MISC[11:10]: YUV to RGB color space conversion
	//                      00:BYPASS
	//                      01:BT.601_YUV->RGB
	//                      10:BT.709_YUV->RGB
	//                      11:JPEG_YUV->RGB
	//JR_MISC[14] : alpha scaling Enable
	IMGDECSND_jr_set_port(JR_MISC, (g_imgdecsnd_jr_reg_info.temp_buf_onLuma)
						|(g_imgdecsnd_jr_reg_info.pd_enable << 1)
						|(g_imgdecsnd_jr_reg_info.rgb_output_onChroma << 2)
						|(g_imgdecsnd_jr_reg_info.odd_symm_filter << 3)
						|(g_imgdecsnd_jr_reg_info.hd_color_space << 4)
						|(g_imgdecsnd_jr_reg_info.rgb_input_onChroma << 5)
						|(g_imgdecsnd_jr_reg_info.bypass_enable << 6)
						|(g_imgdecsnd_jr_reg_info.bw_file << 7)
						|(g_imgdecsnd_jr_reg_info.yuv2RGB_flag << 10)
						|(g_imgdecsnd_jr_reg_info.alpha_scaling << 14));


	if((output_run_time & 0x0f) > 1) // bit[3:0] = total run times
	{
		// get image horizontal / vertical MCU number.
		mcu_num_h = (jpeg_info_data->jpegSOF.Hfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.width+15)>>4) : ((jpeg_info_data->jpegSOF.width+7)>>3);
		mcu_num_v = (jpeg_info_data->jpegSOF.Vfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.height+15)>>4) : ((jpeg_info_data->jpegSOF.height+7)>>3);
		mcu_num_4096_h = (jpeg_info_data->jpegSOF.Hfactor[0] == 2) ? 256 : 512; // 256 = 4096 >> 4, 512 = 4096 >> 3
		mcu_num_4096_v = (jpeg_info_data->jpegSOF.Vfactor[0] == 2) ? 256 : 512; // 256 = 4096 >> 4, 512 = 4096 >> 3

		switch(output_run_time & 0xf0)
		{ // bit[7] = vertical run 2 times, bit[6] = horizontal run 2 times, bit[5:4] = current run time
			case 0x40: // 0100-0000
				IMGDECSND_jr_set_port(JR_MCU_X_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_4096_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			case 0x50: // 0101-0000
				IMGDECSND_jr_set_port(JR_MCU_X_START, mcu_num_4096_h);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			case 0x80: // 1000-0000
				IMGDECSND_jr_set_port(JR_MCU_X_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_4096_v - 1));
				break;
			case 0x90: // 1001-0000
				IMGDECSND_jr_set_port(JR_MCU_X_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, mcu_num_4096_v);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			case 0xc0: // 1100-0000
				IMGDECSND_jr_set_port(JR_MCU_X_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_4096_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_4096_v - 1));
				break;
			case 0xd0: // 1101-0000
				IMGDECSND_jr_set_port(JR_MCU_X_START, mcu_num_4096_h);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_4096_v - 1));
				break;
			case 0xe0: // 1110-0000
				IMGDECSND_jr_set_port(JR_MCU_X_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_4096_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, mcu_num_4096_v);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			case 0xf0: // 1111-0000
				IMGDECSND_jr_set_port(JR_MCU_X_START, mcu_num_4096_h);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, mcu_num_4096_v);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			default:
				IMGDECSND_jr_set_port(JR_MCU_X_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDECSND_jr_set_port(JR_MCU_Y_START, 0);
				IMGDECSND_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
		}
	}

	//reset hw
	g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control | (1 << 5); // FSM software reset
	g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control & ~0x0020;

	if(g_imgdecsnd_jr_reg_info.auto_fetch_enable == 0)
	{
		//non-auto-fetch frame start
		g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control | (1 << 8); // Picture start
		g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control & ~0x0100;
	}
}
EXPORT_SYMBOL(imgdecsnd_hal_jr_setup);

void imgdecsnd_hal_scale_setup(
	SCALE_INFO *scale_info_data, 
	UINT32 working_buf_phyaddr)
{
	UINT32 downsample_rate_w = 0, downsample_rate_h = 0;
	UINT32 h_output_len, v_output_len;

	g_imgdecsnd_jr_reg_info.temp_buf_onLuma = 1;
	g_imgdecsnd_jr_reg_info.pd_enable = 0;
	g_imgdecsnd_jr_reg_info.rgb_output_onChroma = 0;
	g_imgdecsnd_jr_reg_info.odd_symm_filter = 0;
	g_imgdecsnd_jr_reg_info.rgb_input_onChroma = 0;
	g_imgdecsnd_jr_reg_info.bw_file = 0;
	g_imgdecsnd_jr_reg_info.yuv2RGB_flag = 0;

	g_imgdecsnd_jr_reg_info.luma_start = scale_info_data->dst_Ybufphyaddr;
	g_imgdecsnd_jr_reg_info.chroma_start = scale_info_data->dst_UVbufphyaddr;
	g_imgdecsnd_jr_reg_info.temp_buffer = working_buf_phyaddr;

	g_imgdecsnd_jr_reg_info.render_input_offset = JR_DMA_BUF_BASE;
	g_imgdecsnd_jr_reg_info.orientation = ((scale_info_data->dst_type & 0xe000) >> 13); // for orthogonal orientation (EXIF) // 0xe000 for orientation
	g_imgdecsnd_jr_reg_info.rgb_swap = scale_info_data->dst_endian; // 0(Big Endian) or 1(Little Endian) only for RGB domain
	g_imgdecsnd_jr_reg_info.alpha_part = 0xff; // 0~0xff for RGBA/ARGB

	g_imgdecsnd_jr_reg_info.frame_width  = scale_info_data->dst_bufbpl;
	g_imgdecsnd_jr_reg_info.frame_height = scale_info_data->dst_bufline;
	g_imgdecsnd_jr_reg_info.dis_color_mode = (scale_info_data->dst_type & 0xf); // 0xf for orientation

	// set source size and color mode
	g_imgdecsnd_jr_reg_info.src_width = scale_info_data->clip_width;
	g_imgdecsnd_jr_reg_info.src_height = scale_info_data->clip_height;
	g_imgdecsnd_jr_reg_info.src_color_mode = scale_info_data->src_type;

	// set pre_scaler
	g_imgdecsnd_jr_reg_info.pre_scaler = 0;

	// set auto_fetch
	g_imgdecsnd_jr_reg_info.auto_fetch_enable = 1;

	// set down_sample_rate: 1024*1024 = 1x, 512*1024 = 0.5x, 2048*1024 = 2x (*1024 is for accuracy in scaler_index)
	// render_width and render_height must < 2^12=4096 for total 32 bits in UINT32
	if((((scale_info_data->dst_type & 0xe000) >> 13) == RIGHT_ROTATE_90) | (((scale_info_data->dst_type & 0xe000) >> 13) == LEFT_ROTATE_90)) // for orientation
	{
		downsample_rate_w = (scale_info_data->rect_height << 20)/scale_info_data->clip_width;
		downsample_rate_h = (scale_info_data->rect_width << 20)/scale_info_data->clip_height;
	}
	else
	{
		downsample_rate_w = (scale_info_data->rect_width << 20)/scale_info_data->clip_width;
		downsample_rate_h = (scale_info_data->rect_height << 20)/scale_info_data->clip_height;
	}

	// JR scaler only supports the scaling range from x1/1024 to x1024 (0x1<<10 ~ 0x1<<30).
	if(downsample_rate_w == 0)	downsample_rate_w = 1024;
	if(downsample_rate_w > (0x1<<30))	downsample_rate_w = (0x1<<30);
	if(downsample_rate_h == 0)		downsample_rate_h = 1024;
	if(downsample_rate_h > (0x1<<30))	downsample_rate_h = (0x1<<30);

	g_imgdecsnd_jr_reg_info.scaler_index_h = (0x1<<30)/downsample_rate_w; // 1024 = 1x, 512 = 2x, 2048 = 0.5x
	g_imgdecsnd_jr_reg_info.scaler_index_v = (0x1<<30)/downsample_rate_h; // 1024 = 1x, 512 = 2x, 2048 = 0.5x

	downsample_rate_w >>= 10; // 1 <= downsample_rate_w <= 1024*1024(=0x1<<20)
	downsample_rate_h >>= 10; // 1 <= downsample_rate_h <= 1024*1024(=0x1<<20)

	if((downsample_rate_w > 1024) || (downsample_rate_h > 1024))
		g_imgdecsnd_jr_reg_info.hd_color_space = 1;
	else
		g_imgdecsnd_jr_reg_info.hd_color_space = 0;

	if((g_imgdecsnd_jr_reg_info.scaler_index_h == 1024) && (g_imgdecsnd_jr_reg_info.scaler_index_v == 1024))
		g_imgdecsnd_jr_reg_info.bypass_enable = 1;
	else
		g_imgdecsnd_jr_reg_info.bypass_enable = 0;
	if((scale_info_data->dst_type & 0xe000) >> 13) // for orientation
		g_imgdecsnd_jr_reg_info.bypass_enable = 0;

	g_imgdecsnd_jr_reg_info.disp_box_h_bound = ((scale_info_data->rect_width + 0x1) & ~0x1) >> 1; // (>> 1) for unit in 2 pixels
	g_imgdecsnd_jr_reg_info.disp_box_v_bound = ((scale_info_data->rect_height + 0x1) & ~0x1) >> 1; // (>> 1) for unit in 2 pixels

	if((((scale_info_data->dst_type & 0xe000) >> 13) == RIGHT_ROTATE_90) | (((scale_info_data->dst_type & 0xe000) >> 13) == LEFT_ROTATE_90)) // for orientation
	{
		h_output_len = ((1024 * (g_imgdecsnd_jr_reg_info.src_height - 1))/g_imgdecsnd_jr_reg_info.scaler_index_h) + 1;
		v_output_len = ((1024 * (g_imgdecsnd_jr_reg_info.src_width - 1))/g_imgdecsnd_jr_reg_info.scaler_index_v) + 1;
	}
	else
	{
		h_output_len = ((1024 * (g_imgdecsnd_jr_reg_info.src_width - 1))/g_imgdecsnd_jr_reg_info.scaler_index_h) + 1;
		v_output_len = ((1024 * (g_imgdecsnd_jr_reg_info.src_height - 1))/g_imgdecsnd_jr_reg_info.scaler_index_v) + 1;
	}

	if(g_imgdecsnd_jr_reg_info.scaler_index_h < 1024)
		g_imgdecsnd_jr_reg_info.h_filter_phase = (~((((g_imgdecsnd_jr_reg_info.disp_box_h_bound << 1) - h_output_len) * g_imgdecsnd_jr_reg_info.scaler_index_h)/2) + 1) & 0x7ff;
	else if(g_imgdecsnd_jr_reg_info.scaler_index_h == 1024)
		g_imgdecsnd_jr_reg_info.h_filter_phase = 1;
	else
		g_imgdecsnd_jr_reg_info.h_filter_phase = 715;

	if(g_imgdecsnd_jr_reg_info.scaler_index_v < 1024)
		g_imgdecsnd_jr_reg_info.v_filter_phase = (~((((g_imgdecsnd_jr_reg_info.disp_box_v_bound << 1) - v_output_len) * g_imgdecsnd_jr_reg_info.scaler_index_v)/2) + 1) & 0x7ff;
	else if(g_imgdecsnd_jr_reg_info.scaler_index_v == 1024)
		g_imgdecsnd_jr_reg_info.v_filter_phase = 1;
	else
		g_imgdecsnd_jr_reg_info.v_filter_phase = 711;

	// tuning JR coefficient for quality
	IMGDECSND_jr_load_coef_table(downsample_rate_w, downsample_rate_h);

	//enable control commond
	g_pImgDecSndReg->huf.jr_control = 1;

	IMGDECSND_jr_write_coef_table();

	//Output buffer address
	IMGDECSND_jr_set_port(JR_LUMA_BASE, (g_imgdecsnd_jr_reg_info.luma_start >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
	IMGDECSND_jr_set_port(JR_BASE_EXTEND0, (g_imgdecsnd_jr_reg_info.luma_start >> 18) & 0x00003fff);
	IMGDECSND_jr_set_port(JR_CROMA_BASE, (g_imgdecsnd_jr_reg_info.chroma_start >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
	IMGDECSND_jr_set_port(JR_BASE_EXTEND1, (g_imgdecsnd_jr_reg_info.chroma_start >> 18) & 0x00003fff);

	//Temp buffer of JR coefficient table setting: 4-byte unit & 8-byte_align
	IMGDECSND_jr_set_port(JR_TEMP_BUF_BASE, ((g_imgdecsnd_jr_reg_info.temp_buffer + 4) >> 2) & 0x0000fffe); // unit in 4 bytes , 8 bytes align
	IMGDECSND_jr_set_port(JR_BASE_EXTEND2, ((g_imgdecsnd_jr_reg_info.temp_buffer + 4) >> 18) & 0x00003fff);

	// DMA address (Debug progressive JPEG output error, after MJ-baseline JPEG decoding.)
	IMGDECSND_jr_set_port(JR_DMA_CP_BASE, g_imgdecsnd_jr_reg_info.render_input_offset); // [10:0]: Indicates which ping-pong DMA buffer for JR using.

	// set source/display color mode
	IMGDECSND_jr_set_port(JR_SRC_COLOR_MODE, g_imgdecsnd_jr_reg_info.src_color_mode);
	IMGDECSND_jr_set_port(JR_DISP_COLOR_MODE, (g_imgdecsnd_jr_reg_info.dis_color_mode & 0x0f)
									|(g_imgdecsnd_jr_reg_info.alpha_part << 4)
									|(g_imgdecsnd_jr_reg_info.rgb_swap << 12)
									|(g_imgdecsnd_jr_reg_info.orientation << 13));

	// set size of source picture
	IMGDECSND_jr_set_port(JR_SRC_WIDTH, g_imgdecsnd_jr_reg_info.src_width);
	IMGDECSND_jr_set_port(JR_SRC_HEIGHT, g_imgdecsnd_jr_reg_info.src_height);

	//set filter initial phase
	IMGDECSND_jr_set_port(JR_V_PHASE_OFFSET, g_imgdecsnd_jr_reg_info.v_filter_phase);
	IMGDECSND_jr_set_port(JR_H_PHASE_OFFSET, g_imgdecsnd_jr_reg_info.h_filter_phase);

	//scaler index
	IMGDECSND_jr_set_port(JR_SCALER_INDEX_V, (g_imgdecsnd_jr_reg_info.scaler_index_v & 0xffff));
	IMGDECSND_jr_set_port(JR_SCALER_INDEX_H, (g_imgdecsnd_jr_reg_info.scaler_index_h & 0xffff));
	IMGDECSND_jr_set_port(JR_SCALER_INDEX_EXT, ((g_imgdecsnd_jr_reg_info.scaler_index_h >> 16) & 0xf)|(((g_imgdecsnd_jr_reg_info.scaler_index_v >> 16) & 0xf) << 8));

	// set frame buffer size
	IMGDECSND_jr_set_port(JR_FRAME_WIDTH, g_imgdecsnd_jr_reg_info.frame_width & 0xffff); // unit in byte, 8 bytes align
	IMGDECSND_jr_set_port(JR_FRAME_HEIGHT, g_imgdecsnd_jr_reg_info.frame_height & 0xffff); // unit in byte

	// offset from display point to original point
	IMGDECSND_jr_set_port(JR_RENDER_H_INIT, scale_info_data->rect_x >> 1); // [9:0] unit in 2 pixels, 8 pixels align
	IMGDECSND_jr_set_port(JR_RENDER_V_INIT, scale_info_data->rect_y >> 1); // [9:0] unit in 2 pixels, 8 pixels align

	// set size of display pic
	IMGDECSND_jr_set_port(JR_RENDER_H_BOUND, g_imgdecsnd_jr_reg_info.disp_box_h_bound & 0x0fff); // [11:0] unit in 2 pixels, 8 pixels align
	IMGDECSND_jr_set_port(JR_RENDER_V_BOUND, g_imgdecsnd_jr_reg_info.disp_box_v_bound & 0x0fff); // [11:0] unit in 2 pixels, 8 pixels align

	//Pre-scaling asetting
	IMGDECSND_jr_set_port(JR_SRCPP_CTRL, g_imgdecsnd_jr_reg_info.pre_scaler & 0x001f);

	//JR_MISC[0] temp buffer load in luma DRAM
	//JR_MISC[1] Enable drop mode
	//JR_MISC[2] RGB output in chroma DRAM
	//JR_MISC[3] Enable odd-symmetric filter coefficient
	//JR_MISC[4] HD-color space
	//JR_MISC[5] RGB input in Chroma DRAM
	//JR_MISC[6] Bypass mode
	//JR_MISC[7] BW file(luma only file)
	//JR_MISC[9:8]: YUV color space conversion
	//                      00:BYPASS
	//                      01:JPEG_YUV ->BT.601_YUV
	//                      10:JPEG_YUV ->BT.709_YUV
	//JR_MISC[11:10]: YUV to RGB color space conversion
	//                      00:BYPASS
	//                      01:BT.601_YUV->RGB
	//                      10:BT.709_YUV->RGB
	//                      11:JPEG_YUV->RGB
	//JR_MISC[14] : alpha scaling Enable
	IMGDECSND_jr_set_port(JR_MISC, (g_imgdecsnd_jr_reg_info.temp_buf_onLuma)
						|(g_imgdecsnd_jr_reg_info.pd_enable << 1)
						|(g_imgdecsnd_jr_reg_info.rgb_output_onChroma << 2)
						|(g_imgdecsnd_jr_reg_info.odd_symm_filter << 3)
						|(g_imgdecsnd_jr_reg_info.hd_color_space << 4)
						|(g_imgdecsnd_jr_reg_info.rgb_input_onChroma << 5)
						|(g_imgdecsnd_jr_reg_info.bypass_enable << 6)
						|(g_imgdecsnd_jr_reg_info.bw_file << 7)
						|(g_imgdecsnd_jr_reg_info.yuv2RGB_flag << 10)
						|((scale_info_data->src_endian & 0x1) << 12));
}
EXPORT_SYMBOL(imgdecsnd_hal_scale_setup);

int imgdecsnd_hal_scale_start(
	SCALE_INFO *scale_info_data)
{
	UINT32 SrcYBufBase, SrcUVBufBase;
	UINT32 McuLineNum, FetchWidth, i, bpp;
	UINT32 jr_status;
	UINT32 time_out = 3000000; // 3000000 us = 3 sec ?????? too long for frame scaling

	if(scale_info_data->clip_x == 0)
	{
		switch(scale_info_data->src_type)
		{
			default:
			case SCALE_SRC_YUV420:
				McuLineNum = ((scale_info_data->clip_height + 0xf) >> 4);
				break;
			case SCALE_SRC_YUY2:
			case SCALE_SRC_RGBA8888:
			case SCALE_SRC_ARGB8888:
				McuLineNum = ((scale_info_data->clip_height + 0x7) >> 3);
				break;
		}

		SrcYBufBase = scale_info_data->src_Ybufphyaddr + scale_info_data->clip_y * scale_info_data->src_bufbpl;
		SrcUVBufBase = scale_info_data->src_UVbufphyaddr + (scale_info_data->clip_y >> 1) * scale_info_data->src_bufbpl; // only for SCALE_SRC_YUV420
		FetchWidth = scale_info_data->src_bufbpl;

		// set source buffer address: 4-byte_unit, 8-byte_align
		IMGDECSND_jr_set_port(JR_AUTO_SRC_LUMA_BASE, (SrcYBufBase >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
		IMGDECSND_jr_set_port(JR_AUTO_SRC_BASE_EXT0, (SrcYBufBase >> 18) & 0x00003fff);

		IMGDECSND_jr_set_port(JR_AUTO_SRC_CHROMA_BASE, (SrcUVBufBase >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
		IMGDECSND_jr_set_port(JR_AUTO_SRC_BASE_EXT1, (SrcUVBufBase >> 18) & 0x00003fff);

		IMGDECSND_jr_set_port(JR_MCU_LINE_NUM, McuLineNum); // [12:0]: Number of MCU lines in auto fetch DRAM buffer.
		IMGDECSND_jr_set_port(JR_AUTO_FETCH_WIDTH, FetchWidth);  // unit in byte, 4 bytes align

		//reset hw
		g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control | (1 << 5); // FSM software reset
		g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control & ~0x0020;

		//for auto-fetch frame start
		g_pImgDecSndReg->huf.jr_control = 0x0103; // [8]: Picture start. [1]: Automatically fetch source pixels from DRAM. [0]: JR enable.
		g_pImgDecSndReg->huf.jr_control = 0x0003;

	   	//each auto-fetch trigger
		g_pImgDecSndReg->huf.jr_control =(0xfffb)& g_pImgDecSndReg->huf.jr_control;
		g_pImgDecSndReg->huf.jr_control = 0x0004 | g_pImgDecSndReg->huf.jr_control; // [2]: Automatically fetch start.
		g_pImgDecSndReg->huf.jr_control =(0xfffb)& g_pImgDecSndReg->huf.jr_control;

		//each auto-fetch done check
		do
		{
			if(time_out == 0)
			{
				ERR("Wait JR finish: time out!");
				return -1;
			}

			//udelay(1); // this delay will cost 20% CPU loading as scaling 480p 60fps in EDR.
			msleep(1);

			jr_status = g_pImgDecSndReg->huf.jr_status;
			//DEBUG("Wait JR finish: jr_status = 0x%08x\n", jr_status);

			time_out--;
		}while(!(jr_status & 0x04));
	}
	else
	{
		switch(scale_info_data->src_type)
		{
			default:
			case SCALE_SRC_YUV420:
				bpp = 1;
				McuLineNum = ((scale_info_data->clip_height + 0xf) >> 4);
				break;
			case SCALE_SRC_YUY2:
				bpp = 2;
				McuLineNum = ((scale_info_data->clip_height + 0x7) >> 3);
				break;
			case SCALE_SRC_RGBA8888:
				bpp = 4;
				McuLineNum = ((scale_info_data->clip_height + 0x7) >> 3);
				break;
			case SCALE_SRC_ARGB8888:
				bpp = 4;
				McuLineNum = ((scale_info_data->clip_height + 0x7) >> 3);
				break;
		}

		SrcYBufBase = scale_info_data->src_Ybufphyaddr + 
					(scale_info_data->clip_y * scale_info_data->src_bufbpl) + 
					(scale_info_data->clip_x * bpp);
		SrcUVBufBase = scale_info_data->src_UVbufphyaddr + 
					((scale_info_data->clip_y >> 1) * scale_info_data->src_bufbpl) + 
					(scale_info_data->clip_x * bpp); // only for SCALE_SRC_YUV420
		FetchWidth = scale_info_data->src_bufbpl;

		for(i=0; i<McuLineNum; i++)
		{
			// set source buffer address: 4-byte_unit, 8-byte_align
			IMGDECSND_jr_set_port(JR_AUTO_SRC_LUMA_BASE, (SrcYBufBase >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
			IMGDECSND_jr_set_port(JR_AUTO_SRC_BASE_EXT0, (SrcYBufBase >> 18) & 0x00003fff);

			IMGDECSND_jr_set_port(JR_AUTO_SRC_CHROMA_BASE, (SrcUVBufBase >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
			IMGDECSND_jr_set_port(JR_AUTO_SRC_BASE_EXT1, (SrcUVBufBase >> 18) & 0x00003fff);

			if(i ==0)
			{
				IMGDECSND_jr_set_port(JR_MCU_LINE_NUM, 1); // [12:0]: Number of MCU lines in auto fetch DRAM buffer.
				IMGDECSND_jr_set_port(JR_AUTO_FETCH_WIDTH, FetchWidth);  // unit in byte, 4 bytes align

				//reset hw
				g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control | (1 << 5); // FSM software reset
				g_pImgDecSndReg->huf.jr_control = g_pImgDecSndReg->huf.jr_control & ~0x0020;

				//for auto-fetch frame start
				g_pImgDecSndReg->huf.jr_control = 0x0103; // [8]: Picture start. [1]: Automatically fetch source pixels from DRAM. [0]: JR enable.
				g_pImgDecSndReg->huf.jr_control = 0x0003;
			}

		   	//each auto-fetch trigger
			g_pImgDecSndReg->huf.jr_control =(0xfffb)& g_pImgDecSndReg->huf.jr_control;
			g_pImgDecSndReg->huf.jr_control = 0x0004 | g_pImgDecSndReg->huf.jr_control; // [2]: Automatically fetch start.
			g_pImgDecSndReg->huf.jr_control =(0xfffb)& g_pImgDecSndReg->huf.jr_control;

			//each auto-fetch done check
			do
			{
				if(time_out == 0)
				{
					ERR("Wait JR finish: time out!");
					return -1;
				}

				udelay(1);

				jr_status = g_pImgDecSndReg->huf.jr_status;
				//DEBUG("Wait JR finish: jr_status = 0x%08x\n", jr_status);

				time_out--;
			}while(!(jr_status & 0x04));

			switch(scale_info_data->src_type)
			{
				default:
				case SCALE_SRC_YUV420:
					SrcYBufBase += scale_info_data->src_bufbpl * 16;
					SrcUVBufBase += scale_info_data->src_bufbpl * 8;
					break;
				case SCALE_SRC_YUY2:
				case SCALE_SRC_RGBA8888:
				case SCALE_SRC_ARGB8888:
					SrcYBufBase += scale_info_data->src_bufbpl * 8;
					break;
			}
			
			
		}
	}

	return 0;
}
EXPORT_SYMBOL(imgdecsnd_hal_scale_start);

void imgdecsnd_hal_jpeg_hw_init(
	JPEG_INFO *jpeg_info_data)
{
	UINT32 dma_size = 5; // jpeg_huff_mode[10:8]=101: 64 x 4 byte DMA size = 64 words = 256 bytes = 0x100 bytes
	UINT32 input_type, y_block, cb_block, cr_block;

	//reset DMA mover, otherwize after PNG HW Decode , MJ mode may move the wrong bitstream
	g_pImgDecSndReg->jdm.jdm_mode = 1 << 15; // reset JDM only for NEW_MJ_HW

	g_pImgDecSndReg->jdm.jdm_jpeg_offset = 16; // idct output DMA buffer base address (unit: 16-byte) at 16*16 = 64*4 = 64 words = 256 bytes

	g_pImgDecSndReg->huf.jpeg_huff_mode = 0x8000; // reset most HUF registers to default values
	g_pImgDecSndReg->huf.jpeg_huff_mcuno = 0; // restart interval MCU number
	g_pImgDecSndReg->huf.jpeg_huff_dma_offset_0 = 0; //set DMA offset to save the HUF decoded data

	if(jpeg_info_data->jpegSOF.sof_n == 2) // progressive JPEG
	{
		g_pImgDecSndReg->huf.jpeg_huff_acval_offset_0 = 0x0020;  // 32
		g_pImgDecSndReg->huf.jpeg_huff_acval_offset_1 = 0x005c; // 92
		g_pImgDecSndReg->huf.jpeg_huff_mode = 1 << 14; // DMA pixel width = 16-bit mode
		jpeg_info_data->jpegDHT.huff_ac_size = 240; // AC value(symbal) count = 60x4 >= 16x15=240 bytes
	}
	else // baseline JPEG
	{
		g_pImgDecSndReg->huf.jpeg_huff_acval_offset_0 = 0x0040;  // 64
		g_pImgDecSndReg->huf.jpeg_huff_acval_offset_1 = 0x0069; // 105
		g_pImgDecSndReg->huf.jpeg_huff_mode = 0; // DMA pixel width = 32-bit mode
		jpeg_info_data->jpegDHT.huff_ac_size = 162; // AC value(symbal) count = 41x4 >= 16x10+2=162 bytes
	}

	// set jpeg_huff_mode = use DMA1 only | bitstream is from DMA | 64 x 4byte DMA size | DMA bitstream use little endian
	g_pImgDecSndReg->huf.jpeg_huff_mode = g_pImgDecSndReg->huf.jpeg_huff_mode | (1 << 12) | (1 << 11) | ((dma_size & 0x7) << 8) | (0 << 7) | (0x00);
	g_pImgDecSndReg->huf.jpeg_huff_dma_offset_0 = (1 << 15) | (1 << 12) | 128; // set suspending error timing threshold : prevent to stay too long in huffman

	if(jpeg_info_data->jpegSOF.sof_n == 2)
	{
		g_pImgDecSndReg->huf.jpeg_huff_bitstream = 152; //Input bitstream location in DMA offset (unit = 4 bytes) for progressive JPEG
	}
	else
	{
		g_pImgDecSndReg->huf.jpeg_huff_bitstream = 146; //Input bitstream location in DMA offset (unit = 4 bytes) for baseline JPEG
	}

	g_pImgDecSndReg->huf.jpeg_huff_probe = 0x0400;  // for hw debug

	// set JPEG image horizontal / vertical MCU number.
	g_pImgDecSndReg->huf.jpeg_huff_jpeghsize = (jpeg_info_data->jpegSOF.Hfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.width+15)>>4) : ((jpeg_info_data->jpegSOF.width+7)>>3);
	g_pImgDecSndReg->huf.jpeg_huff_jpegvsize = (jpeg_info_data->jpegSOF.Vfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.height+15)>>4) : ((jpeg_info_data->jpegSOF.height+7)>>3);

	if(jpeg_info_data->jpegSOF.components == 1)
	{
		input_type = YUV_444; // YUV_BW;
		y_block = jpeg_info_data->jpegSOF.Hfactor[0] * jpeg_info_data->jpegSOF.Vfactor[0];
		cb_block = 0;
		cr_block = 0;
	}
	else
	{
		y_block = jpeg_info_data->jpegSOF.Hfactor[0] * jpeg_info_data->jpegSOF.Vfactor[0];
		cb_block = jpeg_info_data->jpegSOF.Hfactor[1] * jpeg_info_data->jpegSOF.Vfactor[1];
		cr_block = jpeg_info_data->jpegSOF.Hfactor[2] * jpeg_info_data->jpegSOF.Vfactor[2];

		if(y_block == 4)
		{
			if(jpeg_info_data->jpegSOF.Hfactor[0] == 2)
			{
				input_type = YUV_411_2X2;
			}
			else if(jpeg_info_data->jpegSOF.Hfactor[0] == 1)
			{
				input_type = YUV_411_1X4;
			}
			else
			{
				input_type = YUV_411_4X1;
			}
		}
		else if(y_block == 2)
		{
			if(jpeg_info_data->jpegSOF.Hfactor[0] == 2)
			{
				input_type = YUV_422_2X1;
			}
			else
			{
				input_type = YUV_422_1X2;
			}
		}
		else // y_block = 1
		{
			input_type = YUV_444;
		}
	}
	g_pImgDecSndReg->huf.jpeg_huff_mode = (g_pImgDecSndReg->huf.jpeg_huff_mode & 0xfff0) | input_type;
	g_pImgDecSndReg->huf.jpeg_huff_ctrl =  (1 << 15) | ((cb_block & 0x7) << 12) | ((y_block & 0xf) <<8 );
	g_pImgDecSndReg->huf.jpeg_huff_status = (((y_block + cb_block + cr_block - 1) & 0xf) << 8); // set mcu number
}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_hw_init);

void imgdecsnd_hal_jpeg_fill_bs_tables(
	JPEG_INFO *jpeg_info_data)
{
	IMGDECSND_JPEG_fill_DQT_reg(jpeg_info_data);

	// YCbCr DC/AC tables
	g_pImgDecSndReg->huf.jpeg_huff_table = (UINT32)(0x0000)
									| (((UINT32)jpeg_info_data->jpegSOS.DCtable[0] & 0x03) << 14) 
									| (((UINT32)jpeg_info_data->jpegSOS.ACtable[0] & 0x03) << 12) 
 									| (((UINT32)jpeg_info_data->jpegSOS.DCtable[1] & 0x03) << 10) 
 									| (((UINT32)jpeg_info_data->jpegSOS.ACtable[1] & 0x03) << 8) 
									| (((UINT32)jpeg_info_data->jpegSOS.DCtable[2] & 0x03) << 6) 
									| (((UINT32)jpeg_info_data->jpegSOS.ACtable[2] & 0x03) << 4);

	// DRI
	if(jpeg_info_data->jpegDRI.dri_count)
		g_pImgDecSndReg->huf.jpeg_huff_mcuno = (UINT32)jpeg_info_data->jpegDRI.dri_count;

	IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, 0, DHT_DC0);
	IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, 1, DHT_DC1);
	IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, 0, DHT_AC0);
	IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, 1, DHT_AC1);

	// set picture start
	g_pImgDecSndReg->huf.jpeg_huff_ctrl = g_pImgDecSndReg->huf.jpeg_huff_ctrl | (1 << 2);

}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_fill_bs_tables);

void imgdecsnd_hal_jpeg_init_MJmode_waitqueue_head() // MJmode uses interrupt to replace polling
{
	init_waitqueue_head(&g_imgdecsnd_MJmode_interrupt_wait);
	g_imgdecsnd_MJmode_interrupt_done = false;
}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_init_MJmode_waitqueue_head);

irqreturn_t imgdecsnd_hal_jpeg_decode_bs_MJmode_irq_handler( // MJmode uses interrupt to replace polling
	int irq,
	void* dev_id)
{
	DEBUG("imgdecsnd_hal_jpeg_decode_bs_MJmode_irq_handler enter!!!");
	g_imgdecsnd_MJmode_interrupt_done = true;
	wake_up(&g_imgdecsnd_MJmode_interrupt_wait);

	return IRQ_HANDLED;
}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_decode_bs_MJmode_irq_handler);

int imgdecsnd_hal_jpeg_decode_bs_MJmode(
	JPEG_INFO *jpeg_info_data, 
	UINT32 bs_buf_phyaddr, 
	UINT32 bs_size)
{
	int ret = 0;
	UINT32 jpeg_huff_ctrl, jpeg_huff_mode, jr_status;
	UINT32 DAddr = 146 << 2; // 146 words = 146*4 bytes
	UINT32 mj_start_addr = 0, mj_stop_kbyte_addr = 0;
	UINT32 time_out = 3000; // 3000 ms = 3 sec for 8192x8192 ?????? too long!

	// DMA setting
	g_pImgDecSndReg->jdm.jdm_dma1_mode = (UINT32)(DMA_LEN_256 << 8) | VADDR_MODE_GLOBAL; // [10:8]=DMA transfer length, [3:0]=DMA operation mode
	g_pImgDecSndReg->jdm.jdm_dma1_start_addr = (DAddr >> 2) & 0x7ff; // [10:0] = Indicate the start address of DMA transfers. Unit:word.

	// JR setting
	//g_imgdecsnd_jr_reg_info.render_input_offset = JR_DMA_BUF_BASE;
	//IMGDECSND_jr_set_port(JR_DMA1_CP_BASE, g_imgdecsnd_jr_reg_info.render_input_offset); // [10:0]: Indicates which ping-pong DMA buffer for JR using.

	// set INVQ mode
	g_pImgDecSndReg->invq.jpeg_invq_mode = INVQ_JPEG_MODE | INVQ_JPEG_NOBYPASS;
	g_pImgDecSndReg->jdm.jdm_mode = GRAPH_JPEG_SAT | GRAPH_JPEG_OUT_8 | GRAPH_JPEG_32;

	// enable mj mode
	g_pImgDecSndReg->huf.jpeg_huff_mode = (g_pImgDecSndReg->huf.jpeg_huff_mode & 0xffef) | (1<<4);

	mj_start_addr = bs_buf_phyaddr;

	// set start ptr high
	g_pImgDecSndReg->huf.jpeg_huff_acval_offset_2 = mj_start_addr >> 16;

	// set start ptr low
	g_pImgDecSndReg->huf.jpeg_huff_acval_offset_3 = mj_start_addr & 0xffff;

	mj_stop_kbyte_addr = bs_buf_phyaddr + bs_size;
	mj_stop_kbyte_addr = mj_stop_kbyte_addr >> 10; // The unit of this address is 1K byte.

	// set end ptr high [11:0]
	g_pImgDecSndReg->huf.jpeg_huff_acval_offset_0 = (5 << 13) | ((mj_stop_kbyte_addr >> 8) & 0xfff);

	// set end ptr low [7:0]
	g_pImgDecSndReg->huf.jpeg_huff_acval_offset_1 = (1 << 15) | (mj_stop_kbyte_addr & 0xff);

	// set invq table
	g_pImgDecSndReg->huf.jpeg_huff_scan_ctrl = (((UINT32)jpeg_info_data->jpegSOF.Qtable[0] << 6) | 
										((UINT32)jpeg_info_data->jpegSOF.Qtable[1] << 4) | 
										((UINT32)jpeg_info_data->jpegSOF.Qtable[2] << 2) |
										(UINT32)jpeg_info_data->jpegSOF.Qtable[3]);

	g_imgdecsnd_MJmode_interrupt_done = false; // MJmode uses interrupt to replace polling

	// start to decode one picture in motion JPEG mode.
	g_pImgDecSndReg->huf.jpeg_huff_ctrl = (g_pImgDecSndReg->huf.jpeg_huff_ctrl  & 0xffef) | (1<<4);

	do
	{
		if (wait_event_timeout(g_imgdecsnd_MJmode_interrupt_wait, g_imgdecsnd_MJmode_interrupt_done, (MJMODE_DONE_DEFAULT_TIMEOUT * HZ) / 1000) == 0)
		{
			ERR("MJmode interrupt wait_event_timeout !!!\n");
			ret = -1;
			break;
		} // MJmode uses interrupt to replace polling
		g_imgdecsnd_MJmode_interrupt_done = false; // MJmode uses interrupt to replace polling

		jpeg_huff_ctrl = g_pImgDecSndReg->huf.jpeg_huff_ctrl;
		if(jpeg_huff_ctrl & 0x0060)  // [6]: mj stop, [5]: mj done
		{
			if(jpeg_huff_ctrl & 0x0040)  // [6]: mj stop
			{
			/*
				mj_start_addr = mj_stop_kbyte_addr << 10; // mj_stop_kbyte_addr is 1024-align -> must be 32-align
				// set start ptr high
				g_pImgDecSndReg->huf.jpeg_huff_acval_offset_2 = mj_start_addr >> 16;
				// set start ptr low
				g_pImgDecSndReg->huf.jpeg_huff_acval_offset_3 = mj_start_addr & 0xffff;

				mj_stop_kbyte_addr = mj_start_addr + 1024; // mj_start_addr is 1024-align -> mj_stop_kbyte_addr must be 1024-align
				mj_stop_kbyte_addr = mj_stop_kbyte_addr >> 10; // The unit of this address is 1K byte.
				// set end ptr high [11:0]
				g_pImgDecSndReg->huf.jpeg_huff_acval_offset_0 = (5 << 13) | ((mj_stop_kbyte_addr >> 8) & 0xfff);
				// set end ptr low [7:0]
				g_pImgDecSndReg->huf.jpeg_huff_acval_offset_1 = (1 << 15) | (mj_stop_kbyte_addr & 0xff);

				//MJ restart
				g_pImgDecSndReg->huf.jpeg_huff_ctrl = (g_pImgDecSndReg->huf.jpeg_huff_ctrl  & 0xff7f) | (1<<7);
			*/

				ERR("Motion JPEG is stop!(Note: bitstream buffer should have entire data to finish decoding!)\n");
				ret = -1;
				break;
			}
			else if(jpeg_huff_ctrl & 0x0020)  // [5]: mj done
			{
				DEBUG("Motion JPEG is done!\n");
				ret = 0;
				break;
			}
		}
		else if(g_pImgDecSndReg->huf.jpeg_huff_rd_val  & 0x0200)  // huff decode error
		{
			g_pImgDecSndReg->huf.jpeg_huff_rd_addr = (0x1 << 14);

			if(g_pImgDecSndReg->huf.jpeg_huff_dma_offset_0 & (0x1 << 13))
			{
				ERR("Motion JPEG Error --- Huffman suspending error !!!\n");
			}
			else
			{
				ERR("Motion JPEG Error --- Huffman decoding error !!!\n");
			}
			ret = -1;
			break;
		}

		/* // use interrupt to replace polling
		time_out--;
		if(time_out == 0)
		{
			ERR("MJmode bs jpeg decode: time out!");
			ret = -1;
			break;
		}
		//udelay(1); // Mantis 0003751: this delay will cost CPU 20% loading as decoding 720p MJPEG.
		msleep(1); // this msleep(1) will cost 20ms, it is not good for fps
		*/

		jpeg_huff_mode = g_pImgDecSndReg->huf.jpeg_huff_mode;
	}while(jpeg_huff_mode & 0x10);

	g_pImgDecSndReg->huf.jpeg_huff_mode = g_pImgDecSndReg->huf.jpeg_huff_mode &  0xffef;
	g_pImgDecSndReg->huf.jpeg_huff_acval_offset_0 = 0x8000;
	g_pImgDecSndReg->huf.jpeg_huff_acval_offset_1 = 0x8000;

	IMGDECSND_DMA_clear(VADDR_MODE_CLEAR, 0, DMA_LEN_32);

	//disable register invq_mode for logo bug by selina.chang 070809
	g_pImgDecSndReg->invq.jpeg_invq_mode = 0;

	//when user stop decoding or decode is suspend or error, after huffman close, need to check JR is done.
	time_out = 1000000; // 3000000 us = 3 sec for 8192x8192 JPEG
	do
	{
		if(time_out == 0)
		{
			ERR("Wait JR finish: time out!");
			ret = -1;
			break;
		}

		udelay(1); // Mantis 0003751: this delay will cost CPU 20% loading as decoding 720p MJPEG.
		//msleep(1); // this msleep(1) will cost 20ms, it is not good for fps

		jr_status = g_pImgDecSndReg->huf.jr_status;
		//DEBUG("Wait JR finish: jr_status = 0x%08x\n", jr_status);

		time_out--;
	}while(!(jr_status & 0x40));

	return ret;
}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_decode_bs_MJmode);

void imgdecsnd_hal_jpeg_fill_ms_tables(
	JPEG_INFO *jpeg_info_data)
{
	UINT32 Cr_Td, Cr_Ta;
	UINT32 inter_mode, decode_mode, ratio;

	if(jpeg_info_data->jpegDecode.cur_sos_run == 0)
		IMGDECSND_JPEG_fill_DQT_reg(jpeg_info_data);

	// set huffman table into huffman hardware
	// a. interleave mode (jpegSOS.components == 3)
	// b. non-interleave mode
	if(jpeg_info_data->jpegSOS.components == 3)
	{
		ratio = (jpeg_info_data->jpegSOF.Hfactor[0] * jpeg_info_data->jpegSOF.Vfactor[0]) +
				(jpeg_info_data->jpegSOF.Hfactor[1] * jpeg_info_data->jpegSOF.Vfactor[1]) +
				(jpeg_info_data->jpegSOF.Hfactor[2] * jpeg_info_data->jpegSOF.Vfactor[2]) - 1;

		// Set DC Huffman Table
		// Default: DC(0) put Y table, DC(1) put Cb table.
		// Cr's DC huffman table could be the same as
		// a. Y
		// b. Cb
		// c. none, => we need to change table during decoding
		if(jpeg_info_data->jpegSOS.DCtable[2] == jpeg_info_data->jpegSOS.DCtable[0])
		{
			Cr_Td = 0;
			jpeg_info_data->jpegDecode.ms_change_table &= 0xfd;
		}
		else if(jpeg_info_data->jpegSOS.DCtable[2] == jpeg_info_data->jpegSOS.DCtable[1])
		{
			Cr_Td = 1;
			jpeg_info_data->jpegDecode.ms_change_table &= 0xfd;
		}
		else
		{
			Cr_Td = 1;
			jpeg_info_data->jpegDecode.ms_change_table |= 2;
		}

		// Set AC Huffman Table
		// Default: AC(0) put Y table, AC(1) put Cb table.
		// Cr's AC huffman table could be the same as
		// a. Y
		// b. Cb
		// c. none, => we need to change table during decoding
		if(jpeg_info_data->jpegSOS.ACtable[2] == jpeg_info_data->jpegSOS.ACtable[0])
		{
			Cr_Ta = 0;
			jpeg_info_data->jpegDecode.ms_change_table &= 0xfe;
		}
		else if(jpeg_info_data->jpegSOS.ACtable[2] == jpeg_info_data->jpegSOS.ACtable[1])
		{
			Cr_Ta = 1;
			jpeg_info_data->jpegDecode.ms_change_table &= 0xfe;
		}
		else
		{
			Cr_Ta = 1;
			jpeg_info_data->jpegDecode.ms_change_table |= 1;
		}

		// YCbCr DC/AC tables
		g_pImgDecSndReg->huf.jpeg_huff_table = (0x0000) | (0<<14) | (0<<12) | (1<<10) | (1<<8) | (Cr_Td<<6) | (Cr_Ta<<4);

		// fill hardware huffman table
		IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[0], DHT_DC0);
		IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[1], DHT_DC1);
	}
	else
	{
		ratio = 0;

		// YCbCr DC/AC tables
		g_pImgDecSndReg->huf.jpeg_huff_table = 0x0000;

		// fill hardware huffman table
		if((jpeg_info_data->jpegSOF.components == 1) && (jpeg_info_data->jpegSOS.spectral_selection_start == 0))
		{
			IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[0], DHT_DC0);
		}
		else
		{
			IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.ACtable[jpeg_info_data->jpegSOS.identifier[0]-1], DHT_AC0);
		}
	}

	// interleaving mode
	if(jpeg_info_data->jpegSOS.components > 1)
		inter_mode = 0; // y/cb/cr interleaving mode
	else if(jpeg_info_data->jpegSOS.identifier[0] == 1)
		inter_mode = 1; // y block only
	else if(jpeg_info_data->jpegSOS.identifier[0] == 2)
		inter_mode = 2; // cb block only
	else if(jpeg_info_data->jpegSOS.identifier[0] == 3)
		inter_mode = 3; // cr block only
	else if(jpeg_info_data->jpegSOS.identifier[0] == 4)
		inter_mode = 4; // k block only
	else
		inter_mode = 0; // y/cb/cr interleaving mode

	// decode mode
	if(jpeg_info_data->jpegSOS.spectral_selection_start  == 0)
	{
		if(jpeg_info_data->jpegSOS.successive_approximation_high == 0)
		{
			decode_mode = 4;  // DC first
		}
		else
		{
			decode_mode = 5;  // DC refine
		}
	}
	else
	{
		if(jpeg_info_data->jpegSOS.successive_approximation_high == 0)
		{
			decode_mode = 6;  // AC first
		}
		else
		{
			decode_mode = 7;  // AC refine
		}
	}

	// [14:12] set inter mode, [11:8] set total block number-1, and [6:4] set decode mode
	g_pImgDecSndReg->huf.jpeg_huff_status = ((inter_mode & 0x07) << 12) | (ratio << 8) | (decode_mode << 4);

	// [15:10] set subband start, [9:4] set subband end, [3:0] set approximation low bit
	g_pImgDecSndReg->huf.jpeg_huff_scan_ctrl = ((jpeg_info_data->jpegSOS.spectral_selection_start & 0x3f) << 10) |
										((jpeg_info_data->jpegSOS.spectral_selection_end & 0x3f) << 4) |
										(jpeg_info_data->jpegSOS.successive_approximation_low & 0xf);

	// DRI
	if(jpeg_info_data->jpegDRI.dri_count)
		g_pImgDecSndReg->huf.jpeg_huff_mcuno = (UINT32)jpeg_info_data->jpegDRI.dri_count;

	/*
	// record information (reg_2, reg_3, reg_4) // for Multi-Scan ??????
	jpeg_info_data->jpegDecode.ms_pre_huf_status = g_pImgDecSndReg->huf.jpeg_huff_status;
	jpeg_info_data->jpegDecode.ms_pre_huf_scan_ctrl = g_pImgDecSndReg->huf.jpeg_huff_scan_ctrl;
	jpeg_info_data->jpegDecode.ms_pre_huf_table = g_pImgDecSndReg->huf.jpeg_huff_table;
	*/

	// reset scan (clear scan and picture start)
	g_pImgDecSndReg->huf.jpeg_huff_ctrl = 0x0000;
	g_pImgDecSndReg->huf.jpeg_huff_ctrl = 0x000c;

	// update information to set initial register
	g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = 0x8000;
	g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = 0x4000;
	g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = 0x6000;

}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_fill_ms_tables);

void imgdecsnd_hal_jpeg_decode_ms_init(
	JPEG_INFO *jpeg_info_data, 
	UINT32 *bs_buf_phyaddr,  // *bs_buf_phyaddr for Multi-Scan ??????
	UINT32 working_buf_phyaddr)
{
	UINT32 w, h;
	//UINT32 V, H, last, used_bit; // for Multi-Scan ??????

	// buffer layout
	if(jpeg_info_data->jpegDecode.cur_sos_run == 0)
	{
		jpeg_info_data->jpegDecode.sof_Hi_0 = jpeg_info_data->jpegSOF.Hfactor[0];
		jpeg_info_data->jpegDecode.sof_Vi_0 = jpeg_info_data->jpegSOF.Vfactor[0];
		jpeg_info_data->jpegDecode.horizontal_mcu = (jpeg_info_data->jpegSOF.Hfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.width+15)>>4) : ((jpeg_info_data->jpegSOF.width+7)>>3);
		jpeg_info_data->jpegDecode.vertical_mcu = (jpeg_info_data->jpegSOF.Vfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.height+15)>>4) : ((jpeg_info_data->jpegSOF.height+7)>>3);
		jpeg_info_data->jpegDecode.mcu_horizontal_mb = ((UINT32)jpeg_info_data->jpegSOF.width + 15) >> 4;
		jpeg_info_data->jpegDecode.mcu_horizontal_b = ((UINT32)jpeg_info_data->jpegSOF.width + 7) >> 3;

		w =  ((jpeg_info_data->jpegSOF.width + 15) >> 4) << 4;
		h =  ((jpeg_info_data->jpegSOF.height + 15) >> 4) << 4;
		jpeg_info_data->jpegDecode.y_in = working_buf_phyaddr;
		jpeg_info_data->jpegDecode.cbcr_in = jpeg_info_data->jpegDecode.y_in + (w * h) * 2; // last * 2 for 16-bits data
	}

	jpeg_info_data->jpegDecode.dma_first_flg = 0;
	jpeg_info_data->jpegDecode.mcu_cur_x = 0;
	jpeg_info_data->jpegDecode.ms_work_y = 0;
	jpeg_info_data->jpegDecode.mcu_start = 0;

#if 0 // for Multi-Scan ??????
	if(jpeg_info_data->jpegDecode.cur_sos_run > 0)
	{
		V = (*bs_buf_phyaddr & 0xffffffff) >> 10; // [32:10]
		H = (*bs_buf_phyaddr & 0x3e0); // bs_buf_phyaddr[9:5]
		last = (*bs_buf_phyaddr & 0x1f); // bs_buf_phyaddr[4:0]
		used_bit = (last & 0x3); // bs_buf_phyaddr[1:0]

		if(jpeg_info_data->jpegSOS.components == 3)
		{
			// fill hardware huffman table
			IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[0], DHT_DC0);
			IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[1], DHT_DC1);
		}
		else
		{
			// fill hardware huffman table
			if((jpeg_info_data->jpegSOF.components == 1) && (jpeg_info_data->jpegSOS.spectral_selection_start == 0))
			{
				IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[0], DHT_DC0);
			}
			else
			{
				IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.ACtable[jpeg_info_data->jpegSOS.identifier[0]-1], DHT_AC0);
			}
		}

		// restore register information (reg_2, reg_3, reg_4)
		g_pImgDecSndReg->huf.jpeg_huff_status = jpeg_info_data->jpegDecode.ms_pre_huf_status;
		g_pImgDecSndReg->huf.jpeg_huff_scan_ctrl = jpeg_info_data->jpegDecode.ms_pre_huf_scan_ctrl;
		g_pImgDecSndReg->huf.jpeg_huff_table = jpeg_info_data->jpegDecode.ms_pre_huf_table;

		// reset scan (clear scan and picture start)
		g_pImgDecSndReg->huf.jpeg_huff_ctrl = 0x0000;
		g_pImgDecSndReg->huf.jpeg_huff_ctrl = 0x000c;

		// Restore scan information
		g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[0];
		g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[1];
		g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[4];
		g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[2];

		// Fill new bitstream to DMA
		IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, V, H, (152)*4, DMA_LEN_256);
		IMGDECSND_DMA_wait();

		g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset = (1<<15) |
												(jpeg_info_data->jpegDecode.ms_info[5] & 0x6700) |
												(used_bit << 11) |
												(last >> 2); // 11=8+3 for unit=bit, 2 for unit=word

		g_pImgDecSndReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[3];

		*bs_buf_phyaddr += (256 - last);
	}
#endif

	if(jpeg_info_data->jpegSOS.spectral_selection_start  != 0)
	{
		if(jpeg_info_data->jpegSOS.identifier[0] == 1) // Y
		{
			jpeg_info_data->jpegDecode.horizontal_mcu = ((jpeg_info_data->jpegSOF.width+7)>>3);
			jpeg_info_data->jpegDecode.vertical_mcu = ((jpeg_info_data->jpegSOF.height+7)>>3);

#if 0 // for Multi-Scan ??????
			// mcu_total should be reset
			if(mcu->sof_Vi_0 == 2)
			{
				UINT32 last_y =  jp_info->mcu.vertical-jp_info->mcu.cur_y;
				if(jp_info->ms.num_mbl*2 > last_y)
				{
					mcu_num = (mcu->sof_Hi_0 == 2)?(mcu->horizontal_mcu * last_y):(mcu_num / jp_info->ms.num_mbl * last_y);
				}
				else
				{
					mcu_num = (mcu->sof_Hi_0 == 2)?(mcu->horizontal_mcu * jp_info->ms.num_mbl * 2):(mcu_num * 2);
				}
			}
			else if(mcu->sof_Hi_0 == 2)
			{
				mcu_num =mcu->horizontal_mcu * jp_info->ms.num_mbl;
			}
#endif

			jpeg_info_data->jpegDecode.sof_Hi_0 = 1;
			jpeg_info_data->jpegDecode.sof_Vi_0 = 1;
		}
		else if(g_imgdecsnd_jr_reg_info.pre_scaler)  //(pre_scaler & 0x4)
		{
			if(jpeg_info_data->jpegSOF.Hfactor[1] == 1)
			{
				jpeg_info_data->jpegDecode.horizontal_mcu = ((jpeg_info_data->jpegSOF.width+7)>>3)>>1;
			}
			else
			{
				jpeg_info_data->jpegDecode.horizontal_mcu = ((jpeg_info_data->jpegSOF.width+7)>>3);
			}

			if(jpeg_info_data->jpegSOF.Vfactor[1] == 1)
			{
				jpeg_info_data->jpegDecode.vertical_mcu = ((jpeg_info_data->jpegSOF.height+7)>>3)>>1;
			}
			else
			{
				jpeg_info_data->jpegDecode.vertical_mcu = ((jpeg_info_data->jpegSOF.height+7)>>3);
			}

#if 0 // for Multi-Scan ??????
			if(jp_info->sof.sof_Vi[1] == 2)
			{
				mcu_num = mcu->horizontal_mcu * jp_info->ms.num_mbl * 2;
			}
			else
			{
				mcu_num = mcu->horizontal_mcu * jp_info->ms.num_mbl;
			}

			UINT32 last_y = jp_info->mcu.vertical-jp_info->mcu.cur_y;

			if(jp_info->ms.num_mbl*2 > last_y)
			{
				if(jp_info->sof.sof_Vi[1] == 2)
				{
					mcu_num = (mcu->horizontal_mcu * last_y) * 2;
				}
				else
				{
					mcu_num = (mcu->horizontal_mcu * last_y);
				}
			}
#endif

			jpeg_info_data->jpegDecode.sof_Hi_0 = 1;
			jpeg_info_data->jpegDecode.sof_Vi_0 = 1;
		}
	}

	g_pImgDecSndReg->huf.jpeg_huff_jpeghsize = jpeg_info_data->jpegDecode.horizontal_mcu;
	g_pImgDecSndReg->huf.jpeg_huff_jpegvsize = jpeg_info_data->jpegDecode.vertical_mcu;

	// set the number of mcus in this scan
	jpeg_info_data->jpegDecode.mcu_end = jpeg_info_data->jpegDecode.horizontal_mcu * jpeg_info_data->jpegDecode.vertical_mcu;

}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_decode_ms_init);

int imgdecsnd_hal_jpeg_decode_ms_interleave(
	JPEG_INFO *jpeg_info_data, 
	UINT32 *bs_buf_phyaddr) // *bs_buf_phyaddr for Multi-Scan ??????
{
	UINT32 k, x, y, mcu;
	UINT32 vld_addr, V, H, last, used_bit;
	UINT32 jpeg_huff_status; //, jpeg_huff_bits_info;  // for Multi-Scan ??????
	UINT32 fsm_value = 0;
	UINT32 iBlockV, iBlockH;
	UINT32 Cb_block = jpeg_info_data->jpegSOF.Hfactor[1] * jpeg_info_data->jpegSOF.Vfactor[1]; // only = 1 or 2
	UINT32 CbCr_block = Cb_block * 2; // only = 2 or 4
	UINT32 time_out;

	for(mcu=jpeg_info_data->jpegDecode.mcu_start; mcu<jpeg_info_data->jpegDecode.mcu_end; mcu++)
	{
		//decode mcu
		for(k=0; k<jpeg_info_data->jpegSOF.components; k++)
		{
			for(y=0; y<jpeg_info_data->jpegSOF.Vfactor[k]; y++)
			{
				for(x=0; x<jpeg_info_data->jpegSOF.Hfactor[k]; x++)
				{
					//output address
					switch(k)
					{
						default:
						case 0: // Y
							vld_addr = jpeg_info_data->jpegDecode.y_in 
										+ (((jpeg_info_data->jpegDecode.ms_work_y + y) * (jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64)) 
										+ ((jpeg_info_data->jpegDecode.mcu_cur_x + x) * 64)) * 2; // last * 2 for 16-bits data
							break;
						case 1: // Cb: (y + x) for (x, y) only = (0, 0), (0, 1), (1, 0), due to Cb_mcu = 1x1, 1x2, 2x1, not 2x2
							vld_addr = jpeg_info_data->jpegDecode.cbcr_in + ((mcu * 64 * CbCr_block) + (64 * (y + x))) * 2; // last * 2 for 16-bits data

							// for ps jpeg 422(221212)
							if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
							{
								vld_addr = jpeg_info_data->jpegDecode.cbcr_in 
											+ (((jpeg_info_data->jpegDecode.ms_work_y + y) * jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64) 
											+ ((jpeg_info_data->jpegDecode.mcu_cur_x + x) * 64)) * 2; // last * 2 for 16-bits data
							}

							// reload table
							if(jpeg_info_data->jpegDecode.ms_change_table & 2)
							{
								IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[1], DHT_DC1);
							}
							break;
						case 2: // Cr: (y + x) for (x, y) only = (0, 0), (0, 1), (1, 0), due to Cr_mcu = 1x1, 1x2, 2x1, not 2x2
							vld_addr = jpeg_info_data->jpegDecode.cbcr_in + ((mcu * 64 * CbCr_block) + (64 * Cb_block) + (64 * (y + x))) * 2; // last * 2 for 16-bits data

							//for ps jpeg 422(221212)
							if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
							{
								vld_addr = jpeg_info_data->jpegDecode.cbcr_in 
											+ (((jpeg_info_data->jpegDecode.ms_work_y + y) * jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64) 
											+ ((jpeg_info_data->jpegDecode.mcu_cur_x + 1) * 64)) * 2; // last * 2 for 16-bits data
							}

							// reload table
							if(jpeg_info_data->jpegDecode.ms_change_table & 2)
							{
								IMGDECSND_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[2], DHT_DC1);
							}
							break;
					}
					//DEBUG("mcu = %d, k = %d, y = %d, x = %d, vld_addr = 0x%08x\n", mcu, k, y, x, vld_addr);

					// copy pre-pixels from working buffer into DMA
					iBlockV = (vld_addr & 0xffffffff) >> 10;
					iBlockH = (vld_addr & 0x3e0);

					// prepare data which produce from last scan for Huffman
					if(jpeg_info_data->jpegDecode.cur_sos_run == 0)
					{
						IMGDECSND_DMA_clear(VADDR_MODE_CLEAR, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
						IMGDECSND_DMA_wait();
					}
					else
					{
						//IMGDECSND_DMA_wait();
						IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
						IMGDECSND_DMA_wait();
					}

					// decode block
					time_out = 1000; // 1000 us = 1 ms
					g_pImgDecSndReg->huf.jpeg_huff_ctrl = g_pImgDecSndReg->huf.jpeg_huff_ctrl | (0<<1) | 1;
					do
					{
						jpeg_huff_status = g_pImgDecSndReg->huf.jpeg_huff_status;
						//DEBUG("jpeg_huff_status = 0x%08x\n", jpeg_huff_status);

						if(jpeg_huff_status & 0x0001) // end of block
						{
							//DEBUG("end of block\n");
							break;
						}

						if(jpeg_huff_status & 0x0008)  // bitstream/dma done
						{
							//DEBUG("bitstream/dma done (mcu = %d)\n", mcu);

							V = (*bs_buf_phyaddr & 0xffffffff) >> 10;
							H = *bs_buf_phyaddr & 0x3e0;
							last = *bs_buf_phyaddr & 0x1f;
							used_bit = last & 0x3;

#if 0 // for Multi-Scan ??????
							// updata pre_subcnt
							jpeg_huff_bits_info = g_pImgDecSndReg->huf.jpeg_huff_bits_info;
							jpeg_info_data->jpegDecode.pre_subcnt += (0x0040 - (jpeg_huff_bits_info & 0x00ff)) * 4;
							jpeg_info_data->jpegDecode.pre_subcnt -= (jpeg_huff_bits_info & 0x8000) ? (1) : (0);
							jpeg_info_data->jpegDecode.pre_subcnt -= (jpeg_huff_bits_info & 0x1000) ? (2) : (0);
#endif

							IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, V, H, (152)*4, DMA_LEN_256);
							IMGDECSND_DMA_wait();

							if(jpeg_info_data->jpegDecode.dma_first_flg == 0)
							{
								g_pImgDecSndReg->huf.jpeg_huff_ctrl = 0x000c;
								g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset = 0x8000 | (used_bit << 11) | (last >> 2); // 11=8+3 for unit=bit, 2 for unit=word
								jpeg_info_data->jpegDecode.dma_first_flg = 1;
								//jpeg_info_data->jpegDecode.pre_subcnt = 0;  // for Multi-Scan ??????
							}
							else
							{
								fsm_value = g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset & 0x6000;
								g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset = (used_bit << 11) | (last >> 2) | fsm_value; // 11=8+3 for unit=bit, 2 for unit=word
							}

							*bs_buf_phyaddr += (256 - last);
						}

						if(jpeg_huff_status & 0x0004)  // decode error
						{
							g_pImgDecSndReg->huf.jpeg_huff_rd_addr = (0x4 << 12);

							if(g_pImgDecSndReg->huf.jpeg_huff_dma_offset_0 & (0x1 << 13))
							{
								ERR("Interleave MS JPEG Error --- Huffman suspending error !!!\n");
							}
							else
							{
								ERR("Interleave MS JPEG Error --- Huffman decoding error !!!\n");
							}
							return -1;
						}

						time_out--;
						if(time_out == 0)
						{
							ERR("MS interleave jpeg decode: time out!");
							return -1;
						}
						udelay(1);
					}while(1);

					// save cur-pixels from DMA into working buffer
					//IMGDECSND_DMA_wait();
					IMGDECSND_DMA_save(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
					IMGDECSND_DMA_wait();
				}
			}
		}

		// record the address of coordinate
		jpeg_info_data->jpegDecode.mcu_cur_x += jpeg_info_data->jpegDecode.sof_Hi_0;
		if(jpeg_info_data->jpegDecode.mcu_cur_x >= jpeg_info_data->jpegDecode.mcu_horizontal_b)
		{
			jpeg_info_data->jpegDecode.mcu_cur_x = 0;
			jpeg_info_data->jpegDecode.ms_work_y += jpeg_info_data->jpegDecode.sof_Vi_0;
		}
	}

	return 0;
}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_decode_ms_interleave);

int imgdecsnd_hal_jpeg_decode_ms_noninterleave(
	JPEG_INFO *jpeg_info_data, 
	UINT32 *bs_buf_phyaddr) // *bs_buf_phyaddr for Multi-Scan ??????
{
	UINT32 mcu;
	UINT32 vld_addr, V, H, last, used_bit;
	UINT32 jpeg_huff_status; //, jpeg_huff_bits_info; // for Multi-Scan ??????
	UINT32 fsm_value = 0;
	UINT32 iBlockV, iBlockH;
	UINT32 time_out;

	for(mcu=jpeg_info_data->jpegDecode.mcu_start; mcu<jpeg_info_data->jpegDecode.mcu_end; mcu++)
	{
		//output address
		switch(jpeg_info_data->jpegSOS.identifier[0])
		{
			default:
			case 1: // Y
				vld_addr = jpeg_info_data->jpegDecode.y_in 
							+ ((jpeg_info_data->jpegDecode.ms_work_y * jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64) 
							+ (jpeg_info_data->jpegDecode.mcu_cur_x * 64)) * 2; // last * 2 for 16-bits data
				break;
			case 2: // Cb // jpegSOF.Hfactor x Vfactor always ia 1 x 1 ??????
				if(g_imgdecsnd_jr_reg_info.pre_scaler)  // (pre_scaler & 0x4)
				{
					if(mcu%2 == 0)
					{
						vld_addr = jpeg_info_data->jpegDecode.cbcr_in + (mcu * 64 * 2) * 2; // last * 2 for 16-bits data
					}
					else
					{
						vld_addr = jpeg_info_data->jpegDecode.cbcr_in + (((mcu-1) * 64 * 2) + 64) * 2; // last * 2 for 16-bits data
					}

					// for ps jpeg 422(221212)
					if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
					{
						vld_addr = jpeg_info_data->jpegDecode.cbcr_in 
									+ ((jpeg_info_data->jpegDecode.ms_work_y * jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64) 
									+ (jpeg_info_data->jpegDecode.mcu_cur_x * 2 * 64)) * 2; // last * 2 for 16-bits data
					}
				}
				else
				{
					vld_addr = jpeg_info_data->jpegDecode.cbcr_in + (mcu * 64 * 2) * 2; // last * 2 for 16-bits data
				}
				break;
			case 3: // Cr // jpegSOF.Hfactor x Vfactor always ia 1 x 1 ??????
				if(g_imgdecsnd_jr_reg_info.pre_scaler)  //(pre_scaler & 0x4)
				{
					if(mcu%2 == 0)
					{
						vld_addr = jpeg_info_data->jpegDecode.cbcr_in + ((mcu * 64 * 2) + (64 * 2)) * 2; // last * 2 for 16-bits data
					}
					else
					{
						vld_addr = jpeg_info_data->jpegDecode.cbcr_in + (((mcu-1) * 64 * 2) + (64 * 3)) * 2; // last * 2 for 16-bits data
					}

					// for ps jpeg 422(221212)
					if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
					{
						vld_addr = jpeg_info_data->jpegDecode.cbcr_in 
									+ ((jpeg_info_data->jpegDecode.ms_work_y * jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64) 
									+ ((jpeg_info_data->jpegDecode.mcu_cur_x * 2 + 1) * 64)) * 2; // last * 2 for 16-bits data
					}
				}
				else
				{
					vld_addr = jpeg_info_data->jpegDecode.cbcr_in + ((mcu * 64 * 2) + 64) * 2; // last * 2 for 16-bits data
				}
				break;
		}
		//DEBUG("mcu = %d, k = %d, vld_addr = 0x%08x\n", mcu, jpeg_info_data->jpegSOS.identifier[0], vld_addr);

		// copy pre-pixels from working buffer into DMA
		iBlockV = (vld_addr & 0xffffffff) >> 10;
		iBlockH = (vld_addr & 0x3e0);

		// prepare data which produce from last scan for Huffman
		if(jpeg_info_data->jpegDecode.cur_sos_run == 0)
		{
			IMGDECSND_DMA_clear(VADDR_MODE_CLEAR, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
			IMGDECSND_DMA_wait();
		}
		else
		{
			//IMGDECSND_DMA_wait();
			IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
			IMGDECSND_DMA_wait();
		}

		// decode block
		time_out = 1000; // 1000 us = 1 ms
		g_pImgDecSndReg->huf.jpeg_huff_ctrl = g_pImgDecSndReg->huf.jpeg_huff_ctrl | (0<<1) | 1;
		do
		{
			jpeg_huff_status = g_pImgDecSndReg->huf.jpeg_huff_status;
			//DEBUG("jpeg_huff_status = 0x%08x\n", jpeg_huff_status);

			if(jpeg_huff_status & 0x0001) // end of block
			{
				//DEBUG("end of block\n");
				break;
			}

			if(jpeg_huff_status & 0x0008)  // bitstream/dma done
			{
				//DEBUG("bitstream/dma done (mcu = %d)\n", mcu);

				V = (*bs_buf_phyaddr & 0xffffffff) >> 10;
				H = *bs_buf_phyaddr & 0x3e0;
				last = *bs_buf_phyaddr & 0x1f;
				used_bit = last & 0x3;

#if 0 // for Multi-Scan ??????
				// updata pre_subcnt
				jpeg_huff_bits_info = g_pImgDecSndReg->huf.jpeg_huff_bits_info;
				jpeg_info_data->jpegDecode.pre_subcnt += (0x0040 - (jpeg_huff_bits_info & 0x00ff)) * 4;
				jpeg_info_data->jpegDecode.pre_subcnt -= (jpeg_huff_bits_info & 0x8000) ? (1) : (0);
				jpeg_info_data->jpegDecode.pre_subcnt -= (jpeg_huff_bits_info & 0x1000) ? (2) : (0);
#endif

				IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, V, H, (152)*4, DMA_LEN_256);
				IMGDECSND_DMA_wait();

				if(jpeg_info_data->jpegDecode.dma_first_flg == 0)
				{
					g_pImgDecSndReg->huf.jpeg_huff_ctrl = 0x000c;
					g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset = 0x8000 | (used_bit << 11) | (last >> 2); // 11=8+3 for unit=bit, 2 for unit=word
					jpeg_info_data->jpegDecode.dma_first_flg = 1;
					//jpeg_info_data->jpegDecode.pre_subcnt = 0; // for Multi-Scan ??????
				}
				else
				{
					fsm_value = g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset & 0x6000;
					g_pImgDecSndReg->huf.jpeg_huff_bits_dma_offset = (used_bit << 11) | (last >> 2) | fsm_value; // 11=8+3 for unit=bit, 2 for unit=word
				}

				*bs_buf_phyaddr += (256 - last);
			}

			if(jpeg_huff_status & 0x0004)  // decode error
			{
				g_pImgDecSndReg->huf.jpeg_huff_rd_addr = (0x4 << 12);

				if(g_pImgDecSndReg->huf.jpeg_huff_dma_offset_0 & (0x1 << 13))
				{
					ERR("Non-interleave MS JPEG Error --- Huffman suspending error !!!\n");
				}
				else
				{
					ERR("Non-interleave MS JPEG Error --- Huffman decoding error !!!\n");
				}
				return -1;
			}

			time_out--;
			if(time_out == 0)
			{
				ERR("MS noninterleave jpeg decode: time out!");
				return -1;
			}
			udelay(1);
		}while(1);

		// save cur-pixels from DMA into working buffer
		//IMGDECSND_DMA_wait();
		IMGDECSND_DMA_save(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
		IMGDECSND_DMA_wait();

		// record the address of coordinate
		jpeg_info_data->jpegDecode.mcu_cur_x += jpeg_info_data->jpegDecode.sof_Hi_0;
		if(jpeg_info_data->jpegDecode.mcu_cur_x >= jpeg_info_data->jpegDecode.horizontal_mcu)
		{
			jpeg_info_data->jpegDecode.mcu_cur_x = 0;
			jpeg_info_data->jpegDecode.ms_work_y += jpeg_info_data->jpegDecode.sof_Vi_0;
		}
	}

	return 0;
}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_decode_ms_noninterleave);

void imgdecsnd_hal_jpeg_decode_ms_end(
	JPEG_INFO *jpeg_info_data, 
	UINT32 *bs_buf_phyaddr) // *bs_buf_phyaddr for Multi-Scan ??????
{
	//UINT32 i;
#if 0 // for Multi-Scan ??????
	UINT32 jpeg_huff_bits_info;
	UINT8 *buf_ptr;
#endif

	// 1. save current scan information
#if 0 // for Multi-Scan ??????
	for(i=0; i<5; i++)
	{
		g_pImgDecSndReg->huf.jpeg_huff_rd_addr = (i << 12);
		jpeg_info_data->jpegDecode.ms_info[i] = g_pImgDecSndReg->huf.jpeg_huff_rd_huff_info;
	}
	jpeg_info_data->jpegDecode.ms_info[5] = g_pImgDecSndReg->huf.jpeg_huff_bits_info;
#endif

	// 2. save bitstream index
#if 0 // for Multi-Scan ??????
		jpeg_huff_bits_info = g_pImgDecSndReg->huf.jpeg_huff_bits_info;
		*bs_buf_phyaddr = *bs_buf_phyaddr - (64 * 4) + (((jpeg_huff_bits_info & 0x00ff) - 1) * 4);

		if(jpeg_huff_bits_info & 0x8000)	*bs_buf_phyaddr += 1;
		if(jpeg_huff_bits_info & 0x1000)	*bs_buf_phyaddr += 2;

		*bs_buf_phyaddr -= jpeg_info_data->jpegDecode.pre_subcnt;
		jpeg_info_data->jpegDecode.pre_subcnt = 0;

		buf_ptr = (UINT8 *)gp_chunk_va(*bs_buf_phyaddr);
		if((jpeg_huff_bits_info & 0x800) && (buf_ptr[0] == 0xff))
		{
			*bs_buf_phyaddr += 2;
		}
		else if(jpeg_huff_bits_info & 0x800)
		{
			*bs_buf_phyaddr += 1;
		}
#endif

	// 3. reset scan
	g_pImgDecSndReg->huf.jpeg_huff_ctrl = 0x0000;

	jpeg_info_data->jpegDecode.cur_sos_run++;

	if(jpeg_info_data->jpegSOS.components == 1)
	{
		jpeg_info_data->jpegDecode.sof_Hi_0 = jpeg_info_data->jpegSOF.Hfactor[0]; 
		jpeg_info_data->jpegDecode.sof_Vi_0 = jpeg_info_data->jpegSOF.Vfactor[0];
		jpeg_info_data->jpegDecode.horizontal_mcu = (jpeg_info_data->jpegDecode.sof_Hi_0 == 2) ? ((jpeg_info_data->jpegSOF.width+15)>>4) : ((jpeg_info_data->jpegSOF.width+7)>>3);
		jpeg_info_data->jpegDecode.vertical_mcu = (jpeg_info_data->jpegDecode.sof_Vi_0 == 2) ? ((jpeg_info_data->jpegSOF.height+15)>>4) : ((jpeg_info_data->jpegSOF.height+7)>>3);
	}
}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_decode_ms_end);

int imgdecsnd_hal_jpeg_decode_ms_scale(
	JPEG_INFO *jpeg_info_data)
{
	UINT32 i, j, x, y;
	UINT32 base_y_in, l_y_in, l_cbcr_in;
	UINT32 iBlockH, iBlockV;
	UINT32 scale_b_x=0, scale_b_y=0;
	UINT32 jr_status;
	UINT32 time_out;

	// set grobal variable to local variable
	l_cbcr_in = jpeg_info_data->jpegDecode.cbcr_in;

	for(j=0; j<jpeg_info_data->jpegDecode.vertical_mcu; j++)
	{
		base_y_in = jpeg_info_data->jpegDecode.y_in 
					+ (j * jpeg_info_data->jpegDecode.sof_Vi_0) * (jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64) * 2; // last * 2 for 16-bits data

		//for hw ps jpeg 422(221212)
		if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
		{
			l_cbcr_in = jpeg_info_data->jpegDecode.cbcr_in 
						+ (j * jpeg_info_data->jpegSOF.Vfactor[1]) * (jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64) * 2; // last * 2 for 16-bits data
		}

		for(i=0; i<jpeg_info_data->jpegDecode.horizontal_mcu; i++)
		{
			// Y
			for(y=0; y<jpeg_info_data->jpegDecode.sof_Vi_0; y++)
			{
				for(x=0; x<jpeg_info_data->jpegDecode.sof_Hi_0; x++)
				{
					l_y_in= base_y_in + ((y * jpeg_info_data->jpegDecode.mcu_horizontal_mb * 2 * 64) 
									+ ((i * jpeg_info_data->jpegDecode.sof_Hi_0 + x) * 64)) * 2; // last * 2 for 16-bits data
					//DEBUG("mcu = %d, k = 0, y = %d, x = %d, vld_addr = 0x%08x\n", j*jpeg_info_data->jpegDecode.horizontal_mcu+i, y, x, l_y_in);
					iBlockV = (l_y_in & 0xffffffff) >> 10;
					iBlockH = (l_y_in & 0x3e0);
					//IMGDECSND_DMA_wait();
					IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
					IMGDECSND_DMA_wait();

					// set output address of IDCT & INVQ
					g_pImgDecSndReg->jdm.jdm_jpeg_offset = 64 + y*4 + x*8;
					IMGDECSND_JPEG_graph_idct_8x8(jpeg_info_data, 0);
				}
			}

			// CbCr // jpegSOF.Hfactor x Vfactor always ia 1 x 1 ??????
			if(jpeg_info_data->jpegSOF.components != 1)
			{
				//------------ Cb ------------
				iBlockV = (l_cbcr_in & 0xffffffff) >> 10;
				iBlockH = (l_cbcr_in & 0x3e0);
				//DEBUG("mcu = %d, k = 1, y = 0, x = 0, vld_addr = 0x%08x\n", j*jpeg_info_data->jpegDecode.horizontal_mcu+i, l_cbcr_in);

				//for hw ps jpeg 422(221212)
				if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
					l_cbcr_in += jpeg_info_data->jpegDecode.mcu_horizontal_mb * 64 * 2 * 2; // last * 2 for 16-bits data
				else
					l_cbcr_in += 64 * 2; // last * 2 for 16-bits data

				//IMGDECSND_DMA_wait();
				IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
				IMGDECSND_DMA_wait();

				if(g_imgdecsnd_jr_reg_info.pre_scaler)  //(pre_scaler & 0x4)
				{
					// set output address of IDCT&invq
					g_pImgDecSndReg->jdm.jdm_jpeg_offset = 80;
					IMGDECSND_JPEG_graph_idct_8x8(jpeg_info_data, 1);

					iBlockV = (l_cbcr_in & 0xffffffff) >> 10;
					iBlockH = (l_cbcr_in & 0x3e0);

					//for hw ps jpeg 422(221212)
					if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
						l_cbcr_in -= jpeg_info_data->jpegDecode.mcu_horizontal_mb * 64 * 2 * 2; // last * 2 for 16-bits data
					l_cbcr_in += 64 * 2; // last * 2 for 16-bits data

					//IMGDECSND_DMA_wait();
					IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
					IMGDECSND_DMA_wait();

					g_pImgDecSndReg->jdm.jdm_jpeg_offset = 84;
					IMGDECSND_JPEG_graph_idct_8x8(jpeg_info_data, 1);
				}
				else
				{
					// set output address of IDCT&invq
					g_pImgDecSndReg->jdm.jdm_jpeg_offset = 80;
					IMGDECSND_JPEG_graph_idct_8x8(jpeg_info_data, 1);
				}


				//------------ Cr ------------
				iBlockV = (l_cbcr_in & 0xffffffff) >> 10;
				iBlockH = (l_cbcr_in & 0x3e0);
				//DEBUG("mcu = %d, k = 2, y = 0, x = 0, vld_addr = 0x%08x\n", j*jpeg_info_data->jpegDecode.horizontal_mcu+i, l_cbcr_in);

				//for hw ps jpeg 422(221212)
				if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
					l_cbcr_in += jpeg_info_data->jpegDecode.mcu_horizontal_mb * 64 * 2 * 2; // last * 2 for 16-bits data
				else
					l_cbcr_in += 64 * 2; // last * 2 for 16-bits data

				//IMGDECSND_DMA_wait();
				IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
				IMGDECSND_DMA_wait();

				if(g_imgdecsnd_jr_reg_info.pre_scaler)  // (pre_scaler & 0x4)
				{
					// set output address of IDCT&invq
					g_pImgDecSndReg->jdm.jdm_jpeg_offset = 88;
					IMGDECSND_JPEG_graph_idct_8x8(jpeg_info_data, 2);

					iBlockV = (l_cbcr_in & 0xffffffff) >> 10;
					iBlockH = (l_cbcr_in & 0x3e0);

					//for hw ps jpeg 422(221212)
					if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
						l_cbcr_in -= jpeg_info_data->jpegDecode.mcu_horizontal_mb * 64 * 2 * 2; // last * 2 for 16-bits data
					l_cbcr_in += 64 * 2; // last * 2 for 16-bits data

					//IMGDECSND_DMA_wait();
					IMGDECSND_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
					IMGDECSND_DMA_wait();

					g_pImgDecSndReg->jdm.jdm_jpeg_offset = 92;
					IMGDECSND_JPEG_graph_idct_8x8(jpeg_info_data, 2);
				}
				else
				{
					// set output address of IDCT&invq
					g_pImgDecSndReg->jdm.jdm_jpeg_offset = 88;
					IMGDECSND_JPEG_graph_idct_8x8(jpeg_info_data, 2);
				}
			}

			// render
			IMGDECSND_JPEG_scaler_1_mcu((scale_b_x/jpeg_info_data->jpegDecode.sof_Hi_0), (scale_b_y/jpeg_info_data->jpegDecode.sof_Vi_0));

			//when user stop decoding or decode is suspend or error, after huffman close, need to check JR is done.
			time_out = 10000; // 10000 us = 10 ms by letb123 PCB test
			do
			{
				if(time_out == 0)
				{
					ERR("Wait JR finish: time out!");
					return -1;
				}

				udelay(1);

				jr_status = g_pImgDecSndReg->huf.jr_status;
				//DEBUG("Wait JR finish: jr_status = 0x%08x\n", jr_status);

				time_out--;
			}while(!(jr_status & 0x40));
			
			// updata next x, y address
			scale_b_x += jpeg_info_data->jpegDecode.sof_Hi_0;
			if(scale_b_x >= jpeg_info_data->jpegDecode.mcu_horizontal_b)
			{
				scale_b_x = 0;
				scale_b_y += jpeg_info_data->jpegDecode.sof_Vi_0;
			}
		}
	}

	return 0;
}
EXPORT_SYMBOL(imgdecsnd_hal_jpeg_decode_ms_scale);


