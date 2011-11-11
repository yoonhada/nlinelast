#include "stdafx.h"
#include "Mouse.h"

void Mouse::Initialize()
{
	GetClientRect(m_hWnd,&m_rt);
}
void Mouse::Update()
{
	m_bLKeyUp = true;
}

bool Mouse::LButton_Down()
{
	POINT pt;
	pt = this->GetPosition();

	if(m_rt.left		> pt.x			//ȭ������� ������ �Է� ���� �ʰٽ��ϴ�
		|| m_rt.right	< pt.x
		|| m_rt.top		> pt.y
		|| m_rt.bottom	< pt.y)
		return false;

	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_bLKeyDown		= true;
		m_bLKeyUp		= false;
		m_bLDrag		= true;

		return true;
	}
	return false;
}

bool Mouse::LButton_Up()
{
	if(m_bLKeyUp & m_bLDrag){
		m_bLDrag		= false;
		m_bLKeyDown		= false;

		return true;
	}
	return false;
}

bool Mouse::LButton_Drag()
{
	return m_bLDrag;
}

POINT Mouse::GetPosition()
{
	GetCursorPos(&m_pt);
	ScreenToClient(m_hWnd,&m_pt);

	return m_pt;
}
//bool Mouse::Check_Collision(RECT a_rt)//�̰� ���콺�� �ؾ� �Ǵ°���?? �ؾ� �ɰ� ������...!!
//{
//	GetPosition();
//
//	if(m_pt.x < a_rt.right
//		&& m_pt.x > a_rt.left
//		&& m_pt.y < a_rt.bottom
//		&& m_pt.y > a_rt.top)
//		return true;
//	return false;
//}

bool Mouse::IsClickedLButton()
{
	if(GetAsyncKeyState(VK_LBUTTON) & 0x8001)
		return false;
	return true;
}
	


