#pragma once

#ifndef _MOUSE_H_
#define _MOUSE_H_

class Mouse	: public CSingleton<Mouse> //한번만 누르는건 구현이 되지 않았음 & 0x8001 을 쓰면 되겟는데 함수 이름은 뭘로? LButton One Click? 이상해...
{
	friend class CSingleton<Mouse>;
private:
	HWND		m_hWnd;

	bool		m_bLKeyUp;
	bool		m_bLKeyDown;
	bool		m_bLDrag;

	RECT		m_rt;					//윈도우 크기 밖으로는 나가지 마셔라
	POINT		m_pt;
	
public:
	Mouse()
	{
		m_bLKeyUp			= true;
		m_bLKeyDown			= false;
		m_bLDrag			= false;
	}
	~Mouse(){}

	void Initialize( HWND a_hWnd );
	void Update();

	bool LButton_Up();
	bool LButton_Down();
	bool LButton_Drag();
	
	bool IsClickedLButton();
	POINT GetPosition();
	
};

#endif