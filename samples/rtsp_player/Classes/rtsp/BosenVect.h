
#if !defined	BOSENVect_H__
#define			BOSENVect_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//	define Find function 
typedef int (*PFNCOMPARATOR) ( void * pFirst, void * pSecond );

// define delete function. The default is "delete". 
typedef void (*PFNRELEASE) ( void * pObj );

struct BosenNode
{
	void *				m_pObj;
	struct BosenNode *		m_pNext;
	struct BosenNode *		m_pPrev;
};

#include "stdlib.h"
class CBOSENVect{
public:
	CBOSENVect( bool bIsClear = false, PFNRELEASE pfnRelease = NULL );
	~CBOSENVect();

	void	Init( bool bIsClear = false, PFNRELEASE pfnRelease = NULL );

	int			GetSize(  );
	bool		Append(  void * pObj );
	bool		AppendFirst(   void * pObj );
	void		RemoveAll(  );
	bool		SetElementAt(  int iIndex, void * pObj );
	void *		GetElementAt(  int iIndex );
	bool		InsertElementAt(  int iIndex, void * pObj );
	void *		RemoveElementAt(  int iIndex );
	int			Find( void * pObj, void ** ppResult, PFNCOMPARATOR pfnComp = NULL );
	bool		SetElementAtEx( int iIndex, void * pObj, void ** ppOldObj );
	int			FindIndex( void * pObj, PFNCOMPARATOR pfnComp );

	void		Reverse( );

private:
	
	BosenNode *		m_pHead;
	BosenNode *		m_pTail;
	int				m_iSize;
	int				m_iRef;
	bool			m_bIsClear;
	PFNRELEASE		m_pfnRelease;

};


class CBosenStack{
public:
	CBosenStack(  bool bIsClear = false, PFNRELEASE pfnRelease = NULL )
	:m_clVect( bIsClear, pfnRelease )
	{
	}

	
	~CBosenStack(){
	}

public:
	void		push(  void * pObj );
	void *		pop(  );

	void *		GetElementAt(  int iIndex );

	void		RemoveElementAt( int iIndex )
	{
		m_clVect.RemoveElementAt( iIndex );
	}

	void		RemoveAll();
	bool empty()
	{
		if(m_clVect.GetSize()==0)
			return true;
		else
			return false;
	}
	void *	front()
	{
		return GetElementAt(0);
	}
	int	GetSize()
	{
		return m_clVect.GetSize();
	}

private:
	
	CBOSENVect   m_clVect;
	
};


#endif 

