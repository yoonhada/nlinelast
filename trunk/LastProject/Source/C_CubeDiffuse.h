/**
	@file	C_Cube.h
	@date	2011/09/17
	@author	백경훈
	@brief	큐브 클래스
*/

#ifndef _C_CUBE_H_
#define _C_CUBE_H_

/**
	@class	CCube
	@date	2011/09/17
	@author	백경훈
	@brief	큐브 클래스
*/

class CCube : virtual public IObject, public CObjectSRT
{
public:
	struct CUBEVERTEX
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		//D3DXVECTOR2 t;
		//DWORD color;

		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
		enum { VertexNum = 24 };
	};

	struct CUBEINDEX
	{
		WORD _0, _1, _2;
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

private:
	HRESULT _InitVB( LPDIRECT3DVERTEXBUFFER9 &a_pVB );
	HRESULT _InitIB( LPDIRECT3DINDEXBUFFER9 &a_pIB );

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	LPDIRECT3DTEXTURE9		m_pTexture;	  ///< 텍스쳐

	INT m_iStartVB;
	INT m_iStartIB;
	FLOAT m_fCubeSize;
	DWORD m_dwColor;

	D3DMATERIAL9 m_Mtrl;

	

};

#endif