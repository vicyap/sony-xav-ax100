#ifndef __IMGDECSND_HAL_H__
#define __IMGDECSND_HAL_H__

#include <linux/types.h>
#include <linux/module.h>
#include <mach/common.h>
#include <mach/hal/regmap/reg_system.h>
#include <mach/hal/regmap/reg_imgdecsnd.h>
#include <mach/imgdecsnd.h>
#include <mach/gp_chunkmem.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

#define MOON_SND_IMG_MISC_RESET		0x1 << 25
#define MOON_SND_HUFF_RESET			0x1 << 26
#define MOON_SND_JPG_IQT_RESET		0x1 << 27
#define MOON_SND_JR_RESET				0x1 << 28

#define MOON_SND_IMG_MISC_CLKEN		0x1 << 25
#define MOON_SND_HUFF_CLKEN			0x1 << 26
#define MOON_SND_JPG_IQT_CLKEN		0x1 << 27
#define MOON_SND_JR_CLKEN				0x1 << 28

#define MOON_SND_IMG_MISC_GCLKEN		0x1 << 25
#define MOON_SND_HUFF_GCLKEN			0x1 << 26
#define MOON_SND_JPG_IQT_GCLKEN		0x1 << 27
#define MOON_SND_JR_GCLKEN			0x1 << 28

// JR_FILTER_COEF struct member size:
#define JR_COE_SET_1_SIZE  (JR_COE_V_SET_1_END - JR_COE_V_SET_1_START)
#define JR_COE_SET_2_SIZE  (JR_COE_V_SET_2_END - JR_COE_V_SET_2_START)
#define JR_COE_SET_3_SIZE  (JR_COE_V_SET_3_END - JR_COE_V_SET_3_START)
#define JR_COE_SET_4_SIZE  (JR_COE_V_SET_4_END - JR_COE_V_SET_4_START)

//JPEG header maker code:
#define SOI		0xFFD8
#define EOI		0xFFD9
#define APP0		0xFFE0
#define APP1		0xFFE1
#define DQT		0xFFDB
#define SOF0		0xFFC0
#define SOF1		0xFFC1
#define SOF2		0xFFC2
#define SOF3		0xFFC3
#define SOF5		0xFFC5
#define SOF6		0xFFC6
#define SOF7		0xFFC7
#define JPG		0xFFC8
#define SOF9		0xFFC9
#define SOF10	0xFFCA
#define SOF11	0xFFCB
#define SOF13	0xFFCD
#define SOF14	0xFFCE
#define SOF15	0xFFCF
#define DHT		0xFFC4
#define DRI		0xFFDD
#define SOS		0xFFDA
#define RD0		0xFFD0
#define RD1		0xFFD1
#define RD2		0xFFD2
#define RD3		0xFFD3
#define RD4		0xFFD4
#define RD5		0xFFD5
#define RD6 		0xFFD6
#define RD7		0xFFD7
#define M_NULL1	0xFFBF
#define M_NULL2	0xFFFF

