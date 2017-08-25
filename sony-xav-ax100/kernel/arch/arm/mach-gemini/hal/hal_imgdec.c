
#include <linux/delay.h>
#include <mach/kernel.h>
#include <mach/hal/hal_imgdec.h>

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

JR_FILTER_COEF g_imgdec_jr_coef_tbl[18] = {
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

static const UINT8 g_uiCodeLength[PNG_LEN_SIZE] = {
16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

void *g_imgdec_pHWRegBase = NULL;
void *g_imgdec_pDataRamBase = NULL;
IMG_DEC_REG *g_pImgDecReg = NULL;
JR_REG_INFO g_imgdec_jr_reg_info;

wait_queue_head_t g_imgdec_MJmode_interrupt_wait; // MJmode uses interrupt to replace polling
unsigned char g_imgdec_MJmode_interrupt_done; // MJmode uses interrupt to replace polling

/**************************************************************************
 *             F U N C T I O N    I M P L E M E N T A T I O N S           *
 **************************************************************************/

void IMGDEC_jr_set_port(
	UINT8 addr,
	UINT32 data)
{
	g_pImgDecReg->huf.jr_reg_data = data & 0xffff;
	g_pImgDecReg->huf.jr_reg_num = (UINT32)addr;
	g_pImgDecReg->huf.jr_reg_num = g_pImgDecReg->huf.jr_reg_num | 0x8000;
	g_pImgDecReg->huf.jr_reg_num = 0;
}

UINT16 IMGDEC_jr_get_port(
	UINT8 addr)
{
	UINT16 value;
	g_pImgDecReg->huf.jr_reg_num = (UINT32)addr;
	g_pImgDecReg->huf.jr_reg_num = g_pImgDecReg->huf.jr_reg_num | 0x4000;
	value = (UINT16)(g_pImgDecReg->huf.jr_reg_data & 0xffff);
	g_pImgDecReg->huf.jr_reg_num = 0;
	return value;
}

void IMGDEC_jr_load_coef_table(
	UINT32 downsample_rate_w,
	UINT32 downsample_rate_h)
{
	// horizontal
	if(downsample_rate_w > 1024)
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[0];
	}
	else if(downsample_rate_w == 1024)
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[1];
	}
	else if((downsample_rate_w < 1024) && (downsample_rate_w >= 960))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[2];
	}
	else if((downsample_rate_w < 960) && (downsample_rate_w >= 900))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[3];
	}
	else if((downsample_rate_w < 900) && (downsample_rate_w >= 850))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[4];
	}
	else if((downsample_rate_w < 850) && (downsample_rate_w >= 819))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[5];
	}
	else if((downsample_rate_w < 819) && (downsample_rate_w >= 768))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[6];
	}
	else if((downsample_rate_w < 768) && (downsample_rate_w >= 725))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[7];
	}
	else if((downsample_rate_w < 725) && (downsample_rate_w >= 640))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[8];
	}
	else if((downsample_rate_w < 640) && (downsample_rate_w >= 585))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[9];
	}
	else if((downsample_rate_w < 585) && (downsample_rate_w >= 511))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[10];
	}
	else if((downsample_rate_w < 511) && (downsample_rate_w >= 455))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[11];
	}
	else if((downsample_rate_w < 455) && (downsample_rate_w >= 372))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[12];
	}
	else if((downsample_rate_w < 372) && (downsample_rate_w >= 315))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[13];
	}
	else if((downsample_rate_w < 315) && (downsample_rate_w >= 255))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[14];
	}
	else if((downsample_rate_w < 255) && (downsample_rate_w >= 195))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[15];
	}
	else if((downsample_rate_w < 195) && (downsample_rate_w >= 128))
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[16];
	}
	else
	{
		g_imgdec_jr_reg_info.h_coef = &g_imgdec_jr_coef_tbl[17];
	}

	// vertical
	if(downsample_rate_h > 1024)
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[0];
	}
	else if(downsample_rate_h == 1024)
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[1];
	}
	else if((downsample_rate_h < 1024) && (downsample_rate_h >= 960))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[2];
	}
	else if((downsample_rate_h < 960) && (downsample_rate_h >= 900))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[3];
	}
	else if((downsample_rate_h < 900) && (downsample_rate_h >= 850))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[4];
	}
	else if((downsample_rate_h < 850) && (downsample_rate_h >= 819))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[5];
	}
	else if((downsample_rate_h < 819) && (downsample_rate_h >= 768))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[6];
	}
	else if((downsample_rate_h < 768) && (downsample_rate_h >= 725))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[7];
	}
	else if((downsample_rate_h < 725) && (downsample_rate_h >= 640))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[8];
	}
	else if((downsample_rate_h < 640) && (downsample_rate_h >= 585))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[9];
	}
	else if((downsample_rate_h < 585) && (downsample_rate_h >= 511))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[10];
	}
	else if((downsample_rate_h < 511) && (downsample_rate_h >= 455))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[11];
	}
	else if((downsample_rate_h < 455) && (downsample_rate_h >= 372))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[12];
	}
	else if((downsample_rate_h < 372) && (downsample_rate_h >= 315))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[13];
	}
	else if((downsample_rate_h < 315) && (downsample_rate_h >= 255))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[14];
	}
	else if((downsample_rate_h < 255) && (downsample_rate_h >= 195))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[15];
	}
	else if((downsample_rate_h < 195) && (downsample_rate_h >= 128))
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[16];
	}
	else
	{
		g_imgdec_jr_reg_info.v_coef = &g_imgdec_jr_coef_tbl[17];
	}
}

void IMGDEC_jr_pre_setup(
	JPEG_INFO *jpeg_info_data,
	PNG_INFO *png_info_data,
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
	IMAGE_TYPE image_type;

	if(jpeg_info_data->jpegDecode.output_run_time > 0)
	{
		image_type = IMAGE_JPEG;
		output_run_time = jpeg_info_data->jpegDecode.output_run_time;
	}
	else if(png_info_data->pngDecode.output_run_time > 0)
	{
		image_type = IMAGE_PNG;
		output_run_time = png_info_data->pngDecode.output_run_time;
	}
	else
	{
		image_type = IMAGE_JPEG;
		output_run_time = 1;
		WARNING("output_run_time = 0 in jpegDecode and pngDecode !!!");
	}

	g_imgdec_jr_reg_info.temp_buf_onLuma = 1;
	g_imgdec_jr_reg_info.rgb_output_onChroma = 0;
	g_imgdec_jr_reg_info.odd_symm_filter = 0;
	g_imgdec_jr_reg_info.rgb_input_onChroma = 0;
	g_imgdec_jr_reg_info.bw_file = 0;
	g_imgdec_jr_reg_info.yuv2RGB_flag = 0;

	if((output_run_time & 0x0f) > 1) // bit[3:0] = total run times
		g_imgdec_jr_reg_info.pd_enable = 1;
	else
		g_imgdec_jr_reg_info.pd_enable = 0;

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

	g_imgdec_jr_reg_info.luma_start = outbuf_phyaddr;
	g_imgdec_jr_reg_info.chroma_start = outUVbuf_phyaddr;
	g_imgdec_jr_reg_info.temp_buffer = working_buf_phyaddr;

	g_imgdec_jr_reg_info.render_input_offset = JR_DMA_BUF_BASE;
	g_imgdec_jr_reg_info.orientation = ((decode_info_data->render_type & 0xe000) >> 13); // for orthogonal orientation (EXIF) // 0xe000 for orientation
	g_imgdec_jr_reg_info.rgb_swap = decode_info_data->render_endian; // 0(Big Endian) or 1(Little Endian) only for RGB domain
	g_imgdec_jr_reg_info.alpha_part = 0xff; // 0~0xff for RGBA/ARGB
	g_imgdec_jr_reg_info.frame_width  = decode_info_data->outbuf_width;
	g_imgdec_jr_reg_info.frame_height = decode_info_data->outbuf_height;
	if((decode_info_data->render_type & 0xf) == RENDER_YUV420) // 0xf for orientation
		g_imgdec_jr_reg_info.dis_color_mode = DIS_YUV420;
	else if((decode_info_data->render_type & 0xf) == RENDER_RGB565) // 0xf for orientation
		g_imgdec_jr_reg_info.dis_color_mode = DIS_RGB565;
	else if((decode_info_data->render_type & 0xf) == RENDER_RGBA8888) // 0xf for orientation
		g_imgdec_jr_reg_info.dis_color_mode = DIS_RGBA8888;
	else if((decode_info_data->render_type & 0xf) == RENDER_ARGB8888) // 0xf for orientation
		g_imgdec_jr_reg_info.dis_color_mode = DIS_ARGB8888;
	else
		g_imgdec_jr_reg_info.dis_color_mode = DIS_RGBA8888;

	if(image_type == IMAGE_PNG)
	{
		// set source size and color mode
		g_imgdec_jr_reg_info.src_width = png_info_data->pngIHDR.width;
		g_imgdec_jr_reg_info.src_height = png_info_data->pngIHDR.height;
		g_imgdec_jr_reg_info.src_color_mode = SRC_ARGB8888;
		g_imgdec_jr_reg_info.alpha_scaling = 1;

		// set pre_scaler
		g_imgdec_jr_reg_info.pre_scaler = 0;

		// set auto_fetch
		g_imgdec_jr_reg_info.auto_fetch_enable = 1;

		// set down_sample_rate: 1024*1024 = 1x, 512*1024 = 0.5x, 2048*1024 = 2x (*1024 is for accuracy in scaler_index)
		// render_width and render_height must < 2^12=4096 for total 32 bits in UINT32
		if((((decode_info_data->render_type & 0xe000) >> 13) == RIGHT_ROTATE_90) | (((decode_info_data->render_type & 0xe000) >> 13) == LEFT_ROTATE_90)) // for orientation
		{
			downsample_rate_w = (decode_info_data->render_height << 20)/png_info_data->pngIHDR.width;
			downsample_rate_h = (decode_info_data->render_width << 20)/png_info_data->pngIHDR.height;
		}
		else
		{
			downsample_rate_w = (decode_info_data->render_width << 20)/png_info_data->pngIHDR.width;
			downsample_rate_h = (decode_info_data->render_height << 20)/png_info_data->pngIHDR.height;
		}

		//g_imgdec_jr_reg_info.alphascale_enable = 1; // alphascale_enable ??????
	}
	else // if(image_type == IMAGE_JPEG)
	{
		if(jpeg_info_data->jpegSOF.components == 1)
			g_imgdec_jr_reg_info.bw_file = 1;

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
		g_imgdec_jr_reg_info.src_width  = w_align;
		g_imgdec_jr_reg_info.src_height = h_align;
		g_imgdec_jr_reg_info.src_color_mode = src_type;
		g_imgdec_jr_reg_info.alpha_scaling = 0;

		// set pre_scaler
		if(jpeg_info_data->jpegSOF.Hfactor[1] == 2)			hw_info_pre_scaler = PSCTRL_EN & CHROMA_PS_H;
		else if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)	hw_info_pre_scaler = PSCTRL_EN & CHROMA_PS_V;
		else if(jpeg_info_data->jpegSOF.Hfactor[0] == 4)	hw_info_pre_scaler = PSCTRL_EN & LUMA_PS_H;
		else if(jpeg_info_data->jpegSOF.Vfactor[0] == 4)	hw_info_pre_scaler = PSCTRL_EN & LUMA_PS_V;
		else hw_info_pre_scaler = 0;
		g_imgdec_jr_reg_info.pre_scaler = hw_info_pre_scaler;

		// set auto_fetch
		g_imgdec_jr_reg_info.auto_fetch_enable = 0;

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
	}

	// JR scaler only supports the scaling range from x1/1024 to x1024 (0x1<<10 ~ 0x1<<30).
	if(downsample_rate_w == 0)	downsample_rate_w = 1024;
	if(downsample_rate_w > (0x1<<30))	downsample_rate_w = (0x1<<30);
	if(downsample_rate_h == 0)		downsample_rate_h = 1024;
	if(downsample_rate_h > (0x1<<30))	downsample_rate_h = (0x1<<30);

	g_imgdec_jr_reg_info.scaler_index_h = (0x1<<30)/downsample_rate_w; // 1024 = 1x, 512 = 2x, 2048 = 0.5x
	g_imgdec_jr_reg_info.scaler_index_v = (0x1<<30)/downsample_rate_h; // 1024 = 1x, 512 = 2x, 2048 = 0.5x

	downsample_rate_w >>= 10; // 1 <= downsample_rate_w <= 1024*1024(=0x1<<20)
	downsample_rate_h >>= 10; // 1 <= downsample_rate_h <= 1024*1024(=0x1<<20)

	if((downsample_rate_w > 1024) || (downsample_rate_h > 1024))
		g_imgdec_jr_reg_info.hd_color_space = 1;
	else
		g_imgdec_jr_reg_info.hd_color_space = 0;

	if(image_type == IMAGE_PNG)
	{
		g_imgdec_jr_reg_info.bypass_enable = 0; // bypass_enable = 1 would set all alpha values = alpha_part
	}
	else // if(image_type == IMAGE_JPEG)
	{
		if((g_imgdec_jr_reg_info.scaler_index_h == 1024) && (g_imgdec_jr_reg_info.scaler_index_v == 1024))
			g_imgdec_jr_reg_info.bypass_enable = 1;
		else
			g_imgdec_jr_reg_info.bypass_enable = 0;
	}

	if((decode_info_data->render_type & 0xe000) >> 13) // for orientation
		g_imgdec_jr_reg_info.bypass_enable = 0;

	g_imgdec_jr_reg_info.disp_box_h_bound = ((decode_info_data->render_width + 0xf) & 0xfffffff0) >> 1; // (>> 1) for unit in 2 pixels
	g_imgdec_jr_reg_info.disp_box_v_bound = ((decode_info_data->render_height + 0xf) & 0xfffffff0) >> 1; // (>> 1) for unit in 2 pixels

	if((((decode_info_data->render_type & 0xe000) >> 13) == RIGHT_ROTATE_90) | (((decode_info_data->render_type & 0xe000) >> 13) == LEFT_ROTATE_90)) // for orientation
	{
		h_output_len = ((1024 * (g_imgdec_jr_reg_info.src_height - 1))/g_imgdec_jr_reg_info.scaler_index_h) + 1;
		v_output_len = ((1024 * (g_imgdec_jr_reg_info.src_width - 1))/g_imgdec_jr_reg_info.scaler_index_v) + 1;
	}
	else
	{
		h_output_len = ((1024 * (g_imgdec_jr_reg_info.src_width - 1))/g_imgdec_jr_reg_info.scaler_index_h) + 1;
		v_output_len = ((1024 * (g_imgdec_jr_reg_info.src_height - 1))/g_imgdec_jr_reg_info.scaler_index_v) + 1;
	}

	if(g_imgdec_jr_reg_info.scaler_index_h < 1024)
		g_imgdec_jr_reg_info.h_filter_phase = (~((((g_imgdec_jr_reg_info.disp_box_h_bound << 1) - h_output_len) * g_imgdec_jr_reg_info.scaler_index_h)/2) + 1) & 0x7ff;
	else if(g_imgdec_jr_reg_info.scaler_index_h == 1024)
		g_imgdec_jr_reg_info.h_filter_phase = 1;
	else
		g_imgdec_jr_reg_info.h_filter_phase = 715;

	if(g_imgdec_jr_reg_info.scaler_index_v < 1024)
		g_imgdec_jr_reg_info.v_filter_phase = (~((((g_imgdec_jr_reg_info.disp_box_v_bound << 1) - v_output_len) * g_imgdec_jr_reg_info.scaler_index_v)/2) + 1) & 0x7ff;
	else if(g_imgdec_jr_reg_info.scaler_index_v == 1024)
		g_imgdec_jr_reg_info.v_filter_phase = 1;
	else
		g_imgdec_jr_reg_info.v_filter_phase = 711;

	// tuning JR coefficient for quality
	IMGDEC_jr_load_coef_table(downsample_rate_w, downsample_rate_h);

}

