/**
@file	BillBoard.h
@date	2011/09/28
@author	yoonhada@gmail.com
@brief	빌보드 클래스
*/

#pragma once
#include <list>

class CBillBoard : public virtual IObject
{
	struct _EFFECT{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
	};
	static _EFFECT m_pVtx[4];
	static D3DXVECTOR2 m_pTexUV[16][4];


public:
	CBillBoard( LPDIRECT3DDEVICE9 _pd3dDevice );
	virtual ~CBillBoard();

	virtual VOID Clear();
	virtual HRESULT Create();
	virtual HRESULT Release();
	virtual VOID Render();
	virtual VOID Update();

	VOID SetPosition(D3DXVECTOR3 _vec)			{ m_vPosition = _vec; }
	VOID SetInverMatrix(D3DXMATRIXA16& _mat)	{ m_matInver = _mat; }
	VOID SetWorldMatirx(D3DXMATRIXA16& _mat)	{ m_matWorld = _mat; }
	VOID SetType(INT _nType)					{ m_nType = _nType; }
	VOID SetLife(INT _nLife)					{ m_nLife = _nLife; }
	BOOL IsLife()								{ return ( m_nLife > 0 ); }
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPDIRECT3DTEXTURE9		m_pTexBillboard[4];
	INT m_nStep;
	INT m_nLife;
	DWORD m_dwTick;
	D3DXMATRIXA16 m_matInver;
	D3DXMATRIXA16 m_matWorld;

	// 
	INT m_nType;
	D3DXVECTOR3 m_vPosition;
};
