#include "stdafx.h"
#include "GUIBtn.h"

BOOL	GUIBtn::g_bCapture	=	FALSE;
DWORD	GUIBtn::btnMessage	=	0;

VOID GUIBtn::Initialize()
{
	m_ID			=	0;
	m_Style			=	GBS_PUSH;

	m_State			=	GB_NORMAL;
	m_CheckState	=	GB_NORMAL;
	
	m_bCapture		=	FALSE;
}

VOID GUIBtn::Release()
{
}

//VOID GUIBtn::Create( DWORD _ID, DWORD _Style, FLOAT _fX, FLOAT _fY, FLOAT _fWidth, FLOAT _fHeight, 
//					LPCWSTR _Normal, LPCWSTR _Hot, LPCWSTR _Down, LPCWSTR _Disable )
//{
//	//	Init ID
//	m_ID	=	_ID;
//
//	//	Init Style
//	m_Style		=	_Style;
//
//	//	Init Position
//	m_Data.vecPosition.x = _fX;	
//	m_Data.vecPosition.y = _fY;
//
//	//	Init Source
//	m_Data.fWidth	= _fWidth;
//	m_Data.fHeight	= _fHeight;
//	
//	//	Init Image
//	CreateImage(	m_Data.Image[ GB_NORMAL ],
//					GUIBase::GBS_TOPLEFT,
//					_fX, _fY,
//					_fWidth, _fHeight,
//					_Normal );
//
//	CreateImage(	m_Data.Image[ GB_HOT ],
//					GUIBase::GBS_TOPLEFT,
//					_fX, _fY,
//					_fWidth, _fHeight,
//					_Hot );
//
//	CreateImage(	m_Data.Image[ GB_DOWN ],
//					GUIBase::GBS_TOPLEFT,
//					_fX, _fY,
//					_fWidth, _fHeight,
//					_Down );
//
//	CreateImage(	m_Data.Image[ GB_DISABLE ],
//					GUIBase::GBS_TOPLEFT,
//					_fX, _fY,
//					_fWidth, _fHeight,
//					_Disable );
//}

VOID GUIBtn::Create( DWORD _dID, DWORD _dStyle, IMAGEPARAM& _imgNormal, IMAGEPARAM& _imgHot, IMAGEPARAM& _imgDown, IMAGEPARAM& _imgDisable )
{
	//	Init ID
	m_ID		=	_dID;

	//	Init Style
	m_Style		=	_dStyle;

	//	Init Position
	m_Data.vecPosition.x	= _imgNormal.fX;	
	m_Data.vecPosition.y	= _imgNormal.fY;

	//	Init Source
	m_Data.fWidth	= _imgNormal.fWidth;
	m_Data.fHeight	= _imgNormal.fHeight;
	
	//	Init Image
	CreateImage(	m_Data.Image[ GB_NORMAL ], _imgNormal );
					
	CreateImage(	m_Data.Image[ GB_HOT ], _imgHot );

	CreateImage(	m_Data.Image[ GB_DOWN ], _imgDown );

	CreateImage(	m_Data.Image[ GB_DISABLE ], _imgDisable );
}


VOID GUIBtn::Render()
{
	if( m_State == GB_HIDDEN ) return;

	RenderImage( m_Data.Image[ m_State ] );
}

VOID GUIBtn::SetState( eState _State )
{
	if ( ( m_Style & GBS_CHECK ) != 0 ) {	
		ChangeState( _State );
	}
}

BOOL GUIBtn::IsPtOnMe( POINT pt )
{
	RECT rt;

	//	INT형으로 영역 체크를 하지 말고 FLOAT형으로 하고 싶은데
	//	영역 체크는 모든 UI가 다 써야 되니깐 GUIBase에 넣는것도 좋을지도
	//	일단 그대로 쓴다
	SetRect(	&rt, 
				static_cast< INT >( m_Data.vecPosition.x ),  
				static_cast< INT >( m_Data.vecPosition.y ), 
				static_cast< INT >( m_Data.vecPosition.x + m_Data.fWidth ),
				static_cast< INT >( m_Data.vecPosition.y + m_Data.fHeight ) );

	return ( PtInRect( &rt, pt ) );
}

BOOL GUIBtn::IsPtOnMe( INT x,INT y )
{
	POINT pt = { x, y };

	return IsPtOnMe( pt );
}

VOID GUIBtn::ChangeState(eState _State)
{
	if( m_State == _State) return;

	m_State = _State;
}


VOID GUIBtn::Update( INT x, INT y )
{
	if( m_State == GB_HIDDEN || m_State == GB_DISABLE ) return;

	if( IsPtOnMe( x, y ) == FALSE && m_Style == GBS_PUSH )
		ChangeState( GB_NORMAL );
}

VOID GUIBtn::OnDown( INT x, INT y )	//	마우스커서 위치
{
	if( m_State == GB_HIDDEN || m_State == GB_DISABLE ) return;
	if( IsPtOnMe( x, y ) ) 
	{
		if( ( m_Style & GBS_CHECK ) == 0 ) 
		{
			ChangeState( GB_DOWN );
		} 
		else 
		{
			m_CheckState=m_State;
			ChangeState( m_CheckState == GB_NORMAL ? GB_DOWN : GB_NORMAL );
		}
		m_bCapture = TRUE;
		g_bCapture = TRUE;
	}
}

VOID GUIBtn::OnMove( INT x, INT y )
{
	if( m_State == GB_HIDDEN || m_State == GB_DISABLE ) return;

	if( m_bCapture )
	{
		if( ( m_Style & GBS_CHECK ) == 0 ) 
		{
			if( IsPtOnMe( x, y ) ) 
				ChangeState(GB_DOWN);
			else 
				ChangeState(GB_NORMAL);
		} 
		else 
		{
			if( IsPtOnMe( x, y ) )
				ChangeState( m_CheckState == GB_NORMAL ? GB_DOWN : GB_NORMAL );
			else
				ChangeState( m_CheckState );
		}
	} 
	else 
	{
		// 누군가가 캡처를 하고 있는 상황에서는 핫 상태로 가지 말아야 한다.
		if( !g_bCapture && ( m_Style & GBS_CHECK ) == 0 && IsPtOnMe( x, y ) )
			ChangeState(GB_HOT);
	}
	
}

VOID GUIBtn::OnUp( INT x, INT y )
{
	if( m_bCapture ) 
	{
		m_bCapture = FALSE;
		g_bCapture = FALSE;

		if( ( m_Style & GBS_CHECK ) == 0 ) 
			ChangeState(GB_NORMAL);

		if( IsPtOnMe( x, y ) )
			btnMessage = m_ID;
	}
}

VOID GUIBtn::Enable( BOOL bEnable )
{
	if( bEnable )
		m_State=GB_NORMAL;
	else
		m_State=GB_DISABLE;
}

VOID GUIBtn::Show( BOOL bShow )
{
	if( bShow ) 
		m_State=GB_NORMAL;
	else 
		m_State=GB_HIDDEN;
}

VOID GUIBtn::Move(INT _x,INT _y)
{
	m_Data.vecPosition.x = (FLOAT)_x;
	m_Data.vecPosition.y = (FLOAT)_y;
}
