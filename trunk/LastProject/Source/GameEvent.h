/**
	@class	CGameEvent
	@date	2011/11/25
	@author	yoonhada@gmail.com
	@brief	�����̺�Ʈ
*/

#pragma once

class CGameEvent : public IObject
{
private:

public:
	CGameEvent();
	virtual ~CGameEvent();

	VOID Clear()		{}
	HRESULT Create();	///< �ʱ�ȭ
	HRESULT Release();	///< ������Ʈ�� ����
	VOID Update();		///< Update
	VOID Render();		///< Render
};
