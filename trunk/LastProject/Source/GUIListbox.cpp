#include "stdafx.h"
#include "GUIListbox.h"
#include "GUIBtnManager.h"
#include "GUIScrollbar.h"
#include "GUIFont.h"


VOID GUIListbox::Initialize()
{
	m_dStyle			= LBX_VERT;
	m_bActScrollbar		= FALSE;

	m_iMaxItem			= 10;
	m_iScrollSpeed		= 1;	//	정수형?
	m_iItemGap			= 50;

	m_fX		= 0.0f;	m_fY		= 0.0f;
	m_fWidth	= 0.0f; m_fHeight	= 0.0f;

	//	Init Font Info
	_tcscpy_s( m_aFaceName, L"휴먼매직체" );
	m_iFontWidth	= 10;
	m_iFontHeight	= 10;
	m_dFontColor	= 0x00000000;

	m_pGUIBtnManager	= new GUIBtnManager( GUIBase::m_pd3dDevice, GUIBase::m_pSprite );
	m_pGUIScrollbar		= new GUIScrollbar( GUIBase::m_pd3dDevice, GUIBase::m_pSprite );
}

VOID GUIListbox::Release()
{
	delete m_pGUIBtnManager;
	delete m_pGUIScrollbar;
}

VOID GUIListbox::CopyTextureToItem( INT _iNumItem, LPIMAGE2D _pimg2DTexture, LPIMAGE2D _pimg2DItemNormal, LPIMAGE2D _pimg2DItemDown )
{
	RECT rtTexture, rtItemNormal,rtItemDown;
	
	rtTexture		= _pimg2DTexture->rtSource;
	rtItemNormal	= _pimg2DTexture->rtSource;	//	왼쪽에 붙어 있고 텍스쳐 크기랑 높이가 같기 떄문에 그대로 붙인다
	rtItemDown		= _pimg2DTexture->rtSource;
	
	LPDIRECT3DSURFACE9	pItemNormalSurface	= NULL;
	LPDIRECT3DSURFACE9	pItemDownSurface	= NULL;
	LPDIRECT3DSURFACE9	pTextureSurface		= NULL;
	
	LPDIRECT3DTEXTURE9	ptexItemNormal	= (*_pimg2DItemNormal->vec2Tex[ 0 ])[ 0 ]->pTex;
	LPDIRECT3DTEXTURE9	ptexItemDown	= (*_pimg2DItemDown->vec2Tex[ 0 ])[ 0 ]->pTex;
	LPDIRECT3DTEXTURE9	ptexTexture		= (*_pimg2DTexture->vec2Tex[ 0 ])[ 0 ]->pTex;
	
	ptexItemNormal->GetSurfaceLevel( 0, &pItemNormalSurface );
	ptexItemDown->GetSurfaceLevel( 0, &pItemDownSurface );
	ptexTexture->GetSurfaceLevel( 0, &pTextureSurface );
	
	D3DXLoadSurfaceFromSurface( pItemNormalSurface, NULL, &rtItemNormal,
								pTextureSurface, NULL, &rtTexture,
								D3DX_DEFAULT, 0 );
	
	D3DXLoadSurfaceFromSurface( pItemDownSurface, NULL, &rtItemDown,
								pTextureSurface, NULL, &rtTexture,
								D3DX_DEFAULT, 0 );
	ptexItemNormal->Release();
	ptexItemDown->Release();
	ptexTexture->Release();
}