// EXIF IFD Tag:
#define JPEG_EXIF_INTEROPINDEX					0x0001
#define JPEG_EXIF_INTEROPVERSION				0x0002
#define JPEG_EXIF_PROCESSINGSOFTWARE			0x000B
#define JPEG_EXIF_SUBFILETYPE					0x00FE
#define JPEG_EXIF_OLDSUBFILETYPE				0x00FF
#define JPEG_EXIF_IMAGEWIDTH					0x0100
#define JPEG_EXIF_IMAGEHEIGHT					0x0101
#define JPEG_EXIF_BITSPERSAMPLE				0x0102
#define JPEG_EXIF_COMPRESSION					0x0103
#define JPEG_EXIF_PHOTOMETRICINTERPRETATION	0x0106
#define JPEG_EXIF_THRESHOLDING				0x0107
#define JPEG_EXIF_CELLWIDTH					0x0108
#define JPEG_EXIF_CELLLENGTH					0x0109
#define JPEG_EXIF_FILLORDER						0x010A
#define JPEG_EXIF_DOCUMENTNAME				0x010D
#define JPEG_EXIF_IMAGEDESCRIPTION			0x010E
#define JPEG_EXIF_MAKE							0x010F
#define JPEG_EXIF_MODEL						0x0110
#define JPEG_EXIF_STRIPOFFSETS					0x0111
#define JPEG_EXIF_ORIENTATION					0x0112
#define JPEG_EXIF_SAMPLESPERPIXEL				0x0115
#define JPEG_EXIF_ROWSPERSTRIP				0x0116
#define JPEG_EXIF_STRIPBYTECOUNTS				0x0117
#define JPEG_EXIF_MINSAMPLEVALUE				0x0118
#define JPEG_EXIF_MAXSAMPLEVALUE				0x0119
#define JPEG_EXIF_XRESOLUTION					0x011A
#define JPEG_EXIF_YRESOLUTION					0x011B
#define JPEG_EXIF_PLANARCONFIGURATION		0x011C
#define JPEG_EXIF_PAGENAME						0x011D
#define JPEG_EXIF_XPOSITION					0x011E
#define JPEG_EXIF_YPOSITION					0x011F
#define JPEG_EXIF_FREEOFFSETS					0x0120
#define JPEG_EXIF_FREEBYTECOUNTS				0x0121
#define JPEG_EXIF_GRAYRESPONSEUNIT			0x0122
#define JPEG_EXIF_GRAYRESPONSECURVE			0x0123
#define JPEG_EXIF_T4OPTIONS					0x0124
#define JPEG_EXIF_T6OPTIONS					0x0125
#define JPEG_EXIF_RESOLUTIONUNIT				0x0128
#define JPEG_EXIF_PAGENUMBER					0x0129
#define JPEG_EXIF_COLORRESPONSEUNIT			0x012C
#define JPEG_EXIF_TRANSFERFUNCTION			0x012D
#define JPEG_EXIF_SOFTWARE						0x0131
#define JPEG_EXIF_MODIFYDATE					0x0132
#define JPEG_EXIF_ARTIST						0x013B
#define JPEG_EXIF_HOSTCOMPUTER				0x013C
#define JPEG_EXIF_PREDICTOR					0x013D
#define JPEG_EXIF_WHITEPOINT					0x013E
#define JPEG_EXIF_PRIMARYCHROMATICITIES		0x013F
#define JPEG_EXIF_COLORMAP						0x0140
#define JPEG_EXIF_HALFTONEHINTS				0x0141
#define JPEG_EXIF_TILEWIDTH					0x0142
#define JPEG_EXIF_TILELENGTH					0x0143
#define JPEG_EXIF_TILEOFFSETS					0x0144
#define JPEG_EXIF_TILEBYTECOUNTS				0x0145
#define JPEG_EXIF_BADFAXLINES					0x0146
#define JPEG_EXIF_CLEANFAXDATA				0x0147
#define JPEG_EXIF_CONSECUTIVEBADFAXLINES		0x0148
#define JPEG_EXIF_SUBIFD						0x014A
#define JPEG_EXIF_INKSET						0x014C
#define JPEG_EXIF_INKNAMES						0x014D
#define JPEG_EXIF_NUMBEROFINKS				0x014E
#define JPEG_EXIF_DOTRANGE						0x0150
#define JPEG_EXIF_TARGETPRINTER				0x0151
#define JPEG_EXIF_EXTRASAMPLES				0x0152
#define JPEG_EXIF_SAMPLEFORMAT				0x0153
#define JPEG_EXIF_SMINSAMPLEVALUE				0x0154
#define JPEG_EXIF_SMAXSAMPLEVALUE				0x0155
#define JPEG_EXIF_TRANSFERRANGE				0x0156
#define JPEG_EXIF_CLIPPATH						0x0157
#define JPEG_EXIF_XCLIPPATHUNITS				0x0158
#define JPEG_EXIF_YCLIPPATHUNITS				0x0159
#define JPEG_EXIF_INDEXED						0x015A
#define JPEG_EXIF_JPEGTABLES					0x015B
#define JPEG_EXIF_OPIPROXY						0x015F
#define JPEG_EXIF_GLOBALPARAMETERSIFD		0x0190
#define JPEG_EXIF_PROFILETYPE					0x0191
#define JPEG_EXIF_FAXPROFILE					0x0192
#define JPEG_EXIF_CODINGMETHODS				0x0193
#define JPEG_EXIF_VERSIONYEAR					0x0194
#define JPEG_EXIF_MODENUMBER					0x0195
#define JPEG_EXIF_DECODE						0x01B1
#define JPEG_EXIF_DEFAULTIMAGECOLOR			0x01B2
//#define JPEG_EXIF_JPEGTABLES					0x01B5 // redefined in 0x015B
#define JPEG_EXIF_JPEGPROC						0x0200
#define JPEG_EXIF_THUMBNAILOFFSET				0x0201
#define JPEG_EXIF_THUMBNAILLENGTH				0x0202
#define JPEG_EXIF_JPEGRESTARTINTERVAL			0x0203
#define JPEG_EXIF_JPEGLOSSLESSPREDICTORS		0x0205
#define JPEG_EXIF_JPEGPOINTTRANSFORMS		0x0206
#define JPEG_EXIF_JPEGQTABLES					0x0207
#define JPEG_EXIF_JPEGDCTABLES					0x0208
#define JPEG_EXIF_JPEGACTABLES					0x0209
#define JPEG_EXIF_YCBCRCOEFFICIENTS			0x0211
#define JPEG_EXIF_YCBCRSUBSAMPLING			0x0212
#define JPEG_EXIF_YCBCRPOSITIONING			0x0213
#define JPEG_EXIF_REFERENCEBLACKWHITE		0x0214
#define JPEG_EXIF_STRIPROWCOUNTS				0x022F
#define JPEG_EXIF_APPLICATIONNOTES			0x02BC
#define JPEG_EXIF_RELATEDIMAGEFILEFORMAT		0x1000
#define JPEG_EXIF_RELATEDIMAGEWIDTH			0x1001
#define JPEG_EXIF_RELATEDIMAGEHEIGHT			0x1002
#define JPEG_EXIF_RATING						0x4746
#define JPEG_EXIF_RATINGPERCENT				0x4749
#define JPEG_EXIF_IMAGEID						0x800D
#define JPEG_EXIF_WANGANNOTATION				0x80A4
#define JPEG_EXIF_MATTEING						0x80E3
#define JPEG_EXIF_DATATYPE						0x80E4
#define JPEG_EXIF_IMAGEDEPTH					0x80E5
#define JPEG_EXIF_TILEDEPTH					0x80E6
#define JPEG_EXIF_MODEL2						0x827D
#define JPEG_EXIF_CFAREPEATPATTERNDIM		0x828D
#define JPEG_EXIF_CFAPATTERN2					0x828E
#define JPEG_EXIF_BATTERYLEVEL					0x828F
#define JPEG_EXIF_KODAKIFD						0x8290
#define JPEG_EXIF_COPYRIGHT					0x8298
#define JPEG_EXIF_EXPOSURETIME					0x829A
#define JPEG_EXIF_FNUMBER						0x829D
#define JPEG_EXIF_MDFILETAG					0x82A5
#define JPEG_EXIF_MDSCALEPIXEL					0x82A6
#define JPEG_EXIF_MDCOLORTABLE				0x82A7
#define JPEG_EXIF_MDLABNAME					0x82A8
#define JPEG_EXIF_MDSAMPLEINFO				0x82A9
#define JPEG_EXIF_MDPREPDATE					0x82AA
#define JPEG_EXIF_MDPREPTIME					0x82AB
#define JPEG_EXIF_MDFILEUNITS					0x82AC
#define JPEG_EXIF_PIXELSCALE					0x830E
#define JPEG_EXIF_IPTC_NAA						0x83BB
#define JPEG_EXIF_INTERGRAPHPACKETDATA		0x847E
#define JPEG_EXIF_INTERGRAPHFLAGREGISTERS	0x847F
#define JPEG_EXIF_INTERGRAPHMATRIX			0x8480
#define JPEG_EXIF_MODELTIEPOINT				0x8482
#define JPEG_EXIF_SITE							0x84E0
#define JPEG_EXIF_COLORSEQUENCE				0x84E1
#define JPEG_EXIF_IT8HEADER					0x84E2
#define JPEG_EXIF_RASTERPADDING				0x84E3
#define JPEG_EXIF_BITSPERRUNLENGTH			0x84E4
#define JPEG_EXIF_BITSPEREXTENDEDRUNLENGTH	0x84E5
#define JPEG_EXIF_COLORTABLE					0x84E6
#define JPEG_EXIF_IMAGECOLORINDICATOR		0x84E7
#define JPEG_EXIF_BACKGROUNDCOLORINDICATOR	0x84E8
#define JPEG_EXIF_IMAGECOLORVALUE				0x84E9
#define JPEG_EXIF_BACKGROUNDCOLORVALUE		0x84EA
#define JPEG_EXIF_PIXELINTENSITYRANGE			0x84EB
#define JPEG_EXIF_TRANSPARENCYINDICATOR		0x84EC
#define JPEG_EXIF_COLORCHARACTERIZATION		0x84ED
#define JPEG_EXIF_HCUSAGE						0x84EE
#define JPEG_EXIF_TRAPINDICATOR				0x84EF
#define JPEG_EXIF_CMYKEQUIVALENT				0x84F0
#define JPEG_EXIF_SEMINFO						0x8546
#define JPEG_EXIF_AFCP_IPTC					0x8568
#define JPEG_EXIF_MODELTRANSFORM				0x85D8
#define JPEG_EXIF_WB_GRGBLEVELS				0x8602
#define JPEG_EXIF_LEAFDATA						0x8606
#define JPEG_EXIF_PHOTOSHOPSETTINGS			0x8649
#define JPEG_EXIF_EXIFOFFSET					0x8769
#define JPEG_EXIF_ICC_PROFILE					0x8773
#define JPEG_EXIF_IMAGELAYER					0x87AC
#define JPEG_EXIF_GEOTIFFDIRECTORY			0x87AF
#define JPEG_EXIF_GEOTIFFDOUBLEPARAMS		0x87B0
#define JPEG_EXIF_GEOTIFFASCIIPARAMS			0x87B1
#define JPEG_EXIF_EXPOSUREPROGRAM			0x8822
#define JPEG_EXIF_SPECTRALSENSITIVITY			0x8824
#define JPEG_EXIF_GPSINFO						0x8825
#define JPEG_EXIF_ISO_SPEEDRATING				0x8827
#define JPEG_EXIF_OPTO_ELECTRICCONVFACTOR	0x8828
#define JPEG_EXIF_INTERLACE					0x8829
#define JPEG_EXIF_TIMEZONEOFFSET				0x882A
#define JPEG_EXIF_SELFTIMERMODE				0x882B
#define JPEG_EXIF_FAXRECVPARAMS				0x885C
#define JPEG_EXIF_FAXSUBADDRESS				0x885D
#define JPEG_EXIF_FAXRECVTIME					0x885E
#define JPEG_EXIF_LEAFSUBIFD					0x888A
#define JPEG_EXIF_EXIFVERSION					0x9000
#define JPEG_EXIF_DATETIMEORIGINAL			0x9003
#define JPEG_EXIF_CREATEDATE					0x9004
#define JPEG_EXIF_COMPONENTSCONFIGURATION	0x9101
#define JPEG_EXIF_COMPRESSEDBITSPERPIXEL		0x9102
#define JPEG_EXIF_SHUTTERSPEEDVALUE			0x9201
#define JPEG_EXIF_APERTUREVALUE				0x9202
#define JPEG_EXIF_BRIGHTNESSVALUE				0x9203
#define JPEG_EXIF_EXPOSURECOMPENSATION		0x9204
#define JPEG_EXIF_MAXAPERTUREVALUE			0x9205
#define JPEG_EXIF_SUBJECTDISTANCE				0x9206
#define JPEG_EXIF_METERINGMODE				0x9207
#define JPEG_EXIF_LIGHTSOURCE					0x9208
#define JPEG_EXIF_FLASH							0x9209
#define JPEG_EXIF_FOCALLENGTH					0x920A
//#define JPEG_EXIF_FLASHENERGY					0x920B // redefined in 0xA20B
//#define JPEG_EXIF_SPATIALFREQUENCYRESPONSE	0x920C // redefined in 0xA20C
//#define JPEG_EXIF_NOISE							0x920D // redefined in 0xA20D
//#define JPEG_EXIF_FOCALPLANEXRESOLUTION		0x920E // redefined in 0xA20E
//#define JPEG_EXIF_FOCALPLANEYRESOLUTION		0x920F // redefined in 0xA20F
//#define JPEG_EXIF_FOCALPLANERESOLUTIONUNIT		0x9210 // redefined in 0xA210
//#define JPEG_EXIF_IMAGENUMBER					0x9211 // redefined in 0xA211
//#define JPEG_EXIF_SECURITYCLASSIFICATION		0x9212 // redefined in 0xA212
//#define JPEG_EXIF_IMAGEHISTORY					0x9213 // redefined in 0xA213
#define JPEG_EXIF_SUBJECTAREA					0x9214
//#define JPEG_EXIF_EXPOSUREINDEX				0x9215 // redefined in 0xA215
//#define JPEG_EXIF_TIFF_EPSTANDARDID			0x9216 // redefined in 0xA216
//#define JPEG_EXIF_SENSINGMETHOD				0x9217 // redefined in 0xA217
#define JPEG_EXIF_STONITS						0x923F
#define JPEG_EXIF_MAKERNOTEFACTORY			0x927C //markernote
#define JPEG_EXIF_USERCOMMENT					0x9286
#define JPEG_EXIF_SUBSECTIME					0x9290
#define JPEG_EXIF_SUBSECTIMEORIGINAL			0x9291
#define JPEG_EXIF_SUBSECTIMEDIGITIZED			0x9292
#define JPEG_EXIF_MSDOCUMENTTEXT				0x932F
#define JPEG_EXIF_MSPROPERTYSETSTORAGE		0x9330
#define JPEG_EXIF_MSDOCUMENTTEXTPOSITION	0x9331
#define JPEG_EXIF_IMAGESOURCEDATA			0x935C
#define JPEG_EXIF_XPTITLE						0x9C9B
#define JPEG_EXIF_XPCOMMENT					0x9C9C
#define JPEG_EXIF_XPAUTHOR						0x9C9D
#define JPEG_EXIF_XPKEYWORDS					0x9C9E
#define JPEG_EXIF_XPSUBJECT					0x9C9F
#define JPEG_EXIF_FLASHPIXVERSION				0xA000
#define JPEG_EXIF_COLORSPACE					0xA001
#define JPEG_EXIF_EXIFIMAGEWIDTH				0xA002
#define JPEG_EXIF_EXIFIMAGEHEIGHT				0xA003
#define JPEG_EXIF_RELATEDSOUNDFILE			0xA004
#define JPEG_EXIF_INTEROPOFFSET				0xA005
#define JPEG_EXIF_FLASHENERGY					0xA20B
#define JPEG_EXIF_SPATIALFREQUENCYRESPONSE	0xA20C
#define JPEG_EXIF_NOISE							0xA20D
#define JPEG_EXIF_FOCALPLANEXRESOLUTION		0xA20E
#define JPEG_EXIF_FOCALPLANEYRESOLUTION		0xA20F
#define JPEG_EXIF_FOCALPLANERESOLUTIONUNIT	0xA210
#define JPEG_EXIF_IMAGENUMBER					0xA211
#define JPEG_EXIF_SECURITYCLASSIFICATION		0xA212
#define JPEG_EXIF_IMAGEHISTORY					0xA213
#define JPEG_EXIF_SUBJECTLOCATION				0xA214
#define JPEG_EXIF_EXPOSUREINDEX				0xA215
#define JPEG_EXIF_TIFF_EPSTANDARDID			0xA216
#define JPEG_EXIF_SENSINGMETHOD				0xA217
#define JPEG_EXIF_FILESOURCE					0xA300
#define JPEG_EXIF_SCENETYPE					0xA301
#define JPEG_EXIF_CFAPATTERN					0xA302
#define JPEG_EXIF_CUSTOMRENDERED				0xA401
#define JPEG_EXIF_EXPOSUREMODE				0xA402
#define JPEG_EXIF_WHITEBALANCE				0xA403
#define JPEG_EXIF_DIGITALZOOMRATIO			0xA404
#define JPEG_EXIF_FOCALLENGTHIN35MMFORMAT	0xA405
#define JPEG_EXIF_SCENECAPTURETYPE			0xA406
#define JPEG_EXIF_GAINCONTROL					0xA407
#define JPEG_EXIF_CONTRAST						0xA408
#define JPEG_EXIF_SATURATION					0xA409
#define JPEG_EXIF_SHARPNESS					0xA40A
#define JPEG_EXIF_DEVICESETTINGDESCRIPTION	0xA40B
#define JPEG_EXIF_SUBJECTDISTANCERANGE		0xA40C
#define JPEG_EXIF_IMAGEUNIQUEID				0xA420
#define JPEG_EXIF_GDALMETADATA				0xA480
#define JPEG_EXIF_GDALNODATA					0xA481
#define JPEG_EXIF_GAMMA						0xA500
#define JPEG_EXIF_EXPANDSOFTWARE				0xAFC0
#define JPEG_EXIF_EXPANDLENS					0xAFC1
#define JPEG_EXIF_EXPANDFILM					0xAFC2
#define JPEG_EXIF_EXPANDFILTERLENS			0xAFC3
#define JPEG_EXIF_EXPANDSCANNER				0xAFC4
#define JPEG_EXIF_EXPANDFLASHLAMP				0xAFC5
#define JPEG_EXIF_PIXELFORMAT					0xBC01
#define JPEG_EXIF_TRANSFORMATION				0xBC02
#define JPEG_EXIF_UNCOMPRESSED				0xBC03
#define JPEG_EXIF_IMAGETYPE					0xBC04
//#define JPEG_EXIF_IMAGEWIDTH					0xBC80 // redefined in 0x0100
//#define JPEG_EXIF_IMAGEHEIGHT					0xBC81 // redefined in 0x0101
#define JPEG_EXIF_WIDTHRESOLUTION				0xBC82
#define JPEG_EXIF_HEIGHTRESOLUTION			0xBC83
#define JPEG_EXIF_IMAGEOFFSET					0xBCC0
#define JPEG_EXIF_IMAGEBYTECOUNT				0xBCC1
#define JPEG_EXIF_ALPHAOFFSET					0xBCC2
#define JPEG_EXIF_ALPHABYTECOUNT				0xBCC3
#define JPEG_EXIF_IMAGEDATADISCARD			0xBCC4
#define JPEG_EXIF_ALPHADATADISCARD			0xBCC5
#define JPEG_EXIF_OCESCANJOBDESC				0xC427
#define JPEG_EXIF_OCEAPPLICATIONSELECTOR		0xC428
#define JPEG_EXIF_OCEIDNUMBER					0xC429
#define JPEG_EXIF_OCEIMAGELOGIC				0xC42A
#define JPEG_EXIF_ANNOTATIONS					0xC44F
#define JPEG_EXIF_PRINTIM						0xC4A5
#define JPEG_EXIF_DNGVERSION					0xC612
#define JPEG_EXIF_DNGBACKWARDVERSION			0xC613
#define JPEG_EXIF_UNIQUECAMERAMODEL			0xC614
#define JPEG_EXIF_LOCALIZEDCAMERAMODEL		0xC615
#define JPEG_EXIF_CFAPLANECOLOR				0xC616
#define JPEG_EXIF_CFALAYOUT					0xC617
#define JPEG_EXIF_LINEARIZATIONTABLE			0xC618
#define JPEG_EXIF_BLACKLEVELREPEATDIM			0xC619
#define JPEG_EXIF_BLACKLEVEL					0xC61A
#define JPEG_EXIF_BLACKLEVELDELTAH			0xC61B
#define JPEG_EXIF_BLACKLEVELDELTAV			0xC61C
#define JPEG_EXIF_WHITELEVEL					0xC61D
#define JPEG_EXIF_DEFAULTSCALE					0xC61E
#define JPEG_EXIF_DEFAULTCROPORIGIN			0xC61F
#define JPEG_EXIF_DEFAULTCROPSIZE				0xC620
#define JPEG_EXIF_COLORMATRIX1					0xC621
#define JPEG_EXIF_COLORMATRIX2					0xC622
#define JPEG_EXIF_CAMERACALIBRATION1			0xC623
#define JPEG_EXIF_CAMERACALIBRATION2			0xC624
#define JPEG_EXIF_REDUCTIONMATRIX1			0xC625
#define JPEG_EXIF_REDUCTIONMATRIX2			0xC626
#define JPEG_EXIF_ANALOGBALANCE				0xC627
#define JPEG_EXIF_ASSHOTNEUTRAL				0xC628
#define JPEG_EXIF_ASSHOTWHITEXY				0xC629
#define JPEG_EXIF_BASELINEEXPOSURE			0xC62A
#define JPEG_EXIF_BASELINENOISE				0xC62B
#define JPEG_EXIF_BASELINESHARPNESS			0xC62C
#define JPEG_EXIF_BAYERGREENSPLIT				0xC62D
#define JPEG_EXIF_LINEARRESPONSELIMIT			0xC62E
#define JPEG_EXIF_CAMERASERIALNUMBER			0xC62F
#define JPEG_EXIF_DNGLENSINFO					0xC630
#define JPEG_EXIF_CHROMABLURRADIUS			0xC631
#define JPEG_EXIF_ANTIALIASSTRENGTH			0xC632
#define JPEG_EXIF_SHADOWSCALE					0xC633
#define JPEG_EXIF_SR2PRIVATE					0xC634
#define JPEG_EXIF_MAKERNOTESAFETY				0xC635
#define JPEG_EXIF_RAWIMAGESEGMENTATION		0xC640
#define JPEG_EXIF_CALIBRATIONILLUMINANT1		0xC65A
#define JPEG_EXIF_CALIBRATIONILLUMINANT2		0xC65B
#define JPEG_EXIF_BESTQUALITYSCALE			0xC65C
#define JPEG_EXIF_RAWDATAUNIQUEID			0xC65D
#define JPEG_EXIF_ALIASLAYERMETADATA			0xC660
#define JPEG_EXIF_ORIGINALRAWFILENAME		0xC68B
#define JPEG_EXIF_ORIGINALRAWFILEDATA		0xC68C
#define JPEG_EXIF_ACTIVEAREA					0xC68D
#define JPEG_EXIF_MASKEDAREAS					0xC68E
#define JPEG_EXIF_ASSHOTICCPROFILE			0xC68F
#define JPEG_EXIF_ASSHOTPREPROFILEMATRIX		0xC690
#define JPEG_EXIF_CURRENTICCPROFILE			0xC691
#define JPEG_EXIF_CURRENTPREPROFILEMATRIX	0xC692
#define JPEG_EXIF_COLORIMETRICREFERENCE		0xC6BF
#define JPEG_EXIF_PANASONICTITLE				0xC6D2
#define JPEG_EXIF_PANASONICTITLE2				0xC6D3
#define JPEG_EXIF_CAMERACALIBRATIONSIG		0xC6F3
#define JPEG_EXIF_PROFILECALIBRATIONSIG		0xC6F4
#define JPEG_EXIF_PROFILEIFD					0xC6F5
#define JPEG_EXIF_ASSHOTPROFILENAME			0xC6F6
#define JPEG_EXIF_NOISEREDUCTIONAPPLIED		0xC6F7
#define JPEG_EXIF_PROFILENAME					0xC6F8
#define JPEG_EXIF_PROFILEHUESATMAPDIMS		0xC6F9
#define JPEG_EXIF_PROFILEHUESATMAPDATA1		0xC6FA
#define JPEG_EXIF_PROFILEHUESATMAPDATA2		0xC6FB
#define JPEG_EXIF_PROFILETONECURVE			0xC6FC
#define JPEG_EXIF_PROFILEEMBEDPOLICY			0xC6FD
#define JPEG_EXIF_PROFILECOPYRIGHT			0xC6FE
#define JPEG_EXIF_FORWARDMATRIX1				0xC714
#define JPEG_EXIF_FORWARDMATRIX2				0xC715
#define JPEG_EXIF_PREVIEWAPPLICATIONNAME		0xC716
#define JPEG_EXIF_PREVIEWAPPLICATIONVERSION	0xC717
#define JPEG_EXIF_PREVIEWSETTINGSNAME		0xC718
#define JPEG_EXIF_PREVIEWSETTINGSDIGEST		0xC719
#define JPEG_EXIF_PREVIEWCOLORSPACE			0xC71A
#define JPEG_EXIF_PREVIEWDATETIME				0xC71B
#define JPEG_EXIF_RAWIMAGEDIGEST				0xC71C
#define JPEG_EXIF_ORIGINALRAWFILEDIGEST		0xC71D
#define JPEG_EXIF_SUBTILEBLOCKSIZE				0xC71E
#define JPEG_EXIF_ROWINTERLEAVEFACTOR		0xC71F
#define JPEG_EXIF_PROFILELOOKTABLEDIMS		0xC725
#define JPEG_EXIF_PROFILELOOKTABLEDATA		0xC726
#define JPEG_EXIF_OPCODELIST1					0xC740
#define JPEG_EXIF_OPCODELIST2					0xC741
#define JPEG_EXIF_OPCODELIST3					0xC74E
#define JPEG_EXIF_NOISEPROFILE					0xC761
#define JPEG_EXIF_PADDING						0xEA1C
#define JPEG_EXIF_OFFSETSCHEMA				0xEA1D
#define JPEG_EXIF_OWNERNAME					0xFDE8
#define JPEG_EXIF_SERIALNUMBER					0xFDE9
#define JPEG_EXIF_LENS							0xFDEA
/* //Kodak KDC_IFD Tags
#define JPEG_EXIF_KDC_IFD						0xFE00
#define JPEG_EXIF_RAWFILE							0xFE4C
#define JPEG_EXIF_CONVERTER						0xFE4D
#define JPEG_EXIF_WHITEBALANCE					0xFE4E
#define JPEG_EXIF_EXPOSURE						0xFE51
#define JPEG_EXIF_SHADOWS						0xFE52
#define JPEG_EXIF_BRIGHTNESS						0xFE53
#define JPEG_EXIF_CONTRAST						0xFE54
#define JPEG_EXIF_SATURATION						0xFE55
#define JPEG_EXIF_SHARPNESS						0xFE56
#define JPEG_EXIF_SMOOTHNESS					0xFE57
#define JPEG_EXIF_MOIREFILTER						0xFE58
*/

