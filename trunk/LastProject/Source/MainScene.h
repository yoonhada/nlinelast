#ifndef _MainScene_H_
#define _MainScene_H_

#define _ALPHAMON

#include "Scene.h"

class CCamera;
class CCharactor;
class CMonster;
class CTimeLifeItem;
class ASEViewer;
class TileMap;
class Axis;
class CGameEvent;

class MainGUI;
class OptionScene;

class CMainScene : public IScene
{
public:
	CMainScene();
	virtual ~CMainScene();

	VOID			Clear();
	virtual HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, LPD3DXSPRITE a_Sprite, HWND a_hWnd );
	HRESULT			Release();
	virtual VOID	Update();
	virtual VOID	Render();

	virtual INT		GetSceneNext();
	virtual INT		GetSceneState();

private:

	enum ClassType { NONE, CHARACTOR, MONSTER };

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	OptionScene*			m_pOptionScene;
	MainGUI*				m_pMainGUI;

	// Equipment
	CMatrices*				m_pMatrices;
	CCamera*				m_pCamera;
	Axis*					m_pAxis;
	CLight*					m_pLight;

	// Charactor
	CCharactor*				m_pMyCharactors;
	CCharactor**			m_pOtherCharactors;
	CCharactor*				m_pCharactors;
	CMonster*				m_pMonster;
	CTimeLifeItem *			m_pFirstAidKit;

	// Map
	TileMap*				m_pTileMap;
	ASEViewer*				m_pASEViewer;

	// Game
	CGameEvent*				m_pGameEvent;

	INT						m_iMaxCharaNum;
	INT						m_nClientID;


	INT						m_scnNext;
	INT						m_scnState;

	VOID					CreateCharactor();
};

#endif