VOID GUIListbox::CopyTextToItem( INT _iNumItem, LPIMAGE2D _pimg2DText, LPIMAGE2D _pimg2DItemNormal, LPIMAGE2D _pimg2DItemDown )
{
	RECT rtText, rtItemNormal,rtItemDown;
	
	rtText			= _pimg2DText->rtSource;
	rtItemNormal	= _pimg2DItemNormal->rtSource;	
	rtItemDown		= _pimg2DItemDown->rtSource;

	rtItemNormal.left	= rtItemNormal.right - rtText.right;
	rtItemDown.left		= rtItemDown.right - rtText.right;
	
	LPDIRECT3DSURFACE9	pItemNormalSurface	= NULL;
	LPDIRECT3DSURFACE9	pItemDownSurface	= NULL;
	LPDIRECT3DSURFACE9	pTextSurface		= NULL;
	
	LPDIRECT3DTEXTURE9	ptexItemNormal	= (*_pimg2DItemNormal->vec2Tex[ 0 ])[ 0 ]->pTex;
	LPDIRECT3DTEXTURE9	ptexItemDown	= (*_pimg2DItemDown->vec2Tex[ 0 ])[ 0 ]->pTex;
	LPDIRECT3DTEXTURE9	ptexText		= (*_pimg2DText->vec2Tex[ 0 ])[ 0 ]->pTex;
	
	ptexItemNormal->GetSurfaceLevel( 0, &pItemNormalSurface );
	ptexItemDown->GetSurfaceLevel( 0, &pItemDownSurface );
	ptexText->GetSurfaceLevel( 0, &pTextSurface );
	
	D3DXLoadSurfaceFromSurface( pItemNormalSurface, NULL, &rtItemNormal,
								pTextSurface, NULL, &rtText,
								D3DX_DEFAULT, 0 );
	
	D3DXLoadSurfaceFromSurface( pItemDownSurface, NULL, &rtItemDown,
								pTextSurface, NULL, &rtText,
								D3DX_DEFAULT, 0 );
	ptexItemNormal->Release();
	ptexItemDown->Release();
	ptexText->Release();
}

VOID GUIListbox::CopyItemToFront( INT _iIndex, LPIMAGE2D _pimg2DItem, LPIMAGE2D _pimg2DFront )
{
	INT iValue = m_pGUIScrollbar->GetValue();

	RECT rtFront;
	RECT rtItem;

	//	지저분하다... 깔끔하게 만들 수 있도록 하자
	rtFront.left	= m_Data.rtRear.left;
	rtFront.top		= ( _iIndex * m_iItemGap - m_Data.rtRear.top < 0 ) ? 0 : _iIndex * m_iItemGap - m_Data.rtRear.top;
	rtFront.right	= m_Data.rtRear.right;
	rtFront.bottom	= ( m_Data.rtRear.bottom - ( _iIndex + 1 ) * m_iItemGap < 0 ) ? _pimg2DFront->rtSource.bottom : ( _iIndex + 1 ) * m_iItemGap - m_Data.rtRear.top;
	
	rtItem.left		= _pimg2DItem->rtSource.left;
	rtItem.top		= ( m_Data.rtRear.top - _iIndex * m_iItemGap ) > 0 ? m_Data.rtRear.top - _iIndex * m_iItemGap : 0;
	rtItem.right	= _pimg2DItem->rtSource.right;
	rtItem.bottom	= ( m_Data.rtRear.bottom - ( _iIndex + 1 ) * m_iItemGap < 0 ) ? _pimg2DItem->rtSource.bottom + m_Data.rtRear.bottom - ( _iIndex + 1 ) * m_iItemGap : _pimg2DItem->rtSource.bottom;

	
	LPDIRECT3DSURFACE9	pFrontSurface	= NULL;
	LPDIRECT3DSURFACE9	pItemSurface	= NULL;
	
	LPDIRECT3DTEXTURE9 ptexFront	= (*_pimg2DFront->vec2Tex[ 0 ])[ 0 ]->pTex;
	LPDIRECT3DTEXTURE9 ptexItem		= (*_pimg2DItem->vec2Tex[ 0 ])[ 0 ]->pTex;

	ptexFront->GetSurfaceLevel( 0, &pFrontSurface );
	ptexItem->GetSurfaceLevel( 0, &pItemSurface );
	
	D3DXLoadSurfaceFromSurface( pFrontSurface, NULL, &rtFront,
								pItemSurface, NULL, &rtItem,
								D3DX_DEFAULT, 0 );
	
	pFrontSurface->Release();
	pItemSurface->Release();
}




