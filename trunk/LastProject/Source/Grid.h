/**
	@class	Grid.h
	@date	2011/09/17
	@author	�����
	@brief	�׸��� Ŭ����
*/

#ifndef _GRID_H_
#define _GRID_H_

/**
	@class	CGrid
	@date	2011/09/17
	@author	�����
	@brief	�׸��� Ŭ����
*/

class CGrid : virtual public IObject, public CObjectSRT
{
public:

	struct GRIDVERTEX
	{
		D3DXVECTOR3 p;
		DWORD Color;

		enum { FVF=(D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};


	CGrid();
	virtual ~CGrid();
	VOID Clear();
	HRESULT Create() { return S_OK; }
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();
	VOID Update();
	VOID Render();

private:
	HRESULT _InitVB();

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 ����̽�
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
};

#endif