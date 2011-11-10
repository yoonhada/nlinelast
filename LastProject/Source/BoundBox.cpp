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
			//vRet.y += ABSDEF( GetSize(CBoundBox::MINUSY) );
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
		if ( m_pCharactors )		
		{
			vRet.y -= GetSize(CBoundBox::MINUSY);
			D3DXVec3TransformCoord( &vRet, &vRet, &GetAxisMat() );
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

VOID CBoundBox::SetAngleY(FLOAT fAngle)
{
	// z 축 뒤집혀서 OpenGL 꺼 사용
	D3DXMatrixIdentity( &m_matAxis );
	D3DXMATRIXA16 mat( 
		cosf( fAngle ), 0, sinf( fAngle ), 0, 
		0, 1, 0, 0, 
		-sinf( fAngle ), 0, cosf( fAngle ), 0, 
		0, 0, 0, 1);

	m_matAxis *= mat;
}

VOID CBoundBox::SetAngleZ(FLOAT fAngle)
{
	// z 축 뒤집혀서 OpenGL 꺼 사용
	D3DXMATRIXA16 mat( 
		cosf( fAngle ), -sinf( fAngle ), 0, 0, 
		sinf( fAngle ), cosf( fAngle ), 0, 0, 
		0, 0, 1, 0,
		0, 0, 0, 1);

	m_matAxis *= mat;
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

std::vector <D3DXVECTOR3> * CBoundBox::GetPosVec()
{
	return &_data;
}

VOID CBoundBox::SetPosVec()
{
	D3DXVECTOR3 vPos, vOri = GetPosition();
	for (FLOAT i = GetSize(0); i < GetSize(3); ++i)
	{
		for (FLOAT j = GetSize(1); j < GetSize(4); ++j)
		{
			for (FLOAT k = GetSize(2); k < GetSize(5); ++k)
			{
				vPos.x = static_cast<FLOAT>( static_cast<INT>( vOri.x + i ) );
				vPos.y = static_cast<FLOAT>( static_cast<INT>( vOri.y + j ) );
				vPos.z = static_cast<FLOAT>( static_cast<INT>( vOri.z + k ) );

				_data.push_back( vPos );
			}
		}
	}
}