#define CHUNK_POOL_INDEX	1 // 0 = DRAM B, 1 = DRAM A

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

#define ERR(fmt, arg...)		printk(KERN_ERR "[%s:%d] Error! "fmt, __FUNCTION__, __LINE__, ##arg)
#define WARNING(fmt, arg...)	printk(KERN_WARNING "[%s:%d] Warning! "fmt, __FUNCTION__, __LINE__, ##arg)
#define NOTICE(fmt, arg...)	printk(KERN_NOTICE "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)
#define INFO(fmt, arg...)		printk(KERN_INFO "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)
#define DEBUG(fmt, arg...)		printk(KERN_DEBUG "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

#if 0
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned long			DWORD;

typedef unsigned long long 		UINT64;
typedef unsigned int   			UINT32;
typedef unsigned short 		UINT16;
typedef unsigned char 			UINT8;

typedef long long 				INT64;
typedef int   					INT32;
typedef short 				INT16;
typedef signed char			INT8;
#endif

typedef enum _DHT_TYPE_
{
	DHT_DC0 = 0,
	DHT_DC1 = 1,
	DHT_AC0 = 2,
	DHT_AC1 = 3,
}DHT_TYPE;

// tuning JR coefficient for quality
typedef struct _JR_FILTER_COEF_
{
	unsigned short coef_set_1[JR_COE_SET_1_SIZE];
	unsigned short coef_set_2[JR_COE_SET_2_SIZE];
	unsigned short coef_set_3[JR_COE_SET_3_SIZE];
	unsigned short coef_set_4[JR_COE_SET_4_SIZE];
}JR_FILTER_COEF;

