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

#define FULLSCREEN TRUE

extern VOID FastRand2Init();
extern FLOAT FastRand2();

// 네트워크
#define BUF_SIZE	4048
#define PACKET_SIZE 2024
#define NETWORK_RECV_TIME 0.25f

// File
#define WEAPONFILE L".\\WeaponFile.ini"

// GUI Resource
#define GUIBTN_STORY_SKIP		1000

#define GUIBTN_MENU_START		1001
#define GUIBTN_MENU_END			1002

#define GUIBTN_OPTION_HELP		1010
#define GUIBTN_OPTION_SETUP		1011

#define GUIBTN_LOGIN_GO			1020

#define GUIBTN_LOBBY_START		1030
#define GUIBTN_LOBBY_READY		1031
#define GUIBTN_LOBBY_BACK		1032
#define GUIBTN_LOBBY_SELECT_1	1033
#define GUIBTN_LOBBY_SELECT_2	1034
#define GUIBTN_LOBBY_SELECT_3	1035
#define GUIBTN_LOBBY_SELECT_4	1036

#endif