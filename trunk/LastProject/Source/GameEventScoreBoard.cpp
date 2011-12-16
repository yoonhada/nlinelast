#include "stdafx.h"
#include "GameEventScoreBoard.h"
#include "GUIBackground.h"

VOID GameEventScoreBoard::Initialize()
{
	m_pGUIBase			= NULL;

	for( INT i=0 ; i<MAXIDENTIFIER ; i++ )
		m_pIdentifier[ i ] = NULL;

	m_pDynamicNumber	= NULL;

	for( INT i=0 ; i<MAXPOSITIONALNUMBER ; i++ )
		m_pStaticNumber[ i ] = NULL;

	m_dFrameSpeed		= 15;

	m_fX				= 400.0f;
	m_fY				= 400.0f;

	m_fIdtWidth			= 100.0f;
	m_fIdtHeight		= 100.0f;

	m_fNumWidth			= 50.0f;
	m_fNumHeight		= 50.0f;
}

VOID GameEventScoreBoard::Release()
{
	SAFE_DELETE( m_pGUIBase );

	for( INT i=0 ; i<MAXIDENTIFIER ; i++ )
		SAFE_DELETE( m_pIdentifier[ i ] );

	SAFE_DELETE( m_pDynamicNumber );

	for( INT i=0 ; i<MAXPOSITIONALNUMBER ; i++ )
		SAFE_DELETE( m_pStaticNumber[ i ] );

	DATAVECTOR::iterator itE;
	for( itE = m_vecData.begin() ; itE != m_vecData.end() ; itE++ )
		delete (*itE);
	m_vecData.clear();
}

VOID GameEventScoreBoard::InitIdentifierImage()
{
	FLOAT fX	= 0.0f;
	FLOAT fY	= 0.0f;

	//	Create Identifier
	GUIBase::IMAGEPARAM imgParamIdentifier[ MAXIDENTIFIER ];

	m_pGUIBase->AddFileName( 0, imgParamIdentifier[ GES_DADDY ], L"Img\\Event\\ScoreBoard\\IdentifierDaddy.png" );
	m_pIdentifier[ GES_DADDY ]->Create( fX, fY, m_fIdtWidth, m_fIdtHeight, imgParamIdentifier[ GES_DADDY ] );

	m_pGUIBase->AddFileName( 0, imgParamIdentifier[ GES_MOM ], L"Img\\Event\\ScoreBoard\\IdentifierMom.png" );
	m_pIdentifier[ GES_MOM ]->Create( fX, fY, m_fIdtWidth, m_fIdtHeight, imgParamIdentifier[ GES_MOM ] );

	m_pGUIBase->AddFileName( 0, imgParamIdentifier[ GES_SON ], L"Img\\Event\\ScoreBoard\\IdentifierSon.png" );
	m_pIdentifier[ GES_SON ]->Create( fX, fY, m_fIdtWidth, m_fIdtHeight, imgParamIdentifier[ GES_SON ] );

	m_pGUIBase->AddFileName( 0, imgParamIdentifier[ GES_DAUGHTER ], L"Img\\Event\\ScoreBoard\\IdentifierDaughter.png" );
	m_pIdentifier[ GES_DAUGHTER ]->Create( fX, fY, m_fIdtWidth, m_fIdtHeight, imgParamIdentifier[ GES_DAUGHTER ] );

}

VOID GameEventScoreBoard::InitNumberImage()
{
	FLOAT fX	= 0.0f;
	FLOAT fY	= 0.0f;
	
	//	 Create Dynamic Number
	GUIBase::IMAGEPARAM imgParamDynamicNumber;

	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\0.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\1.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\2.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\3.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\4.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\5.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\6.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\7.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\8.png", m_dFrameSpeed );
	m_pGUIBase->AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\9.png", m_dFrameSpeed );

	m_pDynamicNumber->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamDynamicNumber );

	//	Create Static Number
	GUIBase::IMAGEPARAM imgParamStaticNumber[ MAXPOSITIONALNUMBER ];

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 0 ], L"Img\\Event\\ScoreBoard\\0.png" );
	m_pStaticNumber[ 0 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 0 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 1 ], L"Img\\Event\\ScoreBoard\\1.png" );
	m_pStaticNumber[ 1 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 1 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 2 ], L"Img\\Event\\ScoreBoard\\2.png" );
	m_pStaticNumber[ 2 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 2 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 3 ], L"Img\\Event\\ScoreBoard\\3.png" );
	m_pStaticNumber[ 3 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 3 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 4 ], L"Img\\Event\\ScoreBoard\\4.png" );
	m_pStaticNumber[ 4 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 4 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 5 ], L"Img\\Event\\ScoreBoard\\5.png" );
	m_pStaticNumber[ 5 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 5 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 6 ], L"Img\\Event\\ScoreBoard\\6.png" );
	m_pStaticNumber[ 6 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 6 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 7 ], L"Img\\Event\\ScoreBoard\\7.png" );
	m_pStaticNumber[ 7 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 7 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 8 ], L"Img\\Event\\ScoreBoard\\8.png" );
	m_pStaticNumber[ 8 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 8 ] );

	m_pGUIBase->AddFileName( 0, imgParamStaticNumber[ 9 ], L"Img\\Event\\ScoreBoard\\9.png" );
	m_pStaticNumber[ 9 ]->Create( fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 9 ] );

}