typedef struct _JR_REG_INFO_
{
	unsigned int luma_start;
	unsigned int chroma_start;
	unsigned int temp_buffer;
	unsigned int render_input_offset;
	unsigned char   src_color_mode;
	unsigned char   dis_color_mode;
	unsigned short  alpha_part;
	unsigned char rgb_swap;
	unsigned int orientation;
	unsigned int src_width;
	unsigned int src_height;
	unsigned int v_filter_phase;
	unsigned int h_filter_phase;
	unsigned int scaler_index_h;
	unsigned int scaler_index_v;
	unsigned char auto_fetch_enable;
	unsigned int frame_width;
	unsigned int frame_height;
	unsigned char temp_buf_onLuma;
	unsigned char pd_enable;
	unsigned char rgb_output_onChroma;
	unsigned char odd_symm_filter;
	unsigned char hd_color_space;
	unsigned char rgb_input_onChroma;
	unsigned char bypass_enable;
	unsigned char bw_file;
	unsigned char yuv2RGB_flag;
	unsigned char alpha_scaling;
	unsigned int pre_scaler;
	unsigned int disp_box_h_bound;
	unsigned int disp_box_v_bound;
	JR_FILTER_COEF *h_coef;
	JR_FILTER_COEF *v_coef;
}JR_REG_INFO;

