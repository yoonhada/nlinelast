#ifndef _MainScene_H_
#define _MainScene_H_

#define _ALPHAMON

#include "Scene.h"

class CMonster;
class CBillBoard;
class CCharactor;
class CTimeLifeItem;
class CCamera;
class Axis;
class ASEViewer;
class CGameEvent;
class TileMap;

class CMainScene : public IScene
{
	//friend class CSingleton<CMainManage>;
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

	CMatrices*				m_pMatrices;
	CCamera*				m_pCamera;
	CGrid*					m_pGrid;
	CCharactor*				m_pCharactors;
	//CCharactor*				m_pMyCharactor;
	CMonster*				m_pMonster;
	CLight*					m_pLight;
	CTimeLifeItem * m_pFirstAidKit;

	CCharactor*				m_pCharactorList[4];

	TileMap*				m_pTileMap;
	ASEViewer*				m_pASEViewer;
	
	INT						m_iMaxCharaNum;

	CBillBoard*				m_pBill;
	Axis*					m_pAxis;

	CGameEvent*				m_pGameEvent;

	VOID					CreateCharactor();

	//BOOL m_bHost;
	//WORD m_iClientNumber;

	//로고
	CCharactor* m_pLogo;

	INT						m_scnNext;
	INT						m_scnState;

//public:
//	CCharactor* Get_Charactors()
//	{
//		return m_pCharactors;
//	}
//
//	CCharactor* Get_MyCharactor()
//	{
//		return m_pMyCharactor;
//	}
//
//	CCharactor** Get_CharactorList()
//	{
//		return m_pCharactorList;
//	}
//
//	VOID Set_Host( BOOL a_bHost )
//	{
//		m_bHost = a_bHost;
//	}
//
//	VOID Set_ClientNumber( WORD a_iClientNumber )
//	{
//		m_iClientNumber = a_iClientNumber;
//	}
//
//	WORD Get_ClientNumber()
//	{
//		return m_iClientNumber;
//	}
//
//	CCharactor* Get_pAlphaMon()
//	{
//		return m_pAlphaMon;
//	}
};

#endif