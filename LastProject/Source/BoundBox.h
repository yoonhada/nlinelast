/**
@file	BoundBox.h
@date	2011/09/20
@author	yoonhada@gmail.com	
@brief	충돌 클래스 파일
*/

#pragma once

/**
@class	CBoundBox
@date	2011/09/20
@author	yoonhada@gmail.com	
@brief	충돌 클래스
*/
class CCharactor;

class CBoundBox : public IObject
{
public:
	enum _AXIS { AXIS_X, AXIS_Y, AXIS_Z };
	enum _SIZE { MINUSX, MINUSY, MINUSZ, PLUSX, PLUSY, PLUSZ };

	CBoundBox(CCharactor* = NULL);
	virtual ~CBoundBox(void);

	VOID Clear();
	HRESULT Create()					{ return S_OK; }
	HRESULT Release()					{ return S_OK; }
	VOID Update()						{};
	VOID Render()						{};

	// Get
	FRECT GetFRect();
	FLOAT GetWidth();
	FLOAT GetHeight();
	D3DXVECTOR3 GetPosition(INT = -1);
	D3DXVECTOR3 GetAxisDir(INT n)		{ return m_vAxisDir[n]; }
	D3DXMATRIXA16 GetAxisMat();

	FLOAT GetSize(INT n);
	FLOAT GetRadius();
	FLOAT GetRadiusLong();
	FLOAT GetRadiusShort();

	// Set
	VOID SetSize(INT n, float f)		{ m_fSize[n] = f; }
	VOID SetPosition(D3DXVECTOR3 v)		{ m_vPosition = v; }	///< 중심좌표
	VOID SetAngle(FLOAT fAngle);								///< y축회전

private:
	CCharactor * m_pCharactors;
	D3DXVECTOR3 m_vPosition;		///< 중심좌표
	D3DXVECTOR3 m_vAxisDir[3];		///< 큐브 세축의 단위벡터 ( ?? 필요한가?? )
	FLOAT m_fSize[6];
	//D3DXMATRIXA16 m_vMatWorld;
};