#include "stdafx.h"
#include "GUIBtnManager.h"
#include "GUIBtn.h"

VOID GUIBtnManager::Initialize()
{
	m_iNumData = 0;
}

VOID GUIBtnManager::Release()
{
	DATAVECTOR::iterator itE;
	for( itE = m_vecData.begin() ; itE != m_vecData.end() ; itE++ )
	{
		delete (*itE)->pGUIBtn;
		delete (*itE);
	}
	m_vecData.clear();
}

VOID GUIBtnManager::Create( DWORD _dID, DWORD _dStyle, 
						   GUIBase::IMAGEPARAM& _imgNormal, GUIBase::IMAGEPARAM& _imgHot, GUIBase::IMAGEPARAM& _imgDown, GUIBase::IMAGEPARAM& _imgDisable )
{
	LPDATA	pData = new DATA;

	//	Init ID
	pData->ID	=	_dID;

	//	Init Btn
	pData->pGUIBtn = new GUIBtn( m_pd3dDevice, m_pSprite );

	pData->pGUIBtn->Create( _dID,
							_dStyle,
							_imgNormal,
							_imgHot,
							_imgDown,
							_imgDisable );

	m_vecData.push_back( pData );

	m_iNumData++;
}

VOID GUIBtnManager::OnDown( INT x, INT y )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIBtn->OnDown( x, y );
}

VOID GUIBtnManager::OnMove( INT x, INT y )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIBtn->OnMove( x, y );
}

VOID GUIBtnManager::OnUp( INT x, INT y )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIBtn->OnUp( x, y );
}

VOID GUIBtnManager::Update( INT x, INT y )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIBtn->Update( x, y );
}

VOID GUIBtnManager::GetCommandID( DWORD& _ID )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == GUIBtn::btnMessage )
		{
			_ID = m_vecData[ i ]->ID;
			GUIBtn::btnMessage	=	0;
			break;
		}
	}
	//GUIBtn::btnMessage	=	0;
}

VOID GUIBtnManager::Render()
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIBtn->Render();
}