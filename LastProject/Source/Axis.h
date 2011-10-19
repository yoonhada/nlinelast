/**
	@class	Axis
	@date	2011/08/26
	@author	김종철
	@brief	좌표축
*/

#pragma once

class Axis : public IObject
{
private:
	struct VERTEX
	{
		enum { FVF = ( D3DFVF_XYZ | D3DFVF_DIFFUSE ), };
		D3DXVECTOR3 _pos;
		DWORD _col;
	};

protected:
	LPDIRECT3DDEVICE9		m_pd3dDevice;	///< D3D 디바이스
	LPDIRECT3DVERTEXBUFFER9	m_pVB;			///< 정점 버퍼

	HRESULT	InitVB();	///< 정점 버퍼 생성
	HRESULT	InitVB2();	///< 정점 버퍼 생성

public:
	Axis(LPDIRECT3DDEVICE9 _pd3dDevice);
	virtual ~Axis();

	VOID Clear()		{}
	HRESULT Create();	///< 초기화
	HRESULT Release();	///< 오브젝트들 제거
	VOID Update();		///< Update
	VOID Render();		///< Render

	HRESULT Create( FLOAT fRange, INT nDeep );

	FLOAT m_fSize;
	INT m_nDeep;

};
