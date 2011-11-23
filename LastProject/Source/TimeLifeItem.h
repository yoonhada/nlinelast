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
	~CTimeLifeItem();

	VOID Update();
	VOID Render();

	VOID SetActive( BOOL a_bActive );

private:
	const FLOAT m_fLifeTime;
	FLOAT m_fLife;
};

#endif