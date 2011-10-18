#ifndef _DebugConsole_H_
#define _DebugConsole_H_

class CDebugConsole : public CSingleton<CDebugConsole>
{
	friend class CSingleton<CDebugConsole>;

private:
	HANDLE		m_hOut;

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

public:
		//CDebugConsole::GetInstance()->Message( " " );
	VOID	Messagef( LPWSTR str, ... );
	VOID	Messagef( LPSTR str, ... );
	VOID	Message( LPWSTR str );
	VOID	Message( LPSTR str );

};

#endif