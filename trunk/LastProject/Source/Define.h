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

#define DEG2RAD( x )				( x * ( D3DX_PI / 180.0f ) )			 // Converts degrees to radians
#define RAD2DEG( x )				( x * ( 180.0f / D3DX_PI ) )			 // Converts radians to degrees
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

#endif