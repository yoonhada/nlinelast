/**
	@file	WinBase.h
	@date	2011/09/19
	@author �����
	@brief	������ ���̽� Ŭ����
*/

#ifndef _WinBase_H_
#define _WinBase_H_

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

private:
	LPCTSTR m_lpszTitle; ///< Ÿ��Ʋ��
	HWND m_hWnd;		 ///< �ڵ�
	RECT m_WinRect;		 ///< ���� ũ��
	BOOL m_bReSizing;	 ///< ������¡
	BOOL m_bFirstRun;	 ///< ó�� ����

private:
	static VOID Update();
	static VOID Render();

	ATOM MyRegisterClass(HINSTANCE hInstance);	///< ������ Ŭ���� ���
	BOOL InitInstance(HINSTANCE, INT);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

#endif