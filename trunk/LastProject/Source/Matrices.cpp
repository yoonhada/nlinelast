/**
	@file	Matrices.cpp
	@date	2011/09/19
	@author 백경훈
	@brief	파이프라인 매트릭스 설정
*/

#include "StdAfx.h"
#include "Matrices.h"

CMatrices::CMatrices(VOID)
{
	Clear();
}

CMatrices::~CMatrices(VOID)
{
}

VOID CMatrices::Clear()
{
	ZeroMemory( m_matProj, sizeof(m_matProj) );
}

HRESULT CMatrices::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, FLOAT a_fAspect )
{
	m_pD3dDevice = a_pD3dDevice;
	m_fAspect = a_fAspect;

	return S_OK;
}

VOID CMatrices::SetupProjection()
{
	D3DXMatrixPerspectiveFovLH( &m_matProj, 0.7853981635f, m_fAspect, 1.0f, 10000.0f );

	m_pD3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
}