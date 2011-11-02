#ifndef _DebugConsole_H_
#define _DebugConsole_H_

class CDebugConsole : public CSingleton<CDebugConsole>
{
	friend class CSingleton<CDebugConsole>;

private:
	HANDLE		m_hOut;
	HWND		m_hWnd;

public:
	enum { CSL_TR = 0, CSL_TL, CSL_BR, CSL_BL };	//	Edge

private:
	CDebugConsole()
	{
		this->Initialize();
	}
	~CDebugConsole()
	{
		this->Release();
	}

	VOID		Initialize();
	VOID		Release();

	VOID		GetConsoleHwnd( HWND& _hWnd );
	VOID		GetEdgePosition( DWORD _Edge );

public:
	//	Console::m_Console().Message( " " );
	VOID	Message( LPWSTR str );
	VOID	Message( LPSTR str );

	//	Console::m_Console().Messagef( "%s %d %f", char, int, float );
	VOID	Messagef( LPWSTR str, ... );
	VOID	Messagef( LPSTR str, ... );

	//	모니터가 수평 배치가 아닌 수직 배치일때는 지원하지 않음
	VOID	SetPosition( DWORD _dEdge, BOOL _bTopMost );

};

#endif