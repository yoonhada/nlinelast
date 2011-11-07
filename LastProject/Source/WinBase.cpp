/**
	@file	WinBase.cpp
	@date	2011/09/19
	@author 백경훈
	@brief	윈도우 베이스 클래스
*/

#include "stdafx.h"

HRESULT CWinBase::Create()
{
	m_lpszTitle = TEXT("LastProject"); 
	m_bFirstRun = TRUE;
	ZeroMemory( &m_WinRect, sizeof(m_WinRect) );
	m_hWnd = NULL;
	m_WinRect.right = 800;
	m_WinRect.bottom = 600;
	m_WinRect.top = 0;
	m_WinRect.left = 0;

	FastRand2Init();
	
	m_pDX9 = new CDirectX9;
	m_pManage = CMainManage::GetInstance();
	CDebugConsole::GetInstance();
	CDebugConsole::GetInstance()->SetPosition( CDebugConsole::CSL_TR, TRUE );
	CFrequency::GetInstance();
	CTextureManage::GetInstance();
	CInput::GetInstance();
	CPhysics::GetInstance();
	CTree::GetInstance();

	// 네트워크
#ifndef _NETWORK
	WSAStartup( MAKEWORD( 2, 2), &m_wsadata );
	m_pNetwork = new CNetwork;
	m_pNetwork->CreateSocket();
#endif
	
	// 파일에서 IP 받아오기
	FILE* pFile;
	
	pFile = _wfopen( L"Data/IP.txt", L"r" );

	if( NULL == pFile )
	{
		MessageBox(GHWND, L"IP File Load Error", NULL, MB_OK);
		return E_FAIL;
	}

	fseek( pFile, 0L, SEEK_SET );

	CHAR szTemp[255];
	fscanf( pFile, "%s", szTemp );

	fclose(pFile);

#ifndef _NETWORK
	m_pNetwork->ConnectToServer( szTemp, 20202 );
	m_pNetwork->csLOGON();
#endif

	return S_OK;
}

HRESULT CWinBase::Release()
{
	SAFE_DELETE( m_pDX9 );
	CMatrices::DestoryInstance();
	m_pManage->DestoryInstance();
	CDebugConsole::DestoryInstance();
	CFrequency::DestoryInstance();
	CTextureManage::DestoryInstance();
	CInput::DestoryInstance();
	CPhysics::DestoryInstance();
	CTree::DestoryInstance();
	CDebugInterface::DestoryInstance();

#ifndef _NETWORK
	WSACleanup();
	delete m_pNetwork;
#endif

	return S_OK;
}

BOOL CWinBase::InitInstance(HINSTANCE hInstance, INT nCmdShow)
{
	DWORD Style = FULLSCREEN ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&m_WinRect, Style, FALSE);

	/*WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/

	m_hWnd = CreateWindow(this->m_lpszTitle, this->m_lpszTitle, Style, 0, 0, 
		m_WinRect.right - m_WinRect.left, m_WinRect.bottom - m_WinRect.top, NULL, NULL, hInstance, NULL);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	//현재 해상도 구하기
	DEVMODE DevMode;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode); //현재 해상도 검색

	m_WinRect.right = DevMode.dmPelsWidth;
	m_WinRect.bottom = DevMode.dmPelsHeight;

	//D3d9 디바이스 만들기
	if( S_OK != m_pDX9->Create( m_hWnd ) )
	{
		MessageBox( m_hWnd, L"Create D3D9 Device Error", NULL, MB_OK );
		return FALSE;
	}

	CMatrices::GetInstance()->Create( m_pDX9->GetDevice(), 
		static_cast<FLOAT>(m_pDX9->GetD3dpp().BackBufferWidth) / static_cast<FLOAT>(m_pDX9->GetD3dpp().BackBufferHeight) );

	m_pManage->Create( m_pDX9->GetDevice() );

	CInput::GetInstance()->Create( m_pDX9->GetDevice(), m_hWnd );

	CDebugInterface::GetInstance()->Create( m_pDX9->GetDevice() );

	return TRUE;
}

INT CWinBase::MessageLoop(HINSTANCE hInstance)
{
	MSG msg;

	ZeroMemory( &msg, sizeof( msg ) );

	GetInstance()->MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!GetInstance()->InitInstance (hInstance, SW_MAXIMIZE)) //최대화로 실행
	{
		return FALSE;
	}

	// 기본 메시지 루프입니다.

	BOOL bLoop=TRUE;
	while( msg.message != WM_QUIT )
	{

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 실제 작업은 여기에서.. 
			//CInput::GetInstance().Input();
			Update();
			Render();
		}
	}

	return (INT) msg.wParam;
}

ATOM CWinBase::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_CLASSDC|CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)GetInstance()->WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon (NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);	// 윈도우 기본 바탕색
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= this->m_lpszTitle;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, L"1");

	return RegisterClassEx(&wcex);
}


DWORD WINAPI CWinBase::ThreadFunc(LPVOID pTemp)
{
	return 0;
}

LRESULT CALLBACK CWinBase::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

#ifdef _DEBUG
	if( TwEventWin( hWnd, message, wParam, lParam ) )
	{
		return 0;
	}
#endif

	switch (message) 
	{
	case WM_CREATE:
		//GetInstance()->m_hThread = CreateThread( NULL, 0, ThreadFunc, NULL, 0, &GetInstance()->m_dwThreadID );
		//CloseHandle(GetInstance()->m_hThread);
		return 0;

	case WM_CHAR: 
		return 0;

	case WM_SIZE:
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		CWinBase::DestoryInstance();
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	//return 0;
}

VOID CWinBase::Update()
{
	
	CFrequency::GetInstance()->Update();
	TCHAR buf[256];
	swprintf( buf, 256, TEXT( "%0.4f" ), CFrequency::GetInstance()->getFrequency() );
	SetWindowText( GetInstance()->m_hWnd, buf );

#ifndef _NETWORK
	GetInstance()->m_pNetwork->Update();
#endif
	GetInstance()->m_pManage->Update();
}

VOID CWinBase::Render()
{
	GetInstance()->m_pDX9->beginScene();

	GetInstance()->m_pManage->Render();
	
	GetInstance()->m_pDX9->endScene();
}