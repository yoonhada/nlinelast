/**
	@file	Matrices.h
	@date	2011/09/19
	@author 백경훈
	@brief	파이프라인 매트릭스 설정
*/

#ifndef _MATRICES_H_
#define _MATRICES_H_

/**
	@class	CMatrices
	@date	2011/09/19
	@author 백경훈
	@brief	파이프라인 매트릭스 설정
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
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	FLOAT					m_fAspect;	  ///< 화면 비율 (윈도 넓이 / 높이)

	D3DXMATRIXA16 m_matProj;
};

#endif