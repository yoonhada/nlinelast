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
		SetAngleY( pObj->Get_CharaAngle() );
}

CBoundBox::~CBoundBox(void) 
{
	Release();
}

VOID CBoundBox::Clear()
{
	m_vPosition = D3DXVECTOR3(0, 0, 0);
	m_vDirection = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixIdentity( &m_matAxis );
	m_fSize[0] = m_fSize[1] = m_fSize[2] = m_fSize[3] = m_fSize[4] = m_fSize[5] = 0;

	_data.erase( _data.begin(), _data.end() );
}

D3DXVECTOR3 CBoundBox::GetPosition(INT nPoint) const
{
	D3DXVECTOR3 vRet;

	if ( nPoint == -1 )
	{
		if ( m_pCharactors )		
		{
			vRet = m_pCharactors->Get_CharaPos(); 
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

		vRet = m_vVector[nPoint];
		//switch (nPoint)
		//{
		//case 0:
		//	vRet = D3DXVECTOR3(GetSize(CBoundBox::MINUSX), GetSize(CBoundBox::PLUSY), GetSize(CBoundBox::PLUSZ));
		//	break;
		//case 1:
		//	vRet = D3DXVECTOR3(GetSize(CBoundBox::PLUSX), GetSize(CBoundBox::PLUSY), GetSize(CBoundBox::PLUSZ));
		//	break;
		//case 2:
		//	vRet = D3DXVECTOR3(GetSize(CBoundBox::PLUSX), GetSize(CBoundBox::PLUSY), GetSize(CBoundBox::MINUSZ));
		//	break;
		//case 3:
		//	vRet = D3DXVECTOR3(GetSize(CBoundBox::MINUSX), GetSize(CBoundBox::PLUSY), GetSize(CBoundBox::MINUSZ));
		//	break;
		//case 4:
		//	vRet = D3DXVECTOR3(GetSize(CBoundBox::MINUSX), GetSize(CBoundBox::MINUSY), GetSize(CBoundBox::PLUSZ));
		//	break;
		//case 5:
		//	vRet = D3DXVECTOR3(GetSize(CBoundBox::PLUSX), GetSize(CBoundBox::MINUSY), GetSize(CBoundBox::PLUSZ));
		//	break;
		//case 6:
		//	vRet = D3DXVECTOR3(GetSize(CBoundBox::PLUSX), GetSize(CBoundBox::MINUSY), GetSize(CBoundBox::MINUSZ));
		//	break;
		//case 7:
		//	vRet = D3DXVECTOR3(GetSize(CBoundBox::MINUSX), GetSize(CBoundBox::MINUSY), GetSize(CBoundBox::MINUSZ));
		//	break;
		//}
		if ( m_pCharactors )		
		{
			vRet.y -= GetSize(CBoundBox::MINUSY);
			D3DXVec3TransformCoord( &vRet, &vRet, &m_pCharactors->Get_MatWorld() );		//GetAxisMat() );
		}
		else					
		{
			D3DXVec3TransformCoord( &vRet, &vRet, &GetAxisMat() );
			vRet = GetPosition() + vRet;
		}
	}

	return vRet;
}

FLOAT CBoundBox::GetWidth()
{
	//if (m_pCharactors)		return 15.0f;
	//else					
	return ( ABSDEF(m_fSize[PLUSX]) - ABSDEF(m_fSize[MINUSX]) );
}

FLOAT CBoundBox::GetHeight()
{
	//if (m_pCharactors)		return 15.0f;
	//else					
	return ( ABSDEF(m_fSize[PLUSZ] - ABSDEF(m_fSize[MINUSZ]) ) );
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
	//if (m_pCharactors)		return ( n < 3 ) ? -7.5f : 7.5f;
	//else					
	return ( m_fSize[n] );
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

VOID CBoundBox::MatrixIdentity()
{
	D3DXMatrixIdentity( &m_matAxis );
}

VOID CBoundBox::SetAngleY(FLOAT fAngle)
{
	D3DXMATRIXA16 mat;
	D3DXMatrixRotationY( &mat, fAngle );
	m_matAxis *= mat;
	//m_matAxis = D3DXMATRIXA16( 
	//	cosf( fAngle ), 0, sinf( fAngle ), 0, 
	//	0, 1, 0, 0, 
	//	-sinf( fAngle ), 0, cosf( fAngle ), 0, 
	//	0, 0, 0, 1);
}

VOID CBoundBox::SetAngleZ(FLOAT fAngle)
{
	//D3DXMATRIXA16 mat( 
	//	cosf( fAngle ), sinf( fAngle ), 0, 0, 
	//	-sinf( fAngle ), cosf( fAngle ), 0, 0, 
	//	0, 0, 1, 0,
	//	0, 0, 0, 1);

	//m_matAxis *= mat;
}

D3DXMATRIXA16 CBoundBox::GetAxisMat() const
{
	if ( m_pCharactors )
	{
		return m_pCharactors->Get_MatWorld();
	}
	else
	{
		return m_matAxis;
	}
}


VOID CBoundBox::SetSize(INT n, float f)		
{
		//    v0----- v1
		//   /|      /|
		//  v3------v2|
		//  | |     | |
		//  | |v4---|-|v5
		//  |/      |/
		//  v7------v6
	switch ( n )
	{
	case 0:
		m_vVector[0].x = m_vVector[3].x = m_vVector[4].x = m_vVector[7].x = f;
		break;
	case 1:
		m_vVector[4].y = m_vVector[5].y = m_vVector[6].y = m_vVector[7].y = f;
		break;
	case 2:
		m_vVector[2].z = m_vVector[3].z = m_vVector[6].z = m_vVector[7].z = f;
		break;
	case 3:
		m_vVector[1].x = m_vVector[2].x = m_vVector[5].x = m_vVector[6].x = f;
		break;
	case 4:
		m_vVector[0].y = m_vVector[1].y = m_vVector[2].y = m_vVector[3].y = f;
		break;
	case 5:
		m_vVector[0].z = m_vVector[1].z = m_vVector[4].z = m_vVector[5].z = f;
		break;
	}
}