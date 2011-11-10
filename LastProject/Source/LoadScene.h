#ifndef _CLoadScene_H_
#define _CLoadScene_H_

#include "Scene.h"

class CMonster;
class CBillBoard;
class CCharactor;
class CCamera;
class Axis;
class Map;
class TileMap;

class CLoadScene : public IScene
{
public:
	CLoadScene();
	~CLoadScene();

public:
	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();
	VOID Update();
	VOID Render();

private:
	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스

	CMatrices* m_pMatrices;
	CCamera* m_pCamera;
	CGrid* m_pGrid;
	CCharactor* m_pCharactors;
	CCharactor* m_pMyCharactor;
	CMonster* m_pMonster;
	CLight* m_pLight;

	CCharactor* m_pCharactorList[4];

	Map* m_pMap;
	TileMap* m_pTileMap;

	CCharactor* m_pAlphaMon;

	INT m_iMaxCharaNum;

	CBillBoard* m_pBill;
	Axis* m_pAxis;

	VOID CreateCharactor();

	BOOL m_bHost;
	WORD m_iClientNumber;

	//로고
	CCharactor* m_pLogo;
};

#endif