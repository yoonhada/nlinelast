/**
@class	CGameEvent
@date	2011/11/25
@author	yoonhada@gmail.com
@brief	게임이벤트
*/

#include "stdafx.h"
#include "GameEvent.h"

CGameEvent::CGameEvent()
{
	Clear();
}

CGameEvent::~CGameEvent()
{
	Release();
}

HRESULT CGameEvent::Create()
{
	return S_OK;
}

HRESULT CGameEvent::Release()
{
	return S_OK;
}


VOID CGameEvent::Update()
{
}

VOID CGameEvent::Render()
{
}