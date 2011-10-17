/**
	@file	WinBase.h
	@date	2011/09/19
	@author �����
	@brief	������ ���̽� Ŭ����
*/

#ifndef _WinBase_H_
#define _WinBase_H_

class CNetwork;

/**
	@class	CWinBase
	@date	2011/09/19
	@author �����
	@brief	������ ���̽� Ŭ����
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

	static INT MessageLoop(HINSTANCE hInstance = NULL);		///< �޽���ť
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
	LPCTSTR m_lpszTitle; ///< Ÿ��Ʋ��
	HWND m_hWnd;		 ///< �ڵ�
	RECT m_WinRect;		 ///< ���� ũ��
	BOOL m_bReSizing;	 ///< ������¡
	BOOL m_bFirstRun;	 ///< ó�� ����
	DWORD m_dwThreadID;	 ///< ������
	HANDLE m_hThread;	 ///< ������

	//��Ʈ��ũ
	WSADATA m_wsadata;
	CNetwork* m_pNetwork;

private:
	static VOID Update();
	static VOID Render();

	static DWORD WINAPI ThreadFunc(LPVOID pTemp);
	ATOM MyRegisterClass(HINSTANCE hInstance);	///< ������ Ŭ���� ���
	BOOL InitInstance(HINSTANCE, INT);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

#endif