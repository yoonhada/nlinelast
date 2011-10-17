/**
	@file	WinBase.h
	@date	2011/09/19
	@author 백경훈
	@brief	윈도우 베이스 클래스
*/

#ifndef _WinBase_H_
#define _WinBase_H_

class CNetwork;

/**
	@class	CWinBase
	@date	2011/09/19
	@author 백경훈
	@brief	윈도우 베이스 클래스
*/

class CWinBase : public CSingleton<CWinBase>
{
	friend class CSingleton<CWinBase>;

public:
	CWinBase() 
	{
		Create();
	}
	~CWinBase()
	{
		Release();
	}

	HRESULT Create();
	HRESULT Release();

	static INT MessageLoop(HINSTANCE hInstance = NULL);		///< 메시지큐
	const HWND& Get_hWnd()
	{
		return m_hWnd;
	}
	const RECT& Get_WinRect()
	{
		return m_WinRect;
	}

private:
	CDirectX9* m_pDX9;
	CMainManage* m_pManage;

private:
	LPCTSTR m_lpszTitle; ///< 타이틀명
	HWND m_hWnd;		 ///< 핸들
	RECT m_WinRect;		 ///< 윈도 크기
	BOOL m_bReSizing;	 ///< 리사이징
	BOOL m_bFirstRun;	 ///< 처음 실행
	DWORD m_dwThreadID;	 ///< 쓰레드
	HANDLE m_hThread;	 ///< 쓰레드

	//네트워크
	WSADATA m_wsadata;
	CNetwork* m_pNetwork;

private:
	static VOID Update();
	static VOID Render();

	static DWORD WINAPI ThreadFunc(LPVOID pTemp);
	ATOM MyRegisterClass(HINSTANCE hInstance);	///< 윈도우 클래스 등록
	BOOL InitInstance(HINSTANCE, INT);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

#endif