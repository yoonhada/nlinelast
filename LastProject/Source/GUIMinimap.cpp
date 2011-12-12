//#include "stdafx.h"
//#include "GUIMinimap.h"
//#include "GUIBackground.h"
//
//VOID GUIMinimap::Initialize()
//{
//	m_fMapWidth		= 938.1f;
//	m_fMapHeight	= 1676.6f;
//
//	//	Init Map Info
//	m_fMiniMapX			= 950.0f;
//	m_fMiniMapY			= 100.0f;
//	m_fMiniMapWidth		= 200.0f;
//	m_fMiniMapHeight	= 357.0f;
//
//	m_fRatioX		= m_fMiniMapWidth / m_fMapWidth;
//	m_fRatioY		= m_fMiniMapHeight / m_fMapHeight;
//
//	m_pGUIBase	= new GUIBase( m_pd3dDevice, m_pSprite );
//
//	m_pMap		= new GUIBackground( m_pd3dDevice, m_pSprite );
//	
//	for( INT i=0 ; i<MNM_END ; i++ )
//		m_aData[ i ].pImage = new GUIBackground( m_pd3dDevice, m_pSprite );
//}
//
//VOID GUIMinimap::Release()
//{
//	SAFE_DELETE( m_pGUIBase );
//	SAFE_DELETE( m_pMap );
//
//	for( INT i=0 ; i<MNM_END ; i++ )
//		SAFE_DELETE( m_aData[ i ].pImage );
//}
//
//VOID GUIMinimap::Create()
//{
//	FLOAT fX		= 950.0f;
//	FLOAT fY		= 100.0f;
//	FLOAT fWidth	= 200.0f;
//	FLOAT fHeight	= 357.0f;
//	
//	GUIBase::IMAGEPARAM imgParamMiniMap;
//	m_pGUIBase->AddFileName( 0, imgParamMiniMap, L"Img\\MainScene\\Minimap.jpg" );
//
//	m_pMap->Create( fX, fY, fWidth, fHeight, imgParamMiniMap );
//
//	fX		= 0.0f;
//	fY		= 0.0f;
//	fWidth	= 50.0f;
//	fHeight	= 50.0f;
//	
//	//MNM_MOM = 0, MNM_DADDY, MNM_SON, MNM_DAUGHTET, 
//	//		MNM_FIRSTAIDKIT_0, MNM_FIRSTAIDKIT_1, MNM_FIRSTAIDKIT_2, MNM_FIRSTAIDKIT_3, MNM_FIRSTAIDKIT_4,
//	//		MNM_CROWN, MNM_PANDA, MNM_BEAR,
//	//		MNM_END };
//
//	GUIBase::IMAGEPARAM imgParamMom;
//	m_pGUIBase->AddFileName( 0, imgParamMom, L"Img\\Event\\EventM-2.png" );
//
//	m_aData[ MNM_MOM ].pImage->Create( fX, fY, fWidth, fHeight, imgParamMom );
//
//	GUIBase::IMAGEPARAM imgParamDaddy;
//	m_pGUIBase->AddFileName( 0, imgParamDaddy, L"Img\\Event\\EventF-2.png" );
//
//	m_aData[ MNM_DADDY ].pImage->Create( fX, fY, fWidth, fHeight, imgParamMom );
//
//	GUIBase::IMAGEPARAM imgParamSon;
//	m_pGUIBase->AddFileName( 0, imgParamSon, L"Img\\Event\\EventS-2.png" );
//
//	m_aData[ MNM_SON ].pImage->Create( fX, fY, fWidth, fHeight, imgParamMom );
//
//	GUIBase::IMAGEPARAM imgParamDaughter;
//	m_pGUIBase->AddFileName( 0, imgParamDaughter, L"Img\\Event\\EventD-2.png" );
//
//	m_aData[ MNM_DAUGHTER ].pImage->Create( fX, fY, fWidth, fHeight, imgParamMom );
//
//}
//
//VOID GUIMinimap::Update()
//{
//}
//
//VOID GUIMinimap::Render()
//{
//	m_pMap->Render();
//
//	for( INT i=0 ; i<MNM_END ; i++ )
//	{
//		if( m_aData[ i ].bVisible )
//			m_aData[ i ].pImage->Render();
//	}
//}
//
//VOID GUIMinimap::SetVisible( INT _iIndex, BOOL _bVisible )
//{
//	m_aData[ _iIndex ].bVisible = _bVisible;
//}
//
//VOID GUIMinimap::SetPosition( INT _iIndex, LPD3DXVECTOR3 _pvecPosition )
//{	//	_pvecPositionÀº ¿ùµåÁÂÇ¥
//	FLOAT fX	= m_fMiniMapX + ( _pvecPosition->x * m_fRatioX );
//	FLOAT fY	= 1.0f;
//	FLOAT fZ	= m_f_pvecPosition->y * m_fRatioY;
//
//	m_aData[ _iIndex ].pImage-
//}