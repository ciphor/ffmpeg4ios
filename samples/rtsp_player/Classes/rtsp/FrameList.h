// FrameList.h: interface for the CFrameList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAMELIST_H__D517C6C9_E430_4122_BE39_DC421AEB3028__INCLUDED_)
#define AFX_FRAMELIST_H__D517C6C9_E430_4122_BE39_DC421AEB3028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BosenVect.h"
#define  INITQUEUENUM 10//0

typedef struct 
{
	unsigned char* pContent;
	unsigned long nFrameLength;
	unsigned long nTimeStamp;
	unsigned long nParam1;		// 扩展用
	unsigned long nParam2;		// 扩展用
} MediaData_FRAME;

class CFrameList  
{
public:
	CFrameList();
 	virtual ~CFrameList();

public:
	MediaData_FRAME* GetFreeNote() ;
	MediaData_FRAME* GetDataNote() ;
	void AddToDataList(MediaData_FRAME* t) ;
	void AddToFreeList(MediaData_FRAME* t) ;
	void Reset() ;

private:
	MediaData_FRAME* m_tmpNote ;
	CBosenStack m_datalist ;
	CBosenStack m_freelist ;
};
#endif // !defined(AFX_FRAMELIST_H__D517C6C9_E430_4122_BE39_DC421AEB3028__INCLUDED_)





















