/**
@file	ObjectManage.cpp
@date	2011/11/11
@author	백경훈
@brief	오브젝트 관리 ( 네트워크와 클래스간 사용 )
*/

#include "Stdafx.h"
#include "ObjectManage.h"

#include "Charactor.h"

CObjectManage::CObjectManage()
{
	Clear();
}

CObjectManage::~CObjectManage()
{
	Release();
}

VOID CObjectManage::Clear()
{
	m_bHost = FALSE;
	m_iClientNumber = 0;
	m_pMyCharactor = NULL;
	m_pCharactors = NULL;
	m_iMaxCharaNum = 3;
}

HRESULT CObjectManage::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;
	
	//캐릭터 생성
	m_pMyCharactor = new CCharactor;

	m_pCharactors = new CCharactor[m_iMaxCharaNum];
	
	return S_OK;
}

HRESULT CObjectManage::Release()
{

	SAFE_DELETE(m_pMyCharactor);
	SAFE_DELETE_ARRAY(m_pCharactors);

	return S_OK;
}