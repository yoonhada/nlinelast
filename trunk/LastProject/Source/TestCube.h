/**
	@class	Axis
	@date	2011/08/26
	@author	����ö
	@brief	��ǥ��
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
	LPDIRECT3DDEVICE9		m_pD3dDevice;	///< D3D ����̽�
	LPDIRECT3DVERTEXBUFFER9	m_pVB;			///< ���� ����
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	HRESULT	InitVB();	///< ���� ���� ����
	HRESULT InitIB();


public:
	TestCube( LPDIRECT3DDEVICE9 _pd3dDevice );
	~TestCube();

	HRESULT	Release();

	HRESULT Create();	///< �ʱ�ȭ
	
	VOID Update();		///< Update
	VOID Render();		///< Render
};
