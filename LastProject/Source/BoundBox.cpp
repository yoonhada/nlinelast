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

	if (pObj)
		SetAngle( pObj->Get_CharaAngle() );
}

CBoundBox::~CBoundBox(void) 
{
	Release();
}

VOID CBoundBox::Clear()
{
	m_vPosition = D3DXVECTOR3(0, 0, 0);
	m_vAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vAxisDir[2] = D3DXVECTOR3(0, 0, 1);
	m_fSize[0] = m_fSize[1] = m_fSize[2] = m_fSize[3] = m_fSize[4] = m_fSize[5] = 0;
}

D3DXVECTOR3 CBoundBox::GetPosition(INT nPoint) const
{
	D3DXVECTOR3 vRet;

	if ( nPoint == -1 )
	{
		if ( m_pCharactors )		
		{
			vRet = m_pCharactors->Get_CharaPos(); 
			vRet.y += ABSDEF( GetSize(CBoundBox::MINUSY) );
		}
		else					
		{
			vRet =  m_vPosition;
		}
	}
	else
	{

		//    v0----- v1
		//   /|      /|
		//  v3------v2|
		//  | |     | |
		//  | |v4---|-|v5
		//  |/      |/
		//  v7------v6

		switch (nPoint)
		{
		case 0:
			vRet = D3DXVECTOR3(GetSize(CBoundBox::MINUSX), GetSize(CBoundBox::PLUSY), GetSize(CBoundBox::PLUSZ));
			break;
		case 1:
			vRet = D3DXVECTOR3(GetSize(CBoundBox::PLUSX), GetSize(CBoundBox::PLUSY), GetSize(CBoundBox::PLUSZ));
			break;
		case 2:
			vRet = D3DXVECTOR3(GetSize(CBoundBox::PLUSX), GetSize(CBoundBox::PLUSY), GetSize(CBoundBox::MINUSZ));
			break;
		case 3:
			vRet = D3DXVECTOR3(GetSize(CBoundBox::MINUSX), GetSize(CBoundBox::PLUSY), GetSize(CBoundBox::MINUSZ));
			break;
		case 4:
			vRet = D3DXVECTOR3(GetSize(CBoundBox::MINUSX), GetSize(CBoundBox::MINUSY), GetSize(CBoundBox::PLUSZ));
			break;
		case 5:
			vRet = D3DXVECTOR3(GetSize(CBoundBox::PLUSX), GetSize(CBoundBox::MINUSY), GetSize(CBoundBox::PLUSZ));
			break;
		case 6:
			vRet = D3DXVECTOR3(GetSize(CBoundBox::PLUSX), GetSize(CBoundBox::MINUSY), GetSize(CBoundBox::MINUSZ));
			break;
		case 7:
			vRet = D3DXVECTOR3(GetSize(CBoundBox::MINUSX), GetSize(CBoundBox::MINUSY), GetSize(CBoundBox::MINUSZ));
			break;
		}
		D3DXVec3TransformCoord( &vRet, &vRet, &GetAxisMat() );
		vRet = GetPosition() + vRet;
	}

	return vRet;
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

FRECT CBoundBox::GetFRect()
{
	D3DXVECTOR3 vec = GetPosition();

	FRECT rect = { 
		vec.x + GetSize( CBoundBox::MINUSX ), 
		vec.z + GetSize( CBoundBox::PLUSZ  ), 
		vec.x + GetSize( CBoundBox::PLUSX  ), 
		vec.z + GetSize( CBoundBox::MINUSZ ) };

		return rect;
}

FLOAT CBoundBox::GetSize(INT n) const
{
	if (m_pCharactors)		return ( n < 3 ) ? -7.5f : 7.5f;
	else					return ( m_fSize[n] );
}

FLOAT CBoundBox::GetRadius() const
{
	return sqrt( GetRadiusLong() * GetRadiusLong() + GetRadiusShort() * GetRadiusShort() );
}

FLOAT CBoundBox::GetRadiusLong() const
{
	FLOAT fRet = 0;

	FLOAT fTemp;
	for (int i = 0; i < 6; ++i)
	{
		fTemp = ABSDEF( GetSize( i ) );
		if ( fTemp > fRet )
			fRet = fTemp;
	}

	return fRet;
}

FLOAT CBoundBox::GetRadiusShort() const
{
	FLOAT fRet = FLT_MAX;

	FLOAT fTemp;
	for (int i = 0; i < 6; ++i)
	{
		fTemp = ABSDEF( GetSize( i ) );
		if ( fTemp < fRet )
			fRet = fTemp;
	}

	return fRet;
}



VOID CBoundBox::SetAngle(FLOAT fAngle)
{
	m_vAxisDir[0].x =  cosf(fAngle);
	m_vAxisDir[0].z = -sinf(fAngle);
	m_vAxisDir[2].x =  sinf(fAngle);
	m_vAxisDir[2].z =  cosf(fAngle);
}						///< y축회전

D3DXMATRIXA16 CBoundBox::GetAxisMat() const
{
	return D3DXMATRIXA16(
		m_vAxisDir[0].x, m_vAxisDir[1].x, m_vAxisDir[2].x, 0, 
		m_vAxisDir[0].y, m_vAxisDir[1].y, m_vAxisDir[2].y, 0, 
		m_vAxisDir[0].z, m_vAxisDir[1].z, m_vAxisDir[2].z, 0, 
		0, 0, 0, 1);
}