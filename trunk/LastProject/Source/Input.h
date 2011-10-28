/**
	@file	Input.h
	@date	2011/09/27
	@author	�����
	@brief	Ű����, ���콺 ó�� Ŭ����
*/

#ifndef _INPUT_H_
#define _INPUT_H_


class CInput : public CSingleton<CInput>
{
	friend class CSingleton<CInput>;

public:
	CInput();
	virtual ~CInput();

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, HWND a_hWnd );
	HRESULT Release();

	VOID Update( FLOAT a_fCameraMoveSpeed, FLOAT a_fCameraRotateSpeed, 
		FLOAT a_fFrameTime );

	D3DXVECTOR3& Get_Pos()
	{
		return m_vPos;
	}

	FLOAT& Get_MouseYRotate()
	{
		return m_fYRotate;
	}

	FLOAT& Get_MouseXRotate()
	{
		return m_fXRotate;
	}

	BOOL Get_Lbutton()
	{
		return m_bLbutton;
	}
	
	BOOL Get_Rbutton()
	{
		return m_bRbutton;
	}

	BOOL Get_Endbutton()
	{
		return m_bENdbutton;
	}

	BOOL Get_Homebutton()
	{
		return m_bHomebutton;
	}

	BOOL Get_F9button()
	{
		return m_bF9button;
	}

	VOID Set_F9button(BOOL b)
	{
		m_bF9button = b;
	}


private:
	LPDIRECT3DDEVICE9 m_pD3dDevice;
	HWND			  m_hWnd;					///< ������ �ڵ�

	POINT		m_MousePos;						///< ���� ���콺 ��ġ
	POINT		m_MousePosOld;					///< ���� ���콺 ��ġ

	D3DXVECTOR3 m_vRotate;						///< ȸ�� ��
	D3DXVECTOR3 m_vPos;
	FLOAT		m_fYRotate;
	FLOAT		m_fXRotate;
	BOOL		m_bLbutton;						///< ���� ��ư
	BOOL		m_bRbutton;						///< ������ ��ư
	BOOL		m_bENdbutton;
	BOOL		m_bHomebutton;
	BOOL		m_bF9button;
	//FLOAT		m_fXPos;						///< X�� �̵�
	//FLOAT		m_fZPos;						///< Z�� �̵�

	D3DXMATRIXA16 m_matMatrix;
	
};


#endif