/**
	@file	Input.h
	@date	2011/09/27
	@author	백경훈
	@brief	키보드, 마우스 처리 클래스
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
	HRESULT Create( HWND a_hWnd );
	HRESULT Release();

	VOID Update( FLOAT a_fCameraMoveSpeed, FLOAT a_fCameraRotateSpeed, FLOAT a_fFrameTime );

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
		if( m_iLbuttonCheck == 0 )
		{
			m_bLbutton = TRUE;
		}
		else
		{
			m_bLbutton = FALSE;
		}

		return m_bLbutton;
	}
	
	BOOL Get_Rbutton()
	{
		if( m_iRbuttonCheck == 0 )
		{
			m_bRbutton = TRUE;
		}
		else
		{
			m_bRbutton = FALSE;
		}

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

	BOOL Get_F1button()
	{
		return m_bF1button;
	}

	VOID Set_F9button(BOOL b)
	{
		m_bF9button = b;
	}

	BOOL Get_F8button()
	{
		return m_bF8button;
	}

	BOOL EnableInput(BOOL b = -1)		
	{
		if (b != -1) 
			m_bEnable = b;

		return m_bEnable; 
	}

private:
	HWND			  m_hWnd;					///< 윈도우 핸들

	POINT		m_MousePos;						///< 현재 마우스 위치
	POINT		m_MousePosOld;					///< 이전 마우스 위치

	D3DXVECTOR3 m_vRotate;						///< 회전 값
	D3DXVECTOR3 m_vPos;
	FLOAT		m_fYRotate;
	FLOAT		m_fXRotate;
	BOOL		m_bLbutton;						///< 왼쪽 버튼
	BOOL		m_iLbuttonCheck;
	BOOL		m_bRbutton;						///< 오른쪽 버튼
	INT			m_iRbuttonCheck;
	BOOL		m_bENdbutton;
	BOOL		m_bHomebutton;
	BOOL		m_bF1button;
	BOOL		m_bF9button;
	BOOL		m_bF8button;
	//FLOAT		m_fXPos;						///< X축 이동
	//FLOAT		m_fZPos;						///< Z축 이동
	D3DXMATRIXA16 m_matMatrix;

	BOOL		m_bEnable;						///< 입력 활성화
};


#endif