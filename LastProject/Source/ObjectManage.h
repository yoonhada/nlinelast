#ifndef _MainManage_H_
#define _MainManage_H_

class CMonster;
class CCharactor;

class CObjectManage : public CSingleton<CObjectManage>
{
	friend class CSingleton<CObjectManage>;

public:

	VOID	Clear();
	HRESULT Create();
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

	CCharactor* Get_AlphaMon()
	{
		return m_pAlphaMon;
	}

	CMonster* Get_Monster()
	{
		return m_pMonster;
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

	VOID Set_pAlphaMon( CCharactor* a_pAlphaMon )
	{
		m_pAlphaMon = a_pAlphaMon;
	}

	VOID Set_Monster( CMonster* a_pMonster )
	{
		m_pMonster = a_pMonster;
	}

private:
	CObjectManage();
	~CObjectManage();

	CCharactor* m_pMyCharactor;
	CCharactor* m_pCharactors;
	CMonster* m_pMonster;
	CCharactor* m_pAlphaMon;

	CCharactor** m_ppCharactorList;

	BOOL m_bHost;
	WORD m_iClientNumber;
};

#endif;