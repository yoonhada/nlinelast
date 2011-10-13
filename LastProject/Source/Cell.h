/**
	@file	Cell.h
	@date	2011/09/17
	@author	백경훈
	@brief	큐브 클래스
*/

#ifdef _CELLCUBE

#ifndef _CELL_H_
#define _CELL_H_

/**
	@class	CCell
	@date	2011/09/17
	@author	백경훈
	@brief	큐브 클래스
*/

namespace EnumCubeCellFace
{
	enum CubeCellFace { LEFT, RIGHT, TOP, BOTTOM, BACK, FRONT };
}

class CCell
{
public:
	struct CUBEVERTEX
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR2 t;

		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
		enum { VertexNum = 4 };
	};

	struct CUBEINDEX
	{
		WORD _0, _1, _2;
	};

	CCell();
	virtual ~CCell();

	VOID    Clear();
	//HRESULT Create() { return S_OK; }
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, 
		LPDIRECT3DVERTEXBUFFER9 &a_pVB, LPDIRECT3DINDEXBUFFER9 &a_pIB, 
		INT a_iStartVB, INT a_iStartIB, FLOAT a_fCubeSize=0.5f, INT a_iFaceNum=0 );

	//HRESULT InitTexture( LPDIRECT3DTEXTURE9 a_pTexture );
	
	//VOID Update();
	VOID Render();
	HRESULT Release();

	VOID Set_FaceVisible( const INT a_iVisible )
	{
		m_iVisible = a_iVisible;
	}

	const INT Get_FaceVisble()
	{
		return m_iVisible;
	}

private:
	HRESULT _InitVB( LPDIRECT3DVERTEXBUFFER9 &a_pVB );
	HRESULT _InitIB( LPDIRECT3DINDEXBUFFER9 &a_pIB );

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	//LPDIRECT3DTEXTURE9		m_pTexture;	  ///< 텍스쳐

	INT m_iStartVB;
	INT m_iStartIB;
	FLOAT m_fCubeSize;
	INT m_iFaceNum;
	INT m_iVisible;

	

};

#endif

#endif