typedef struct _JPEG_APP0_
{
	unsigned short length;
	unsigned char identifier[5];
	unsigned short version;
	unsigned char units;
	unsigned short Xdenity;
	unsigned short Ydenity;
	unsigned char Xthumbnail;
	unsigned char Ythumbnail;
	unsigned char *RGBn;
} JPEG_APP0;

typedef struct _JPEG_APP1_
{
	unsigned short length;
	unsigned char jpeg_exif_format;
	unsigned char little_endian;
	unsigned int ifd0_offset;
	unsigned short ifd0_entry_no;
	unsigned char reserved[2];
} JPEG_APP1;

typedef struct _JPEG_DQT_
{
	unsigned short length;
	unsigned char count;
	unsigned char Pq[2];
	unsigned char Tq[2];
	unsigned short Qs[2][64];
	unsigned char reserved[1];
} JPEG_DQT;

typedef struct _JPEG_SOF_
{
	unsigned short sof_n;
	unsigned short length;
	unsigned char precision;
	unsigned short height;
	unsigned short width;
	unsigned char components;
	unsigned char identifier[4];
	unsigned char Hfactor[4];
	unsigned char Vfactor[4];
	unsigned char Qtable[4];
	unsigned int image_type;
	unsigned char reserved[2];
} JPEG_SOF;

