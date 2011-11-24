#include "stdafx.h"
#include "GUIListbox.h"
#include "GUIScrollbar.h"


VOID GUIListbox::Initialize()
{
	m_dStyle			= LBX_VERT;
	m_bActScrollbar		= FALSE;

	m_pGUIScrollbar = new GUIScrollbar( GUIBase::m_pd3dDevice, GUIBase::m_pSprite );
}

VOID GUIListbox::Release()
{
	delete m_pGUIScrollbar;

	ITEMLIST::iterator itE;
	for( itE = m_lstItem.begin() ; itE != m_lstItem.end() ; itE++ )
		delete (*itE);
	m_lstItem.clear();
}

VOID GUIListbox::CopyItemToBack( LPIMAGE _pimgItem, LPIMAGE _pimgBack )
{
	RECT rtBack, rtItem;
	
	SetRect( &rtBack,	static_cast<INT>( ( _pimgItem->vecTrans.x - _pimgItem->vecScale.x * 0.5f ) - m_Data.fX ),
						static_cast<INT>( ( _pimgItem->vecTrans.y - _pimgItem->vecScale.y * 0.5f ) - m_Data.fY ),
						static_cast<INT>( ( _pimgItem->vecTrans.x - _pimgItem->vecScale.x * 0.5f ) - m_Data.fX ) + static_cast<INT>( _pimgItem->vecScale.x ),
						static_cast<INT>( ( _pimgItem->vecTrans.y - _pimgItem->vecScale.y * 0.5f ) - m_Data.fY ) + static_cast<INT>( _pimgItem->vecScale.y ) );

	SetRect( &rtItem,	0, 
						0, 
						static_cast<INT>( _pimgItem->vecScale.x ), 
						static_cast<INT>( _pimgItem->vecScale.y ) );

	LPDIRECT3DSURFACE9	pBackSurface	= NULL;
	LPDIRECT3DSURFACE9	pItemSurface	= NULL;

	LPDIRECT3DTEXTURE9	ptexItem	= _pimgItem->vec2Tex[ 0 ][ 0 ]->pTex;
	LPDIRECT3DTEXTURE9	ptexBack	= _pimgBack->vec2Tex[ 0 ][ 0 ]->pTex;

	ptexBack->GetSurfaceLevel( 0, &pBackSurface );
	ptexItem->GetSurfaceLevel( 0, &pItemSurface );

	D3DXLoadSurfaceFromSurface( pBackSurface, NULL, &rtBack,
								pItemSurface, NULL, &rtItem,
								D3DX_DEFAULT, 0 );
	
	pBackSurface->Release();
	pItemSurface->Release();
}

VOID GUIListbox::CopyBackToFront( LPIMAGE _pimgBack, LPIMAGE _pimgFront )
{
	INT iValue = m_pGUIScrollbar->GetValue();

	RECT rtBackToFront;
	SetRect(	&rtBackToFront,	m_Data.rtFront.left,
								m_Data.rtFront.top + iValue * static_cast<INT>( m_Data.fScrollSpeed ),
								m_Data.rtFront.right,
								m_Data.rtFront.bottom + iValue * static_cast<INT>( m_Data.fScrollSpeed ) );

	LPDIRECT3DSURFACE9	pFrontSurface	= NULL;
	LPDIRECT3DSURFACE9	pBackSurface	= NULL;
	
	LPDIRECT3DTEXTURE9 ptexFront	= _pimgFront->vec2Tex[ 0 ][ 0 ]->pTex;
	LPDIRECT3DTEXTURE9 ptexBack	= _pimgBack->vec2Tex[ 0 ][ 0 ]->pTex;

	ptexFront->GetSurfaceLevel( 0, &pFrontSurface );
	ptexBack->GetSurfaceLevel( 0, &pBackSurface );
	
	D3DXLoadSurfaceFromSurface( pFrontSurface, NULL, &m_Data.rtFront,
								pBackSurface, NULL, &rtBackToFront,
								D3DX_DEFAULT, 0 );
	
	pFrontSurface->Release();
	pBackSurface->Release();
}


