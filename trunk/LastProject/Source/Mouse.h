#pragma once

#ifndef _MOUSE_H_
#define _MOUSE_H_

class Mouse	//�ѹ��� �����°� ������ ���� �ʾ��� & 0x8001 �� ���� �ǰٴµ� �Լ� �̸��� ����? LButton One Click? �̻���...
{
private:
	HWND		m_hWnd;

	bool		m_bLKeyUp;
	bool		m_bLKeyDown;
	bool		m_bLDrag;

	RECT		m_rt;					//������ ũ�� �����δ� ������ ���Ŷ�
	POINT		m_pt;
	
public:
	Mouse(HWND a_hWnd) : m_hWnd(a_hWnd)
	{
		m_bLKeyUp			= true;
		m_bLKeyDown			= false;
		m_bLDrag			= false;
		this->Initialize();	
	}
	~Mouse(){}

	void Initialize();
	void Update();

	bool LButton_Up();
	bool LButton_Down();
	bool LButton_Drag();
	
	bool IsClickedLButton();
	POINT GetPosition();
	
};

#endif