#include "stdafx.h"

#include "SocketServer.h"
#include "UI_Manager.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif


VOID Draw_Menu()
{
	cout << "======================================================================" << endl;
	cout << "CLIENT_NUMBER      ID      STATE      HOST      PING      BUFFER_STATE" << endl;
	cout << "======================================================================" << endl;
}


INT main( INT argc, CHAR* argv[] )
{
#ifdef _DEBUG
	// 메모릭 릭 탐지
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	CSocketServer SocketServer;
	SocketServer.Initialize();
	SocketServer.StartServer( 20202 );
/*
	Draw_Menu();

	CUI_Manager UIManager;
	UIManager.Initialize();
*/
	while( 1 )
	{
		SocketServer.UpdateFrame();
/*	
		if( GetKeyState( 'Q' ) & 0x8000 )
		{
			break;
		}
*/
		Sleep( 1 );
	}

	return 0;
}