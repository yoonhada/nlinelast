#include "stdafx.h"
#include "GUIScrollbar.h"

VOID GUIScrollbar::Initialize()
{
	//	Init Style
	m_dStyle		= SCL_VERT;

	//	Init Range
	m_Data.iMin		= 0;
	m_Data.iMax		= 100;

	//	Init iValue
	m_Data.iValue	= 0;

	//	Init keyValue
	m_bLKeyDown		=	FALSE;
	m_bLKeyUp		=	TRUE;
	m_bLKeyDrag		=	FALSE;
}

VOID GUIScrollbar::Release()
{
}

VOID GUIScrollbar::Create( IMAGEPARAM& _imgThumb, IMAGEPARAM& _imgBack )
{
	//	Set Rect
	SetRect( &m_Data.rtThumb, 0, 0, static_cast< INT >( _imgThumb.fWidth ), static_cast< INT >( _imgThumb.fHeight ) );
	SetRect( &m_Data.rtBack, 0, 0, static_cast< INT >( _imgBack.fWidth ), static_cast< INT >( _imgBack.fHeight ) );

	//	Set Back Position
	m_Data.ptBackPosition.x = static_cast< INT >( _imgBack.fX );
	m_Data.ptBackPosition.y = static_cast< INT >( _imgBack.fY );

	//	Set TGap, TSize
	SetStyle( m_dStyle );

	//	Create Image
	CreateImage3D( m_Data.img3DThumb, _imgThumb );
	CreateImage3D( m_Data.img3DBack, _imgBack );
}

VOID GUIScrollbar::SetStyle( DWORD _dStyle )
{
	m_dStyle	=	_dStyle;

	switch( m_dStyle )
	{
	case SCL_HROZ:
		m_Data.iTSize	=	m_Data.rtThumb.right;
		m_Data.iTGap	=	m_Data.rtBack.bottom - m_Data.rtThumb.bottom;
		break;
	case SCL_VERT:
		m_Data.iTSize	=	m_Data.rtThumb.bottom;
		m_Data.iTGap	=	m_Data.rtBack.right - m_Data.rtThumb.right;
		break;
	}
}
	
VOID GUIScrollbar::GetThumbRect( RECT *trt )
{
	INT	x, y;
	
	switch( m_dStyle )
	{
	case SCL_HROZ:
		x = MulDiv(	m_Data.iValue, 
					m_Data.rtBack.right - m_Data.iTSize, 
					m_Data.iMax - m_Data.iMin );
		
		SetRect(	trt, 
					x, 
					m_Data.rtBack.top + m_Data.iTGap,
					x + m_Data.iTSize, 
					m_Data.rtBack.bottom - m_Data.iTGap );
		break;
	case SCL_VERT:
		y = MulDiv(	m_Data.iValue, 
					m_Data.rtBack.bottom - m_Data.iTSize,		
					m_Data.iMax - m_Data.iMin );
		SetRect(	trt, 
					m_Data.rtBack.left + m_Data.iTGap, 
					y,
					m_Data.rtBack.right - m_Data.iTGap, 
					y + m_Data.iTSize );
		break;
	}

}
VOID GUIScrollbar::PosFromPixel( INT _iPixel )
{
	INT iWidth;

	switch( m_dStyle )
	{
	case SCL_HROZ:
		iWidth	=	m_Data.rtBack.right - m_Data.iTSize;
		break;
	case SCL_VERT:
		iWidth	=	m_Data.rtBack.bottom - m_Data.iTSize;
		break;
	}

	m_Data.iValue	=	MulDiv( _iPixel, m_Data.iMax - m_Data.iMin, iWidth );
	m_Data.iValue	=	max( m_Data.iMin, min( m_Data.iMax, m_Data.iValue ) );

}

VOID GUIScrollbar::SetRange( INT _iMin, INT _iMax )
{
	m_Data.iMin	=	_iMin;
	m_Data.iMax	=	_iMax;
}


VOID GUIScrollbar::Render()
{
	RenderImage3D( &m_Data.img3DBack );
	RenderImage3D( &m_Data.img3DThumb );
}

VOID GUIScrollbar::Update()
{
	RECT trt;
	GetThumbRect( &trt );
	Image3DTranslate(	&m_Data.img3DThumb, 
						static_cast< FLOAT >( m_Data.ptBackPosition.x + trt.left + m_Data.img3DThumb.vecScale.x * 0.5f ),
						static_cast< FLOAT >( m_Data.ptBackPosition.y + trt.top + m_Data.img3DThumb.vecScale.y * 0.5f ),
						0.0f );
	
	m_bLKeyUp = TRUE;
}

VOID GUIScrollbar::OnDown( INT x, INT y )
{
	RECT rt;
	SetRect(	&rt, 
				m_Data.ptBackPosition.x, 
				m_Data.ptBackPosition.y,
				m_Data.ptBackPosition.x + m_Data.rtBack.right, 
				m_Data.ptBackPosition.y + m_Data.rtBack.bottom );

	POINT pt;
	pt.x = x;	
	pt.y = y;

	if( PtInRect( &rt, pt ) )
	{
		pt.x	=	pt.x - rt.left;
		pt.y	=	pt.y - rt.top;

		RECT trt;
		GetThumbRect( &trt );
		if( PtInRect( &trt, pt ) & !m_bLKeyDrag )
		{
			switch( m_dStyle )
			{
			case SCL_HROZ:
				m_Data.iOffset	=	trt.left - pt.x;
				break;
			case SCL_VERT:
				m_Data.iOffset	=	trt.top - pt.y;
				break;
			}
			m_bLKeyDrag = TRUE;
		}
		else
		{
			switch( m_dStyle )
			{
			case SCL_HROZ:
				PosFromPixel( pt.x - m_Data.iTSize / 2 );
				break;
			case SCL_VERT:
				PosFromPixel( pt.y - m_Data.iTSize / 2 );
				break;
			}
		}
	}
}

VOID GUIScrollbar::OnMove( INT x, INT y )
{
	if( m_bLKeyDrag == TRUE )
	{
		x	=	x - m_Data.ptBackPosition.x;
		y	=	y - m_Data.ptBackPosition.y;

		switch( m_dStyle )
		{
		case SCL_HROZ:
			PosFromPixel( x +  m_Data.iOffset );
			break;
		case SCL_VERT:
			PosFromPixel( y +  m_Data.iOffset );
			break;
		}
	}
}

VOID GUIScrollbar::OnUp( INT x, INT y )
{
	if( m_bLKeyUp & m_bLKeyDrag )
	{
		m_bLKeyDrag	=	FALSE;
		m_bLKeyDown	=	FALSE;
	}
}
