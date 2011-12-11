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
	virtual ~CDebugConsole()
	{
		this->Release();
	}

	VOID		Initialize();
	VOID		Release();

	VOID		GetConsoleHwnd( HWND& _hWnd );
	VOID		GetEdgePosition( DWORD _Edge );

public:
	//	CDebugConsole::m_Console().Message( " " );
	VOID	Message( LPWSTR str );
	VOID	Message( LPSTR str );

	//	CDebugConsole::m_Console().Messagef( "%s %d %f", char, int, float );
	VOID	Messagef( LPWSTR str, ... );
	VOID	Messagef( LPSTR str, ... );

	//	����Ͱ� ���� ��ġ�� �ƴ� ���� ��ġ�϶��� �������� ����
	VOID	SetPosition( DWORD _dEdge, BOOL _bTopMost );

	VOID	MessageQube(D3DXVECTOR3 *_vec);
};

#endif