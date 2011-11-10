#ifndef _SCENEMANAGE_H_
#define _SCENEMANAGE_H_

class CSceneManage : public CSingleton<CSceneManage>
{
	friend class CSingleton<CSceneManage>;
public:
	CSceneManage();
	~CSceneManage();

	VOID NextScene( INT a_iScene );
	VOID PrevScene( INT a_iScene );

	BOOL ChangeScene( CManage* a_pScene );

	VOID Clear();
	HRESULT Create();
	HRESULT Release();
	VOID Update();
	VOID Render();
	
private:

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	
	CManage* m_pScene;
	CManage* m_pPrevScene;

};

#endif