/**
	@file	Cube.h
	@date	2011/09/17
	@author	백경훈
	@brief	큐브 클래스
*/

#ifdef _CELLCUBE

#ifndef _CUBE_H_
#define _CUBE_H_

#include "Cell.h"

/**
	@class	CCube
	@date	2011/09/17
	@author	백경훈
	@brief	큐브 클래스
*/

class CCube : public CObjectSRT
{
public:

	struct CUBEVERTEX
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR2 t;

		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
		enum { VertexNum = 24 };
	};

	struct CUBEINDEX
	{
		WORD _0, _1, _2;
		enum { IndexNum = 2 };
	};

	CCube();
	virtual ~CCube();

	VOID    Clear();
	HRESULT Create() { return S_OK; }
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, 
		LPDIRECT3DVERTEXBUFFER9 &a_pVB, LPDIRECT3DINDEXBUFFER9 &a_pIB, 
		INT a_iStartVB, INT a_iStartIB, FLOAT a_fCubeSize=0.5f );

	HRESULT InitTexture( DWORD a_Color, DWORD a_OutLineColor=0xff0000ff );
	
	VOID Update();
	VOID Render();
	HRESULT Release();

	VOID Set_FaceVisible( INT a_iFace, const INT a_iVisible )
	{
		m_pCell[a_iFace].Set_FaceVisible(a_iVisible);
	}

	const INT Get_FaceVisble( INT a_iFace )
	{
		return m_pCell[a_iFace].Get_FaceVisble();
	}

private:
	CCell* m_pCell;
	HRESULT _InitVB( LPDIRECT3DVERTEXBUFFER9 &a_pVB );
	HRESULT _InitIB( LPDIRECT3DINDEXBUFFER9 &a_pIB );

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	LPDIRECT3DTEXTURE9		m_pTexture;	  ///< 텍스쳐

	INT m_iStartVB;
	INT m_iStartIB;
	FLOAT m_fCubeSize;

	

};

#endif

#endif