//VOID GUIListbox::Create( GUIBase::IMAGEPARAM& _imgFront, GUIBase::IMAGEPARAM& _imgBack )
//{
//	m_Data.fX		= _imgFront.fX;
//	m_Data.fY		= _imgFront.fY;
//	m_Data.fWidth	= _imgFront.fWidth;
//	m_Data.fHeight	= _imgFront.fHeight;
//
//	INT iWidth				= static_cast<INT>( _imgFront.fWidth );
//	INT iHeight				= static_cast<INT>( _imgFront.fHeight );
//	INT iRatioBackToFront	= static_cast<INT>( m_Data.fRatioBackToFront );
//
//	SetRect( &m_Data.rtFront,	0,
//								0,
//								iWidth,
//								iHeight );
//
//	SetRect( &m_Data.rtBack,	0,
//								0,
//								iWidth,
//								iHeight * iRatioBackToFront );
//
//	CreateImage( m_Data.imgFront, _imgFront );
//	CreateImage( m_Data.imgBack, _imgBack );
//
//	m_Data.fRatioBackToFront = _imgBack.fHeight / _imgFront.fHeight;
//
//	//	Test Scrollbar
//	//	외부에서 만들어 포인터로 연결만 시킬까??
//	GUIBase::IMAGEPARAM	imgThumb, imgBack;
//
//	imgThumb.dPivotType	= GUIBase::GBS_TOPLEFT;
//	imgThumb.fX			= _imgFront.fX + _imgFront.fWidth;
//	imgThumb.fY			= _imgFront.fY;
//	imgThumb.fWidth		= 20.0f;
//	imgThumb.fHeight	= 20.0f;
//
//	imgBack.dPivotType	= GUIBase::GBS_TOPLEFT;
//	imgBack.fX			= _imgFront.fX + _imgFront.fWidth;
//	imgBack.fY			= _imgFront.fY;
//	imgBack.fWidth		= 20.0f;
//	imgBack.fHeight		= _imgFront.fHeight;
//	
//	AddFileName( 0, imgThumb, L"Image\\Brick.png" );
//	AddFileName( 0, imgBack, L"Image\\Brick.png" );
//
//	m_pGUIScrollbar->Create( imgThumb, imgBack );
//
//}

VOID GUIListbox::Create( FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, IMAGEPARAM& _imgBackground )
{
	//	Set Font X, Y
	m_iFontX	= m_iFontWidth / 2;
	m_iFontY	= ( m_iItemGap - m_iFontHeight ) / 2;

	//	Init Info
	m_fX		= _fX;
	m_fY		= _fY;
	m_fWidth	= _fWidth;
	m_fHeight	= _fHeight;

	//	Init Image
	GUIBase::IMAGEPARAM	img2DFront;
	
	AddFileName( 0, img2DFront, L"Img\\Listbox1.png" );
	CreateImage2D( m_Data.img2DFront, _fX, _fY, _fWidth, _fHeight, img2DFront );
	CreateImage3D( m_Data.img3DBackground, _fX, _fY, _fWidth, _fHeight, _imgBackground );

	//	Test Scrollbar
	//	외부에서 만들어 포인터로 연결만 시킬까??
	GUIBase::IMAGEPARAM	imgThumb, imgBack;

	imgThumb.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgThumb.fX			= _fX + _fWidth;
	imgThumb.fY			= _fY + 20;
	imgThumb.fWidth		= 20.0f;
	imgThumb.fHeight	= 20.0f;

	imgBack.dPivotType	= GUIBase::GBS_TOPLEFT;
	imgBack.fX			= _fX + _fWidth;
	imgBack.fY			= _fY + 20.0f;
	imgBack.fWidth		= 20.0f;
	imgBack.fHeight		= _fHeight - 40.0f;
	
	AddFileName( 0, imgThumb, L"Img\\Scrollbar_Thumb.png" );
	AddFileName( 0, imgBack, L"Img\\Scrollbar_Back.png" );

	m_pGUIScrollbar->Create( imgThumb, imgBack );

	//	Create Button
	GUIBase::IMAGEPARAM imgParamUpNormal, imgParamUpDown, imgParamUpHot, imgParamUpDisable;

	FLOAT fX			= _fX + _fWidth;
	FLOAT fY			= _fY;
	FLOAT fWidth		= 20.0f;
	FLOAT fHeight		= 20.0f;

	AddFileName( 0, imgParamUpNormal, L"Img\\arrow-up-Normal.png" );
	AddFileName( 0, imgParamUpHot, L"Img\\arrow-up-Hot.png" );
	AddFileName( 0, imgParamUpDown, L"Img\\arrow-up-Down.png" );
	AddFileName( 0, imgParamUpDisable, L"Img\\arrow-up-Disable.png" );

	m_pGUIBtnManager->Create( GLB_BTN_UP, 0, fX, fY, fWidth, fHeight, imgParamUpNormal, imgParamUpHot, imgParamUpDown, imgParamUpDisable );

	GUIBase::IMAGEPARAM imgParamDownNormal, imgParamDownDown, imgParamDownHot, imgParamDownDisable;

	fX			= _fX + _fWidth;
	fY			= _fY + _fHeight - 20.0f;
	fWidth		= 20.0f;
	fHeight		= 20.0f;

	AddFileName( 0, imgParamDownNormal, L"Img\\arrow-down-Normal.png" );
	AddFileName( 0, imgParamDownHot, L"Img\\arrow-down-Hot.png" );
	AddFileName( 0, imgParamDownDown, L"Img\\arrow-down-Down.png" );
	AddFileName( 0, imgParamDownDisable, L"Img\\arrow-down-Disable.png" );

	m_pGUIBtnManager->Create( GLB_BTN_DOWN, 0, fX, fY, fWidth, fHeight, imgParamDownNormal, imgParamDownHot, imgParamDownDown, imgParamDownDisable );
}