typedef struct _JPEG_DHT_
{
	unsigned short length;
	unsigned char DC_mincode[4][24];
	unsigned char AC_mincode[4][24];
	unsigned char DC_offset[4][8];
	unsigned short AC_offset[4][16];
	unsigned char DC_value[4][16];
	unsigned char AC_value[4][256];
	unsigned char DC_value_cnt[4];
	unsigned short AC_value_cnt[4];
	unsigned int huff_ac_size;
	unsigned char reserved[2];
} JPEG_DHT;

typedef struct _JPEG_DRI_
{
	unsigned short length;
	unsigned short dri_count;
} JPEG_DRI;

typedef struct _JPEG_SOS_
{
	unsigned short length;
	unsigned char components;
	unsigned char identifier[4];
	unsigned char DCtable[4];
	unsigned char ACtable[4];
	unsigned char spectral_selection_start;
	unsigned char spectral_selection_end;
	unsigned char successive_approximation_high;
	unsigned char successive_approximation_low;
	unsigned char reserved[1];
} JPEG_SOS;

typedef struct _JPEG_DECODE_
{
	char *filename;
	unsigned int file_size;
	unsigned int bitstream_start;
	unsigned int bitstream_end;
	unsigned char output_run_time;

	// for progressive JPEG decoder:
	unsigned int y_in;
	unsigned int cbcr_in;
	unsigned int horizontal_mcu;
	unsigned int vertical_mcu;
	unsigned int mcu_horizontal_mb;
	unsigned int mcu_horizontal_b;
	//unsigned int ms_pre_huf_status; // for Multi-Scan ??????
	//unsigned int ms_pre_huf_scan_ctrl; // for Multi-Scan ??????
	//unsigned int ms_pre_huf_table; // for Multi-Scan ??????
	//unsigned int ms_info[6]; // for Multi-Scan ??????
	//signed char pre_subcnt; // for Multi-Scan ??????
	unsigned char ms_change_table;
	unsigned char sof_Hi_0;
	unsigned char sof_Vi_0;
	unsigned char dma_first_flg;
	unsigned int mcu_start;
	unsigned int mcu_end;
	unsigned int mcu_cur_x;
	unsigned int ms_work_y;
	unsigned int cur_sos_run;
	unsigned char reserved[3];
} JPEG_DECODE;

