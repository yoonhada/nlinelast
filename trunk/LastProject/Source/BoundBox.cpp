/**
@file	BoundBox.cpp
@date	2011/09/26
@author	yoonhada@gmail.com	
@brief	충돌 클래스 파일
*/

#include "Stdafx.h"
#include "BoundBox.h"

#include "Charactor.h"
/**
@class	CBoundBox
@date	2011/09/26
@author	yoonhada@gmail.com	
@brief	충돌 클래스
*/

CBoundBox::CBoundBox(CCharactor * pObj) 
: m_pCharactors(pObj)
{
	Clear(); 
}

CBoundBox::~CBoundBox(void) 
{
	Release();
}

D3DXVECTOR3 CBoundBox::GetPosition()
{
	if (m_pCharactors)		return m_pCharactors->Get_CharaPos(); 
	else					return m_vPosition;
}

FLOAT CBoundBox::GetWidth()
{
	if (m_pCharactors)		return 15.0f;
	else					return ( ABSDEF(m_fSize[PLUSX]) - ABSDEF(m_fSize[MINUSX]) );
}

FLOAT CBoundBox::GetHeight()
{
	if (m_pCharactors)		return 15.0f;
	else					return ( ABSDEF(m_fSize[PLUSZ] - ABSDEF(m_fSize[MINUSZ]) ) );
}

RECT CBoundBox::GetRect()
{
	RECT rect; 
	D3DXVECTOR3 vec = GetPosition();
	
	rect.left   = static_cast<LONG>( 0.5f + vec.x + GetSize( CBoundBox::MINUSX ) );
	rect.right  = static_cast<LONG>( 0.5f + vec.x + GetSize( CBoundBox::PLUSX  ) );
	rect.top    = static_cast<LONG>( 0.5f + vec.z + GetSize( CBoundBox::PLUSZ  ) );
	rect.bottom = static_cast<LONG>( 0.5f + vec.z + GetSize( CBoundBox::MINUSZ ) );
	
	return rect;
}

FLOAT CBoundBox::GetSize(INT n)			
{
	if (m_pCharactors)		return ( n < 3 ) ? -7.5f : 7.5f;
	else					return ( m_fSize[n] );
}
