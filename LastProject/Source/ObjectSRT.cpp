/**
	@file	I_ObjectSRT.cpp
	@date	2011/09/20
	@author	백경훈
	@brief	오브젝트 SRT 클래스
*/

#include "StdAfx.h"
#include "ObjectSRT.h"

CObjectSRT::CObjectSRT(VOID)
{
	D3DXMatrixIdentity( &m_matWorld );
	m_ControlScale[0] = m_ControlScale[1] = m_ControlScale[2] = 1.0f;
	ZeroMemory( m_ControlRotate, sizeof(m_ControlRotate) );
	ZeroMemory( m_ControlTranslate, sizeof(m_ControlTranslate) );
}

CObjectSRT::~CObjectSRT(VOID)
{
}

VOID CObjectSRT::Calcul_MatWorld()
{
	D3DXMatrixScaling( &m_matScale, m_ControlScale[0], m_ControlScale[1], m_ControlScale[2] );


	D3DXMatrixRotationX( &m_matRotate[0], m_ControlRotate[0]  );
	D3DXMatrixRotationY( &m_matRotate[1], m_ControlRotate[1]  );
	D3DXMatrixRotationZ( &m_matRotate[2], m_ControlRotate[2]  );

	D3DXMatrixMultiply( &m_matRotate[0], &m_matRotate[0], &m_matRotate[1] );
	D3DXMatrixMultiply( &m_matRotate[0], &m_matRotate[0], &m_matRotate[2] );

	D3DXMatrixTranslation( &m_matTranslate, m_ControlTranslate[0], m_ControlTranslate[1], m_ControlTranslate[2] );
	
	//m_matWorld =  m_matScale * ( m_matRotate[0] * m_matRotate[1] * m_matRotate[2] ) * m_matTranslate;

	D3DXMatrixMultiply( &m_matWorld, &m_matScale, &m_matRotate[0] );

	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, &m_matTranslate );
}
