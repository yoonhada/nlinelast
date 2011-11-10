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
	D3DXVECTOR3 GetPosition(INT = -1) const;
	D3DXVECTOR3 GetDirection() const 		{ return m_vDirection; }
	D3DXMATRIXA16 GetAxisMat() const;
	std::vector <D3DXVECTOR3> * GetPosVec();

	FLOAT GetSize(INT n) const;
	FLOAT GetRadius() const;
	FLOAT GetRadiusLong() const;
	FLOAT GetRadiusShort() const;

	// Set
	VOID SetSize(INT n, float f)		{ m_fSize[n] = f; }
	VOID SetPosition(D3DXVECTOR3 v)		{ m_vPosition = v; }	///< 중심좌표
	VOID SetDirection(D3DXVECTOR3 v)	{ m_vDirection = v; }	///< 동좌표
	VOID SetAngleX(FLOAT fAngle);								///< x축회전
	VOID SetAngleY(FLOAT fAngle);								///< y축회전
	VOID SetAngleZ(FLOAT fAngle);								///< z축회전
	VOID SetPosVec();
private:
	CCharactor * m_pCharactors;
	D3DXVECTOR3 m_vPosition;		///< 중심좌표
	D3DXMATRIXA16 m_matAxis;		///< 큐브 매트릭스
	D3DXVECTOR3 m_vDirection;		///< 동좌표
	FLOAT m_fSize[6];
	std::vector <D3DXVECTOR3> _data;
	//D3DXMATRIXA16 m_vMatWorld;
};