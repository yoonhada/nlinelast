/**
	@class	Axis
	@date	2011/08/26
	@author	김종철
	@brief	좌표축
*/

#pragma once

class TestCube
{
private:
	struct VERTEX
	{
		enum { FVF = ( D3DFVF_XYZ | D3DFVF_DIFFUSE ), };
		D3DXVECTOR3 _pos;
		DWORD _col;
	};

	struct INDEX
	{
		WORD _0, _1, _2;
	};

protected:
	LPDIRECT3DDEVICE9		m_pD3dDevice;	///< D3D 디바이스
	LPDIRECT3DVERTEXBUFFER9	m_pVB;			///< 정점 버퍼
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	HRESULT	InitVB();	///< 정점 버퍼 생성
	HRESULT InitIB();


public:
	TestCube( LPDIRECT3DDEVICE9 _pd3dDevice );
	~TestCube();

	HRESULT	Release();

	HRESULT Create();	///< 초기화
	
	VOID Update();		///< Update
	VOID Render();		///< Render
};
