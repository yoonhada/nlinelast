/**
@file	BoundBox.h
@date	2011/09/20
@author	yoonhada@gmail.com	
@brief	�浹 Ŭ���� ����
*/

#pragma once

/**
@class	CBoundBox
@date	2011/09/20
@author	yoonhada@gmail.com	
@brief	�浹 Ŭ����
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
	VOID SetPosition(D3DXVECTOR3 v)		{ m_vPosition = v; }	///< �߽���ǥ
	VOID SetDirection(D3DXVECTOR3 v)	{ m_vDirection = v; }	///< ����ǥ
	VOID SetAngleX(FLOAT fAngle);								///< x��ȸ��
	VOID SetAngleY(FLOAT fAngle);								///< y��ȸ��
	VOID SetAngleZ(FLOAT fAngle);								///< z��ȸ��
	VOID SetPosVec();
private:
	CCharactor * m_pCharactors;
	D3DXVECTOR3 m_vPosition;		///< �߽���ǥ
	D3DXMATRIXA16 m_matAxis;		///< ť�� ��Ʈ����
	D3DXVECTOR3 m_vDirection;		///< ����ǥ
	FLOAT m_fSize[6];
	std::vector <D3DXVECTOR3> _data;
	//D3DXMATRIXA16 m_vMatWorld;
};