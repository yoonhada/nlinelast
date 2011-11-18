/**
@file	ObjectManage.cpp
@date	2011/11/11
@author	백경훈
@brief	오브젝트 관리 ( 네트워크와 클래스간 사용 )
*/

#include "Stdafx.h"
#include "ObjectManage.h"

#include "Charactor.h"
#include "Monster.h"

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

	m_wTotalDestroyPart = 0;
}

HRESULT CObjectManage::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;
	
	//캐릭터 생성
	m_pMyCharactor = new CCharactor;

	m_pCharactors = new CCharactor[m_iMaxCharaNum];

	m_pCharactorList[0] = m_pMyCharactor;
	m_pCharactorList[1] = &m_pCharactors[0];
	m_pCharactorList[2] = &m_pCharactors[1];
	m_pCharactorList[3] = &m_pCharactors[2];

	Set_CharactorList( m_pCharactorList );

	m_pMonster = new CMonster;
	
	return S_OK;
}

VOID CObjectManage::RefreshCharList()
{
	m_pCharactorList[0] = m_pMyCharactor;
	m_pCharactorList[1] = &m_pCharactors[0];
	m_pCharactorList[2] = &m_pCharactors[1];
	m_pCharactorList[3] = &m_pCharactors[2];

	Set_CharactorList( m_pCharactorList );
}

HRESULT CObjectManage::Release()
{

	SAFE_DELETE(m_pMyCharactor);
	SAFE_DELETE_ARRAY(m_pCharactors);
	SAFE_DELETE(m_pMonster);

	return S_OK;
}