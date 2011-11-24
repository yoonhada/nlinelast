#ifndef _CScene_H_
#define _CScene_H_

class IScene
{
public:
	// ¾À »óÅÂ°ª
	enum SCENE_STATE
	{
		SCENE_END	= 0,
		SCENE_RUNNING,
		SCENE_LOGO,
		SCENE_STORY,
		SCENE_MENU,
		SCENE_LOBBY,
		SCENE_MAIN
	};

public:
	IScene() {}
	virtual ~IScene() {}

	//virtual VOID	Clear() = 0;
	virtual HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, LPD3DXSPRITE a_pSprite, HWND a_hWnd ) = 0;
	//virtual HRESULT Release() = 0;
	virtual VOID	Update() = 0;
	virtual VOID	Render() = 0;


	virtual INT		GetSceneNext() = 0;
	virtual INT		GetSceneState() = 0;

	static INT		g_iNumStage;

};

#endif