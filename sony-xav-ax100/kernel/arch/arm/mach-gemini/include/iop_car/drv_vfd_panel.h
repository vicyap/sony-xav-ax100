/******************************************************************************/
/**
*
* \file		drv_vfd_panel.h
*
* \brief		external header for VFD Interface Function
*
* \note		Copyright (c) 2009 Sunplus-prof Technologies - All right reserved
*
* \author		wangyong
*
******************************************************************************/
#ifndef	_DRV_VFD_PANEL_H
#define	_DRV_VFD_PANEL_H

/*****************************************************************************/
//#include <types.h> //Kaney mark
#include <mach/common.h> //Kaney add
#include "drv_common.h" //Kaney add
/*****************************************************************************/
/**
 * \brief		 VFD display event type.
*/
typedef enum _VfdEventType
{
	VFD_EVENT_GLOBAL_TIMER = 0,	///< global timer
	VFD_EVENT_COLON_ON,///< colon on
	VFD_EVENT_COLON_OFF,///< colon off
	VFD_EVENT_CUR_TIME,	///< current time
	VFD_EVENT_CURPROG_NO,///< current program number	
	VFD_EVENT_CURMULTIMEDIA_NO,///< current multimedia number
	VFD_EVENT_IPANEL_SHOWTEXT,
	VFD_EVENT_IPANEL_SETINDICATOR,
	VFD_EVENT_NETOFF,
	VFD_EVENT_CURTV_NO,		//current TV Program number
	VFD_EVENT_CURRADIO_NO,	//current Radio Program number
	VFD_EVENT_NO_PROG,		//display no program info
	VFD_EVENT_DATA,			//dispay data broadcast program
	VFD_EVENT_SINGAL_LOCK,	//display nim singal lock
	VFD_EVENT_SINGAL_UNLOCK,//display nim singal unlock
	VFD_EVENT_MAX,	///< vfd envent max
}VfdEventType_t;
#define MAX_VFD_KEY_EVENT 3 /**< max vfd key event*/
/*****************************************************************************/
/******************************************************************************************/
/**
 * \fn		int DRV_VfdUpdateData(VfdEventType_t eCurEvevtType)
 *
 * \brief		Send VFD panel update display event
 *
 * \param		eCurEvevtType:  Update display event type
 *
 * \return		\e DRV_SUCCESS Send event successfully.\n
 *			\e DRV_WARN_BUFFER_FULL Event Queue full.\n
 *      \e DRV_WARN_UNINITED VFD panel driver uninit.\n
 *
 ******************************************************************************************/
extern int DRV_VfdUpdateData(VfdEventType_t eCurEvevtType);


/******************************************************************************************/
/**
 * \fn		int DRV_VfdPanelInit(void)
 *
 * \brief		Initialize VFD panel driver
 *
 * \return		\e DRV_SUCCESS Initialize VFD panel driver successfully.\n
 *			\e DRV_WARN_INITED VFD panel driver already Inited.\n
 *      \e DRV_ERR_MEMORY_ALLOC Alloc memory & resource failed.\n
 *
 ******************************************************************************************/
extern int DRV_VfdPanelInit(void);


/******************************************************************************************/
/**
 * \fn		int DRV_VfdPanelFinalize(void)
 *
 * \brief		Finalize VFD panel driver
 *
 * \return		\e DRV_SUCCESS Finalize VFD panel driver successfully.\n
 *			\e DRV_WARN_UNINITED VFD panel driver not Inited.\n
 *      \e DRV_WARN_UNINITED VFD panel driver uninit.\n
 *			\e DRV_ERR_TIMEOUT Kill VFD driver thread timeout.\n
 *
 ******************************************************************************************/
extern int DRV_VfdPanelFinalize(void);	


/******************************************************************************************/
/**
 * \fn		void DRV_VfdDsr(void)
 *
 * \brief		Handle VFD interrupt.
 *
 ******************************************************************************************/
extern void DRV_VfdDsr(void);

/******************************************************************************************/
/**
 * \fn		int DRV_VfdGetKeyEvent(UINT32 *adKeyEvent,UINT32 *pKeyCnt)
 *
 * \brief		Get vfd key respond event.
 *
 * \param	adKeyEvent:  key event
 * \param	pKeyCnt:  key count
 *
 * \return		\e DRV_SUCCESS Key event received.\n
 *			\e DRV_WARN_NO_ACTION No key event received.\n
 *      \e DRV_WARN_UNINITED VFD panel driver uninit.\n
 *
 ******************************************************************************************/
extern int DRV_VfdGetKeyEvent(UINT32 *adKeyEvent,UINT32 *pKeyCnt);

/******************************************************************************************/
/**
 * \fn		int DRV_VfdKeyInit(void)
 *
 * \brief		Finalize VFD panel driver
 *
 * \return		\e DRV_SUCCESS Initialize VFD key driver successfully.\n
 *      \e DRV_WARN_UNINITED VFD panel driver uninit.\n
 *
 ******************************************************************************************/
extern int DRV_VfdKeyInit(void);

extern void DRV_VFDStandby_SetupIop(void);//2010/8/3 lizhan added

extern UINT8 DRV_Game_GetVFDKey(UINT8 i) ;//jgliu 2011-07-12 add for mantis 142140, Fix NES game compile error when support VFD

extern int VFD_Finalize(void); //Kaney add
extern vfunc vfd_Standby_Get_SetupIopFunct(void); //Kaney add

/*****************************************************************************/
#endif /* _DRV_VFD_PANEL_H */