void IMGDEC_jr_write_h_coef(
	void)
{
    UINT32 start_maddr;
    UINT32 tab_offset;

    tab_offset = 0;
    for(start_maddr=JR_COE_H_SET_1_START; start_maddr<JR_COE_H_SET_1_END; start_maddr++)
    {
        IMGDEC_jr_set_port(start_maddr, g_imgdec_jr_reg_info.h_coef->coef_set_1[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_H_SET_2_START; start_maddr<JR_COE_H_SET_2_END; start_maddr++)
    {
        IMGDEC_jr_set_port(start_maddr, g_imgdec_jr_reg_info.h_coef->coef_set_2[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_H_SET_3_START; start_maddr<JR_COE_H_SET_3_END; start_maddr++)
    {
        IMGDEC_jr_set_port(start_maddr, g_imgdec_jr_reg_info.h_coef->coef_set_3[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_H_SET_4_START; start_maddr<JR_COE_H_SET_4_END; start_maddr++)
    {
        IMGDEC_jr_set_port(start_maddr, g_imgdec_jr_reg_info.h_coef->coef_set_4[tab_offset]);
        tab_offset ++;
    }
}


void IMGDEC_jr_write_v_coef(
	void)
{
    UINT32 start_maddr;
    UINT32 tab_offset;

    tab_offset = 0;
    for(start_maddr=JR_COE_V_SET_1_START; start_maddr<JR_COE_V_SET_1_END; start_maddr++)
    {
        IMGDEC_jr_set_port(start_maddr, g_imgdec_jr_reg_info.v_coef->coef_set_1[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_V_SET_2_START; start_maddr<JR_COE_V_SET_2_END; start_maddr++)
    {
        IMGDEC_jr_set_port(start_maddr, g_imgdec_jr_reg_info.v_coef->coef_set_2[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_V_SET_3_START; start_maddr<JR_COE_V_SET_3_END; start_maddr++)
    {
        IMGDEC_jr_set_port(start_maddr, g_imgdec_jr_reg_info.v_coef->coef_set_3[tab_offset]);
        tab_offset ++;
    }
    tab_offset = 0;
    for(start_maddr=JR_COE_V_SET_4_START; start_maddr<JR_COE_V_SET_4_END; start_maddr++)
    {
        IMGDEC_jr_set_port(start_maddr, g_imgdec_jr_reg_info.v_coef->coef_set_4[tab_offset]);
        tab_offset ++;
    }
}

void IMGDEC_jr_write_coef_table( // tuning JR coefficient for quality
	void)
{
	IMGDEC_jr_set_port(JR_WRITE_COE_CHROMA, 0x0000); // JR_WRITE_COE_CHROMA = 0: Write luma filter coefficients
	IMGDEC_jr_write_v_coef();
	IMGDEC_jr_write_h_coef();

	IMGDEC_jr_set_port(JR_WRITE_COE_CHROMA, 0x0001); // JR_WRITE_COE_CHROMA = 1: Write chroma filter coefficients
	IMGDEC_jr_write_v_coef();
	IMGDEC_jr_write_h_coef();

	IMGDEC_jr_set_port(JR_WRITE_COE_CHROMA, 0x0000);
}

void IMGDEC_DMA_load(
	UINT32 AddrMode,
	UINT32 VAddr,
	UINT32 HAddr,
	UINT32 DAddr,
	UINT32 Len)
{
	g_pImgDecReg->jdm.jdm_dma1_mode = (Len << 8) | (AddrMode & 0x0f);
	g_pImgDecReg->jdm.jdm_dma1_start_addr = (DAddr >> 2) & 0x7ff;
	g_pImgDecReg->jdm.jdm_dma1_xaddr = ((VAddr & 0xffff0000) >> 6) | HAddr;
	g_pImgDecReg->jdm.jdm_dma1_yaddr = VAddr & 0xffff;
}

void IMGDEC_DMA_save(
	UINT32 AddrMode,
	UINT32 VAddr,
	UINT32 HAddr,
	UINT32 DAddr,
	UINT32 Len)
{
	g_pImgDecReg->jdm.jdm_dma1_mode = (Len << 8) | (1 << 5) | (AddrMode & 0x0f);
	g_pImgDecReg->jdm.jdm_dma1_start_addr = (DAddr >> 2) & 0x7ff;
	g_pImgDecReg->jdm.jdm_dma1_xaddr = ((VAddr & 0xffff0000) >> 6) | HAddr;
	g_pImgDecReg->jdm.jdm_dma1_yaddr = VAddr & 0xffff;
}

void IMGDEC_DMA_clear(
	UINT32 AddrMode,
	UINT32 DAddr,
	UINT32 Len)
{
	g_pImgDecReg->jdm.jdm_dma1_mode = (Len << 8) | (AddrMode & 0x0f);
	g_pImgDecReg->jdm.jdm_dma1_start_addr = (DAddr >> 2) & 0x7ff;
	g_pImgDecReg->jdm.jdm_dma1_xaddr = 0;
	g_pImgDecReg->jdm.jdm_dma1_yaddr = 0;
}

void IMGDEC_DMA_wait(
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

		jdm_dma1_done = g_pImgDecReg->jdm.jdm_dma1_done;
		//DEBUG("jdm_dma1_done = 0x%08x\n", jdm_dma1_done);

		time_out--;
	}while(!(jdm_dma1_done & 0x1));
}

void IMGDEC_JPEG_fill_DQT_reg(
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
				g_pImgDecReg->invq.jpeg_invq_qmx_par = (addr << 8) | (UINT32)jpeg_info_data->jpegDQT.Qs[i][j];
			else if(jpeg_info_data->jpegDQT.Pq[i]  == 1) // for 16bit DQT, remove high byte
				g_pImgDecReg->invq.jpeg_invq_qmx_par = (addr << 8) | (UINT32)(jpeg_info_data->jpegDQT.Qs[i][j] & 0x00ff);
		}
	}
}

void IMGDEC_JPEG_fill_DHT_reg(
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
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_mincode[table_id][i];
				base_address += 0x100;
			}
			// offset
			for(i=0; i<8; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_offset[table_id][i];
				base_address += 0x100;
			}
			// value
			for(i=0; i<8; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_value[table_id][i];
				base_address += 0x100;
			}
			break;
		case DHT_DC1:
			base_address = 0x30 << 8;
			// min codeword
			for(i=0; i<24; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_mincode[table_id][i];
				base_address += 0x100;
			}
			// offset
			for(i=0; i<8; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_offset[table_id][i];
				base_address += 0x100;
			}
			// value
			for(i=0; i<8; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.DC_value[table_id][i];
				base_address += 0x100;
			}
			break;
		case DHT_AC0:
			base_address = 0x80 << 8;
			// min codeword
			for(i=0; i<24; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.AC_mincode[table_id][i];
				base_address += 0x100;
			}
			// offset
			for(i=0; i<16; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.AC_offset[table_id][i];
				base_address += 0x100;
			}
			DMA_address = 0;
			// value (write to DMA)
			for(i=0; i<jpeg_info_data->jpegDHT.huff_ac_size; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_acval_0 = DMA_address | (UINT32)jpeg_info_data->jpegDHT.AC_value[table_id][i];
				DMA_address += 0x100;
			}
			break;
		case DHT_AC1:
			base_address = 0xB0 << 8;
			// min codeword
			for(i=0; i<24; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.AC_mincode[table_id][i];
				base_address += 0x100;
			}
			// offset
			for(i=0; i<16; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = base_address | (UINT32)jpeg_info_data->jpegDHT.AC_offset[table_id][i];
				base_address += 0x100;
			}
			DMA_address = 0;
			// value (write to DMA)
			for(i=0; i<jpeg_info_data->jpegDHT.huff_ac_size; i++)
			{
				g_pImgDecReg->huf.jpeg_huff_wr_acval_1 = DMA_address | (UINT32)jpeg_info_data->jpegDHT.AC_value[table_id][i];
				DMA_address += 0x100;
			}
			break;
		default:
			break;
	}
}

void IMGDEC_JPEG_graph_idct_8x8(
	JPEG_INFO *jpeg_info_data,
	UINT32 component)
{
	UINT32 jdm_mode;
	UINT32 time_out = 1000; // 1000 us = 1 ms

	g_pImgDecReg->invq.jpeg_invq_mode = ((UINT32)jpeg_info_data->jpegSOF.Qtable[component] << 2) | INVQ_JPEG_MODE | INVQ_JPEG_NOBYPASS;

	// start to decode
	if(jpeg_info_data->jpegSOF.sof_n == 2) // progressive JPEG
	{
		g_pImgDecReg->jdm.jdm_mode = GRAPH_JPEG | GRAPH_JPEG_SAT | GRAPH_JPEG_OUT_8;
	}
	else // baseline JPEG
	{
		g_pImgDecReg->jdm.jdm_mode = GRAPH_JPEG | GRAPH_JPEG_SAT | GRAPH_JPEG_OUT_8 | GRAPH_JPEG_32;
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

		jdm_mode = g_pImgDecReg->jdm.jdm_mode;
		//DEBUG("jdm_mode = 0x%08x\n", jdm_mode);

		time_out--;
	}while(jdm_mode & (1<<4));

	g_pImgDecReg->invq.jpeg_invq_mode = 0;
}//graph_idct_8x8

void IMGDEC_JPEG_scaler_1_mcu(
	INT32 mcu_x,
	INT32 mcu_y)
{
	//DEBUG("mcu_x = %d, mcu_y = %d\n", mcu_x, mcu_y);

	IMGDEC_jr_set_port(JR_MCU_X, mcu_x);
	IMGDEC_jr_set_port(JR_MCU_Y, mcu_y);

	g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control & 0xffef;
	g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control | 0x10;
	g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control & 0xffef;
}

//---------------------------------------------------------------

void IMGDEC_PNG_fill_palette(
	UINT8 *pColorMap)
{
	// Xilinx 7045 has not CBUS, CPU can't write PNG HW palette table into DMA_BUF
#if 0
	UINT32 i, palette_data;
	UINT32 *DMA_BUF = (UINT32 *)g_imgdec_pDataRamBase;

	// DMA_BUF0, from 0~63, 64 words
	for(i=0; i<64; i++)
	{
		//palette_data = (palette[i] << 8) | 0xff;
		palette_data = ((UINT32)pColorMap[3 * i + 2] << 24) |
					((UINT32)pColorMap[3 * i + 1] << 16) |
					((UINT32)pColorMap[3 * i] << 8) | 0xff;

		DMA_BUF[192 + i] = palette_data;
	}

	// DMA_BUF1, from 16~127, 112 words
	for(i=0; i<112; i++)
	{
		//palette_data = (palette[64+i] << 8) | 0xff;
		palette_data = ((UINT32)pColorMap[3 * (64 + i) + 2] << 24) |
					((UINT32)pColorMap[3 * (64 + i) + 1] << 16) |
					((UINT32)pColorMap[3 * (64 + i)] << 8) | 0xff;

		DMA_BUF[256 + 16 + i] = palette_data;
	}

	// DMA_BUF2, from 0~79, 80 words
	for(i=0; i<80; i++)
	{
		//palette_data = (palette[64+112+i] << 8) | 0xff;
		palette_data = ((UINT32)pColorMap[3 * (64 + 112 + i) + 2] << 24) |
					((UINT32)pColorMap[3 * (64 + 112 + i) + 1] << 16) |
					((UINT32)pColorMap[3 * (64 + 112 + i)] << 8) | 0xff;

		DMA_BUF[384 + i] = palette_data;
	}
#else // use JDM to move palette data from DRAM to DMA_BUF
	UINT8 *palette_chunk_buf = NULL;
	UINT32 size = 1088; // 1088 = ((64 + 112 + 80) * 4) + (32 * 2), here (32 * 2) for 32-bytes-align in jdm_dma1_xaddr
	UINT32 i, offset, pos;
	UINT32 palette_chunk_buf_phyaddr;
	UINT32 VAddr, HAddr, DAddr;

	palette_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
	if(palette_chunk_buf == NULL)
	{
		WARNING("gp_chunk_malloc_nocache(palette_chunk_buf) and IMGDEC_PNG_fill_palette fail!\n");
		return;
	}
	else
	{
		//DEBUG("palette_chunk_buf.addr=0x%08x, palette_chunk_buf.phy_addr=0x%08x\n",
		//	(UINT32)palette_chunk_buf, gp_chunk_pa(palette_chunk_buf));
		memset(palette_chunk_buf, 0, size);
	}

	offset = gp_chunk_pa(palette_chunk_buf) & 0x1f;
	if(offset != 0)		offset = 32 - offset;
	palette_chunk_buf_phyaddr = gp_chunk_pa(palette_chunk_buf) + offset;

	for(i=0; i<256; i++) // 256 = 64 + 112 + 80
	{
		// palette_data = 0xff | (RGB << 8);
		// big-endian in palette_chunk_buf
		pos = 3 * i;
		palette_chunk_buf[offset++] = 0xff; // A
		palette_chunk_buf[offset++] = pColorMap[pos]; // R
		palette_chunk_buf[offset++] = pColorMap[pos + 1]; // G
		palette_chunk_buf[offset++] = pColorMap[pos + 2]; // B
	}

	DAddr = 192 << 2;
	// DMA_BUF0, from 0~63, 64 words = 4 x 64 bytes
	for(i=0; i<4; i++)
	{
		VAddr = (palette_chunk_buf_phyaddr & 0xffffffff) >> 10; // buf_phyaddr[32:10]
		HAddr = (palette_chunk_buf_phyaddr & 0x3e0); // buf_phyaddr[9:5]
		IMGDEC_DMA_load(VADDR_MODE_GLOBAL, VAddr, HAddr, DAddr, DMA_LEN_64);
		IMGDEC_DMA_wait();
		palette_chunk_buf_phyaddr += 64;
		DAddr += 64;
	}

	DAddr = (256 + 16) << 2;
	// DMA_BUF1, from 16~127, 112 words = 7 x 64 bytes
	for(i=0; i<7; i++)
	{
		VAddr = (palette_chunk_buf_phyaddr & 0xffffffff) >> 10; // buf_phyaddr[32:10]
		HAddr = (palette_chunk_buf_phyaddr & 0x3e0); // buf_phyaddr[9:5]
		IMGDEC_DMA_load(VADDR_MODE_GLOBAL, VAddr, HAddr, DAddr, DMA_LEN_64);
		IMGDEC_DMA_wait();
		palette_chunk_buf_phyaddr += 64;
		DAddr += 64;
	}

	DAddr = 384 << 2;
	// DMA_BUF2, from 0~79, 80 words = 5 x 64 bytes
	for(i=0; i<5; i++)
	{
		VAddr = (palette_chunk_buf_phyaddr & 0xffffffff) >> 10; // buf_phyaddr[32:10]
		HAddr = (palette_chunk_buf_phyaddr & 0x3e0); // buf_phyaddr[9:5]
		IMGDEC_DMA_load(VADDR_MODE_GLOBAL, VAddr, HAddr, DAddr, DMA_LEN_64);
		IMGDEC_DMA_wait();
		palette_chunk_buf_phyaddr += 64;
		DAddr += 64;
	}

	gp_chunk_free(palette_chunk_buf);
#endif
}

void IMGDEC_PNG_pull_bs_byte(
	PNG_INFO *png_info_data,
	UINT8 *bs_buf_ptr)
{
	if(png_info_data->pngIDAT.chunk_remain_bytes == 0)
	{
		png_info_data->pngIDAT.chunk_length = ((UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 4] << 24) |
											((UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 5] << 16) |
											((UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 6] << 8) |
											(UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 7];
		png_info_data->pngIDAT.chunk_remain_bytes = png_info_data->pngIDAT.chunk_length;
		png_info_data->pngIDAT.bs_byte_offset += 12;
	}
	png_info_data->pngIDAT.chunk_remain_bytes -= 1;
	png_info_data->pngIDAT.hold_bits += (UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset] << png_info_data->pngIDAT.bs_bit_offset;
	png_info_data->pngIDAT.bs_bit_offset += 8;
	png_info_data->pngIDAT.bs_byte_offset += 1;
}

void IMGDEC_PNG_need_bs_bits(
	PNG_INFO *png_info_data,
	UINT8 *bs_buf_ptr,
	UINT32 need_bits)
{
	while(png_info_data->pngIDAT.bs_bit_offset < need_bits)
	{
		if(png_info_data->pngIDAT.chunk_remain_bytes == 0)
		{
			png_info_data->pngIDAT.chunk_length = ((UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 4] << 24) |
												((UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 5] << 16) |
												((UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 6] << 8) |
												(UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 7];
			png_info_data->pngIDAT.chunk_remain_bytes = png_info_data->pngIDAT.chunk_length;
			png_info_data->pngIDAT.bs_byte_offset += 12;
		}
		png_info_data->pngIDAT.chunk_remain_bytes -= 1;
		png_info_data->pngIDAT.hold_bits += (UINT32)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset] << png_info_data->pngIDAT.bs_bit_offset;
		png_info_data->pngIDAT.bs_bit_offset += 8;
		png_info_data->pngIDAT.bs_byte_offset += 1;
	}
}

UINT32 IMGDEC_PNG_get_bs_bits(
	PNG_INFO *png_info_data,
	UINT32 len)
{
	return (UINT32)(png_info_data->pngIDAT.hold_bits & ((0x1 << len) - 1));
}

void IMGDEC_PNG_drop_bs_bits(
	PNG_INFO *png_info_data,
	UINT32 len)
{
	png_info_data->pngIDAT.hold_bits >>= len;
	png_info_data->pngIDAT.bs_bit_offset -= len;
}

void IMGDEC_PNG_init_bs_bits(
	PNG_INFO *png_info_data)
{
	png_info_data->pngIDAT.hold_bits = 0;
	png_info_data->pngIDAT.bs_bit_offset = 0;
}

void IMGDEC_PNG_inflate_table(
	PNG_INFO *png_info_data,
	UINT32 codes)
{
	UINT32 i, j, k;
	UINT16 size;
	UINT16 code;

	// accumulate lengths for codes (assumes lens[] all in 0~15)
	for(i=0; i<16; i++)
		png_info_data->pngIDAT.count[i] = 0;

	for(i=0; i<codes; i++)
		png_info_data->pngIDAT.count[png_info_data->pngIDAT.lens[i]] += 1;

	// generate offsets into symbol table for each length for sorting
	png_info_data->pngIDAT.offset[1] = 0;
	for(i=1; i<15; i++)
		png_info_data->pngIDAT.offset[i + 1] = png_info_data->pngIDAT.offset[i] + png_info_data->pngIDAT.count[i];

	// record the number of symbols with zero length
	png_info_data->pngIDAT.zero_length = 0;

	// sort symbols by length, by symbol order within each length
	for(i=0; i<codes; i++)
	{
		if(png_info_data->pngIDAT.lens[i] != 0)
		{
			png_info_data->pngIDAT.sym_table[png_info_data->pngIDAT.offset[png_info_data->pngIDAT.lens[i]]] = i;
			png_info_data->pngIDAT.offset[png_info_data->pngIDAT.lens[i]] += 1;
		}
		else
			png_info_data->pngIDAT.zero_length += 1;
	}

	// generate look ahead table
	for(i=0; i<288; i++)
		png_info_data->pngIDAT.huffcode[i] = 0;

	for (i=0; i<16; i++)
	{
		png_info_data->pngIDAT.mincode[i] = 0;
		png_info_data->pngIDAT.valptr[i] = 0;
	}

	// Figure C.1: Generate Huffman code sizes
	k = 0;
	for(j=1; j<16; j++)
	{
		for(i=1; i<=png_info_data->pngIDAT.count[j]; i++)
		{
			png_info_data->pngIDAT.huffsize[k] = j;
			k++;
		}
	}
	png_info_data->pngIDAT.huffsize[k] = 0;

	// Figure C.2: Generate Huffman codes
	code = 0;
	size =png_info_data->pngIDAT.huffsize[0];
	k = 0;
	while(png_info_data->pngIDAT.huffsize[k])
	{
		while(png_info_data->pngIDAT.huffsize[k] == size)
		{
			png_info_data->pngIDAT.huffcode[k] = code;
			k++;
			code++;

			if(k >= 288)
				break;
		}
		code <<= 1;
		size++;

		if((k >= 288) || (png_info_data->pngIDAT.huffsize[k] < size))
			break;
	}

	// Figure F.15: Decode table generation
	k = 0;
	code = 0;
	for(j=1; j<16; j++)
	{
		if(png_info_data->pngIDAT.count[j])
		{
			png_info_data->pngIDAT.valptr[j] = k;
			png_info_data->pngIDAT.mincode[j] = png_info_data->pngIDAT.huffcode[k];
			code = png_info_data->pngIDAT.huffcode[k] + png_info_data->pngIDAT.count[j];
			k += png_info_data->pngIDAT.count[j];
		}
		else
		{
			code <<= 1;
			png_info_data->pngIDAT.mincode[j] = code;
			png_info_data->pngIDAT.valptr[j] = k;
		}
	}
}

void IMGDEC_PNG_write_len_table(
	PNG_INFO *png_info_data)
{
	UINT32 i, temp32;
	UINT32 time_out = 1000; // 1000 us = 1 ms

	// Set burst mode as '1' to prevent DMA mover writing DMA buffer 1
	temp32 = 0xc000;			// {1'b1, 1'b1, 5'd0, 9'd0}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;

	// Check if DMA buffer 1 is busy (DMA mover is already runing)
	do
	{
		if(time_out == 0)
		{
			WARNING("Wait DMA mover to finish jobs: time out!");
			break;
		}

		udelay(1);

		temp32 = g_pImgDecReg->png.png_dec_ctrl;
		//DEBUG("png_dec_ctrl = 0x%08x\n", temp32);

		time_out--;
	}while(temp32 & 0x2); // Wait DMA mover to finish jobs

	// Write length symbol
	temp32 = 0xc000; // {1'b1, 1'b1, 5'd0, 9'd0}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;

	// (nlen - zero_lenght) + 1 symobls, write one more symbol to pack last symbol into a word
	for(i=0; i<=(png_info_data->pngIDAT.nlen - png_info_data->pngIDAT.zero_length); i++)
	{
		temp32 = png_info_data->pngIDAT.sym_table[i];
		g_pImgDecReg->png.png_huf_tb_data = temp32;
	}

	// Write length mincode
	temp32 = 0xc000 + 0x180;	// {1'b1, 1'b1, 5'd0, 9'h180}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;

	for(i=1; i<16; i++)
	{
		temp32 = png_info_data->pngIDAT.mincode[i];
		g_pImgDecReg->png.png_huf_tb_data = temp32;
	}

	// Write length group base
	temp32 = 0xc000 + 0x190;	// {1'b1, 1'b1, 5'd0, 9'h190}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;

	for(i=1; i<16; i++)
	{
		temp32 = png_info_data->pngIDAT.valptr[i];
		g_pImgDecReg->png.png_huf_tb_data = temp32;
	}

	// Set burst mode as '0' to allow DMA mover writing DMA buffer 1
	temp32 = 0x0000;			// {1'b0, 1'b0, 5'd0, 9'd0}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;
}

void IMGDEC_PNG_write_dis_table(
	PNG_INFO *png_info_data)
{
	UINT32 i, temp32;
	UINT32 time_out = 1000; // 1000 us = 1 ms

	// Set burst mode as '1' to prevent DMA mover writing DMA buffer 1
	temp32 = 0xc000; // {1'b1, 1'b1, 5'd0, 9'd0}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;

	// Check if DMA buffer 1 is busy (DMA mover is already runing)
	do
	{
		if(time_out == 0)
		{
			WARNING("Wait DMA mover to finish jobs: time out!");
			break;
		}

		udelay(1);

		temp32 = g_pImgDecReg->png.png_dec_ctrl;
		//DEBUG("png_dec_ctrl = 0x%08x\n", temp32);

		time_out--;
	}while(temp32 & 0x2); // Wait DMA mover to finish jobs

	// Write distance symbol
	temp32 = 0xc000 + 0x1c0; // {1'b1, 1'b1, 5'd0, 9'h1c0}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;

	// (nlen - zero_lenght) + 1 symobls, write one more symbol to pack last symbol into a word
	for(i=0; i<=(png_info_data->pngIDAT.ndist - png_info_data->pngIDAT.zero_length); i++)
	{
		temp32 = png_info_data->pngIDAT.sym_table[i];
		g_pImgDecReg->png.png_huf_tb_data = temp32;
	}

	// Write distance mincode
	temp32 = 0xc000 + 0x1a0; // {1'b1, 1'b1, 5'd0, 9'h1a0}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;

	for(i=1; i<16; i++)
	{
		temp32 = png_info_data->pngIDAT.mincode[i];
		g_pImgDecReg->png.png_huf_tb_data = temp32;
	}

	// Write distance group base
	temp32 = 0xc000 + 0x1b0; // {1'b1, 1'b1, 5'd0, 9'h1b0}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;

	for(i=1; i<16; i++)
	{
		temp32 = png_info_data->pngIDAT.valptr[i];
		g_pImgDecReg->png.png_huf_tb_data = temp32;
	}

	// Set burst mode as '0' to allow DMA mover writing DMA buffer 1
	temp32 = 0x0000; // {1'b0, 1'b0, 5'd0, 9'd0}
	g_pImgDecReg->png.png_huf_tb_ctrl = temp32;
}

void IMGDEC_PNG_create_Huffman_tables(
	PNG_INFO *png_info_data,
	UINT8 *bs_buf_ptr)
{
	UINT32 i, k;
	UINT32 root;
	UINT32 max;
	UINT32 min;
	UINT32 huff;
	UINT32 len;
	UINT32 val;
	UINT32 incr;
	UINT32 fill;
	UINT32 copy;

	IMGDEC_PNG_need_bs_bits(png_info_data, bs_buf_ptr, 14);

	png_info_data->pngIDAT.nlen = IMGDEC_PNG_get_bs_bits(png_info_data, 5) + 257;
	IMGDEC_PNG_drop_bs_bits(png_info_data, 5);
	//DEBUG("nlen = 0x%08x\n", png_info_data->pngIDAT.nlen);

	png_info_data->pngIDAT.ndist = IMGDEC_PNG_get_bs_bits(png_info_data, 5) + 1;
	IMGDEC_PNG_drop_bs_bits(png_info_data, 5);
	//DEBUG("ndist = 0x%08x\n", png_info_data->pngIDAT.ndist);

	png_info_data->pngIDAT.ncode = IMGDEC_PNG_get_bs_bits(png_info_data, 4) + 4;
	IMGDEC_PNG_drop_bs_bits(png_info_data, 4);
	//DEBUG("ncode = 0x%08x\n", png_info_data->pngIDAT.ncode);

	for(i=0; i<png_info_data->pngIDAT.ncode; i++)
	{
		IMGDEC_PNG_need_bs_bits(png_info_data, bs_buf_ptr, 3);
		png_info_data->pngIDAT.lens[g_uiCodeLength[i]] = IMGDEC_PNG_get_bs_bits(png_info_data, 3);
		IMGDEC_PNG_drop_bs_bits(png_info_data, 3);
	}

	while(i < 19)
	{
		png_info_data->pngIDAT.lens[g_uiCodeLength[i]] = 0;
		i++;
	}

	// accumulate lengths for codes (assumes lens[] all in 0~15)
	for(i=0; i<16; i++)
		png_info_data->pngIDAT.count[i] = 0;

	for(i=0; i<19; i++)
		png_info_data->pngIDAT.count[png_info_data->pngIDAT.lens[i]]++;

	// generate offsets into symbol table for each length for sorting
	png_info_data->pngIDAT.offset[1] = 0;
	for(i=1; i<15; i++)
		png_info_data->pngIDAT.offset[i + 1] = png_info_data->pngIDAT.offset[i] + png_info_data->pngIDAT.count[i];

	// sort symbols by length, by symbol order within each length
	for(i=0; i<19; i++)
	{
		if(png_info_data->pngIDAT.lens[i] != 0)
		{
			png_info_data->pngIDAT.sym_table[png_info_data->pngIDAT.offset[png_info_data->pngIDAT.lens[i]]] = i;
			png_info_data->pngIDAT.offset[png_info_data->pngIDAT.lens[i]]++;
		}
	}

	root = 7;

	max = 15;
	while(png_info_data->pngIDAT.count[max] == 0 && max > 1)
		max--;

	//if(root > max)
	//	root = max;

	min = 1;
	while(png_info_data->pngIDAT.count[min] == 0 && min < 15)
		min++;

	if(root < min)
		root = min;

	/* Create Huffman table for code-length alphabet */
	/* Huffman codes are detected from "right" to "left", the Huffman
		table here is built with constant length entries, so that we can
		use constant length stream as table address to get a symbol */
	huff = 0;				/* starting code */
	k = 0;				/* starting code symbol */
	len = min;			/* starting code length */

	while(!(png_info_data->pngIDAT.count[len] == 0 && len == max))
	{
		val = png_info_data->pngIDAT.sym_table[k];

		/* replicate for those indices with low len bits equal to huff */
		incr = 1 << len;
		fill = 1 << root;
		min = fill;				/* save offset to next table */

		fill = fill - incr;
		png_info_data->pngIDAT.len_table_val[huff + fill] = val;
		png_info_data->pngIDAT.len_table_bits[huff + fill] = len;

		while(fill != 0)
		{
			fill = fill - incr;
			png_info_data->pngIDAT.len_table_val[huff + fill] = val;
			png_info_data->pngIDAT.len_table_bits[huff + fill] = len;
		}

		/* backwards increment the len-bit code huff */
		// Huffman codes are detected from 'right' to 'left'
		incr = 1 << (len - 1);
		while(huff & incr)
			incr >>= 1;
		if(incr != 0)
		{
			huff &= (incr - 1);
			huff += incr;
		}
		else
			huff = 0;

		/* go to next symbol, update count, len */
		k++;
		png_info_data->pngIDAT.count[len]--;

		/* get length of next symbol in sym_table */
		while (png_info_data->pngIDAT.count[len] == 0 && len < max)
			len = png_info_data->pngIDAT.lens[png_info_data->pngIDAT.sym_table[k]];
	}

	i = 0;
	while(i < (png_info_data->pngIDAT.nlen + png_info_data->pngIDAT.ndist))
	{
		// Can NOT use IMGDEC_PNG_need_bs_bits(7) because it may across chunks
		// if root is set as max, the index bits should be dynamically adjusted
		len = png_info_data->pngIDAT.len_table_bits[IMGDEC_PNG_get_bs_bits(png_info_data, 7)];

		while(len > png_info_data->pngIDAT.bs_bit_offset)
		{
			IMGDEC_PNG_pull_bs_byte(png_info_data, bs_buf_ptr);
			len = png_info_data->pngIDAT.len_table_bits[IMGDEC_PNG_get_bs_bits(png_info_data, 7)];
		}

		val = png_info_data->pngIDAT.len_table_val[IMGDEC_PNG_get_bs_bits(png_info_data, 7)];

		if (val < 16)
		{
			png_info_data->pngIDAT.lens[i] = val;
			IMGDEC_PNG_need_bs_bits(png_info_data, bs_buf_ptr, len);
			IMGDEC_PNG_drop_bs_bits(png_info_data, len);
			i++;
		}
		else
		{
			if(val == 16)
			{
				IMGDEC_PNG_need_bs_bits(png_info_data, bs_buf_ptr, (len + 2));
				IMGDEC_PNG_drop_bs_bits(png_info_data, len);
				copy = 3 + IMGDEC_PNG_get_bs_bits(png_info_data, 2);
				IMGDEC_PNG_drop_bs_bits(png_info_data, 2);
				if(i > 0) 
					len = png_info_data->pngIDAT.lens[i - 1];
				else
					len = 0;
			}
			else if(val == 17)
			{
				IMGDEC_PNG_need_bs_bits(png_info_data, bs_buf_ptr, (len + 3));
				IMGDEC_PNG_drop_bs_bits(png_info_data, len);
				copy = 3 + IMGDEC_PNG_get_bs_bits(png_info_data, 3);
				IMGDEC_PNG_drop_bs_bits(png_info_data, 3);
				len = 0;
			}
			else // val = 18
			{
				IMGDEC_PNG_need_bs_bits(png_info_data, bs_buf_ptr, (len + 7));
				IMGDEC_PNG_drop_bs_bits(png_info_data, len);
				copy = 11 + IMGDEC_PNG_get_bs_bits(png_info_data, 7);
				IMGDEC_PNG_drop_bs_bits(png_info_data, 7);
				len = 0;
			}

			while(copy != 0)
			{
				png_info_data->pngIDAT.lens[i] = len;
				copy--;
				i++;
			}
		}
	}

	// build literal/lenght table
	IMGDEC_PNG_inflate_table(png_info_data, png_info_data->pngIDAT.nlen);

	IMGDEC_PNG_write_len_table(png_info_data);

	// build distance table
	for(i=0; i<png_info_data->pngIDAT.ndist; i++)
		png_info_data->pngIDAT.lens[i] = png_info_data->pngIDAT.lens[png_info_data->pngIDAT.nlen + i];

	IMGDEC_PNG_inflate_table(png_info_data, png_info_data->pngIDAT.ndist);

	IMGDEC_PNG_write_dis_table(png_info_data);
}

void IMGDEC_PNG_parse_block_headers(
	PNG_INFO *png_info_data,
	UINT8 *bs_buf_ptr)
{
	IMGDEC_PNG_need_bs_bits(png_info_data, bs_buf_ptr, 3);

	png_info_data->pngIDAT.block_final = (UINT8)(IMGDEC_PNG_get_bs_bits(png_info_data, 1)); // BFINAL
	IMGDEC_PNG_drop_bs_bits(png_info_data, 1);
	//DEBUG("block_final = 0x%02x\n", png_info_data->pngIDAT.block_final);

	png_info_data->pngIDAT.block_type = (UINT8)(IMGDEC_PNG_get_bs_bits(png_info_data, 2)); // BTYPE
	IMGDEC_PNG_drop_bs_bits(png_info_data, 2);
	//DEBUG("block_type = 0x%02x\n", png_info_data->pngIDAT.block_type);

	// Set block type, and last block
	g_pImgDecReg->png.png_blk_info = ((UINT32)(png_info_data->pngIDAT.block_type & 0x3) << 1) |
									(UINT32)(png_info_data->pngIDAT.block_final & 0x1);

	switch(png_info_data->pngIDAT.block_type)
	{
		case 0:
			//DEBUG("Uncomporessed block.\n");
			break;

		case 1:
			//DEBUG("Comporessed block with fixed Huffman codes.\n");
			break;

		case 2:
			//DEBUG("Comporessed block with dynamic Huffman codes.\n");
			IMGDEC_PNG_create_Huffman_tables(png_info_data, bs_buf_ptr);
			break;

		default:
			WARNING("Invalid block type.\n");
			break;
	}
}

//---------------------------------------------------------------

void imgdec_clock_enable(void)
{
	moonReg_t *pMoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	pMoonReg->reset[1] &= ~(MOON_IMG_MISC_RESET &
							MOON_HUFF_RESET &
							MOON_JPG_IQT_RESET &
							MOON_JR_RESET &
							MOON_PNG_RESET	);

	pMoonReg->clken[1] |= (MOON_IMG_MISC_CLKEN &
							MOON_HUFF_CLKEN &
							MOON_JPG_IQT_CLKEN &
							MOON_JR_CLKEN &
							MOON_PNG_CLKEN	);

	pMoonReg->gclken[1] &= ~(MOON_IMG_MISC_GCLKEN &
							MOON_HUFF_GCLKEN &
							MOON_JPG_IQT_GCLKEN &
							MOON_JR_GCLKEN &
							MOON_PNG_GCLKEN);

	//DEBUG("imgdec_clock_enable: pMoonReg->reset[1] = 0x%08x, pMoonReg->clken[1] = 0x%08x, pMoonReg->gclken[1] = 0x%08x\n",
	//													pMoonReg->reset[1], pMoonReg->clken[1], pMoonReg->gclken[1]);
}
EXPORT_SYMBOL(imgdec_clock_enable);

void imgdec_clock_disable(void)
{
}
EXPORT_SYMBOL(imgdec_clock_disable);

int imgdec_hal_reg_mmap(
	void)
{
#if 0
	request_mem_region(RGST_BASE, RGST_SIZE, "Register_Map");
	g_imgdec_pHWRegBase = ioremap(RGST_BASE, RGST_SIZE);
	if(g_imgdec_pHWRegBase == NULL)
	{
		ERR("ioremap for HW register failed\n");
		return -1;
	}
	g_pImgDecReg = (IMG_DEC_REG *)(g_imgdec_pHWRegBase + RGST_IMG_DEC_HUF_OFFSET);
#else // Hardware register memory address by hardware.h
	g_pImgDecReg = (IMG_DEC_REG *)LOGI_ADDR_IMG_DEC_HUF_REG;
#endif

	return 0;
}
EXPORT_SYMBOL(imgdec_hal_reg_mmap);

void imgdec_hal_reg_unmmap(
	void)
{
#if 0
	iounmap(g_imgdec_pHWRegBase);
	g_imgdec_pHWRegBase = NULL;
	g_pImgDecReg = NULL;
#else // Hardware register memory address by hardware.h
	g_pImgDecReg = NULL;
#endif
}
EXPORT_SYMBOL(imgdec_hal_reg_unmmap);

void imgdec_hal_print_all_registers( // only for test!!! (waiting to be removed as release)
	void)
{
	printk("\n");
	printk("g_pImgDecReg->huf.jpeg_huff_mode = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_mode);
	printk("g_pImgDecReg->huf.jpeg_huff_ctrl = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_ctrl);
	printk("g_pImgDecReg->huf.jpeg_huff_status = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_status);
	printk("g_pImgDecReg->huf.jpeg_huff_scan_ctrl = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_scan_ctrl);
	printk("g_pImgDecReg->huf.jpeg_huff_table = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_table);
	printk("g_pImgDecReg->huf.jpeg_huff_mcuno = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_mcuno);
	printk("g_pImgDecReg->huf.jpeg_huff_jpeghsize = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_jpeghsize);
	printk("g_pImgDecReg->huf.jpeg_huff_jpegvsize = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_jpegvsize);
	printk("g_pImgDecReg->huf.jpeg_huff_bitstream = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_bitstream);
	printk("g_pImgDecReg->huf.jpeg_huff_bits_dma_offset = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_bits_dma_offset);
	printk("g_pImgDecReg->huf.jpeg_huff_wr_huff_info = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_wr_huff_info);
	printk("g_pImgDecReg->huf.jpeg_huff_bits_info = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_bits_info);
	printk("g_pImgDecReg->huf.jpeg_huff_rd_huff_info = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_rd_huff_info);
	printk("g_pImgDecReg->huf.jpeg_huff_dma_offset_0 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_dma_offset_0);
	printk("g_pImgDecReg->huf.jpeg_huff_dma_offset_1 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_dma_offset_1);
	printk("g_pImgDecReg->huf.jpeg_huff_acval_offset_0 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_acval_offset_0);
	printk("g_pImgDecReg->huf.jpeg_huff_acval_offset_1 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_acval_offset_1);
	printk("g_pImgDecReg->huf.jpeg_huff_acval_offset_2 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_acval_offset_2);
	printk("g_pImgDecReg->huf.jpeg_huff_acval_offset_3 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_acval_offset_3);
	printk("g_pImgDecReg->huf.jpeg_huff_wr_dc_01 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_wr_dc_01);
	printk("g_pImgDecReg->huf.jpeg_huff_wr_dc_23 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_wr_dc_23);
	printk("g_pImgDecReg->huf.jpeg_huff_wr_acval_0 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_wr_acval_0);
	printk("g_pImgDecReg->huf.jpeg_huff_wr_acval_1 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_wr_acval_1);
	printk("g_pImgDecReg->huf.jpeg_huff_wr_acval_2 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_wr_acval_2);
	printk("g_pImgDecReg->huf.jpeg_huff_wr_acval_3 = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_wr_acval_3);
	printk("g_pImgDecReg->huf.jpeg_huff_rd_addr = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_rd_addr);
	printk("g_pImgDecReg->huf.jpeg_huff_rd_val = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_rd_val);
	printk("g_pImgDecReg->huf.jpeg_huff_probe = 0x%08x\n", g_pImgDecReg->huf.jpeg_huff_probe);
	printk("g_pImgDecReg->huf.jr_control = 0x%08x\n", g_pImgDecReg->huf.jr_control);
	printk("g_pImgDecReg->huf.jr_status = 0x%08x\n", g_pImgDecReg->huf.jr_status);
	printk("g_pImgDecReg->huf.jr_reg_num = 0x%08x\n", g_pImgDecReg->huf.jr_reg_num);
	printk("g_pImgDecReg->huf.jr_reg_data = 0x%08x\n", g_pImgDecReg->huf.jr_reg_data);
	printk("\n");
	printk("g_pImgDecReg->jdm.jdm_mode = 0x%08x\n", g_pImgDecReg->jdm.jdm_mode);
	printk("g_pImgDecReg->jdm.jdm_status = 0x%08x\n", g_pImgDecReg->jdm.jdm_status);
	printk("g_pImgDecReg->jdm.jdm_dma1_mode = 0x%08x\n", g_pImgDecReg->jdm.jdm_dma1_mode);
	printk("g_pImgDecReg->jdm.jdm_dma1_xaddr = 0x%08x\n", g_pImgDecReg->jdm.jdm_dma1_xaddr);
	printk("g_pImgDecReg->jdm.jdm_dma1_yaddr = 0x%08x\n", g_pImgDecReg->jdm.jdm_dma1_yaddr);
	printk("g_pImgDecReg->jdm.jdm_dma1_done = 0x%08x\n", g_pImgDecReg->jdm.jdm_dma1_done);
	printk("g_pImgDecReg->jdm.jdm_dma1_start_addr = 0x%08x\n", g_pImgDecReg->jdm.jdm_dma1_start_addr);
	printk("g_pImgDecReg->jdm.jdm_dma1_block_num = 0x%08x\n", g_pImgDecReg->jdm.jdm_dma1_block_num);
	printk("g_pImgDecReg->jdm.jdm_jpeg_offset = 0x%08x\n", g_pImgDecReg->jdm.jdm_jpeg_offset);
	printk("\n");
	printk("g_pImgDecReg->invq.jpeg_invq_qmx_par = 0x%08x\n", g_pImgDecReg->invq.jpeg_invq_qmx_par);
	printk("g_pImgDecReg->invq.jpeg_invq_mode = 0x%08x\n", g_pImgDecReg->invq.jpeg_invq_mode);
	printk("g_pImgDecReg->invq.jpeg_invq_chksum = 0x%08x\n", g_pImgDecReg->invq.jpeg_invq_chksum);
	printk("\n");
	printk("g_pImgDecReg->png.png_dec_mode = 0x%08x\n", g_pImgDecReg->png.png_dec_mode);
	printk("g_pImgDecReg->png.png_bs_str_addr_m = 0x%08x\n", g_pImgDecReg->png.png_bs_str_addr_m);
	printk("g_pImgDecReg->png.png_bs_str_addr_l = 0x%08x\n", g_pImgDecReg->png.png_bs_str_addr_l);
	printk("g_pImgDecReg->png.png_bs_end_addr_m = 0x%08x\n", g_pImgDecReg->png.png_bs_end_addr_m);
	printk("g_pImgDecReg->png.png_bs_end_addr_l = 0x%08x\n", g_pImgDecReg->png.png_bs_end_addr_l);
	printk("g_pImgDecReg->png.png_bs_buf_addr = 0x%08x\n", g_pImgDecReg->png.png_bs_buf_addr);
	printk("g_pImgDecReg->png.png_bs_offset = 0x%08x\n", g_pImgDecReg->png.png_bs_offset);
	printk("g_pImgDecReg->png.png_bs_used_cnt = 0x%08x\n", g_pImgDecReg->png.png_bs_used_cnt);
	printk("g_pImgDecReg->png.png_bs_used_bit = 0x%08x\n", g_pImgDecReg->png.png_bs_used_bit);
	printk("g_pImgDecReg->png.png_dec_ctrl = 0x%08x\n", g_pImgDecReg->png.png_dec_ctrl);
	printk("g_pImgDecReg->png.png_img_width = 0x%08x\n", g_pImgDecReg->png.png_img_width);
	printk("g_pImgDecReg->png.png_img_height = 0x%08x\n", g_pImgDecReg->png.png_img_height);
	printk("g_pImgDecReg->png.png_img_type = 0x%08x\n", g_pImgDecReg->png.png_img_type);
	printk("g_pImgDecReg->png.png_chunk_size_m = 0x%08x\n", g_pImgDecReg->png.png_chunk_size_m);
	printk("g_pImgDecReg->png.png_chunk_size_l = 0x%08x\n", g_pImgDecReg->png.png_chunk_size_l);
	printk("g_pImgDecReg->png.png_blk_info = 0x%08x\n", g_pImgDecReg->png.png_blk_info);
	printk("g_pImgDecReg->png.png_huf_tb_addr = 0x%08x\n", g_pImgDecReg->png.png_huf_tb_addr);
	printk("g_pImgDecReg->png.png_huf_tb_ctrl = 0x%08x\n", g_pImgDecReg->png.png_huf_tb_ctrl);
	printk("g_pImgDecReg->png.png_huf_tb_data = 0x%08x\n", g_pImgDecReg->png.png_huf_tb_data);
	printk("g_pImgDecReg->png.png_interrupt = 0x%08x\n", g_pImgDecReg->png.png_interrupt);
	printk("g_pImgDecReg->png.png_scl_ext_addr_m = 0x%08x\n", g_pImgDecReg->png.png_scl_ext_addr_m);
	printk("g_pImgDecReg->png.png_scl_ext_addr_l = 0x%08x\n", g_pImgDecReg->png.png_scl_ext_addr_l);
	printk("g_pImgDecReg->png.png_scl_buf_addr = 0x%08x\n", g_pImgDecReg->png.png_scl_buf_addr);
	printk("g_pImgDecReg->png.png_bw_ext_addr_m = 0x%08x\n", g_pImgDecReg->png.png_bw_ext_addr_m);
	printk("g_pImgDecReg->png.png_bw_ext_addr_l = 0x%08x\n", g_pImgDecReg->png.png_bw_ext_addr_l);
	printk("g_pImgDecReg->png.png_rem_chunk_size_m = 0x%08x\n", g_pImgDecReg->png.png_rem_chunk_size_m);
	printk("g_pImgDecReg->png.png_rem_chunk_size_l = 0x%08x\n", g_pImgDecReg->png.png_rem_chunk_size_l);
	printk("g_pImgDecReg->png.png_pxl_ext_addr_m = 0x%08x\n", g_pImgDecReg->png.png_pxl_ext_addr_m);
	printk("g_pImgDecReg->png.png_pxl_ext_addr_l = 0x%08x\n", g_pImgDecReg->png.png_pxl_ext_addr_l);
	printk("g_pImgDecReg->png.png_pxl_line = 0x%08x\n", g_pImgDecReg->png.png_pxl_line);
	printk("g_pImgDecReg->png.png_probe_addr = 0x%08x\n", g_pImgDecReg->png.png_probe_addr);
	printk("g_pImgDecReg->png.png_probe_value = 0x%08x\n", g_pImgDecReg->png.png_probe_value);
	printk("\n");
}
EXPORT_SYMBOL(imgdec_hal_print_all_registers);

int imgdec_hal_data_ram_mmap(
	void)
{
	request_mem_region(DATA_RAM_BASE, DATA_RAM_SIZE, "Data_RAM_Map");
	g_imgdec_pDataRamBase = ioremap(DATA_RAM_BASE, DATA_RAM_SIZE);
	if(g_imgdec_pDataRamBase == NULL)
	{
		ERR("ioremap for PNG data RAM failed\n");
		return -1;
	}

	return 0;
}
EXPORT_SYMBOL(imgdec_hal_data_ram_mmap);

void imgdec_hal_data_ram_unmmap(
	void)
{
	iounmap(g_imgdec_pDataRamBase);
	g_imgdec_pDataRamBase = NULL;
}
EXPORT_SYMBOL(imgdec_hal_data_ram_unmmap);

void imgdec_hal_jr_setup(
	JPEG_INFO *jpeg_info_data,
	PNG_INFO *png_info_data,
	DECODE_INFO *decode_info_data,
	UINT32 working_buf_phyaddr)
{
	UINT8 output_run_time;
	UINT32 mcu_num_h, mcu_num_v;
	UINT32 mcu_num_4096_h, mcu_num_4096_v;
	IMAGE_TYPE image_type;

	if(jpeg_info_data->jpegDecode.output_run_time > 0)
	{
		image_type = IMAGE_JPEG;
		output_run_time = jpeg_info_data->jpegDecode.output_run_time;
	}
	else if(png_info_data->pngDecode.output_run_time > 0)
	{
		image_type = IMAGE_PNG;
		output_run_time = png_info_data->pngDecode.output_run_time;
	}
	else
	{
		image_type = IMAGE_JPEG;
		output_run_time = 1;
		WARNING("output_run_time = 0 in jpegDecode and pngDecode !!!");
	}

	IMGDEC_jr_pre_setup(jpeg_info_data, png_info_data, decode_info_data, working_buf_phyaddr);

	//enable control commond
	g_pImgDecReg->huf.jr_control = 1;

	IMGDEC_jr_write_coef_table();

	//Output buffer address
	IMGDEC_jr_set_port(JR_LUMA_BASE, (g_imgdec_jr_reg_info.luma_start >> 2) & 0x0000ffff); // 4-byte unit
	IMGDEC_jr_set_port(JR_BASE_EXTEND0, (g_imgdec_jr_reg_info.luma_start >> 18) & 0x00003fff);
	IMGDEC_jr_set_port(JR_CROMA_BASE, (g_imgdec_jr_reg_info.chroma_start >> 2) & 0x0000ffff); // 4-byte unit
	IMGDEC_jr_set_port(JR_BASE_EXTEND1, (g_imgdec_jr_reg_info.chroma_start >> 18) & 0x00003fff);

	//Temp buffer of JR coefficient table setting: 4-byte unit & 8-byte_align
	IMGDEC_jr_set_port(JR_TEMP_BUF_BASE, ((g_imgdec_jr_reg_info.temp_buffer + 4) >> 2) & 0x0000fffe);
	IMGDEC_jr_set_port(JR_BASE_EXTEND2, ((g_imgdec_jr_reg_info.temp_buffer + 4) >> 18) & 0x00003fff);

	// DMA address (Debug progressive JPEG output error, after MJ-baseline JPEG decoding.)
	IMGDEC_jr_set_port(JR_DMA_CP_BASE, g_imgdec_jr_reg_info.render_input_offset); // [10:0]: Indicates which ping-pong DMA buffer for JR using.

	// set source/display color mode
	IMGDEC_jr_set_port(JR_SRC_COLOR_MODE, g_imgdec_jr_reg_info.src_color_mode);
	IMGDEC_jr_set_port(JR_DISP_COLOR_MODE, (g_imgdec_jr_reg_info.dis_color_mode & 0x0f)
									|(g_imgdec_jr_reg_info.alpha_part << 4)
									|(g_imgdec_jr_reg_info.rgb_swap << 12)
									|(g_imgdec_jr_reg_info.orientation << 13));

	// set size of source picture
	IMGDEC_jr_set_port(JR_SRC_WIDTH, g_imgdec_jr_reg_info.src_width);
	IMGDEC_jr_set_port(JR_SRC_HEIGHT, g_imgdec_jr_reg_info.src_height);

	//set filter initial phase
	IMGDEC_jr_set_port(JR_V_PHASE_OFFSET, g_imgdec_jr_reg_info.v_filter_phase);
	IMGDEC_jr_set_port(JR_H_PHASE_OFFSET, g_imgdec_jr_reg_info.h_filter_phase);

	//scaler index
	IMGDEC_jr_set_port(JR_SCALER_INDEX_V, (g_imgdec_jr_reg_info.scaler_index_v & 0xffff));
	IMGDEC_jr_set_port(JR_SCALER_INDEX_H, (g_imgdec_jr_reg_info.scaler_index_h & 0xffff));
	IMGDEC_jr_set_port(JR_SCALER_INDEX_EXT, ((g_imgdec_jr_reg_info.scaler_index_h >> 16) & 0xf)|(((g_imgdec_jr_reg_info.scaler_index_v >> 16) & 0xf) << 8));

	// set frame buffer size
	IMGDEC_jr_set_port(JR_FRAME_WIDTH, g_imgdec_jr_reg_info.frame_width & 0xffff);
	IMGDEC_jr_set_port(JR_FRAME_HEIGHT, g_imgdec_jr_reg_info.frame_height & 0xffff);

	// offset from display point to original point
	IMGDEC_jr_set_port(JR_RENDER_V_INIT, 0); // [9:0] unit in 2 pixels
	IMGDEC_jr_set_port(JR_RENDER_H_INIT, 0); // [9:0] unit in 2 pixels

	// set size of display pic
	IMGDEC_jr_set_port(JR_RENDER_H_BOUND, g_imgdec_jr_reg_info.disp_box_h_bound & 0x0fff); // [11:0] unit in 2 pixels
	IMGDEC_jr_set_port(JR_RENDER_V_BOUND, g_imgdec_jr_reg_info.disp_box_v_bound & 0x0fff); // [11:0] unit in 2 pixels

	//Pre-scaling asetting
	IMGDEC_jr_set_port(JR_SRCPP_CTRL, g_imgdec_jr_reg_info.pre_scaler & 0x001f);

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
	IMGDEC_jr_set_port(JR_MISC, (g_imgdec_jr_reg_info.temp_buf_onLuma)
						|(g_imgdec_jr_reg_info.pd_enable << 1)
						|(g_imgdec_jr_reg_info.rgb_output_onChroma << 2)
						|(g_imgdec_jr_reg_info.odd_symm_filter << 3)
						|(g_imgdec_jr_reg_info.hd_color_space << 4)
						|(g_imgdec_jr_reg_info.rgb_input_onChroma << 5)
						|(g_imgdec_jr_reg_info.bypass_enable << 6)
						|(g_imgdec_jr_reg_info.bw_file << 7)
						|(g_imgdec_jr_reg_info.yuv2RGB_flag << 10)
						|(g_imgdec_jr_reg_info.alpha_scaling << 14));


	if((output_run_time & 0x0f) > 1) // bit[3:0] = total run times
	{
		// get image horizontal / vertical MCU number.
		if(image_type == IMAGE_PNG)
		{
			mcu_num_h = (png_info_data->pngIHDR.width + 7) >> 3;
			mcu_num_v = (png_info_data->pngIHDR.height + 7) >> 3;
			mcu_num_4096_h = 512; // 512 = 4096 >> 3
			mcu_num_4096_v = 512; // 512 = 4096 >> 3
		}
		else // if(image_type == IMAGE_JPEG)
		{
			mcu_num_h = (jpeg_info_data->jpegSOF.Hfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.width+15)>>4) : ((jpeg_info_data->jpegSOF.width+7)>>3);
			mcu_num_v = (jpeg_info_data->jpegSOF.Vfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.height+15)>>4) : ((jpeg_info_data->jpegSOF.height+7)>>3);
			mcu_num_4096_h = (jpeg_info_data->jpegSOF.Hfactor[0] == 2) ? 256 : 512; // 256 = 4096 >> 4, 512 = 4096 >> 3
			mcu_num_4096_v = (jpeg_info_data->jpegSOF.Vfactor[0] == 2) ? 256 : 512; // 256 = 4096 >> 4, 512 = 4096 >> 3
		}

		switch(output_run_time & 0xf0)
		{ // bit[7] = vertical run 2 times, bit[6] = horizontal run 2 times, bit[5:4] = current run time
			case 0x40: // 0100-0000
				IMGDEC_jr_set_port(JR_MCU_X_START, 0);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_4096_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, 0);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			case 0x50: // 0101-0000
				IMGDEC_jr_set_port(JR_MCU_X_START, mcu_num_4096_h);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, 0);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			case 0x80: // 1000-0000
				IMGDEC_jr_set_port(JR_MCU_X_START, 0);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, 0);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_4096_v - 1));
				break;
			case 0x90: // 1001-0000
				IMGDEC_jr_set_port(JR_MCU_X_START, 0);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, mcu_num_4096_v);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			case 0xc0: // 1100-0000
				IMGDEC_jr_set_port(JR_MCU_X_START, 0);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_4096_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, 0);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_4096_v - 1));
				break;
			case 0xd0: // 1101-0000
				IMGDEC_jr_set_port(JR_MCU_X_START, mcu_num_4096_h);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, 0);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_4096_v - 1));
				break;
			case 0xe0: // 1110-0000
				IMGDEC_jr_set_port(JR_MCU_X_START, 0);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_4096_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, mcu_num_4096_v);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			case 0xf0: // 1111-0000
				IMGDEC_jr_set_port(JR_MCU_X_START, mcu_num_4096_h);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, mcu_num_4096_v);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
			default:
				IMGDEC_jr_set_port(JR_MCU_X_START, 0);
				IMGDEC_jr_set_port(JR_MCU_X_END, (mcu_num_h - 1));
				IMGDEC_jr_set_port(JR_MCU_Y_START, 0);
				IMGDEC_jr_set_port(JR_MCU_Y_END, (mcu_num_v - 1));
				break;
		}
	}

	//reset hw
	g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control | (1 << 5); // FSM software reset
	g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control & ~0x0020;

	if(g_imgdec_jr_reg_info.auto_fetch_enable == 0)
	{
		//non-auto-fetch frame start
		g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control | (1 << 8); // Picture start
		g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control & ~0x0100;
	}
}
EXPORT_SYMBOL(imgdec_hal_jr_setup);

void imgdec_hal_scale_setup(
	SCALE_INFO *scale_info_data,
	UINT32 working_buf_phyaddr)
{
	UINT32 downsample_rate_w = 0, downsample_rate_h = 0;
	UINT32 h_output_len, v_output_len;

	g_imgdec_jr_reg_info.temp_buf_onLuma = 1;
	g_imgdec_jr_reg_info.pd_enable = 0;
	g_imgdec_jr_reg_info.rgb_output_onChroma = 0;
	g_imgdec_jr_reg_info.odd_symm_filter = 0;
	g_imgdec_jr_reg_info.rgb_input_onChroma = 0;
	g_imgdec_jr_reg_info.bw_file = 0;
	g_imgdec_jr_reg_info.yuv2RGB_flag = 0;

	g_imgdec_jr_reg_info.luma_start = scale_info_data->dst_Ybufphyaddr;
	g_imgdec_jr_reg_info.chroma_start = scale_info_data->dst_UVbufphyaddr;
	g_imgdec_jr_reg_info.temp_buffer = working_buf_phyaddr;

	g_imgdec_jr_reg_info.render_input_offset = JR_DMA_BUF_BASE;
	g_imgdec_jr_reg_info.orientation = ((scale_info_data->dst_type & 0xe000) >> 13); // for orthogonal orientation (EXIF) // 0xe000 for orientation
	g_imgdec_jr_reg_info.rgb_swap = scale_info_data->dst_endian; // 0(Big Endian) or 1(Little Endian) only for RGB domain
	g_imgdec_jr_reg_info.alpha_part = 0xff; // 0~0xff for RGBA/ARGB

	g_imgdec_jr_reg_info.frame_width  = scale_info_data->dst_bufbpl;
	g_imgdec_jr_reg_info.frame_height = scale_info_data->dst_bufline;
	g_imgdec_jr_reg_info.dis_color_mode = (scale_info_data->dst_type & 0xf); // 0xf for orientation

	// set source size and color mode
	g_imgdec_jr_reg_info.src_width = scale_info_data->clip_width;
	g_imgdec_jr_reg_info.src_height = scale_info_data->clip_height;
	g_imgdec_jr_reg_info.src_color_mode = scale_info_data->src_type;

	// set pre_scaler
	g_imgdec_jr_reg_info.pre_scaler = 0;

	// set auto_fetch
	g_imgdec_jr_reg_info.auto_fetch_enable = 1;

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

	g_imgdec_jr_reg_info.scaler_index_h = (0x1<<30)/downsample_rate_w; // 1024 = 1x, 512 = 2x, 2048 = 0.5x
	g_imgdec_jr_reg_info.scaler_index_v = (0x1<<30)/downsample_rate_h; // 1024 = 1x, 512 = 2x, 2048 = 0.5x

	downsample_rate_w >>= 10; // 1 <= downsample_rate_w <= 1024*1024(=0x1<<20)
	downsample_rate_h >>= 10; // 1 <= downsample_rate_h <= 1024*1024(=0x1<<20)

	if((downsample_rate_w > 1024) || (downsample_rate_h > 1024))
		g_imgdec_jr_reg_info.hd_color_space = 1;
	else
		g_imgdec_jr_reg_info.hd_color_space = 0;

	if((g_imgdec_jr_reg_info.scaler_index_h == 1024) && (g_imgdec_jr_reg_info.scaler_index_v == 1024))
		g_imgdec_jr_reg_info.bypass_enable = 1;
	else
		g_imgdec_jr_reg_info.bypass_enable = 0;
	if((scale_info_data->dst_type & 0xe000) >> 13) // for orientation
		g_imgdec_jr_reg_info.bypass_enable = 0;

	g_imgdec_jr_reg_info.disp_box_h_bound = ((scale_info_data->rect_width + 0x1) & ~0x1) >> 1; // (>> 1) for unit in 2 pixels
	g_imgdec_jr_reg_info.disp_box_v_bound = ((scale_info_data->rect_height + 0x1) & ~0x1) >> 1; // (>> 1) for unit in 2 pixels

	if((((scale_info_data->dst_type & 0xe000) >> 13) == RIGHT_ROTATE_90) | (((scale_info_data->dst_type & 0xe000) >> 13) == LEFT_ROTATE_90)) // for orientation
	{
		h_output_len = ((1024 * (g_imgdec_jr_reg_info.src_height - 1))/g_imgdec_jr_reg_info.scaler_index_h) + 1;
		v_output_len = ((1024 * (g_imgdec_jr_reg_info.src_width - 1))/g_imgdec_jr_reg_info.scaler_index_v) + 1;
	}
	else
	{
		h_output_len = ((1024 * (g_imgdec_jr_reg_info.src_width - 1))/g_imgdec_jr_reg_info.scaler_index_h) + 1;
		v_output_len = ((1024 * (g_imgdec_jr_reg_info.src_height - 1))/g_imgdec_jr_reg_info.scaler_index_v) + 1;
	}

	if(g_imgdec_jr_reg_info.scaler_index_h < 1024)
		g_imgdec_jr_reg_info.h_filter_phase = (~((((g_imgdec_jr_reg_info.disp_box_h_bound << 1) - h_output_len) * g_imgdec_jr_reg_info.scaler_index_h)/2) + 1) & 0x7ff;
	else if(g_imgdec_jr_reg_info.scaler_index_h == 1024)
		g_imgdec_jr_reg_info.h_filter_phase = 1;
	else
		g_imgdec_jr_reg_info.h_filter_phase = 715;

	if(g_imgdec_jr_reg_info.scaler_index_v < 1024)
		g_imgdec_jr_reg_info.v_filter_phase = (~((((g_imgdec_jr_reg_info.disp_box_v_bound << 1) - v_output_len) * g_imgdec_jr_reg_info.scaler_index_v)/2) + 1) & 0x7ff;
	else if(g_imgdec_jr_reg_info.scaler_index_v == 1024)
		g_imgdec_jr_reg_info.v_filter_phase = 1;
	else
		g_imgdec_jr_reg_info.v_filter_phase = 711;

	// tuning JR coefficient for quality
	IMGDEC_jr_load_coef_table(downsample_rate_w, downsample_rate_h);

	//enable control commond
	g_pImgDecReg->huf.jr_control = 1;

	IMGDEC_jr_write_coef_table();

	//Output buffer address
	IMGDEC_jr_set_port(JR_LUMA_BASE, (g_imgdec_jr_reg_info.luma_start >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
	IMGDEC_jr_set_port(JR_BASE_EXTEND0, (g_imgdec_jr_reg_info.luma_start >> 18) & 0x00003fff);
	IMGDEC_jr_set_port(JR_CROMA_BASE, (g_imgdec_jr_reg_info.chroma_start >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
	IMGDEC_jr_set_port(JR_BASE_EXTEND1, (g_imgdec_jr_reg_info.chroma_start >> 18) & 0x00003fff);

	//Temp buffer of JR coefficient table setting: 4-byte unit & 8-byte_align
	IMGDEC_jr_set_port(JR_TEMP_BUF_BASE, ((g_imgdec_jr_reg_info.temp_buffer + 4) >> 2) & 0x0000fffe); // unit in 4 bytes , 8 bytes align
	IMGDEC_jr_set_port(JR_BASE_EXTEND2, ((g_imgdec_jr_reg_info.temp_buffer + 4) >> 18) & 0x00003fff);

	// DMA address (Debug progressive JPEG output error, after MJ-baseline JPEG decoding.)
	IMGDEC_jr_set_port(JR_DMA_CP_BASE, g_imgdec_jr_reg_info.render_input_offset); // [10:0]: Indicates which ping-pong DMA buffer for JR using.

	// set source/display color mode
	IMGDEC_jr_set_port(JR_SRC_COLOR_MODE, g_imgdec_jr_reg_info.src_color_mode);
	IMGDEC_jr_set_port(JR_DISP_COLOR_MODE, (g_imgdec_jr_reg_info.dis_color_mode & 0x0f)
									|(g_imgdec_jr_reg_info.alpha_part << 4)
									|(g_imgdec_jr_reg_info.rgb_swap << 12)
									|(g_imgdec_jr_reg_info.orientation << 13));

	// set size of source picture
	IMGDEC_jr_set_port(JR_SRC_WIDTH, g_imgdec_jr_reg_info.src_width);
	IMGDEC_jr_set_port(JR_SRC_HEIGHT, g_imgdec_jr_reg_info.src_height);

	//set filter initial phase
	IMGDEC_jr_set_port(JR_V_PHASE_OFFSET, g_imgdec_jr_reg_info.v_filter_phase);
	IMGDEC_jr_set_port(JR_H_PHASE_OFFSET, g_imgdec_jr_reg_info.h_filter_phase);

	//scaler index
	IMGDEC_jr_set_port(JR_SCALER_INDEX_V, (g_imgdec_jr_reg_info.scaler_index_v & 0xffff));
	IMGDEC_jr_set_port(JR_SCALER_INDEX_H, (g_imgdec_jr_reg_info.scaler_index_h & 0xffff));
	IMGDEC_jr_set_port(JR_SCALER_INDEX_EXT, ((g_imgdec_jr_reg_info.scaler_index_h >> 16) & 0xf)|(((g_imgdec_jr_reg_info.scaler_index_v >> 16) & 0xf) << 8));

	// set frame buffer size
	IMGDEC_jr_set_port(JR_FRAME_WIDTH, g_imgdec_jr_reg_info.frame_width & 0xffff); // unit in byte, 8 bytes align
	IMGDEC_jr_set_port(JR_FRAME_HEIGHT, g_imgdec_jr_reg_info.frame_height & 0xffff); // unit in byte

	// offset from display point to original point
	IMGDEC_jr_set_port(JR_RENDER_H_INIT, scale_info_data->rect_x >> 1); // [9:0] unit in 2 pixels, 8 pixels align
	IMGDEC_jr_set_port(JR_RENDER_V_INIT, scale_info_data->rect_y >> 1); // [9:0] unit in 2 pixels, 8 pixels align

	// set size of display pic
	IMGDEC_jr_set_port(JR_RENDER_H_BOUND, g_imgdec_jr_reg_info.disp_box_h_bound & 0x0fff); // [11:0] unit in 2 pixels, 8 pixels align
	IMGDEC_jr_set_port(JR_RENDER_V_BOUND, g_imgdec_jr_reg_info.disp_box_v_bound & 0x0fff); // [11:0] unit in 2 pixels, 8 pixels align

	//Pre-scaling asetting
	IMGDEC_jr_set_port(JR_SRCPP_CTRL, g_imgdec_jr_reg_info.pre_scaler & 0x001f);

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
	IMGDEC_jr_set_port(JR_MISC, (g_imgdec_jr_reg_info.temp_buf_onLuma)
						|(g_imgdec_jr_reg_info.pd_enable << 1)
						|(g_imgdec_jr_reg_info.rgb_output_onChroma << 2)
						|(g_imgdec_jr_reg_info.odd_symm_filter << 3)
						|(g_imgdec_jr_reg_info.hd_color_space << 4)
						|(g_imgdec_jr_reg_info.rgb_input_onChroma << 5)
						|(g_imgdec_jr_reg_info.bypass_enable << 6)
						|(g_imgdec_jr_reg_info.bw_file << 7)
						|(g_imgdec_jr_reg_info.yuv2RGB_flag << 10)
						|((scale_info_data->src_endian & 0x1) << 12));
}
EXPORT_SYMBOL(imgdec_hal_scale_setup);

int imgdec_hal_scale_start(
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
		IMGDEC_jr_set_port(JR_AUTO_SRC_LUMA_BASE, (SrcYBufBase >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
		IMGDEC_jr_set_port(JR_AUTO_SRC_BASE_EXT0, (SrcYBufBase >> 18) & 0x00003fff);

		IMGDEC_jr_set_port(JR_AUTO_SRC_CHROMA_BASE, (SrcUVBufBase >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
		IMGDEC_jr_set_port(JR_AUTO_SRC_BASE_EXT1, (SrcUVBufBase >> 18) & 0x00003fff);

		IMGDEC_jr_set_port(JR_MCU_LINE_NUM, McuLineNum); // [12:0]: Number of MCU lines in auto fetch DRAM buffer.
		IMGDEC_jr_set_port(JR_AUTO_FETCH_WIDTH, FetchWidth);  // unit in byte, 4 bytes align

		//reset hw
		g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control | (1 << 5); // FSM software reset
		g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control & ~0x0020;

		//for auto-fetch frame start
		g_pImgDecReg->huf.jr_control = 0x0103; // [8]: Picture start. [1]: Automatically fetch source pixels from DRAM. [0]: JR enable.
		g_pImgDecReg->huf.jr_control = 0x0003;

	   	//each auto-fetch trigger
		g_pImgDecReg->huf.jr_control =(0xfffb)& g_pImgDecReg->huf.jr_control;
		g_pImgDecReg->huf.jr_control = 0x0004 | g_pImgDecReg->huf.jr_control; // [2]: Automatically fetch start.
		g_pImgDecReg->huf.jr_control =(0xfffb)& g_pImgDecReg->huf.jr_control;

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

			jr_status = g_pImgDecReg->huf.jr_status;
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
			IMGDEC_jr_set_port(JR_AUTO_SRC_LUMA_BASE, (SrcYBufBase >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
			IMGDEC_jr_set_port(JR_AUTO_SRC_BASE_EXT0, (SrcYBufBase >> 18) & 0x00003fff);

			IMGDEC_jr_set_port(JR_AUTO_SRC_CHROMA_BASE, (SrcUVBufBase >> 2) & 0x0000ffff); // unit in 4 bytes , 8 bytes align
			IMGDEC_jr_set_port(JR_AUTO_SRC_BASE_EXT1, (SrcUVBufBase >> 18) & 0x00003fff);

			if(i ==0)
			{
				IMGDEC_jr_set_port(JR_MCU_LINE_NUM, 1); // [12:0]: Number of MCU lines in auto fetch DRAM buffer.
				IMGDEC_jr_set_port(JR_AUTO_FETCH_WIDTH, FetchWidth);  // unit in byte, 4 bytes align

				//reset hw
				g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control | (1 << 5); // FSM software reset
				g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control & ~0x0020;

				//for auto-fetch frame start
				g_pImgDecReg->huf.jr_control = 0x0103; // [8]: Picture start. [1]: Automatically fetch source pixels from DRAM. [0]: JR enable.
				g_pImgDecReg->huf.jr_control = 0x0003;
			}

		   	//each auto-fetch trigger
			g_pImgDecReg->huf.jr_control =(0xfffb)& g_pImgDecReg->huf.jr_control;
			g_pImgDecReg->huf.jr_control = 0x0004 | g_pImgDecReg->huf.jr_control; // [2]: Automatically fetch start.
			g_pImgDecReg->huf.jr_control =(0xfffb)& g_pImgDecReg->huf.jr_control;

			//each auto-fetch done check
			do
			{
				if(time_out == 0)
				{
					ERR("Wait JR finish: time out!");
					return -1;
				}

				udelay(1);

				jr_status = g_pImgDecReg->huf.jr_status;
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
EXPORT_SYMBOL(imgdec_hal_scale_start);

void imgdec_hal_jpeg_hw_init(
	JPEG_INFO *jpeg_info_data)
{
	UINT32 dma_size = 5; // jpeg_huff_mode[10:8]=101: 64 x 4 byte DMA size = 64 words = 256 bytes = 0x100 bytes
	UINT32 input_type, y_block, cb_block, cr_block;

	//reset DMA mover, otherwize after PNG HW Decode , MJ mode may move the wrong bitstream
	g_pImgDecReg->jdm.jdm_mode = 1 << 15; // reset JDM only for NEW_MJ_HW

	g_pImgDecReg->jdm.jdm_jpeg_offset = 16; // idct output DMA buffer base address (unit: 16-byte) at 16*16 = 64*4 = 64 words = 256 bytes

	g_pImgDecReg->huf.jpeg_huff_mode = 0x8000; // reset most HUF registers to default values
	g_pImgDecReg->huf.jpeg_huff_mcuno = 0; // restart interval MCU number
	g_pImgDecReg->huf.jpeg_huff_dma_offset_0 = 0; //set DMA offset to save the HUF decoded data

	if(jpeg_info_data->jpegSOF.sof_n == 2) // progressive JPEG
	{
		g_pImgDecReg->huf.jpeg_huff_acval_offset_0 = 0x0020;  // 32
		g_pImgDecReg->huf.jpeg_huff_acval_offset_1 = 0x005c; // 92
		g_pImgDecReg->huf.jpeg_huff_mode = 1 << 14; // DMA pixel width = 16-bit mode
		jpeg_info_data->jpegDHT.huff_ac_size = 240; // AC value(symbal) count = 60x4 >= 16x15=240 bytes
	}
	else // baseline JPEG
	{
		g_pImgDecReg->huf.jpeg_huff_acval_offset_0 = 0x0040;  // 64
		g_pImgDecReg->huf.jpeg_huff_acval_offset_1 = 0x0069; // 105
		g_pImgDecReg->huf.jpeg_huff_mode = 0; // DMA pixel width = 32-bit mode
		jpeg_info_data->jpegDHT.huff_ac_size = 162; // AC value(symbal) count = 41x4 >= 16x10+2=162 bytes
	}

	// set jpeg_huff_mode = use DMA1 only | bitstream is from DMA | 64 x 4byte DMA size | DMA bitstream use little endian
	g_pImgDecReg->huf.jpeg_huff_mode = g_pImgDecReg->huf.jpeg_huff_mode | (1 << 12) | (1 << 11) | ((dma_size & 0x7) << 8) | (0 << 7) | (0x00);
	g_pImgDecReg->huf.jpeg_huff_dma_offset_0 = (1 << 15) | (1 << 12) | 128; // set suspending error timing threshold : prevent to stay too long in huffman

	if(jpeg_info_data->jpegSOF.sof_n == 2)
	{
		g_pImgDecReg->huf.jpeg_huff_bitstream = 152; //Input bitstream location in DMA offset (unit = 4 bytes) for progressive JPEG
	}
	else
	{
		g_pImgDecReg->huf.jpeg_huff_bitstream = 146; //Input bitstream location in DMA offset (unit = 4 bytes) for baseline JPEG
	}

	g_pImgDecReg->huf.jpeg_huff_probe = 0x0400;  // for hw debug

	// set JPEG image horizontal / vertical MCU number.
	g_pImgDecReg->huf.jpeg_huff_jpeghsize = (jpeg_info_data->jpegSOF.Hfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.width+15)>>4) : ((jpeg_info_data->jpegSOF.width+7)>>3);
	g_pImgDecReg->huf.jpeg_huff_jpegvsize = (jpeg_info_data->jpegSOF.Vfactor[0] == 2) ? ((jpeg_info_data->jpegSOF.height+15)>>4) : ((jpeg_info_data->jpegSOF.height+7)>>3);

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
	g_pImgDecReg->huf.jpeg_huff_mode = (g_pImgDecReg->huf.jpeg_huff_mode & 0xfff0) | input_type;
	g_pImgDecReg->huf.jpeg_huff_ctrl =  (1 << 15) | ((cb_block & 0x7) << 12) | ((y_block & 0xf) <<8 );
	g_pImgDecReg->huf.jpeg_huff_status = (((y_block + cb_block + cr_block - 1) & 0xf) << 8); // set mcu number
}
EXPORT_SYMBOL(imgdec_hal_jpeg_hw_init);

void imgdec_hal_jpeg_fill_bs_tables(
	JPEG_INFO *jpeg_info_data)
{
	IMGDEC_JPEG_fill_DQT_reg(jpeg_info_data);

	// YCbCr DC/AC tables
	g_pImgDecReg->huf.jpeg_huff_table = (UINT32)(0x0000)
									| (((UINT32)jpeg_info_data->jpegSOS.DCtable[0] & 0x03) << 14)
									| (((UINT32)jpeg_info_data->jpegSOS.ACtable[0] & 0x03) << 12)
 									| (((UINT32)jpeg_info_data->jpegSOS.DCtable[1] & 0x03) << 10)
 									| (((UINT32)jpeg_info_data->jpegSOS.ACtable[1] & 0x03) << 8)
									| (((UINT32)jpeg_info_data->jpegSOS.DCtable[2] & 0x03) << 6)
									| (((UINT32)jpeg_info_data->jpegSOS.ACtable[2] & 0x03) << 4);

	// DRI
	if(jpeg_info_data->jpegDRI.dri_count)
		g_pImgDecReg->huf.jpeg_huff_mcuno = (UINT32)jpeg_info_data->jpegDRI.dri_count;

	IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, 0, DHT_DC0);
	IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, 1, DHT_DC1);
	IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, 0, DHT_AC0);
	IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, 1, DHT_AC1);

	// set picture start
	g_pImgDecReg->huf.jpeg_huff_ctrl = g_pImgDecReg->huf.jpeg_huff_ctrl | (1 << 2);

}
EXPORT_SYMBOL(imgdec_hal_jpeg_fill_bs_tables);

void imgdec_hal_jpeg_init_MJmode_waitqueue_head() // MJmode uses interrupt to replace polling
{
	init_waitqueue_head(&g_imgdec_MJmode_interrupt_wait);
	g_imgdec_MJmode_interrupt_done = false;
}
EXPORT_SYMBOL(imgdec_hal_jpeg_init_MJmode_waitqueue_head);

irqreturn_t imgdec_hal_jpeg_decode_bs_MJmode_irq_handler( // MJmode uses interrupt to replace polling
	int irq,
	void* dev_id)
{
	DEBUG("imgdec_hal_jpeg_decode_bs_MJmode_irq_handler enter!!!");
	g_imgdec_MJmode_interrupt_done = true;
	wake_up(&g_imgdec_MJmode_interrupt_wait);

	return IRQ_HANDLED;
}
EXPORT_SYMBOL(imgdec_hal_jpeg_decode_bs_MJmode_irq_handler);

int imgdec_hal_jpeg_decode_bs_MJmode(
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
	g_pImgDecReg->jdm.jdm_dma1_mode = (UINT32)(DMA_LEN_256 << 8) | VADDR_MODE_GLOBAL; // [10:8]=DMA transfer length, [3:0]=DMA operation mode
	g_pImgDecReg->jdm.jdm_dma1_start_addr = (DAddr >> 2) & 0x7ff; // [10:0] = Indicate the start address of DMA transfers. Unit:word.

	// JR setting
	//g_imgdec_jr_reg_info.render_input_offset = JR_DMA_BUF_BASE;
	//IMGDEC_jr_set_port(JR_DMA1_CP_BASE, g_imgdec_jr_reg_info.render_input_offset); // [10:0]: Indicates which ping-pong DMA buffer for JR using.

	// set INVQ mode
	g_pImgDecReg->invq.jpeg_invq_mode = INVQ_JPEG_MODE | INVQ_JPEG_NOBYPASS;
	g_pImgDecReg->jdm.jdm_mode = GRAPH_JPEG_SAT | GRAPH_JPEG_OUT_8 | GRAPH_JPEG_32;

	// enable mj mode
	g_pImgDecReg->huf.jpeg_huff_mode = (g_pImgDecReg->huf.jpeg_huff_mode & 0xffef) | (1<<4);

	mj_start_addr = bs_buf_phyaddr;

	// set start ptr high
	g_pImgDecReg->huf.jpeg_huff_acval_offset_2 = mj_start_addr >> 16;

	// set start ptr low
	g_pImgDecReg->huf.jpeg_huff_acval_offset_3 = mj_start_addr & 0xffff;

	mj_stop_kbyte_addr = bs_buf_phyaddr + bs_size;
	mj_stop_kbyte_addr = mj_stop_kbyte_addr >> 10; // The unit of this address is 1K byte.

	// set end ptr high [11:0]
	g_pImgDecReg->huf.jpeg_huff_acval_offset_0 = (5 << 13) | ((mj_stop_kbyte_addr >> 8) & 0xfff);

	// set end ptr low [7:0]
	g_pImgDecReg->huf.jpeg_huff_acval_offset_1 = (1 << 15) | (mj_stop_kbyte_addr & 0xff);

	// set invq table
	g_pImgDecReg->huf.jpeg_huff_scan_ctrl = (((UINT32)jpeg_info_data->jpegSOF.Qtable[0] << 6) |
										((UINT32)jpeg_info_data->jpegSOF.Qtable[1] << 4) |
										((UINT32)jpeg_info_data->jpegSOF.Qtable[2] << 2) |
										(UINT32)jpeg_info_data->jpegSOF.Qtable[3]);

	g_imgdec_MJmode_interrupt_done = false; // MJmode uses interrupt to replace polling

	// start to decode one picture in motion JPEG mode.
	g_pImgDecReg->huf.jpeg_huff_ctrl = (g_pImgDecReg->huf.jpeg_huff_ctrl  & 0xffef) | (1<<4);

	do
	{
		if (wait_event_timeout(g_imgdec_MJmode_interrupt_wait, g_imgdec_MJmode_interrupt_done, (MJMODE_DONE_DEFAULT_TIMEOUT * HZ) / 1000) == 0)
		{
			ERR("MJmode interrupt wait_event_timeout !!!\n");
			ret = -1;
			break;
		} // MJmode uses interrupt to replace polling
		g_imgdec_MJmode_interrupt_done = false; // MJmode uses interrupt to replace polling

		jpeg_huff_ctrl = g_pImgDecReg->huf.jpeg_huff_ctrl;
		if(jpeg_huff_ctrl & 0x0060)  // [6]: mj stop, [5]: mj done
		{
			if(jpeg_huff_ctrl & 0x0040)  // [6]: mj stop
			{
			/*
				mj_start_addr = mj_stop_kbyte_addr << 10; // mj_stop_kbyte_addr is 1024-align -> must be 32-align
				// set start ptr high
				g_pImgDecReg->huf.jpeg_huff_acval_offset_2 = mj_start_addr >> 16;
				// set start ptr low
				g_pImgDecReg->huf.jpeg_huff_acval_offset_3 = mj_start_addr & 0xffff;

				mj_stop_kbyte_addr = mj_start_addr + 1024; // mj_start_addr is 1024-align -> mj_stop_kbyte_addr must be 1024-align
				mj_stop_kbyte_addr = mj_stop_kbyte_addr >> 10; // The unit of this address is 1K byte.
				// set end ptr high [11:0]
				g_pImgDecReg->huf.jpeg_huff_acval_offset_0 = (5 << 13) | ((mj_stop_kbyte_addr >> 8) & 0xfff);
				// set end ptr low [7:0]
				g_pImgDecReg->huf.jpeg_huff_acval_offset_1 = (1 << 15) | (mj_stop_kbyte_addr & 0xff);

				//MJ restart
				g_pImgDecReg->huf.jpeg_huff_ctrl = (g_pImgDecReg->huf.jpeg_huff_ctrl  & 0xff7f) | (1<<7);
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
		else if(g_pImgDecReg->huf.jpeg_huff_rd_val  & 0x0200)  // huff decode error
		{
			g_pImgDecReg->huf.jpeg_huff_rd_addr = (0x1 << 14);

			if(g_pImgDecReg->huf.jpeg_huff_dma_offset_0 & (0x1 << 13))
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

		jpeg_huff_mode = g_pImgDecReg->huf.jpeg_huff_mode;
	}while(jpeg_huff_mode & 0x10);

	g_pImgDecReg->huf.jpeg_huff_mode = g_pImgDecReg->huf.jpeg_huff_mode &  0xffef;
	g_pImgDecReg->huf.jpeg_huff_acval_offset_0 = 0x8000;
	g_pImgDecReg->huf.jpeg_huff_acval_offset_1 = 0x8000;

	IMGDEC_DMA_clear(VADDR_MODE_CLEAR, 0, DMA_LEN_32);

	//disable register invq_mode for logo bug by selina.chang 070809
	g_pImgDecReg->invq.jpeg_invq_mode = 0;

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

		jr_status = g_pImgDecReg->huf.jr_status;
		//DEBUG("Wait JR finish: jr_status = 0x%08x\n", jr_status);

		time_out--;
	}while(!(jr_status & 0x40));

	return ret;
}
EXPORT_SYMBOL(imgdec_hal_jpeg_decode_bs_MJmode);

void imgdec_hal_jpeg_fill_ms_tables(
	JPEG_INFO *jpeg_info_data)
{
	UINT32 Cr_Td, Cr_Ta;
	UINT32 inter_mode, decode_mode, ratio;

	if(jpeg_info_data->jpegDecode.cur_sos_run == 0)
		IMGDEC_JPEG_fill_DQT_reg(jpeg_info_data);

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
		g_pImgDecReg->huf.jpeg_huff_table = (0x0000) | (0<<14) | (0<<12) | (1<<10) | (1<<8) | (Cr_Td<<6) | (Cr_Ta<<4);

		// fill hardware huffman table
		IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[0], DHT_DC0);
		IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[1], DHT_DC1);
	}
	else
	{
		ratio = 0;

		// YCbCr DC/AC tables
		g_pImgDecReg->huf.jpeg_huff_table = 0x0000;

		// fill hardware huffman table
		if((jpeg_info_data->jpegSOF.components == 1) && (jpeg_info_data->jpegSOS.spectral_selection_start == 0))
		{
			IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[0], DHT_DC0);
		}
		else
		{
			IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.ACtable[jpeg_info_data->jpegSOS.identifier[0]-1], DHT_AC0);
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
	g_pImgDecReg->huf.jpeg_huff_status = ((inter_mode & 0x07) << 12) | (ratio << 8) | (decode_mode << 4);

	// [15:10] set subband start, [9:4] set subband end, [3:0] set approximation low bit
	g_pImgDecReg->huf.jpeg_huff_scan_ctrl = ((jpeg_info_data->jpegSOS.spectral_selection_start & 0x3f) << 10) |
										((jpeg_info_data->jpegSOS.spectral_selection_end & 0x3f) << 4) |
										(jpeg_info_data->jpegSOS.successive_approximation_low & 0xf);

	// DRI
	if(jpeg_info_data->jpegDRI.dri_count)
		g_pImgDecReg->huf.jpeg_huff_mcuno = (UINT32)jpeg_info_data->jpegDRI.dri_count;

	/*
	// record information (reg_2, reg_3, reg_4) // for Multi-Scan ??????
	jpeg_info_data->jpegDecode.ms_pre_huf_status = g_pImgDecReg->huf.jpeg_huff_status;
	jpeg_info_data->jpegDecode.ms_pre_huf_scan_ctrl = g_pImgDecReg->huf.jpeg_huff_scan_ctrl;
	jpeg_info_data->jpegDecode.ms_pre_huf_table = g_pImgDecReg->huf.jpeg_huff_table;
	*/

	// reset scan (clear scan and picture start)
	g_pImgDecReg->huf.jpeg_huff_ctrl = 0x0000;
	g_pImgDecReg->huf.jpeg_huff_ctrl = 0x000c;

	// update information to set initial register
	g_pImgDecReg->huf.jpeg_huff_wr_huff_info = 0x8000;
	g_pImgDecReg->huf.jpeg_huff_wr_huff_info = 0x4000;
	g_pImgDecReg->huf.jpeg_huff_wr_huff_info = 0x6000;

}
EXPORT_SYMBOL(imgdec_hal_jpeg_fill_ms_tables);

void imgdec_hal_jpeg_decode_ms_init(
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
			IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[0], DHT_DC0);
			IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[1], DHT_DC1);
		}
		else
		{
			// fill hardware huffman table
			if((jpeg_info_data->jpegSOF.components == 1) && (jpeg_info_data->jpegSOS.spectral_selection_start == 0))
			{
				IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[0], DHT_DC0);
			}
			else
			{
				IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.ACtable[jpeg_info_data->jpegSOS.identifier[0]-1], DHT_AC0);
			}
		}

		// restore register information (reg_2, reg_3, reg_4)
		g_pImgDecReg->huf.jpeg_huff_status = jpeg_info_data->jpegDecode.ms_pre_huf_status;
		g_pImgDecReg->huf.jpeg_huff_scan_ctrl = jpeg_info_data->jpegDecode.ms_pre_huf_scan_ctrl;
		g_pImgDecReg->huf.jpeg_huff_table = jpeg_info_data->jpegDecode.ms_pre_huf_table;

		// reset scan (clear scan and picture start)
		g_pImgDecReg->huf.jpeg_huff_ctrl = 0x0000;
		g_pImgDecReg->huf.jpeg_huff_ctrl = 0x000c;

		// Restore scan information
		g_pImgDecReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[0];
		g_pImgDecReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[1];
		g_pImgDecReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[4];
		g_pImgDecReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[2];

		// Fill new bitstream to DMA
		IMGDEC_DMA_load(VADDR_MODE_GLOBAL, V, H, (152)*4, DMA_LEN_256);
		IMGDEC_DMA_wait();

		g_pImgDecReg->huf.jpeg_huff_bits_dma_offset = (1<<15) |
												(jpeg_info_data->jpegDecode.ms_info[5] & 0x6700) |
												(used_bit << 11) |
												(last >> 2); // 11=8+3 for unit=bit, 2 for unit=word

		g_pImgDecReg->huf.jpeg_huff_wr_huff_info = jpeg_info_data->jpegDecode.ms_info[3];

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
		else if(g_imgdec_jr_reg_info.pre_scaler)  //(pre_scaler & 0x4)
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

	g_pImgDecReg->huf.jpeg_huff_jpeghsize = jpeg_info_data->jpegDecode.horizontal_mcu;
	g_pImgDecReg->huf.jpeg_huff_jpegvsize = jpeg_info_data->jpegDecode.vertical_mcu;

	// set the number of mcus in this scan
	jpeg_info_data->jpegDecode.mcu_end = jpeg_info_data->jpegDecode.horizontal_mcu * jpeg_info_data->jpegDecode.vertical_mcu;

}
EXPORT_SYMBOL(imgdec_hal_jpeg_decode_ms_init);

int imgdec_hal_jpeg_decode_ms_interleave(
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
								IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[1], DHT_DC1);
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
								IMGDEC_JPEG_fill_DHT_reg(jpeg_info_data, jpeg_info_data->jpegSOS.DCtable[2], DHT_DC1);
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
						IMGDEC_DMA_clear(VADDR_MODE_CLEAR, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
						IMGDEC_DMA_wait();
					}
					else
					{
						//IMGDEC_DMA_wait();
						IMGDEC_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
						IMGDEC_DMA_wait();
					}

					// decode block
					time_out = 1000; // 1000 us = 1 ms
					g_pImgDecReg->huf.jpeg_huff_ctrl = g_pImgDecReg->huf.jpeg_huff_ctrl | (0<<1) | 1;
					do
					{
						jpeg_huff_status = g_pImgDecReg->huf.jpeg_huff_status;
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
							jpeg_huff_bits_info = g_pImgDecReg->huf.jpeg_huff_bits_info;
							jpeg_info_data->jpegDecode.pre_subcnt += (0x0040 - (jpeg_huff_bits_info & 0x00ff)) * 4;
							jpeg_info_data->jpegDecode.pre_subcnt -= (jpeg_huff_bits_info & 0x8000) ? (1) : (0);
							jpeg_info_data->jpegDecode.pre_subcnt -= (jpeg_huff_bits_info & 0x1000) ? (2) : (0);
#endif

							IMGDEC_DMA_load(VADDR_MODE_GLOBAL, V, H, (152)*4, DMA_LEN_256);
							IMGDEC_DMA_wait();

							if(jpeg_info_data->jpegDecode.dma_first_flg == 0)
							{
								g_pImgDecReg->huf.jpeg_huff_ctrl = 0x000c;
								g_pImgDecReg->huf.jpeg_huff_bits_dma_offset = 0x8000 | (used_bit << 11) | (last >> 2); // 11=8+3 for unit=bit, 2 for unit=word
								jpeg_info_data->jpegDecode.dma_first_flg = 1;
								//jpeg_info_data->jpegDecode.pre_subcnt = 0;  // for Multi-Scan ??????
							}
							else
							{
								fsm_value = g_pImgDecReg->huf.jpeg_huff_bits_dma_offset & 0x6000;
								g_pImgDecReg->huf.jpeg_huff_bits_dma_offset = (used_bit << 11) | (last >> 2) | fsm_value; // 11=8+3 for unit=bit, 2 for unit=word
							}

							*bs_buf_phyaddr += (256 - last);
						}

						if(jpeg_huff_status & 0x0004)  // decode error
						{
							g_pImgDecReg->huf.jpeg_huff_rd_addr = (0x4 << 12);

							if(g_pImgDecReg->huf.jpeg_huff_dma_offset_0 & (0x1 << 13))
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
					//IMGDEC_DMA_wait();
					IMGDEC_DMA_save(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
					IMGDEC_DMA_wait();
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
EXPORT_SYMBOL(imgdec_hal_jpeg_decode_ms_interleave);

int imgdec_hal_jpeg_decode_ms_noninterleave(
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
				if(g_imgdec_jr_reg_info.pre_scaler)  // (pre_scaler & 0x4)
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
				if(g_imgdec_jr_reg_info.pre_scaler)  //(pre_scaler & 0x4)
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
			IMGDEC_DMA_clear(VADDR_MODE_CLEAR, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
			IMGDEC_DMA_wait();
		}
		else
		{
			//IMGDEC_DMA_wait();
			IMGDEC_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
			IMGDEC_DMA_wait();
		}

		// decode block
		time_out = 1000; // 1000 us = 1 ms
		g_pImgDecReg->huf.jpeg_huff_ctrl = g_pImgDecReg->huf.jpeg_huff_ctrl | (0<<1) | 1;
		do
		{
			jpeg_huff_status = g_pImgDecReg->huf.jpeg_huff_status;
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
				jpeg_huff_bits_info = g_pImgDecReg->huf.jpeg_huff_bits_info;
				jpeg_info_data->jpegDecode.pre_subcnt += (0x0040 - (jpeg_huff_bits_info & 0x00ff)) * 4;
				jpeg_info_data->jpegDecode.pre_subcnt -= (jpeg_huff_bits_info & 0x8000) ? (1) : (0);
				jpeg_info_data->jpegDecode.pre_subcnt -= (jpeg_huff_bits_info & 0x1000) ? (2) : (0);
#endif

				IMGDEC_DMA_load(VADDR_MODE_GLOBAL, V, H, (152)*4, DMA_LEN_256);
				IMGDEC_DMA_wait();

				if(jpeg_info_data->jpegDecode.dma_first_flg == 0)
				{
					g_pImgDecReg->huf.jpeg_huff_ctrl = 0x000c;
					g_pImgDecReg->huf.jpeg_huff_bits_dma_offset = 0x8000 | (used_bit << 11) | (last >> 2); // 11=8+3 for unit=bit, 2 for unit=word
					jpeg_info_data->jpegDecode.dma_first_flg = 1;
					//jpeg_info_data->jpegDecode.pre_subcnt = 0; // for Multi-Scan ??????
				}
				else
				{
					fsm_value = g_pImgDecReg->huf.jpeg_huff_bits_dma_offset & 0x6000;
					g_pImgDecReg->huf.jpeg_huff_bits_dma_offset = (used_bit << 11) | (last >> 2) | fsm_value; // 11=8+3 for unit=bit, 2 for unit=word
				}

				*bs_buf_phyaddr += (256 - last);
			}

			if(jpeg_huff_status & 0x0004)  // decode error
			{
				g_pImgDecReg->huf.jpeg_huff_rd_addr = (0x4 << 12);

				if(g_pImgDecReg->huf.jpeg_huff_dma_offset_0 & (0x1 << 13))
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
		//IMGDEC_DMA_wait();
		IMGDEC_DMA_save(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
		IMGDEC_DMA_wait();

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
EXPORT_SYMBOL(imgdec_hal_jpeg_decode_ms_noninterleave);

void imgdec_hal_jpeg_decode_ms_end(
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
		g_pImgDecReg->huf.jpeg_huff_rd_addr = (i << 12);
		jpeg_info_data->jpegDecode.ms_info[i] = g_pImgDecReg->huf.jpeg_huff_rd_huff_info;
	}
	jpeg_info_data->jpegDecode.ms_info[5] = g_pImgDecReg->huf.jpeg_huff_bits_info;
#endif

	// 2. save bitstream index
#if 0 // for Multi-Scan ??????
		jpeg_huff_bits_info = g_pImgDecReg->huf.jpeg_huff_bits_info;
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
	g_pImgDecReg->huf.jpeg_huff_ctrl = 0x0000;

	jpeg_info_data->jpegDecode.cur_sos_run++;

	if(jpeg_info_data->jpegSOS.components == 1)
	{
		jpeg_info_data->jpegDecode.sof_Hi_0 = jpeg_info_data->jpegSOF.Hfactor[0];
		jpeg_info_data->jpegDecode.sof_Vi_0 = jpeg_info_data->jpegSOF.Vfactor[0];
		jpeg_info_data->jpegDecode.horizontal_mcu = (jpeg_info_data->jpegDecode.sof_Hi_0 == 2) ? ((jpeg_info_data->jpegSOF.width+15)>>4) : ((jpeg_info_data->jpegSOF.width+7)>>3);
		jpeg_info_data->jpegDecode.vertical_mcu = (jpeg_info_data->jpegDecode.sof_Vi_0 == 2) ? ((jpeg_info_data->jpegSOF.height+15)>>4) : ((jpeg_info_data->jpegSOF.height+7)>>3);
	}
}
EXPORT_SYMBOL(imgdec_hal_jpeg_decode_ms_end);

int imgdec_hal_jpeg_decode_ms_scale(
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
					//IMGDEC_DMA_wait();
					IMGDEC_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
					IMGDEC_DMA_wait();

					// set output address of IDCT & INVQ
					g_pImgDecReg->jdm.jdm_jpeg_offset = 64 + y*4 + x*8;
					IMGDEC_JPEG_graph_idct_8x8(jpeg_info_data, 0);
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

				//IMGDEC_DMA_wait();
				IMGDEC_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
				IMGDEC_DMA_wait();

				if(g_imgdec_jr_reg_info.pre_scaler)  //(pre_scaler & 0x4)
				{
					// set output address of IDCT&invq
					g_pImgDecReg->jdm.jdm_jpeg_offset = 80;
					IMGDEC_JPEG_graph_idct_8x8(jpeg_info_data, 1);

					iBlockV = (l_cbcr_in & 0xffffffff) >> 10;
					iBlockH = (l_cbcr_in & 0x3e0);

					//for hw ps jpeg 422(221212)
					if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
						l_cbcr_in -= jpeg_info_data->jpegDecode.mcu_horizontal_mb * 64 * 2 * 2; // last * 2 for 16-bits data
					l_cbcr_in += 64 * 2; // last * 2 for 16-bits data

					//IMGDEC_DMA_wait();
					IMGDEC_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
					IMGDEC_DMA_wait();

					g_pImgDecReg->jdm.jdm_jpeg_offset = 84;
					IMGDEC_JPEG_graph_idct_8x8(jpeg_info_data, 1);
				}
				else
				{
					// set output address of IDCT&invq
					g_pImgDecReg->jdm.jdm_jpeg_offset = 80;
					IMGDEC_JPEG_graph_idct_8x8(jpeg_info_data, 1);
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

				//IMGDEC_DMA_wait();
				IMGDEC_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
				IMGDEC_DMA_wait();

				if(g_imgdec_jr_reg_info.pre_scaler)  // (pre_scaler & 0x4)
				{
					// set output address of IDCT&invq
					g_pImgDecReg->jdm.jdm_jpeg_offset = 88;
					IMGDEC_JPEG_graph_idct_8x8(jpeg_info_data, 2);

					iBlockV = (l_cbcr_in & 0xffffffff) >> 10;
					iBlockH = (l_cbcr_in & 0x3e0);

					//for hw ps jpeg 422(221212)
					if(jpeg_info_data->jpegSOF.Vfactor[1] == 2)
						l_cbcr_in -= jpeg_info_data->jpegDecode.mcu_horizontal_mb * 64 * 2 * 2; // last * 2 for 16-bits data
					l_cbcr_in += 64 * 2; // last * 2 for 16-bits data

					//IMGDEC_DMA_wait();
					IMGDEC_DMA_load(VADDR_MODE_GLOBAL, iBlockV, iBlockH, 0, DMA_LEN_128); // DMA_LEN_128 for 64 x 16-bits data
					IMGDEC_DMA_wait();

					g_pImgDecReg->jdm.jdm_jpeg_offset = 92;
					IMGDEC_JPEG_graph_idct_8x8(jpeg_info_data, 2);
				}
				else
				{
					// set output address of IDCT&invq
					g_pImgDecReg->jdm.jdm_jpeg_offset = 88;
					IMGDEC_JPEG_graph_idct_8x8(jpeg_info_data, 2);
				}
			}

			// render
			IMGDEC_JPEG_scaler_1_mcu((scale_b_x/jpeg_info_data->jpegDecode.sof_Hi_0), (scale_b_y/jpeg_info_data->jpegDecode.sof_Vi_0));

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

				jr_status = g_pImgDecReg->huf.jr_status;
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
EXPORT_SYMBOL(imgdec_hal_jpeg_decode_ms_scale);

//---------------------------------------------------------------

void imgdec_hal_png_hw_init(
	PNG_INFO *png_info_data,
	UINT32 LZ_backward_buf_phyaddr,
	UINT32 scanline_buf_phyaddr,
	UINT32 decoded_buf_phyaddr)
{
	UINT32 bs_endian = 0; // DMA bitstream use 0: big-endian, 1: little-endian.
	UINT32 gray_out_format = PNG_ARGB;
	UINT32 zlib_mode = 0; // 0: decoding PNG images, 1: decoding ZLIB streams.
	UINT32 dma_pixel_buffers = 0; // 0: single pixel buffer, 1: ping-pong pixel buffer.
	UINT32 pixel_buffer_select = 0; // 0: use 1st 512B DMA SRAM as pixel buffer, 1: use 2nd ...
	UINT32 palette_output = 1; // 0: output palette index, 1: output RGB value (in ARGB format).
	UINT32 output_swap = 0; // 0: big-endian, 1: little-endian.

	if(png_info_data->pngIHDR.color_type == PNG_RGB_INDEX_IMAGE)
		png_info_data->pngDecode.bs_dma_size = 3; // 16 x 4byte DMA size
	else
		png_info_data->pngDecode.bs_dma_size = 4; // 32 x 4byte DMA size (128 bytes)


	// ------------ start Init ------------

	// Assert software reset
	g_pImgDecReg->png.png_dec_mode = 0x0001;

	// Set DMA buffer size and bitstream endian
	g_pImgDecReg->png.png_dec_mode = (bs_endian << 1) |
									((png_info_data->pngDecode.bs_dma_size & 0x7) << 2) |
									((gray_out_format & 0x3) << 5) |
									(zlib_mode << 7) |
									(dma_pixel_buffers << 9) |
									(pixel_buffer_select << 10) |
									(palette_output << 11) |
									(output_swap << 12);

	IMGDEC_jr_set_port(JR_DMA1_CP_BASE, 0x180);

	// Set bitstream buffer base address
	g_pImgDecReg->png.png_bs_buf_addr = 0x0000;

	// Set Huffman table base address
	g_pImgDecReg->png.png_huf_tb_addr = 0x0020; // size = 0x0020 - 0x0000 = 32 words

	// Set scanline buffer base address
	g_pImgDecReg->png.png_scl_buf_addr = 0x00c0; // size = 0x00c0 - 0x0020 = 160 words

	if(png_info_data->pngIHDR.color_type == PNG_RGB_INDEX_IMAGE)
	{
		// Set Huffman table base address
		g_pImgDecReg->png.png_huf_tb_addr = 0x0010; // size = 0x0010 - 0x0000 = 16 words

		// Set scanline buffer base address
		g_pImgDecReg->png.png_scl_buf_addr = 0x00a0; // size = 0x00a0 - 0x0010 = 144 words
	}

	// Set backward data external base address
	g_pImgDecReg->png.png_bw_ext_addr_m = LZ_backward_buf_phyaddr >> 16;
	g_pImgDecReg->png.png_bw_ext_addr_l = LZ_backward_buf_phyaddr & 0xffff;

	// Set scanline data external base address
	g_pImgDecReg->png.png_scl_ext_addr_m = scanline_buf_phyaddr >> 16;
	g_pImgDecReg->png.png_scl_ext_addr_l = scanline_buf_phyaddr & 0xffff;

	// Set decoded pixel data external base address
	g_pImgDecReg->png.png_pxl_ext_addr_m = decoded_buf_phyaddr >> 16;
	g_pImgDecReg->png.png_pxl_ext_addr_l = (decoded_buf_phyaddr & 0xffff) | 0x1; // bit[0]: reset flag


	// ------------ set IHDR ------------

	g_pImgDecReg->png.png_img_width = png_info_data->pngIHDR.width & 0xffff;

	g_pImgDecReg->png.png_img_height = png_info_data->pngIHDR.height & 0xffff;

	g_pImgDecReg->png.png_img_type =  ((png_info_data->pngIHDR.interlace_method & 0x1) << 8) |
									((png_info_data->pngIHDR.color_type & 0x7) << 5) |
									(png_info_data->pngIHDR.bit_depth & 0x1f);

	// Write number of render lines (minus 1)
#if PNG_JR_PIPELINE_ENABLE
	g_pImgDecReg->png.png_pxl_line = 7; // = (PNG MCU height)8 - 1
#else
	g_pImgDecReg->png.png_pxl_line = png_info_data->pngIHDR.height - 1;
#endif


	// ------------ set PLTE ------------

	IMGDEC_PNG_fill_palette(png_info_data->pngPLTE.pColorMap);

}
EXPORT_SYMBOL(imgdec_hal_png_hw_init);

int imgdec_hal_png_decode(
	PNG_INFO *png_info_data,
	UINT32 bs_buf_phyaddr,
	UINT32 bs_size,
	UINT8 *bs_buf_ptr,
	UINT32 LZ_backward_buf_phyaddr,
	UINT32 decoded_buf_phyaddr)
{
	UINT32 bsbuf_phyaddr_startptr;
	UINT32 bsbuf_phyaddr_endptr;
	UINT32 offset;

	// for Decode block data
	UINT8 render_finish;
	UINT32 png_interrupt;
	UINT32 png_bs_used_cnt;
	UINT32 png_bs_used_bit;
	UINT32 bs_used_dma_cnt_ext;
	UINT32 bs_used_byte;
	UINT32 bs_used_bit;
	UINT32 chunk_remain_byte;
	UINT32 reg_read_data;
	UINT32 bs_offset;
	UINT32 pre_bs_pos;
	UINT32 cur_bs_pos;
	UINT32 adler_chksum;
	UINT32 time_out = 10000000; // 10000000 us = 10 sec for 8192x8192 ?????? too long!

#if PNG_JR_PIPELINE_ENABLE
	UINT8 end_of_image = false;
	UINT32 decode_line_on_frame = 0;
#endif

	bsbuf_phyaddr_startptr = bs_buf_phyaddr;

	// Set bistream external memory start address
	g_pImgDecReg->png.png_bs_str_addr_m = bsbuf_phyaddr_startptr >> 16;
	g_pImgDecReg->png.png_bs_str_addr_l = bsbuf_phyaddr_startptr & 0xffff;

	bsbuf_phyaddr_endptr = bsbuf_phyaddr_startptr + bs_size;
	offset = 32 - (bsbuf_phyaddr_endptr & 0x1f);
	bsbuf_phyaddr_endptr += offset;

	// Set bistream external memory end address
	g_pImgDecReg->png.png_bs_end_addr_m = bsbuf_phyaddr_endptr >> 16;
	g_pImgDecReg->png.png_bs_end_addr_l = bsbuf_phyaddr_endptr & 0xffff;

	// Set chunk size
	g_pImgDecReg->png.png_chunk_size_m = png_info_data->pngIDAT.chunk_length >> 16;
	g_pImgDecReg->png.png_chunk_size_l = png_info_data->pngIDAT.chunk_length & 0xffff;


	//------ Decode block data untill end of imgae (begin) ------

	IMGDEC_PNG_init_bs_bits(png_info_data);

	IMGDEC_PNG_parse_block_headers(png_info_data, bs_buf_ptr);

	// Write offset value of current position from chunk type
	g_pImgDecReg->png.png_bs_offset =  ((png_info_data->pngIDAT.bs_byte_offset << 3) -
									(png_info_data->pngIDAT.bs_bit_offset & 0x7)) & 0x7fff ; // [14:0]

	// Start PNG decoder
	g_pImgDecReg->png.png_dec_ctrl = 0x0001;

	render_finish = false;
	while(!render_finish)
	{
		if(time_out == 0)
		{
			ERR("PNG decode: time out!");
			return -1;
		}
		udelay(1);
		time_out--;

		png_interrupt = g_pImgDecReg->png.png_interrupt;
		//DEBUG("png_interrupt = 0x%08x\n", png_interrupt);

		if(png_interrupt & 0x1) // Bitstream empty interrupt.
		{
			// If input file has already been loaded into SDRAM, it means decode error!
			WARNING("PNG bitstream empty interrupt!\n");

			// Clear interrupt
			g_pImgDecReg->png.png_interrupt = 0x0100;

		/*
			bsbuf_phyaddr_startptr = bsbuf_phyaddr_endptr;

			// Set bistream external memory start address
			g_pImgDecReg->png.png_bs_str_addr_m = bsbuf_phyaddr_startptr >> 16;
			g_pImgDecReg->png.png_bs_str_addr_l = bsbuf_phyaddr_startptr & 0xffff;

			bsbuf_phyaddr_endptr = bsbuf_phyaddr_startptr + bs_size;
			offset = 32 - (bsbuf_phyaddr_endptr & 0x1f);
			bsbuf_phyaddr_endptr += offset;

			// Set bistream external memory end address
			g_pImgDecReg->png.png_bs_end_addr_m = bsbuf_phyaddr_endptr >> 16;
			g_pImgDecReg->png.png_bs_end_addr_l = bsbuf_phyaddr_endptr & 0xffff;
		*/
		}
		else if(png_interrupt & 0x2) // End of block interrupt.
		{
			png_bs_used_cnt = g_pImgDecReg->png.png_bs_used_cnt;
			png_bs_used_bit = g_pImgDecReg->png.png_bs_used_bit;
			bs_used_dma_cnt_ext = (png_bs_used_bit & 0xe000) >> 13; // [15:13]
			bs_used_byte = (png_bs_used_bit & 0x1ff8) >> 3; // [12:3]
			bs_used_bit = png_bs_used_bit & 0x7; // [2:0]

			// Calculate the total used bytes
			if (bs_used_dma_cnt_ext != 0)
			{
				png_info_data->pngIDAT.dma_count_ext += bs_used_dma_cnt_ext;
				g_pImgDecReg->png.png_bs_used_bit = 0x0000; // clear extension bits of DMA count
			}
			png_info_data->pngIDAT.bs_byte_offset = (png_info_data->pngIDAT.dma_count_ext << 16);
			png_info_data->pngIDAT.bs_byte_offset += png_bs_used_cnt;
			png_info_data->pngIDAT.bs_byte_offset *= (8 << png_info_data->pngDecode.bs_dma_size);
			png_info_data->pngIDAT.bs_byte_offset += bs_used_byte;
			pre_bs_pos = png_info_data->pngIDAT.bs_byte_offset;

			if(png_interrupt & 0x8) // End of ZLIB interrupt.
			{
				//DEBUG("PNG end of block and ZLIB interrupt!\n");

				// Clear interrupt, clear both EOB and EOZ
				g_pImgDecReg->png.png_interrupt = 0x0a00;
			}
			else
			{
				//DEBUG("PNG end of block interrupt!\n");

				// png_info_data->pngIDAT.chunk_remain_bytes = 1; // mantis bug ??????
				IMGDEC_PNG_init_bs_bits(png_info_data);
				IMGDEC_PNG_need_bs_bits(png_info_data, bs_buf_ptr, bs_used_bit);
				IMGDEC_PNG_drop_bs_bits(png_info_data, bs_used_bit);

				// read remaining chunk size
				chunk_remain_byte = ((g_pImgDecReg->png.png_rem_chunk_size_m & 0xffff) << 16) |
									(g_pImgDecReg->png.png_rem_chunk_size_l & 0xffff);

				png_info_data->pngIDAT.chunk_remain_bytes = chunk_remain_byte;

				// If EOB is near to chunk boundary, record how many chunks needed to have at least 16 bits
				// while((png_info_data->pngIDAT.chunk_remain_bytes*8 + png_info_data->pngIDAT.bs_bit_offset) < 16) ??????

				IMGDEC_PNG_parse_block_headers(png_info_data, bs_buf_ptr);

				// Calculate the bitstream offset
				cur_bs_pos = png_info_data->pngIDAT.bs_byte_offset;

				cur_bs_pos = (cur_bs_pos << 3) - png_info_data->pngIDAT.bs_bit_offset;

				pre_bs_pos = (pre_bs_pos << 3) + bs_used_bit;

				bs_offset = cur_bs_pos - pre_bs_pos;

				// Skip the chunk headers that hardware already parsed
				// bs_offset = bs_offset - (stPngHwInfo->skip_chunk_num * 12 * 8); // stPngHwInfo->skip_chunk_num = ??????

				// Write offset value
				g_pImgDecReg->png.png_bs_offset = bs_offset & 0x7fff; // [14:0]

				// Clear interrupt
				g_pImgDecReg->png.png_interrupt = 0x0200;

				// Start PNG decoder
				g_pImgDecReg->png.png_dec_ctrl = 0x0001;
			}
		}
		else if(png_interrupt & 0x4) // End of chunk interrupt.
		{
			// No need to start decoder becase EOC does NOT de-assert dec_start
			//DEBUG("PNG end of chunk interrupt!\n");

			// Clear interrupt
			g_pImgDecReg->png.png_interrupt = 0x0400;
		}
		else if(png_interrupt & 0x8) // End of ZLIB interrupt.
		{
			//DEBUG("PNG end of ZLIB interrupt!\n");

			// Clear interrupt
			g_pImgDecReg->png.png_interrupt = 0x0800;
		}
		else if(png_interrupt & 0x10) // End of image interrupt.
		{
			DEBUG("PNG end of image interrupt!\n");

#if PNG_JR_PIPELINE_ENABLE
			end_of_image = true;
#else
			// finish decoding whole picture
			render_finish = true;
#endif

			// Clear interrupt
			g_pImgDecReg->png.png_interrupt = 0x1000;

			// Read Adler32 checksum value
			g_pImgDecReg->png.png_probe_addr = 0x0000;
			reg_read_data = g_pImgDecReg->png.png_probe_value;
			g_pImgDecReg->png.png_probe_addr = 0x0001;
			adler_chksum = g_pImgDecReg->png.png_probe_value;
			adler_chksum = (reg_read_data << 16) | adler_chksum;
			//DEBUG("PNG Adler32 checksum: 0x%08x\n", adler_chksum);

			if(png_interrupt & 0x40) // Decode error interrupt.
			{
				// Check error type
				reg_read_data =  g_pImgDecReg->png.png_dec_ctrl;
				DEBUG("reg_read_data = 0x%08x\n", reg_read_data);
				if(reg_read_data & 0x02)
					ERR("PNG decode error interrupt: Error Type = DMA busy!\n");
				else if(reg_read_data & 0x04)
					ERR("PNG decode error interrupt: Error Type = Chunk Type Error!\n");
				else if(reg_read_data & 0x08)
					ERR("PNG decode error interrupt: Error Type = Adler Checksum Error!\n");
				else if(reg_read_data & 0x10)
					ERR("PNG decode error interrupt: Error Type = Decode Suspending Error!\n");
				else if(reg_read_data & 0x20)
					ERR("PNG decode error interrupt: Error Type = Non-compressed Block Length Error!\n");
				else
					ERR("PNG decode error interrupt: Error Type = Unknown Type Error!\n");

				return -1;
			}
		}
		else if(png_interrupt & 0x20) // Pixel buffer full (ready) interrupt.
		{
			// Clear interrupt
			g_pImgDecReg->png.png_interrupt = 0x2000;

#if PNG_JR_PIPELINE_ENABLE
			//DEBUG("PNG Pixel buffer full (ready) interrupt!\n");

			// Set decoded pixel data external base address
			g_pImgDecReg->png.png_pxl_ext_addr_m = decoded_buf_phyaddr >> 16;
			g_pImgDecReg->png.png_pxl_ext_addr_l = (decoded_buf_phyaddr & 0xffff) | 0x1; // bit[0]: reset flag

			if(imgdec_hal_png_scale(png_info_data, decoded_buf_phyaddr, decode_line_on_frame) < 0)
				return -1;

			decode_line_on_frame += 8; // 8 for PNG MCU height
			if(decode_line_on_frame >= png_info_data->pngIHDR.height)
			{
				// finish decoding whole picture
				render_finish = true;
				DEBUG("PNG image JR output finish!\n");

				png_interrupt = g_pImgDecReg->png.png_interrupt;
				if(!end_of_image && !(png_interrupt & 0x10))
					WARNING("Can't get PNG end of image interrupt!");
			}
			else
			{
				if((png_info_data->pngIHDR.height - decode_line_on_frame) < 8) // 8 for PNG MCU height
					g_pImgDecReg->png.png_pxl_line = png_info_data->pngIHDR.height - decode_line_on_frame - 1;
				else
					g_pImgDecReg->png.png_pxl_line = 7; // = (PNG MCU height)8 - 1
			}

			time_out = 1000000; // 1000000 x 1 us = 1 sec ?????? too long for PNG decode 1 MCU line
#else
			// If ouput buffer has enough big size in SDRAM, it means decode error!
			WARNING("PNG Pixel buffer full (ready) interrupt!\n");

			// finish decoding whole picture
			render_finish = true;
#endif
		}
		else if(png_interrupt & 0x40) // Decode error interrupt.
		{
			// Check error type
			reg_read_data =  g_pImgDecReg->png.png_dec_ctrl;
			DEBUG("reg_read_data = 0x%08x\n", reg_read_data);
			if(reg_read_data & 0x02)
				ERR("PNG decode error interrupt: Error Type = DMA busy!\n");
			else if(reg_read_data & 0x04)
				ERR("PNG decode error interrupt: Error Type = Chunk Type Error!\n");
			else if(reg_read_data & 0x08)
				ERR("PNG decode error interrupt: Error Type = Adler Checksum Error!\n");
			else if(reg_read_data & 0x10)
				ERR("PNG decode error interrupt: Error Type = Decode Suspending Error!\n");
			else if(reg_read_data & 0x20)
				ERR("PNG decode error interrupt: Error Type = Non-compressed Block Length Error!\n");
			else
				ERR("PNG decode error interrupt: Error Type = Unknown Type Error!\n");

			return -1;
		}
	}

	//------ Decode block data untill end of imgae (end) ------

	return 0;
}
EXPORT_SYMBOL(imgdec_hal_png_decode);

int imgdec_hal_png_scale(
	PNG_INFO *png_info_data,
	UINT32 decoded_buf_phyaddr,
	UINT32 run_time)
{
	UINT32 jr_status;
	UINT32 time_out = 3000000; // 3000000 us = 3 sec ?????? too long for PNG scaling

	if(run_time==0)
	{
		// set source buffer address: 4-byte_unit, 8-byte_align
		IMGDEC_jr_set_port(JR_AUTO_SRC_LUMA_BASE, (decoded_buf_phyaddr >> 2) & 0x0000ffff);
		IMGDEC_jr_set_port(JR_AUTO_SRC_BASE_EXT0, (decoded_buf_phyaddr >> 18) & 0x00003fff);

		IMGDEC_jr_set_port(JR_AUTO_SRC_CHROMA_BASE, (decoded_buf_phyaddr >> 2) & 0x0000ffff);
		IMGDEC_jr_set_port(JR_AUTO_SRC_BASE_EXT1, (decoded_buf_phyaddr >> 18) & 0x00003fff);

		// set auto-fetch
#if PNG_JR_PIPELINE_ENABLE
		IMGDEC_jr_set_port(JR_MCU_LINE_NUM, 1); // [12:0]: Number of MCU lines in auto fetch DRAM buffer.
#else
		IMGDEC_jr_set_port(JR_MCU_LINE_NUM, ((png_info_data->pngIHDR.height + 7) >> 3)); // [12:0]: Number of MCU lines in auto fetch DRAM buffer.
#endif
		IMGDEC_jr_set_port(JR_AUTO_FETCH_WIDTH, (png_info_data->pngIHDR.width << 2)); // for Auto-fetch source buffer

		//reset hw
		g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control | (1 << 5); // FSM software reset
		g_pImgDecReg->huf.jr_control = g_pImgDecReg->huf.jr_control & ~0x0020;

		//for auto-fetch frame start
		g_pImgDecReg->huf.jr_control = 0x0103; // [8]: Picture start. [1]: Automatically fetch source pixels from DRAM. [0]: JR enable.
		g_pImgDecReg->huf.jr_control = 0x0003;
	}

   	//each auto-fetch trigger
	g_pImgDecReg->huf.jr_control =(0xfffb)& g_pImgDecReg->huf.jr_control;
	g_pImgDecReg->huf.jr_control = 0x0004 | g_pImgDecReg->huf.jr_control; // [2]: Automatically fetch start.
	g_pImgDecReg->huf.jr_control =(0xfffb)& g_pImgDecReg->huf.jr_control;

	//each auto-fetch done check
	do
	{
		if(time_out == 0)
		{
			ERR("Wait JR finish: time out!");
			return -1;
		}

		udelay(1);

		jr_status = g_pImgDecReg->huf.jr_status;
		//DEBUG("Wait JR finish: jr_status = 0x%08x\n", jr_status);

		time_out--;
	}while(!(jr_status & 0x04));

	return 0;
}
EXPORT_SYMBOL(imgdec_hal_png_scale);