VOID GUIListbox::AddItem( LPWSTR _Text, LPWSTR _TextureFileName )
{
	INT		iNumItem		= m_Data.lstItem.size();
	FLOAT	fNumItem		= static_cast<FLOAT>( m_Data.lstItem.size() );
	FLOAT	fItemGap		= static_cast<FLOAT>( m_iItemGap );
	
	FLOAT fTextX	= 0.0f,	fTextY		= 0.0f,	fTextWidth		= 0.0f,	fTextHeight		= 0.0f;
	FLOAT fTextureX = 0.0f,	fTextureY	= 0.0f,	fTextureWidth	= 0.0f,	fTextureHeight	= 0.0f;
	
	//	Create Item
	LPITEM pItem	= new ITEM;

	IMAGEPARAM imgParamItemNormal, imgParamItemDown;
	
	AddFileName( 0, imgParamItemNormal, L"Img\\Listbox1.png" );
	AddFileName( 0, imgParamItemDown, L"Img\\Listbox1.png" );

	pItem->pimg2DNormal = new IMAGE2D;
	pItem->pimg2DDown	= new IMAGE2D;
	
	CreateImage2D( *pItem->pimg2DNormal, 0.0f, 0.0f, m_fWidth, fItemGap, imgParamItemNormal );
	CreateImage2D( *pItem->pimg2DDown, 0.0f, 0.0f, m_fWidth, fItemGap, imgParamItemDown );
	
	//	Create TextureImage
	if( _TextureFileName != NULL )
	{
		GUIBase::IMAGEPARAM imgParamTexture;

		fTextureX		= m_fX;
		fTextureY		= m_fY + ( fItemGap * fNumItem );
		fTextureWidth	= fItemGap;
		fTextureHeight	= fItemGap;

		AddFileName( 0, imgParamTexture, _TextureFileName );
	
		LPIMAGE2D pimg2DTexture = new IMAGE2D;
		CreateImage2D( *pimg2DTexture, fTextureX, fTextureY, fTextureWidth, fTextureHeight, imgParamTexture );
	
		CopyTextureToItem( iNumItem, pimg2DTexture, pItem->pimg2DNormal, pItem->pimg2DDown );

		delete pimg2DTexture;
	}

	//	Create TextImage
	GUIBase::IMAGEPARAM imgParamText;

	fTextX		= m_fX + fTextureWidth;
	fTextY		= m_fY + ( fItemGap * fNumItem );
	fTextWidth	= m_fWidth - fTextureWidth;
	fTextHeight	= fItemGap;

	LPIMAGE2D pimg2DText = new IMAGE2D;
	AddFileName( 0, imgParamText, L"Img\\Listbox1.png" );
	CreateImage2D( *pimg2DText, fTextX, fTextY, fTextWidth, fTextHeight, imgParamText ); 
	
	GUIFont::GetInstance().Create( m_aFaceName, m_iFontWidth, m_iFontHeight, m_pd3dDevice );
	GUIFont::GetInstance().DrawOnTexture(	_Text, m_dFontColor, (*pimg2DText->vec2Tex[ 0 ])[ 0 ]->pTex, 
											m_iFontX,
											m_iFontY,
											static_cast<INT>( fTextWidth ), 
											static_cast<INT>( fTextHeight ) );

	CopyTextToItem( iNumItem, pimg2DText, pItem->pimg2DNormal, pItem->pimg2DDown );
	delete pimg2DText;
	
	//	Add Item List
	INT iItemSize = m_Data.lstItem.size();

	if( m_iMaxItem < iItemSize )
	{
		ITEMLIST::iterator itE = m_Data.lstItem.begin();
		delete (*itE);
		m_Data.lstItem.pop_front();
	}
	m_Data.lstItem.push_back( pItem );
	

	//	Activate Scrollbar
	//	push_back을 했으니 갱신한다
	iItemSize = m_Data.lstItem.size();
	INT iHeight = static_cast<INT>( m_fHeight );
	
	INT iMaxScrollbarValue = m_iItemGap * iItemSize - iHeight;
	if( iMaxScrollbarValue < 0 )
	{
		m_bActScrollbar = FALSE;
		iMaxScrollbarValue = 0;
	}
	else
	{
		m_pGUIScrollbar->SetValue( iMaxScrollbarValue );
		m_bActScrollbar = TRUE;
	}

	m_pGUIScrollbar->SetRange( 0, iMaxScrollbarValue );
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

VOID GUIListbox::Update( INT _iX, INT _iY )
{
	// Update Rear
	INT iValue = m_pGUIScrollbar->GetValue();
	SetRect(	&m_Data.rtRear,	m_Data.img2DFront.rtSource.left,
								m_Data.img2DFront.rtSource.top		+ iValue * m_iScrollSpeed,
								m_Data.img2DFront.rtSource.right,
								m_Data.img2DFront.rtSource.bottom	+ iValue * m_iScrollSpeed );
	
	INT iStart, iEnd, iItemSize;
	iStart		= m_Data.rtRear.top / m_iItemGap;
	iEnd		= m_Data.rtRear.bottom / m_iItemGap + 1;
	iItemSize	= m_Data.lstItem.size();

	if( iEnd > iItemSize )
		iEnd = iItemSize;

	ITEMLIST::iterator itE;
	INT i = 0;
	for( itE = m_Data.lstItem.begin() ; itE != m_Data.lstItem.end() ; itE++ )
	{
		if( iStart <= i && i < iEnd )
			CopyItemToFront( i , (*itE)->pimg2DDown, &m_Data.img2DFront );
		i++;
	}
	
	if( m_bActScrollbar )
	{
		m_pGUIScrollbar->Update();
		m_pGUIBtnManager->Update( _iX, _iY );

		INT iValue = m_pGUIScrollbar->GetValue();

		if( m_pGUIBtnManager->GetState( GLB_BTN_UP ) == 2 )
			iValue--;
		if( m_pGUIBtnManager->GetState( GLB_BTN_DOWN ) == 2 )
			iValue++;

		if( 0 > iValue )
			iValue = 0;
		else if( iValue > m_pGUIScrollbar->GetMaxRange() )
				iValue = m_pGUIScrollbar->GetMaxRange();

		m_pGUIScrollbar->SetValue( iValue );
	}
	
}

VOID GUIListbox::Render()
{
	RenderImage3D( &m_Data.img3DBackground );
	RenderImage2D( &m_Data.img2DFront );	

	if( m_bActScrollbar )
	{
		m_pGUIScrollbar->Render();
		m_pGUIBtnManager->Render();
	}
}

VOID GUIListbox::OnDown( INT x, INT y )
{
	if( m_bActScrollbar )
	{
		m_pGUIBtnManager->OnDown( x, y );
		m_pGUIScrollbar->OnDown( x, y );
	}
}

VOID GUIListbox::OnMove( INT x, INT y )
{
	if( m_bActScrollbar )
	{
		m_pGUIBtnManager->OnMove( x, y );
		m_pGUIScrollbar->OnMove( x, y );
	}
}

VOID GUIListbox::OnUp( INT x, INT y )
{
	if( m_bActScrollbar )
	{
		m_pGUIBtnManager->OnUp( x, y );
		m_pGUIScrollbar->OnUp( x, y );
	}
}

VOID GUIListbox::SetFont(LPWSTR _pFaceName, INT _iWidth, INT _iHeight, DWORD _dColor )
{
	_tcscpy_s( m_aFaceName, _pFaceName );
	m_iFontWidth	= _iWidth;
	m_iFontHeight	= _iHeight;
	m_dFontColor	= _dColor;
}
