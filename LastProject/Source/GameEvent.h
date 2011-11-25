/**
	@class	CGameEvent
	@date	2011/11/25
	@author	yoonhada@gmail.com
	@brief	게임이벤트
*/

#pragma once

class CGameEvent : public IObject
{
private:

public:
	CGameEvent();
	virtual ~CGameEvent();

	VOID Clear()		{}
	HRESULT Create();	///< 초기화
	HRESULT Release();	///< 오브젝트들 제거
	VOID Update();		///< Update
	VOID Render();		///< Render
};
