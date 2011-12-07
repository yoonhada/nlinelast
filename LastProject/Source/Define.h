#ifndef _DEFINE_H_
#define _DEFINE_H_

#define _24CUBE
//#define _8CUBE
//#define _CELLCUBE

// define
#define GHWND ( CWinBase::GetInstance()->Get_hWnd() )
#define GWINRECT ( CWinBase::GetInstance()->Get_WinRect() )

// 윈도우 크기
#define WINDOW_X 1024
#define WINDOW_Y 768


// 안전한 메모리 해제를 위한 매크로
#define SAFE_DELETE( p )			{ if ( p ) { delete	( p );		( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p )		{ if ( p ) { delete[] ( p );	( p ) = NULL; } }

#define SAFE_RELEASE( p )			{ if ( p ) { ( p )->Release();	( p ) = NULL; } }

#define ABSDEF( x )					( ( ( x ) > 0 ) ? ( x ) : - ( x ) )	

#define FULLSCREEN FALSE

extern VOID FastRand2Init();
extern FLOAT FastRand2();

// 네트워크
#define BUF_SIZE	4048
#define PACKET_SIZE 2024
#define NETWORK_RECV_TIME 0.25f

// File
#define WEAPONFILE L".\\WeaponFile.ini"

// GUI Resource
// GUI State
enum GUIBTN { UP			= 100,	DOWN, DISABLE, 
			  STORY_SKIP	= 1000, 
			  MENU_START	= 1010,	MENU_END, 
			  OPTION_HELP	= 1020,	OPTION_SETUP, 
			  LOGIN_GO		= 1030,	LOGIN_EXIT, 
			  LOBBY_START	= 1040,	LOBBY_READY, LOBBY_BACK, LOBBY_SELECT_1, LOBBY_SELECT_2, LOBBY_SELECT_3, LOBBY_SELECT_4
};

namespace MTP_FUN
{
	const float RAD_TO_DEG = 180.0f / 3.14159265f;
	const float DEG_TO_RAD = 3.14159265f / 180.0f;

	template <int degrees>
	struct Deg2Rad
	{
		static const float radians;
	};

	template <int degrees>
	const float Deg2Rad<degrees>::radians = degrees * 3.14159265f / 180.0f;
}


//#define GUIBTN_UP			100
//#define GUIBTN_DOWN				101
//#define GUIBTN_DISABLE			102
//
//#define GUIBTN_STORY_SKIP		1000
//
//#define GUIBTN_MENU_START		1010
//#define GUIBTN_MENU_END			1011
//
//#define GUIBTN_OPTION_HELP		1020
//#define GUIBTN_OPTION_SETUP		1021
//
//#define GUIBTN_LOGIN_GO			1030
//#define GUIBTN_LOGIN_EXIT		1031
//
//#define GUIBTN_LOBBY_START		1040
//#define GUIBTN_LOBBY_READY		1041
//#define GUIBTN_LOBBY_BACK		1042
//#define GUIBTN_LOBBY_SELECT_1	1043
//#define GUIBTN_LOBBY_SELECT_2	1044
//#define GUIBTN_LOBBY_SELECT_3	1045
//#define GUIBTN_LOBBY_SELECT_4	1046

#endif