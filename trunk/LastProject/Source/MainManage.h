#ifndef _MainManage_H_
#define _MainManage_H_

class CMonster;
class CBillBoard;
class CCharactor;
class CCamera;
class Axis;
class Map;

class CMainManage : public CSingleton<CMainManage>
{
	friend class CSingleton<CMainManage>;
public:
	CMainManage();
	~CMainManage();

	VOID	Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();
	VOID	Update();
	VOID	Render();

private:
	enum ClassType { NONE, CHARACTOR, MONSTER };

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

	CCharactor* m_pAlphaMon;

	INT m_iMaxCharaNum;

	CBillBoard* m_pBill;
	Axis* m_pAxis;

	VOID CreateCharactor();

	BOOL m_bHost;
	WORD m_iClientNumber;

	//로고
	CCharactor* m_pLogo;

public:
	CCharactor* Get_Charactors()
	{
		return m_pCharactors;
	}

	CCharactor* Get_MyCharactor()
	{
		return m_pMyCharactor;
	}

	CCharactor** Get_CharactorList()
	{
		return m_pCharactorList;
	}

	VOID Set_Host( BOOL a_bHost )
	{
		m_bHost = a_bHost;
	}

	VOID Set_ClientNumber( WORD a_iClientNumber )
	{
		m_iClientNumber = a_iClientNumber;
	}

	WORD Get_ClientNumber()
	{
		return m_iClientNumber;
	}

	CCharactor* Get_pAlphaMon()
	{
		return m_pAlphaMon;
	}
};

#endif