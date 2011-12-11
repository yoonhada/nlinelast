/**
	@file	CharCube.h
	@date	2011/09/17
	@author	�����
	@brief	ĳ���� ť�� Ŭ����
*/

#ifndef _CHARCUBE_H_
#define _CHARCUBE_H_

#include "Cube24Vertex.h"
#include "Cube8Vertex.h"
#include "Cube.h"

namespace EnumCharFrame
{
	enum FrameEnum { BASE, WALK1, WALK2, ATTACK1, ATTACK2, A, TEMP1, TEMP2, TEMP3, TEMP4, ANIFRAME, MAXFRAME };
}

namespace EnumCubeType
{
	enum CubeType { NONE = -1, BONE, MEAT, CORE, HIDEMEAT };
};

namespace ConstCubeColor
{
	const DWORD BONE = 0xffff0000;
	const DWORD MEAT = 0xff0000ff;
	const DWORD CORE = 0xff00ff00;
}

/**
	@class	CCharCube
	@date	2011/09/17
	@author	�����
	@brief	ĳ���� ť�� Ŭ����
*/

class CCharCube : public CCube
{
public:
	CCharCube() {};
	virtual ~CCharCube() {};

public:
	VOID Set_NumIndex( const INT a_iIndex, const INT a_iNumIndex )
	{
		m_stInfo[a_iIndex].m_iNumIndex = a_iNumIndex;
	}
	VOID Set_Pos( const INT a_iIndex, const D3DXVECTOR3& a_vecPos )
	{
		m_stInfo[a_iIndex].m_vecPos = a_vecPos;
	}
	VOID Set_Visible( const INT a_iIndex, const BOOL a_bVisible )
	{
		m_stInfo[a_iIndex].m_bVisible = a_bVisible;
	}
	VOID Set_Color( const INT a_iIndex, const DWORD& a_dwColor )
	{
		m_stInfo[a_iIndex].m_dwColor = a_dwColor;
	}
	VOID Set_Type( const INT a_iIndex, const INT a_iType )
	{
		m_stInfo[a_iIndex].m_iType = a_iType;
	}
	VOID Set_FriendCubeVecIndex( const INT a_iIndex, const INT a_iFriendNum, const INT a_iVecIndex )
	{
		m_stInfo[a_iIndex].m_iFriendCubeVecIndex[a_iFriendNum] = a_iVecIndex;
	}
	VOID Set_Matrix( const INT a_Index, const D3DXMATRIXA16& a_matMatrix )
	{
		m_stInfo[a_Index].m_matMatrix = a_matMatrix;
	}

	const INT Get_NumIndex( const INT a_iIndex )
	{
		return m_stInfo[a_iIndex].m_iNumIndex;
	}
	const D3DXVECTOR3& Get_Pos( const INT a_iIndex )
	{
		return m_stInfo[a_iIndex].m_vecPos;
	}
	const BOOL Get_Visible( const INT a_iIndex )
	{
		return m_stInfo[a_iIndex].m_bVisible;
	}
	const DWORD& Get_Color( const INT a_iIndex )
	{
		return m_stInfo[a_iIndex].m_dwColor;
	}
	const INT Get_Type( const INT a_iIndex )
	{
		return m_stInfo[a_iIndex].m_iType;
	}
	const INT Get_FriendCubeVecIndex( const INT a_iIndex, const INT a_iFriendNum )
	{
		return m_stInfo[a_iIndex].m_iFriendCubeVecIndex[a_iFriendNum];
	}
	const D3DXMATRIXA16& Get_Matrix( const INT a_iIndex )
	{
		return m_stInfo[a_iIndex].m_matMatrix;
	}

private:
	struct Info{
		INT			m_iNumIndex;				///< �ε���
		D3DXVECTOR3 m_vecPos;					///< 0,0,0 ������ ��ġ
		BOOL		m_bVisible;					///< ������ �Ⱥ�����
		DWORD		m_dwColor;					///< ����
		INT			m_iType;					///< ����
		INT			m_iFriendCubeVecIndex[6];   ///< �̿� �ڽ� ���� �ε��� ( ����, ������, ��, �Ʒ�, ��, �� (0~5) ���� )

		D3DXMATRIXA16 m_matMatrix;				///< ť�� ��Ʈ����

		Info() : m_iNumIndex(0), m_bVisible(FALSE) , m_dwColor(0x00000000), m_iType(-1)
		{
			ZeroMemory( m_vecPos, sizeof(m_vecPos) );
			memset( m_iFriendCubeVecIndex, -1, sizeof(m_iFriendCubeVecIndex) );
			D3DXMatrixIdentity( &m_matMatrix );
		}
	}m_stInfo[EnumCharFrame::MAXFRAME];
	

};

#endif