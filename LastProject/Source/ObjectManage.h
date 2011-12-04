/**
@file	ObjectManage.h
@date	2011/11/11
@author	백경훈
@brief	오브젝트 관리 ( 네트워크와 클래스간 사용 )
*/

#ifndef _MainManage_H_
#define _MainManage_H_

class CMonster;
class CCharactor;
class CTimeLifeItem;
class ASEViewer;
class LobbyScene;

class CObjectManage : public CSingleton<CObjectManage>
{
	friend class CSingleton<CObjectManage>;

private:
	CObjectManage();
	virtual ~CObjectManage();

	LPDIRECT3DDEVICE9	m_pD3dDevice;
	LPD3DXSPRITE		m_pSprite;

	BOOL				m_bHost;
	WORD				m_iClientNumber;
	INT					m_iMaxCharaNum;

	INT					m_nCharTable[4];
	CCharactor*			m_pCharactors;
	CCharactor**		m_ppVirtualCharactors;

	CMonster*			m_pClown;
	CMonster*			m_pPanda;
	CMonster*			m_pMonster;
	CTimeLifeItem*		m_pFirstAidKit;

	ASEViewer*			m_pASEViewer;

	LobbyScene*			m_pLobbyScene;


private:
	//typedef struct _DESTROYDATA
	//{
		WORD m_wTotalDestroyPart;
		WORD m_wDestroyPart[7];
		WORD m_wDestroyCount[7];
		std::vector<WORD> m_NetworkSendTempVector;
		D3DXVECTOR3 m_vDestroyDir;
	//} DestroyData;

	//DestroyData m_DestoryDataCharactor;
	//DestroyData m_DestoryDataMonster;

public:

	VOID	Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();
	//VOID	Update();
	//VOID	Render();

	HRESULT LoadLoadingObject();
	HRESULT LoadLobbyObject();
	HRESULT LoadMainObject();

public:
	// Get
	INT Get_MaxCharaNum()				{ return m_iMaxCharaNum;		}
	WORD Get_ClientNumber()				{ return m_iClientNumber;		}
	CCharactor** Get_Charactors()		{ return m_ppVirtualCharactors; }
	CCharactor* Get_Charactor()			{ return m_pCharactors;			}

	CMonster* Get_Clown()				{ return m_pClown;				}
	CMonster* Get_Panda()				{ return m_pPanda;				}
	CMonster* Get_Monster()				{ return m_pClown;				}
	CTimeLifeItem * Get_FirstAidKit()	{ return m_pFirstAidKit;		}
	ASEViewer* Get_ASEViewer()			{ return m_pASEViewer;			}
	BOOL IsHost()						{ return m_bHost;				}
	INT* Get_CharTable()				{ return m_nCharTable; }
	INT Get_CharTable(INT n)			{ return m_nCharTable[n]; }

	// Set
	VOID Set_Host( BOOL a_bHost )						{ m_bHost = a_bHost; }
	VOID Set_ClientNumber( WORD a_iClientNumber )		{ m_iClientNumber = a_iClientNumber; }

public:
	VOID Set_Char(INT nSelect, INT nChar, BOOL = FALSE );
	VOID Set_CharTable( INT * nTable );

	VOID Set_PushBackNetworkSendTempVector( WORD a_wData);
	VOID Set_NetworkSendDestroyData( CHAR a_cDestroyPart, WORD a_wDestroyCount, D3DXVECTOR3& a_vDestroyDir);
	VOID Send_NetworkSendDestroyData(BOOL IsMonster );

	LobbyScene*		GetLobbyScene(){ return m_pLobbyScene; }
	VOID			SetLobbyScene( LobbyScene* a_pLobbyScene ){ m_pLobbyScene = a_pLobbyScene; }

	LPD3DXSPRITE	GetSprite()			{ return m_pSprite; }
};

#endif;