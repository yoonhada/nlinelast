#include "stdafx.h"
#include "GUIBtnManager.h"
#include "GUIBtn.h"

VOID GUIBtnManager::Initialize()
{
	m_lstData.clear();
}

VOID GUIBtnManager::Release()
{
	Cleanup();
}

VOID GUIBtnManager::Cleanup()
{
	DATALIST::iterator itE;
	for( itE = m_lstData.begin() ; itE != m_lstData.end() ; itE++ )
	{
		delete (*itE)->pGUIBtn;
		delete (*itE);
	}
	m_lstData.clear();
}

VOID GUIBtnManager::Create( DWORD _ID, DWORD _Style, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, 
							LPCWSTR _Normal, LPCWSTR _Hot, LPCWSTR _Down, LPCWSTR _Disable )
{
	LPDATA	pData = new DATA;

	//	Init ID
	pData->ID	=	_ID;

	//	Init Btn
	pData->pGUIBtn = new GUIBtn( m_pd3dDevice );

	pData->pGUIBtn->Create( _ID,
							_Style,
							_fX, _fY,
							_fWidth, _fHeight,
							_Normal,
							_Hot,
							_Down,
							_Disable );

	m_lstData.push_back( pData );
}

VOID GUIBtnManager::OnDown( INT x, INT y )
{
	DATALIST::iterator itE;
	for( itE = m_lstData.begin() ; itE != m_lstData.end() ; itE++ )
		(*itE)->pGUIBtn->OnDown( x, y );
}

VOID GUIBtnManager::OnMove( INT x, INT y )
{
	DATALIST::iterator itE;
	for( itE = m_lstData.begin() ; itE != m_lstData.end() ; itE++ )
		(*itE)->pGUIBtn->OnMove( x, y );
}

VOID GUIBtnManager::OnUp( INT x, INT y )
{
	DATALIST::iterator itE;
	for( itE = m_lstData.begin() ; itE != m_lstData.end() ; itE++ )
		(*itE)->pGUIBtn->OnUp( x, y );
}

VOID GUIBtnManager::Update( INT x, INT y )
{
	DATALIST::iterator itE;
	for( itE = m_lstData.begin() ; itE != m_lstData.end() ; itE++ )
		(*itE)->pGUIBtn->Update( x, y );
}

VOID GUIBtnManager::GetCommandID( DWORD& _ID )
{
	DATALIST::iterator itE;
	for( itE = m_lstData.begin() ; itE != m_lstData.end() ; itE++ )
	{
		if( (*itE)->ID == GUIBtn::btnMessage )
		{
			_ID = (*itE)->ID;
			break;
		}
	}
	GUIBtn::btnMessage	=	0;
}

VOID GUIBtnManager::Render()
{
	DATALIST::iterator itE;
	for( itE = m_lstData.begin() ; itE != m_lstData.end() ; itE++ )
		(*itE)->pGUIBtn->Render();
	
}