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
	//    v0----- v1
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v4---|-|v5
	//  |/      |/
	//  v7------v6

	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vVertex[0] = m_vVertex[1] = m_vVertex[2] = m_vVertex[3] = m_vVertex[4] = \
		m_vVertex[5] = m_vVertex[6] = m_vVertex[7] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity( &m_matAxis );
	m_fSize[0] = m_fSize[1] = m_fSize[2] = m_fSize[3] = m_fSize[4] = m_fSize[5] = 0.0f;
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
		vRet = m_vVertex[nPoint];
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

VOID CBoundBox::SetScale(FLOAT fScale)
{
	m_matAxis._11 = fScale;
	m_matAxis._33 = fScale;
}

VOID CBoundBox::SetAngleY(FLOAT fAngle)
{
	// z 축 뒤집혀서 OpenGL 꺼 사용	
	m_matAxis = D3DXMATRIXA16( 
		cosf( fAngle ), 0, -sinf( fAngle ), 0, 
		0, 1, 0, 0, 
		sinf( fAngle ), 0, cosf( fAngle ), 0, 
		0, 0, 0, 1);
}

VOID CBoundBox::SetAngleZ(FLOAT fAngle)
{
	D3DXMATRIXA16 mat( 
		cosf( fAngle ), sinf( fAngle ), 0, 0, 
		-sinf( fAngle ), cosf( fAngle ), 0, 0, 
		0, 0, 1, 0,
		0, 0, 0, 1);

	m_matAxis = mat * m_matAxis ;
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
	m_fSize[n] = f; 

	//    v0----- v1
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v4---|-|v5
	//  |/      |/
	//  v7------v6

	switch ( n )
	{
	case MINUSX:
		m_vVertex[0].x = f;
		m_vVertex[3].x = f;
		m_vVertex[4].x = f;
		m_vVertex[7].x = f;
		break;
	case MINUSY:
		m_vVertex[4].y = f;
		m_vVertex[5].y = f;
		m_vVertex[6].y = f;
		m_vVertex[7].y = f;
		break;
	case MINUSZ:
		m_vVertex[2].z = f;
		m_vVertex[3].z = f;
		m_vVertex[6].z = f;
		m_vVertex[7].z = f;
		break;
	case PLUSX:
		m_vVertex[1].x = f;
		m_vVertex[2].x = f;
		m_vVertex[5].x = f;
		m_vVertex[6].x = f;
		break;
	case PLUSY:
		m_vVertex[0].y = f;
		m_vVertex[1].y = f;
		m_vVertex[2].y = f;
		m_vVertex[3].y = f;
		break;
	case PLUSZ:
		m_vVertex[0].z = f;
		m_vVertex[1].z = f;
		m_vVertex[4].z = f;
		m_vVertex[5].z = f;
		break;
	}
}