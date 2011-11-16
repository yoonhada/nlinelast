/**
	@file	WinBase.cpp
	@date	2011/09/19
	@author �����
	@brief	������ ���̽� Ŭ����
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
	CSceneManage::GetInstance();
	CObjectManage::GetInstance();
	CDebugConsole::GetInstance();
	CDebugConsole::GetInstance()->SetPosition( CDebugConsole::CSL_TR, TRUE );
	CFrequency::GetInstance();
	//CTextureManage::GetInstance();
	CInput::GetInstance();
	CPhysics::GetInstance();
	CTree::GetInstance();

	Astar::GetInstance();

	m_bThreadEndCheck = TRUE;

	//��ǲ ������
	/*m_hThread = (HANDLE)_beginthreadex(NULL, 0, InputThread, NULL, 0, &m_uiThreadID);
	if(m_hThread == 0)
	{
		MessageBox(NULL, L"Create Input Thread Error",NULL, MB_OK);
	}*/

	return S_OK;
}

HRESULT CWinBase::Release()
{
	SAFE_DELETE( m_pDX9 );
	CMatrices::DestoryInstance();
	CSceneManage::DestoryInstance();
	CObjectManage::DestoryInstance();
	CDebugConsole::DestoryInstance();
	CFrequency::DestoryInstance();
	//CTextureManage::DestoryInstance();
	CInput::DestoryInstance();
	CPhysics::DestoryInstance();
	CTree::DestoryInstance();
	CDebugInterface::DestoryInstance();

#ifndef _NETWORK
	WSACleanup();
	CNetwork::DestoryInstance();
#endif

	Astar::DestoryInstance();

	m_bThreadEndCheck = FALSE;

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

	//���� �ػ� ���ϱ�
	DEVMODE DevMode;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode); //���� �ػ� �˻�

	m_WinRect.right = DevMode.dmPelsWidth;
	m_WinRect.bottom = DevMode.dmPelsHeight;

	//D3d9 ����̽� �����
	if( S_OK != m_pDX9->Create( m_hWnd ) )
	{
		MessageBox( m_hWnd, L"Create D3D9 Device Error", NULL, MB_OK );
		return FALSE;
	}

	CMatrices::GetInstance()->Create( m_pDX9->GetDevice(), 
		static_cast<FLOAT>(m_pDX9->GetD3dpp().BackBufferWidth) / static_cast<FLOAT>(m_pDX9->GetD3dpp().BackBufferHeight) );

	CSceneManage::GetInstance()->Create( m_pDX9->GetDevice() );

	CInput::GetInstance()->Create( m_hWnd );

	CDebugInterface::GetInstance()->Create( m_pDX9->GetDevice() );

	return TRUE;
}

INT CWinBase::MessageLoop(HINSTANCE hInstance)
{
	MSG msg;

	ZeroMemory( &msg, sizeof( msg ) );

	GetInstance()->MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!GetInstance()->InitInstance (hInstance, SW_MAXIMIZE)) //�ִ�ȭ�� ����
	{
		return FALSE;
	}

	// �⺻ �޽��� �����Դϴ�.

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
			// ���� �۾��� ���⿡��.. 
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
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);	// ������ �⺻ ������
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= this->m_lpszTitle;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, L"1");

	return RegisterClassEx(&wcex);
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

UINT WINAPI CWinBase::InputThread(LPVOID lParam)
{
	CDebugConsole::GetInstance()->Messagef( L"** Input Thread Start **\n" );
	//��ǲ ������Ʈ
	while (GetInstance()->m_bThreadEndCheck)
	{
		CInput::GetInstance()->Update( 50.0f, 150.0f, CFrequency::GetInstance()->getFrametime() );
		Sleep(1);
	}

	MessageBox( NULL, L"End", NULL, MB_OK );
	return 0;
}

VOID CWinBase::Update()
{
	
	CFrequency::GetInstance()->Update();
	TCHAR buf[256];
	swprintf( buf, 256, TEXT( "%0.4f" ), CFrequency::GetInstance()->getFrequency() );
	SetWindowText( GetInstance()->m_hWnd, buf );

	CInput::GetInstance()->Update( 50.0f, 150.0f, CFrequency::GetInstance()->getFrametime() );
	CSceneManage::GetInstance()->Update();
}

VOID CWinBase::Render()
{
	GetInstance()->m_pDX9->beginScene();

	CSceneManage::GetInstance()->Render();
	
	GetInstance()->m_pDX9->endScene();
}