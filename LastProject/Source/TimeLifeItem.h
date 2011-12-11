/**
	@file	TimeLifeItem.h
	@date	2011/11/22
	@author	yoonhada@gmail.com
	@brief	아이템 클래스
*/

#ifndef _TIMELIFEITEM_H_
#define _TIMELIFEITEM_H_

/**
	@class	CTimeLifeItem
	@date	2011/11/22
	@author	yoonhada@gmail.com
	@brief	아이템 클래스
*/
class CCharactor;

class CTimeLifeItem : public CCharactor
{
public:
	CTimeLifeItem();
	virtual ~CTimeLifeItem();

	VOID Update();
	VOID Render();

	VOID SetDying();
	VOID SetActive( BOOL a_bActive );
	VOID Set_MonsterNumber( INT nNum )	{ m_iMonsterNumber = nNum; }

	INT Get_MonsterNumber()							{ return m_iMonsterNumber; }

private:
	const FLOAT m_fLifeTime;
	FLOAT m_fLife;
	INT m_iMonsterNumber;
};

#endif