VOID GUIListbox::Create( GUIBase::IMAGEPARAM& _imgFront, GUIBase::IMAGEPARAM& _imgBack )
{
	m_Data.fX		= _imgFront.fX;
	m_Data.fY		= _imgFront.fY;
	m_Data.fWidth	= _imgFront.fWidth;
	m_Data.fHeight	= _imgFront.fHeight;

	INT iWidth				= static_cast<INT>( _imgFront.fWidth );
	INT iHeight				= static_cast<INT>( _imgFront.fHeight );
	INT iRatioBackToFront	= static_cast<INT>( m_Data.fRatioBackToFront );

	SetRect( &m_Data.rtFront,	0,
								0,
								iWidth,
								iHeight );

	SetRect( &m_Data.rtBack,	0,
								0,
								iWidth,
								iHeight * iRatioBackToFront );

	CreateImage( m_Data.imgFront, _imgFront );
	CreateImage( m_Data.imgBack, _imgBack );

	m_Data.fRatioBackToFront = _imgBack.fHeight / _imgFront.fHeight;

	//	Test Scrollbar
	//	외부에서 만들어 포인터로 연결만 시킬까??
	GUIBase::IMAGEPARAM	imgThumb, imgBack;

	imgThumb.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgThumb.fX			= _imgFront.fX + _imgFront.fWidth;
	imgThumb.fY			= _imgFront.fY;
	imgThumb.fWidth		= 20.0f;
	imgThumb.fHeight	= 20.0f;

	imgBack.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgBack.fX			= _imgFront.fX + _imgFront.fWidth;
	imgBack.fY			= _imgFront.fY;
	imgBack.fWidth		= 20.0f;
	imgBack.fHeight		= _imgFront.fHeight;
	
	//AddFileName( 0, imgThumb, L"Image\\Brick.png" );
	//AddFileName( 0, imgBack, L"Image\\Brick.png" );

	m_pGUIScrollbar->Create( imgThumb, imgBack );

}

VOID GUIListbox::AddItem( LPWSTR _Text, LPWSTR _TextureFileName )
{
	INT iNumItem	= m_lstItem.size();
	
	FLOAT	fNumItem	= static_cast<FLOAT>( iNumItem );
	FLOAT	fItemGap	= static_cast<FLOAT>( m_Data.iItemGap );

	//	Set Texture Info
	FLOAT fTextureX			= m_Data.fX;
	FLOAT fTextureY			= m_Data.fY + fItemGap * fNumItem;
	FLOAT fTextureWidth		= 0.0f;
	FLOAT fTextureHeight	= 0.0f;

	if( _TextureFileName != NULL )
	{
		fTextureWidth	= fItemGap;
		fTextureHeight	= fItemGap;
	}
	
	//	Set Text Info
	FLOAT fTextX		= fTextureX + fTextureWidth;
	FLOAT fTextY		= fTextureY;
	FLOAT fTextWidth	= m_Data.fWidth - fTextureWidth;
	FLOAT fTextHeight	= m_Data.fHeight;

	//	Create Item
	LPITEM pItem	= new ITEM;

	//	Create Item::imgText
	GUIBase::IMAGEPARAM imgParam;

	imgParam.fX			= fTextX;
	imgParam.fY			= fTextY;
	imgParam.fWidth		= fTextWidth;
	imgParam.fHeight	= fTextHeight;

	pItem->pimgText = new IMAGE;
	CreateImage( *pItem->pimgText, imgParam );

	SetFont( L"궁서", 10, 10 );
	DrawFontOnTexture(	_Text, 
						0x00000000, 
						pItem->pimgText->vec2Tex[ 0 ][ 0 ]->pTex, 
						static_cast<INT>( fTextWidth ), 
						static_cast<INT>( fTextHeight ) );

	CopyItemToBack( pItem->pimgText, &m_Data.imgBack );

	//	Creaet Texture
	if( _TextureFileName != NULL )
	{
		imgParam.fX			= fTextureX;
		imgParam.fY			= fTextureY;
		imgParam.fWidth		= fTextureWidth;
		imgParam.fHeight	= fTextureHeight;

		AddFileName( 0, imgParam, _TextureFileName );
	
		pItem->pimgTexture = new IMAGE;
		CreateImage( *pItem->pimgTexture, imgParam );
	
		CopyItemToBack( pItem->pimgTexture, &m_Data.imgBack );
	}

	//	Add Item List
	m_lstItem.push_back( pItem );


	//	Activate Scrollbar
	FLOAT fItemsHeight = fNumItem * fItemGap;
	if( fItemsHeight > m_Data.fHeight )
	{
		m_bActScrollbar				= TRUE;
		m_Data.fRatioBackToFront	= fItemsHeight / m_Data.fHeight;
		m_Data.fScrollSpeed			= ( ( fItemsHeight - m_Data.fHeight ) * m_Data.fRatioBackToFront ) / 100.0f;
	}
	else 
		m_bActScrollbar				= FALSE;
	
}

