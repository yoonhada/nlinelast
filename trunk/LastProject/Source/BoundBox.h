/**
@file	BoundBox.h
@date	2011/09/20
@author	yoonhada@gmail.com	
@brief	충돌 클래스 파일
*/

#pragma once

class CCharactor;

/**
@class	CBoundBox
@date	2011/09/20
@author	yoonhada@gmail.com	
@brief	충돌 클래스
*/
class CBoundBox : public IObject
{
public:
	enum _AXIS { AXIS_X, AXIS_Y, AXIS_Z };
	enum _SIZE { MINUSX, MINUSY, MINUSZ, PLUSX, PLUSY, PLUSZ };

	CBoundBox(CCharactor* = NULL);
	virtual ~CBoundBox(void);

	VOID Clear()						{};
	HRESULT Create()					{ return S_OK; }
	HRESULT Release()					{ return S_OK; }
	VOID Update()						{};
	VOID Render()						{};

	// Get
	RECT GetRect();
	FLOAT GetWidth();
	FLOAT GetHeight();
	D3DXVECTOR3 GetPosition();
	FLOAT GetSize(INT n);

	// Set
	VOID SetSize(INT n, float f)		{ m_fSize[n] = f; }
	VOID SetPosition(D3DXVECTOR3 v)		{ m_vPosition = v; }		///< 중심좌표

private:
	CCharactor * m_pCharactors;
	D3DXVECTOR3 m_vPosition;		///< 중심좌표
	D3DXVECTOR3 m_vAxisDir[3];		///< 큐브 세축의 단위벡터 ( ?? 필요한가?? )
	FLOAT m_fSize[6];
	//D3DXMATRIXA16 m_vMatWorld;
};