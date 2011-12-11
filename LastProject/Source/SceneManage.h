#ifndef _SCENEMANAGE_H_
#define _SCENEMANAGE_H_

class IScene;
class LoadingScene;

class CSceneManage : public CSingleton<CSceneManage>
{
	friend class CSingleton<CSceneManage>;
public:
	CSceneManage();
	virtual ~CSceneManage();

	//BOOL OrderChangeScene( IScene* a_pScene );

	VOID		Clear();
	HRESULT		Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT		Release();
	VOID		Update();
	VOID		Render();

	INT			GetCurrentScene()	{ return m_scnCurrentState; }

private:

	static UINT WINAPI ThreadFunc(LPVOID lParam);
	
	//VOID				ChangeScene();
	VOID				ChangeScene( INT _scnNext );

	UINT				m_uiThreadID;	///< 쓰레드
	HANDLE				m_hThread;		///< 쓰레드

	LPDIRECT3DDEVICE9	m_pD3dDevice;	///< d3d9 디바이스
	LPD3DXSPRITE		m_pSprite;
	HWND				m_hWnd;
	
	//IScene*				m_pLoadScene;
	IScene*				m_pScene;
	IScene*				m_pNextScene;
	LoadingScene*		m_pLoadingScene;
	//IScene*				m_pNextScene;
	//IScene*				m_pPrevScene;
	INT					m_scnLodingState;
	INT					m_scnCurrentState;

};

#endif