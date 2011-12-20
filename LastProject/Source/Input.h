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

	// Get
	D3DXVECTOR3& Get_Pos()		{ return m_vPos; }

	FLOAT& Get_MouseYRotate()	{ return m_fYRotate; }
	FLOAT& Get_MouseXRotate()	{ return m_fXRotate; }
	
	BOOL Get_Lbutton()			{ return ( m_bLbutton == TRUE ); }
	BOOL Get_Rbutton()			{ return ( m_bRbutton == TRUE ); }
	BOOL Get_ESCKey()			{ return ( m_bESCKey == TRUE ); }
	BOOL Get_Endbutton()		{ return ( m_bEndbutton ); }
	BOOL Get_Homebutton()		{ return ( m_bHomebutton ); }
	BOOL Get_F1button()			{ return ( m_bF1button == TRUE ); }
	BOOL Get_F8button()			{ return ( m_bF8button == TRUE ); }
	BOOL Get_F9button()			{ return ( m_bF9button == TRUE ); }
	BOOL Get_FunKey(INT nInput);
	BOOL Get_NumKey(INT nInput);

	// Set
	VOID EnableInput( BOOL bEnable );

private:
	HWND		 m_hWnd;						///< 윈도우 핸들

	POINT		m_MousePos;						///< 현재 마우스 위치
	POINT		m_MousePosOld;					///< 이전 마우스 위치

	D3DXVECTOR3 m_vRotate;						///< 회전 값
	D3DXVECTOR3 m_vPos;
	FLOAT		m_fYRotate;
	FLOAT		m_fXRotate;
	BOOL		m_bLbutton;						///< 왼쪽 버튼
	BOOL		m_bRbutton;						///< 오른쪽 버튼
	BOOL		m_bESCKey;
	BOOL		m_bEndbutton;
	BOOL		m_bHomebutton;
	BOOL		m_bF1button;
	BOOL		m_bF8button;
	BOOL		m_bF9button;
	BOOL		m_bFunKey[12];
	BOOL		m_bNumKey[10];
	D3DXMATRIXA16 m_matMatrix;

	BOOL		m_bEnable;						///< 입력 활성화

	VOID MouseMoveCenter();
};


#endif