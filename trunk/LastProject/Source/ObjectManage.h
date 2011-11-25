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

class CObjectManage : public CSingleton<CObjectManage>
{
	friend class CSingleton<CObjectManage>;

public:

	VOID	Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();
	//VOID	Update();
	//VOID	Render();

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
		return m_ppCharactorList;
	}

	WORD Get_ClientNumber()
	{
		return m_iClientNumber;
	}

	CMonster* Get_Monster()
	{
		return m_pMonster;
	}

	CTimeLifeItem * Get_FirstAidKit()
	{
		return m_pFirstAidKit;
	}

	INT Get_MaxCharaNum()
	{
		return m_iMaxCharaNum;
	}

	ASEViewer* Get_ASEViewer()
	{
		return 	m_pASEViewer;
	}


public:
	VOID Set_Host( BOOL a_bHost )
	{
		m_bHost = a_bHost;
	}

	VOID Set_ClientNumber( WORD a_iClientNumber )
	{
		m_iClientNumber = a_iClientNumber;
	}

	VOID Set_Charactors( CCharactor* a_pCharactors )
	{
		m_pCharactors = a_pCharactors;
	}

	VOID Set_MyCharactor( CCharactor* a_pMyCharactor )
	{
		m_pMyCharactor = a_pMyCharactor;
	}

	VOID Set_CharactorList( CCharactor** a_ppCharactorList )
	{
		m_ppCharactorList = a_ppCharactorList;
	}

	VOID Set_Monster( CMonster* a_pMonster )
	{
		m_pMonster = a_pMonster;
	}

	VOID RefreshCharList();

private:
	CObjectManage();
	~CObjectManage();

	LPDIRECT3DDEVICE9 m_pD3dDevice;

	INT m_iMaxCharaNum;

	CCharactor* m_pMyCharactor;
	CCharactor* m_pCharactors;
	CCharactor* m_pCharactorList[4];
	CCharactor** m_ppCharactorList;

	CMonster* m_pMonster;

	CTimeLifeItem* m_pFirstAidKit;

	ASEViewer* m_pASEViewer;

	BOOL m_bHost;
	WORD m_iClientNumber;

private:
	WORD m_wTotalDestroyPart;
	WORD m_wDestroyPart[7];
	WORD m_wDestroyCount[7];
	std::vector<WORD> m_NetworkSendTempVector;
	D3DXVECTOR3 m_vDestroyDir;

public:
	VOID Set_PushBackNetworkSendTempVector( WORD a_wData )
	{
		CDebugConsole::GetInstance()->Messagef( L"PushBack Size : %d / Data : %d\n", m_NetworkSendTempVector.size(), a_wData );
		m_NetworkSendTempVector.push_back(a_wData);
	}

	VOID Set_NetworkSendDestroyData( CHAR a_cDestroyPart, WORD a_wDestroyCount, D3DXVECTOR3& a_vDestroyDir )
	{
		m_wDestroyPart[m_wTotalDestroyPart] = a_cDestroyPart;
		m_wDestroyCount[m_wTotalDestroyPart] = a_wDestroyCount;
		m_vDestroyDir = a_vDestroyDir;
		
		++m_wTotalDestroyPart;
	}

	VOID Send_NetworkSendDestroyData()
	{
		if( m_wTotalDestroyPart > 0)
		{
			CNetwork::GetInstance()->CS_UTOM_ATTACK( m_vDestroyDir, m_wTotalDestroyPart, m_wDestroyPart, m_wDestroyCount, m_NetworkSendTempVector );
			m_NetworkSendTempVector.clear();
			CDebugConsole::GetInstance()->Messagef( L"m_NetworkSendTempVector Size : %d\n", m_NetworkSendTempVector.size() );
			m_wTotalDestroyPart = 0;
			ZeroMemory( &m_wDestroyPart, sizeof(m_wDestroyPart) );
			ZeroMemory( &m_wDestroyCount, sizeof(m_wDestroyCount) );
			ZeroMemory( m_vDestroyDir, sizeof(m_vDestroyDir) );
		}
	}
};

#endif;