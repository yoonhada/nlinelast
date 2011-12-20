#include "stdafx.h"
#include "GameEventScoreBoard.h"
#include "Mouse.h"
#include "GUIBtnManager.h"

VOID GameEventScoreBoard::Initialize()
{
	m_pMouse				= new Mouse();
	m_pGUIBtnManager		= new GUIBtnManager( m_pd3dDevice, m_pSprite );

	m_dState				= GES_HIDDEN;

	m_dScoreFrameSpeed		= 15;
	m_dPositionalFrameSpeed	= 800;

	m_fBgdX				= 150.0f;
	m_fBgdY				= 110.0f;
	m_fBgdWidth			= 1040.0f;
	m_fBgdHeight		= 800.0f;

	m_fIdtWidth			= 80.0f;
	m_fIdtHeight		= 120.0f;

	m_fNumWidth			= 40.0f;
	m_fNumHeight		= 40.0f;

	m_dEndingBeginTime		= 0;
	m_dEndingFrameSpeed		= 5000;
}

VOID GameEventScoreBoard::Release()
{
	delete m_pMouse;
	delete m_pGUIBtnManager;

	DATAVECTOR::iterator itE;
	for( itE = m_vecData.begin() ; itE != m_vecData.end() ; itE++ )
		delete (*itE);
	m_vecData.clear();
}

VOID GameEventScoreBoard::CreateEndingImage()
{
	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pd3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );
	
	GUIBase::IMAGEPARAM imgParamEnding;

	AddFileName( 0, imgParamEnding, L"Img\\Event\\ScoreBoard\\Ending_0.png", 300 );
	AddFileName( 0, imgParamEnding, L"Img\\Event\\ScoreBoard\\Ending_1.png", 300 );

	CreateImage2D( m_img2DEnding, 0.0f, 0.0f, fWidth, fHeight, imgParamEnding );
}

VOID GameEventScoreBoard::CreateSceneImage()
{
	//	Create Background
	D3DVIEWPORT9 Vp;
	m_pd3dDevice->GetViewport( &Vp );

	FLOAT fWidth	= static_cast<FLOAT>( Vp.Width );
	FLOAT fHeight	= static_cast<FLOAT>( Vp.Height );
	
	GUIBase::IMAGEPARAM imgParamScene;

	AddFileName( 0, imgParamScene, L"Img\\Event\\ScoreBoard\\GrayScene.png" );
	CreateImage2D( m_img2DScene, 0.0f, 0.0f, fWidth, fHeight, imgParamScene );
}

VOID GameEventScoreBoard::CreateBackgroundImage()
{
	GUIBase::IMAGEPARAM imgParamBackground;

	AddFileName( 0, imgParamBackground, L"Img\\Event\\ScoreBoard\\Background.png" );
	CreateImage3D( m_img3DBackground, m_fBgdX, m_fBgdY, m_fBgdWidth, m_fBgdHeight, imgParamBackground );
}

VOID GameEventScoreBoard::CreateIdentifierImage()
{
	FLOAT fX	= 0.0f;
	FLOAT fY	= 0.0f;

	//	Create Identifier
	GUIBase::IMAGEPARAM imgParamIdentifier[ MAX_IDENTIFIER ];

	AddFileName( 0, imgParamIdentifier[ GES_DADDY ], L"Img\\Event\\ScoreBoard\\IdentifierDaddy.png" );
	CreateImage3D( m_aimg3DIdentifier[ GES_DADDY ], fX, fY, m_fIdtWidth, m_fIdtHeight, imgParamIdentifier[ GES_DADDY ] );
	
	AddFileName( 0, imgParamIdentifier[ GES_MOM ], L"Img\\Event\\ScoreBoard\\IdentifierMom.png" );
	CreateImage3D( m_aimg3DIdentifier[ GES_MOM ], fX, fY, m_fIdtWidth, m_fIdtHeight, imgParamIdentifier[ GES_MOM ] );

	AddFileName( 0, imgParamIdentifier[ GES_SON ], L"Img\\Event\\ScoreBoard\\IdentifierSon.png" );
	CreateImage3D( m_aimg3DIdentifier[ GES_SON ], fX, fY, m_fIdtWidth, m_fIdtHeight, imgParamIdentifier[ GES_SON ] );

	AddFileName( 0, imgParamIdentifier[ GES_DAUGHTER ], L"Img\\Event\\ScoreBoard\\IdentifierDaughter.png" );
	CreateImage3D( m_aimg3DIdentifier[ GES_DAUGHTER ], fX, fY, m_fIdtWidth, m_fIdtHeight, imgParamIdentifier[ GES_DAUGHTER ] );
}

