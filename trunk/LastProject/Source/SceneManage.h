#ifndef _SCENEMANAGE_H_
#define _SCENEMANAGE_H_

class IScene;

class CSceneManage : public CSingleton<CSceneManage>
{
	friend class CSingleton<CSceneManage>;
public:
	CSceneManage();
	~CSceneManage();

	BOOL OrderChangeScene( IScene* a_pScene );

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();
	VOID Update();
	VOID Render();

private:

	static UINT WINAPI ThreadFunc(LPVOID lParam);
	VOID ChangeScene();

	UINT m_uiThreadID;	 ///< ������
	HANDLE m_hThread;	 ///< ������

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 ����̽�
	
	IScene* m_pLoadScene;
	IScene* m_pScene;
	IScene* m_pNextScene;
	IScene* m_pPrevScene;

};

#endif