VOID GameEventScoreBoard::Create( LPDIRECT3DDEVICE9 _pd3dDevice, LPD3DXSPRITE _pSprite )
{
	//	New GUIBase
	m_pGUIBase				= new GUIBase( m_pd3dDevice, m_pSprite );
	
	//	New Identifier
	for( INT i=0 ; i<MAXIDENTIFIER ; i++ )
		m_pIdentifier[ i ]	= new GUIBackground( m_pd3dDevice, m_pSprite );

	//	New Dynamic Number
	m_pDynamicNumber		= new GUIBackground( m_pd3dDevice, m_pSprite );
	
	//	New Static Number
	for( INT i=0 ; i<MAXPOSITIONALNUMBER ; i++ )
		m_pStaticNumber[ i ]= new GUIBackground( m_pd3dDevice, m_pSprite );

	InitIdentifierImage();
	InitNumberImage();
}

VOID GameEventScoreBoard::Update()
{
	INT iNumData = m_vecData.size();

	for( INT i=0 ; i<iNumData ; i++ )
	{
		m_vecData[ i ]->dCurrentTime = timeGetTime();

		if( m_vecData[ i ]->dCurrentTime > m_vecData[ i ]->dBeginTime + m_vecData[ i ]->dFrameSpeed )
		{
			m_vecData[ i ]->dBeginTime = m_vecData[ i ]->dCurrentTime;

			m_vecData[ i ]->apScore[ m_vecData[ i ]->iCurrentPositionalNumber ] = m_pStaticNumber[ m_vecData[ i ]->aiScore[ m_vecData[ i ]->iCurrentPositionalNumber ] ];

			m_vecData[ i ]->iCurrentPositionalNumber++;
			if( m_vecData[ i ]->iCurrentPositionalNumber >= m_vecData[ i ]->iPositionalNumber )
				m_vecData[ i ]->iCurrentPositionalNumber = m_vecData[ i ]->iPositionalNumber;
		}
	}
}

VOID GameEventScoreBoard::Render()
{
	INT iNumData = m_vecData.size();

	for( INT i=0 ; i<iNumData ; i++ )
	{
		for( INT i=0 ; i<m_vecData[ i ]->iPositionalNumber ; i++ )
		{
			D3DXVECTOR3 vecPosition  = m_vecData[ i ]->vecTranslate;
			vecPosition.x += i * 50.0f;

			m_vecData[ i ]->apScore[ i ]->SetPosition( &vecPosition );
			m_vecData[ i ]->apScore[ i ]->Render();
		}
	}
}

VOID GameEventScoreBoard::AddData( DWORD _dID, DWORD _dIdentifier )
{
	if( _dIdentifier < 0 || _dIdentifier >= MAXIDENTIFIER )
	{
		MessageBox( NULL, L"GameEventScoreBoard::AddData(){ _dIdentifier > 0 || _dIdentifier <= MAXIDENTIFIER }", NULL, MB_OK );
		return;
	}

	LPDATA pData = new DATA;

	pData->pIdentifier = m_pIdentifier[ _dIdentifier ];

	INT iNumData = m_vecData.size();
	pData->vecTranslate = D3DXVECTOR3( m_fX, m_fY + static_cast<FLOAT>( iNumData * 100 ), 0.0f );
	
	m_vecData.push_back( pData );
}

INT GameEventScoreBoard::GetNumberOfDecimal( INT _iNumber )
{
	if( _iNumber < 0 )_iNumber = -_iNumber;

	if ( _iNumber > 99999)
	{
		if ( _iNumber > 99999999)
		{
			if ( _iNumber > 999999999) return 10;
			else  return 9;
		} 
		else
		{
			if ( _iNumber > 9999999) return 8;
			else if ( _iNumber > 999999) return 7;
			else return 6;
		}
	} 
	else 
	{
		if ( _iNumber > 999) 
		{
			if ( _iNumber > 9999) return 5;
			else return 4;
		} 
		else 
		{
			if ( _iNumber > 99) return 3;
			else if ( _iNumber > 9) return 2;
			else return 1;
		}
	}
}

VOID GameEventScoreBoard::SetScore( DWORD _dID, INT _iScore )
{
	INT iDecimal = GetNumberOfDecimal( _iScore );

	LPDATA pData = NULL;
	
	//	Find Data
	for( INT i=0 ; i<MAXIDENTIFIER ; i++ )
	{
		if( m_vecData[ i ]->dID == _dID )
		{
			pData = m_vecData[ i ];
			break;
		}
	}

	//	Set PositionalNumber
	pData->iPositionalNumber = iDecimal;

	//	Set Score
	for( INT i=0 ; i<iDecimal ; i++ )
	{
		INT  Multiplier = static_cast<INT>( pow( static_cast<DOUBLE>( 10 ), i ) );
		pData->aiScore[ i ] = _iScore / Multiplier % 10; 
	}
}