typedef struct _JPEG_INFO_
{
	JPEG_APP0 jpegAPP0;
	JPEG_APP1 jpegAPP1;
	JPEG_DQT jpegDQT;
	JPEG_SOF jpegSOF;
	JPEG_DHT jpegDHT;
	JPEG_DRI jpegDRI;
	JPEG_SOS jpegSOS;
	JPEG_DECODE jpegDecode;
} JPEG_INFO;

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

void imgdecsnd_clock_enable(void);
void imgdecsnd_clock_disable(void);
int imgdecsnd_hal_reg_mmap(void);
void imgdecsnd_hal_reg_unmmap(void);
void imgdecsnd_hal_print_all_registers(void);
//---------------------------------------------------------------
void imgdecsnd_hal_jr_setup(
	JPEG_INFO *jpeg_info_data, 
	DECODE_INFO *decode_info_data, 
	UINT32 working_buf_phyaddr);
void imgdecsnd_hal_scale_setup(
	SCALE_INFO *scale_info_data, 
	UINT32 working_buf_phyaddr);
int imgdecsnd_hal_scale_start(
	SCALE_INFO *scale_info_data);
//---------------------------------------------------------------
void imgdecsnd_hal_jpeg_hw_init(
	JPEG_INFO *jpeg_info_data);
