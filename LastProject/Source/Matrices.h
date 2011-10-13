/**
	@file	Matrices.h
	@date	2011/09/19
	@author �����
	@brief	���������� ��Ʈ���� ����
*/

#ifndef _MATRICES_H_
#define _MATRICES_H_

/**
	@class	CMatrices
	@date	2011/09/19
	@author �����
	@brief	���������� ��Ʈ���� ����
*/

class CMatrices : public CSingleton<CMatrices>
{
	friend class CSingleton<CMatrices>;

public:
	CMatrices(VOID);
	~CMatrices(VOID);

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, FLOAT a_fAspect );

	VOID SetupModeltoWorld( const D3DXMATRIXA16& a_matWorld );
	VOID SetupProjection();

	const D3DXMATRIXA16* Get_matProj()
	{
		return &m_matProj;
	}


private:
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 ����̽�
	FLOAT					m_fAspect;	  ///< ȭ�� ���� (���� ���� / ����)

	D3DXMATRIXA16 m_matProj;
};

#endif