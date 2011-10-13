/**
	@file	Cube8Vertex.h
	@date	2011/09/17
	@author	백경훈
	@brief	8버텍스 큐브 클래스
*/

#ifdef _8CUBE

#ifndef _CUBE8Vertex_H_
#define _CUBE8Vertex_H_

/**
	@file	CCube
	@date	2011/09/17
	@author	백경훈
	@brief	8버텍스 큐브 클래스
*/

class CCube : virtual public IObject, public CObjectSRT
{
public:
	struct CUBEVERTEX
	{
		D3DXVECTOR3 p;
		DWORD		color;

		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
		enum { VertexNum = 8 };
	};

	struct CUBEINDEX
	{
		WORD _0, _1, _2;
		enum { IndexNum = 12 };
	};

	CCube();
	virtual ~CCube();

	VOID Clear();
	HRESULT Create() { return S_OK; }
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice,
		LPDIRECT3DVERTEXBUFFER9 &a_pVB, LPDIRECT3DINDEXBUFFER9 &a_pIB, INT a_iStartVB, 
		INT a_iStartIB, FLOAT a_fCubeSize=0.5f );
	HRESULT InitTexture( DWORD a_Color, DWORD a_OutLineColor=0xff0000ff );
	
	VOID Update();
	VOID Render();
	HRESULT Release();



private:
	HRESULT _InitVB( LPDIRECT3DVERTEXBUFFER9 &a_pVB );
	HRESULT _InitIB( LPDIRECT3DINDEXBUFFER9 &a_pIB );

	INT m_iStartVB;
	INT m_iStartIB;

	DWORD m_Color;
	FLOAT m_fCubeSize;

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스	

};

#endif

#endif