void imgdecsnd_hal_jpeg_fill_bs_tables(
	JPEG_INFO *jpeg_info_data);
void imgdecsnd_hal_jpeg_init_MJmode_waitqueue_head(void); // MJmode uses interrupt to replace polling
irqreturn_t imgdecsnd_hal_jpeg_decode_bs_MJmode_irq_handler( // MJmode uses interrupt to replace polling
	int irq,
	void* dev_id);
int imgdecsnd_hal_jpeg_decode_bs_MJmode(
	JPEG_INFO *jpeg_info_data, 
	UINT32 bs_buf_phyaddr, 
	UINT32 bs_size);
void imgdecsnd_hal_jpeg_fill_ms_tables(
	JPEG_INFO *jpeg_info_data);
void imgdecsnd_hal_jpeg_decode_ms_init(
	JPEG_INFO *jpeg_info_data, 
	UINT32 *bs_buf_phyaddr, 
	UINT32 working_buf_phyaddr);
int imgdecsnd_hal_jpeg_decode_ms_interleave(
	JPEG_INFO *jpeg_info_data, 
	UINT32 *bs_buf_phyaddr);
int imgdecsnd_hal_jpeg_decode_ms_noninterleave(
	JPEG_INFO *jpeg_info_data, 
	UINT32 *bs_buf_phyaddr);
void imgdecsnd_hal_jpeg_decode_ms_end(
	JPEG_INFO *jpeg_info_data, 
	UINT32 *bs_buf_phyaddr);
int imgdecsnd_hal_jpeg_decode_ms_scale(
	JPEG_INFO *jpeg_info_data);

#endif // __IMGDECSND_HAL_H__

