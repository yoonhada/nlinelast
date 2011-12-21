#include "stdafx.h"
#include "GUIEditManager.h"
#include "GUIEdit.h"
#include "GUIWriting.h"

VOID GUIEditManager::Initialize()
{
	m_iNumData = 0;
}

VOID GUIEditManager::Release()
{
	DATAVECTOR::iterator itE;
	for( itE = m_vecData.begin() ; itE != m_vecData.end() ; itE++ )
	{
		delete (*itE)->pGUIEdit;
		delete (*itE);
	}
	m_vecData.clear();
}

VOID GUIEditManager::CheckFocus()
{
	GUIWriting::GetInstance().DisconnectText();

	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->pGUIEdit->GetFocus() )
		{
			GUIWriting::GetInstance().ConnectText(	m_vecData[ i ]->pGUIEdit->GetText(),
													m_vecData[ i ]->pGUIEdit->GetTextLength() );
	
		}
	}
}

//VOID GUIEditManager::Create( DWORD _dID, DWORD _dStyle, GUIBase::IMAGEPARAM& _imgParamEdit, GUIBase::IMAGEPARAM& _imgParamEdit )
//{
//	LPDATA	pData = new DATA;
//
//	//	Init ID
//	pData->ID	=	_dID;
//
//	//	Init Btn
//	pData->pGUIEdit = new GUIEdit( m_pd3dDevice, m_pSprite );
//
//	pData->pGUIEdit->Create(	_dID,
//								_dStyle,
//								_imgNormal,
//								_imgHot,
//								_imgDown,
//								_imgDisable );
//
//	m_vecData.push_back( pData );
//
//	m_iNumData++;
//}

VOID GUIEditManager::Create( DWORD _dID, DWORD _dStyle, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight,
								GUIBase::IMAGEPARAM& _imgParamEdit, GUIBase::IMAGEPARAM& _imgParamCaret )
{
	LPDATA	pData = new DATA;

	//	Init ID
	pData->ID	=	_dID;

	//	Init Edit
	pData->pGUIEdit = new GUIEdit( m_pd3dDevice, m_pSprite );

	pData->pGUIEdit->Create(	_dID,
								_fX, _fY,
								_fWidth, _fHeight,
								_imgParamEdit,
								_imgParamCaret );

	pData->pGUIEdit->SetStyle( _dStyle );

	m_vecData.push_back( pData );

	m_iNumData++;
}
VOID GUIEditManager::OnDown( INT x, INT y )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIEdit->OnDown( x, y );
}

VOID GUIEditManager::OnMove( INT x, INT y )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIEdit->OnMove( x, y );
}

VOID GUIEditManager::OnUp( INT x, INT y )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIEdit->OnUp( x, y );
}

VOID GUIEditManager::Update()
{
	BOOL bReturn = GUIWriting::GetInstance().GetReturn();

	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		/*if( m_vecData[ i ]->pGUIEdit->Update( bReturn ) )
			GUIWriting::GetInstance().ConnectText(	m_vecData[ i ]->pGUIEdit->GetText(),
													m_vecData[ i ]->pGUIEdit->GetTextLength() );*/
		m_vecData[ i ]->pGUIEdit->Update( bReturn );
	}

	CheckFocus();
}

VOID GUIEditManager::GetCommandID( DWORD& _ID )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == GUIEdit::EdtMessage )
		{
			GUIEdit::EdtMessage	=	0;
			_ID = m_vecData[ i ]->ID;
			break;
		}
	}
	
}

VOID GUIEditManager::SetStyle( DWORD _dID, DWORD _dStyle )
{	
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == _dID )
		{
			m_vecData[ i ]->pGUIEdit->SetStyle( _dStyle );
			break;
		}
	}
}

VOID GUIEditManager::SetState( DWORD _dID, DWORD _dState )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == _dID )
		{
			m_vecData[ i ]->pGUIEdit->SetState( _dState );
			break;
		}
	}
}

VOID GUIEditManager::SetFont( DWORD _dID, LPWSTR _pFaceName, INT _iWidth, INT _iHeight )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == _dID )
		{
			m_vecData[ i ]->pGUIEdit->SetFont( _pFaceName, _iWidth, _iHeight );
			break;
		}
	}
}

VOID GUIEditManager::SetText( DWORD _dID, LPWSTR _pcText )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == _dID )
		{
			m_vecData[ i ]->pGUIEdit->SetText( _pcText );
			break;
		}
	}
}

BOOL GUIEditManager::TakeMessage( DWORD _dID, LPWSTR _pcText )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == _dID )
		{
			if( m_vecData[ i ]->pGUIEdit->TakeMessage( _pcText ) )
				return TRUE;
		}
	}

	return FALSE;
}

VOID GUIEditManager::Render()
{
	for( INT i=0 ; i<m_iNumData ; i++ )
		m_vecData[ i ]->pGUIEdit->Render();
}

BOOL GUIEditManager::GetFocus( DWORD _dID )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == _dID )
		{
			return m_vecData[ i ]->pGUIEdit->GetFocus();
		}
	}

	return FALSE;
}

VOID GUIEditManager::TextClear( DWORD _dID )
{
	for( INT i=0 ; i<m_iNumData ; i++ )
	{
		if( m_vecData[ i ]->ID == _dID )
		{
			m_vecData[ i ]->pGUIEdit->TextClear();
		}
	}
}