VOID GUIListbox::SetItemGap( INT _iItemGap )
{
	/*m_Data.iItemGap = _iItemGap;

	FLOAT fItemGap	= static_cast<FLOAT>( _iItemGap );

	FLOAT fTextureX, fTextureY;
	FLOAT fTextureWidth, fTextureHeight;
	
	FLOAT fTextX, fTextY;
	FLOAT fTextWidth, fTextHeight;
	
	FLOAT fNumItem = 0.0f;

	ITEMLIST::iterator itE;
	for( itE = m_lstItem.begin() ; itE != m_lstItem.end() ; itE++ )
	{
		fTextureX			= m_Data.fX;
		fTextureY			= m_Data.fY + fItemGap * fNumItem;
		fTextureWidth		= 0.0f;
		fTextureHeight		= 0.0f;

		if( (*itE)->pimgTexture != NULL )
		{
			fTextureWidth	= fItemGap;
			fTextureHeight	= fItemGap;

			ImageTranslate( *(*itE)->pimgTexture, fTextureX, fTextureY, 0.0f );
			ImageScale( *(*itE)->pimgTexture, fTextureWidth, fTextureHeight, 1.0f );

			CopyItemToBack( (*itE)->pimgTexture, &m_Data.imgBack );
		}

		fTextX		= fTextureX + fTextureWidth;
		fTextY		= fTextureY;
		fTextWidth	= m_Data.fWidth - fTextureWidth;
		fTextHeight	= m_Data.fHeight;

		ImageTranslate( *(*itE)->pimgText, fTextX, fTextY, 0.0f );
		ImageScale( *(*itE)->pimgText, fTextWidth, fTextHeight, 1.0f );

		CopyItemToBack( (*itE)->pimgText, &m_Data.imgBack );

		fNumItem += 1.0f;
	}*/
		
}

VOID GUIListbox::Update()
{
	if( m_bActScrollbar )
		m_pGUIScrollbar->Update();

	CopyBackToFront( &m_Data.imgBack, &m_Data.imgFront );
	
}

VOID GUIListbox::Render()
{
	RenderImage( m_Data.imgFront );	

	if( m_bActScrollbar )
		m_pGUIScrollbar->Render();
}

VOID GUIListbox::OnDown( INT x, INT y )
{
	if( m_bActScrollbar )
		m_pGUIScrollbar->OnDown( x, y );
}

VOID GUIListbox::OnMove( INT x, INT y )
{
	if( m_bActScrollbar )
		m_pGUIScrollbar->OnMove( x, y );
}

VOID GUIListbox::OnUp( INT x, INT y )
{
	if( m_bActScrollbar )
		m_pGUIScrollbar->OnUp( x, y );
}