VOID GameEventScoreBoard::CreateNumberImage()
{
	FLOAT fX	= 0.0f;
	FLOAT fY	= 0.0f;
	
	//	 Create Dynamic Number
	GUIBase::IMAGEPARAM imgParamDynamicNumber;

	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\0.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\1.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\2.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\3.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\4.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\5.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\6.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\7.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\8.png", m_dScoreFrameSpeed );
	AddFileName( 0, imgParamDynamicNumber, L"Img\\Event\\ScoreBoard\\9.png", m_dScoreFrameSpeed );

	CreateImage3D( m_img3DDynamicNumber, fX, fY, m_fNumWidth, m_fNumHeight, imgParamDynamicNumber );
	
	//	Create Static Number
	GUIBase::IMAGEPARAM imgParamStaticNumber[ MAX_POSITIONAL ];

	AddFileName( 0, imgParamStaticNumber[ 0 ], L"Img\\Event\\ScoreBoard\\0.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 0 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 0 ] );

	AddFileName( 0, imgParamStaticNumber[ 1 ], L"Img\\Event\\ScoreBoard\\1.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 1 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 1 ] );

	AddFileName( 0, imgParamStaticNumber[ 2 ], L"Img\\Event\\ScoreBoard\\2.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 2 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 2 ] );

	AddFileName( 0, imgParamStaticNumber[ 3 ], L"Img\\Event\\ScoreBoard\\3.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 3 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 3 ] );

	AddFileName( 0, imgParamStaticNumber[ 4 ], L"Img\\Event\\ScoreBoard\\4.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 4 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 4 ] );

	AddFileName( 0, imgParamStaticNumber[ 5 ], L"Img\\Event\\ScoreBoard\\5.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 5 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 5 ] );

	AddFileName( 0, imgParamStaticNumber[ 6 ], L"Img\\Event\\ScoreBoard\\6.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 6 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 6 ] );

	AddFileName( 0, imgParamStaticNumber[ 7 ], L"Img\\Event\\ScoreBoard\\7.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 7 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 7 ] );

	AddFileName( 0, imgParamStaticNumber[ 8 ], L"Img\\Event\\ScoreBoard\\8.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 8 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 8 ] );

	AddFileName( 0, imgParamStaticNumber[ 9 ], L"Img\\Event\\ScoreBoard\\9.png" );
	CreateImage3D( m_aimg3DStaticNumber[ 9 ], fX, fY, m_fNumWidth, m_fNumHeight, imgParamStaticNumber[ 9 ] );

}

VOID GameEventScoreBoard::CreateButton()
{
	FLOAT fX		= 530.0f;
	FLOAT fY		= 760.0f;
	FLOAT fWidth	= 320.0f;
	FLOAT fHeight	= 80.0f;

	IMAGEPARAM imgParamNormal, imgParamHot, imgParamDown, imgParamDisable;
	
	AddFileName( 0, imgParamNormal, L"Img\\Event\\ScoreBoard\\End_Normal.png" );
	AddFileName( 0, imgParamHot, L"Img\\Event\\ScoreBoard\\End_Hot.png" );
	AddFileName( 0, imgParamDown, L"Img\\Event\\ScoreBoard\\End_Down.png" );
	AddFileName( 0, imgParamDisable, L"Img\\Event\\ScoreBoard\\End_Down.png" );

	m_pGUIBtnManager->Create( MAIN_SCORE, 0, fX, fY, fWidth, fHeight, imgParamNormal, imgParamHot, imgParamDown, imgParamDisable );
}

VOID GameEventScoreBoard::Create()
{
	CreateEndingImage();
	CreateSceneImage();
	CreateBackgroundImage();
	CreateIdentifierImage();
	CreateNumberImage();

	CreateButton();

	m_pMouse->Initialize( m_hWnd );
}

VOID GameEventScoreBoard::Update()
{
	if( m_dState == GES_HIDDEN || m_dState == GES_GRAY || m_dState == GES_ENDING )
		return;

	//	Update Number
	INT iDataSize = m_vecData.size();

	for( INT i=0 ; i<iDataSize ; i++ )
	{
		m_vecData[ i ]->dCurrentTime = timeGetTime();

		if( m_vecData[ i ]->dCurrentTime > m_vecData[ i ]->dBeginTime + m_dPositionalFrameSpeed )
		{
			m_vecData[ i ]->dBeginTime = m_vecData[ i ]->dCurrentTime;

			m_vecData[ i ]->apimg3DScore[ m_vecData[ i ]->iCurrentPositionalNumber ] = &m_aimg3DStaticNumber[ m_vecData[ i ]->aiScore[ m_vecData[ i ]->iCurrentPositionalNumber ] ];

			m_vecData[ i ]->iCurrentPositionalNumber++;
			if( m_vecData[ i ]->iCurrentPositionalNumber >= MAX_POSITIONAL )
				m_vecData[ i ]->iCurrentPositionalNumber = MAX_POSITIONAL;
		}
	}
	
	//	Update Button
	POINT pt = m_pMouse->GetPosition();

	m_pMouse->Update();
	m_pGUIBtnManager->Update( pt.x, pt.y );
	
	static BOOL bFirst = FALSE;
	
	if( m_pMouse->LButton_Down() )
	{
		if( bFirst == FALSE )
		{
			OnDown( pt.x, pt.y );
			bFirst = TRUE;
		}
		//if( GrpBtn::g_bCapture == FALSE )
		//{
			//	버튼을 누르면 g_bCapture = TREU 가 된다
			//	영역체크에 쓴다
		//}
	}
	
	OnMove( pt.x, pt.y );
	
	if( m_pMouse->LButton_Up() )
	{
		OnUp( pt.x, pt.y );
		bFirst = FALSE;
	}
}

VOID GameEventScoreBoard::Render()
{
	if( m_dState == GES_HIDDEN )
		return;

	//	Render Ending
	if( m_dState == GES_ENDING )
	{
		RenderImage2D( &m_img2DEnding );

		DWORD dCurrentTime = timeGetTime();

		if( dCurrentTime > m_dEndingBeginTime + m_dEndingFrameSpeed )
			m_dState = GES_GRAY;
	}

	//	Render GrayScene
	static INT iAlpha = 0;
	if( iAlpha < 255 )
		iAlpha += 1;
	else
		m_dState = GES_NORMAL;

	RenderImage2D( &m_img2DScene, D3DCOLOR_ARGB( iAlpha, 255, 255, 255 ) );

	if( m_dState == GES_GRAY )
		return;
	
	//	Render Background
	RenderImage3D( &m_img3DBackground );

	//	Render Identifier, ScoreNumber
	INT iNumData = m_vecData.size();

	for( INT j=0 ; j<iNumData ; j++ )
	{
		LPDATA	pData = m_vecData[ j ];

		D3DXVECTOR3 vecPosition  = pData->vecTranslate;
		Image3DTranslate( pData->pimg3DIdentifier, vecPosition.x, vecPosition.y, vecPosition.z );
		RenderImage3D( pData->pimg3DIdentifier );

		vecPosition.x += 200.0f;

		INT l = MAX_POSITIONAL - 1;
		for( INT i=0 ; i<MAX_POSITIONAL ; i++ )
		{
			vecPosition.x += m_fNumWidth;

			Image3DTranslate( pData->apimg3DScore[ l ], vecPosition.x, vecPosition.y, vecPosition.z );
			RenderImage3D( pData->apimg3DScore[ l ] );

			l--;
		}
	}
	
	//	Render Button
	m_pGUIBtnManager->Render();
}

VOID GameEventScoreBoard::AddData( DWORD _dID, DWORD _dIdentifier )
{
	if( _dIdentifier < 0 || _dIdentifier >= MAX_IDENTIFIER )
	{
		MessageBox( NULL, L"GameEventScoreBoard::AddData(){ _dIdentifier > 0 || _dIdentifier <= MAX_IDENTIFIER }", NULL, MB_OK );
		return;
	}

	LPDATA pData = new DATA;

	pData->dID				= _dID;
	pData->pimg3DIdentifier	= &m_aimg3DIdentifier[ _dIdentifier ];

	for( INT i=0 ; i<MAX_POSITIONAL ; i++ )
		pData->apimg3DScore[ i ] = &m_img3DDynamicNumber;

	INT iDataSize = m_vecData.size();
	FLOAT fGapX	= 160.0f;
	FLOAT fGapY = 200.0f + 120.0f * static_cast<FLOAT>( iDataSize + 1 );
	pData->vecTranslate = D3DXVECTOR3( m_fBgdX + fGapX, m_fBgdY + fGapY, 0.0f );
	
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
	for( INT i=0 ; i<MAX_IDENTIFIER ; i++ )
	{
		if( m_vecData[ i ]->dID == _dID )
		{
			pData = m_vecData[ i ];
			break;
		}
	}

	pData->iPositionalNumber = iDecimal;

	//	Set Score
	for( INT i=0 ; i<iDecimal ; i++ )
	{
		INT  Multiplier = static_cast<INT>( pow( static_cast<DOUBLE>( 10 ), i ) );
		pData->aiScore[ i ] = _iScore / Multiplier % 10; 
	}
}

VOID GameEventScoreBoard::Skip()
{
	m_dPositionalFrameSpeed = 0;
}

VOID GameEventScoreBoard::Command( DWORD& _dOut )
{
	m_pGUIBtnManager->GetCommandID( _dOut );
}

VOID GameEventScoreBoard::SetState( DWORD _dState )
{
	m_dState = _dState;

	m_dEndingBeginTime = timeGetTime();
}

VOID GameEventScoreBoard::OnDown( INT _iX, INT _iY )
{
	m_pGUIBtnManager->OnDown( _iX, _iY );
}

VOID GameEventScoreBoard::OnMove( INT _iX, INT _iY )
{
	m_pGUIBtnManager->OnMove( _iX, _iY );
}

VOID GameEventScoreBoard::OnUp( INT _iX, INT _iY )
{
	m_pGUIBtnManager->OnUp( _iX, _iY );
}