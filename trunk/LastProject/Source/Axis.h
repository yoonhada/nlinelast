/**
	@class	Axis
	@date	2011/08/26
	@author	����ö
	@brief	��ǥ��
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
	LPDIRECT3DDEVICE9		m_pd3dDevice;	///< D3D ����̽�
	LPDIRECT3DVERTEXBUFFER9	m_pVB;			///< ���� ����

	HRESULT	InitVB();	///< ���� ���� ����
	HRESULT	InitVB2();	///< ���� ���� ����

public:
	Axis(LPDIRECT3DDEVICE9 _pd3dDevice);
	virtual ~Axis();

	VOID Clear()		{}
	HRESULT Create();	///< �ʱ�ȭ
	HRESULT Release();	///< ������Ʈ�� ����
	VOID Update();		///< Update
	VOID Render();		///< Render

	HRESULT Create( FLOAT fRange, INT nDeep );

	FLOAT m_fSize;
	INT m_nDeep;

};
