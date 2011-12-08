#ifndef _Monster_H_
#define _Monster_H_

#include "StateMachine.h"

class CCharactor;

class CMonster : public CObjectSRT
{
public:
	// �ִϸ��̼� ��ȣ
/*
	enum { ANIM_MOVE = 0, ANIM_SPIN_ATTACK = 1, ANIM_MELEE_ATTACK = 2, ANIM_STAND = 3,
		   ANIM_EVENT = 4, ANIM_DASH = 5, ANIM_SLIDING = 6, ANIM_MELEE_ATTACK2 = 7, };
*/

	enum { ANIM_MOVE = 3, ANIM_SPIN_ATTACK = 4, ANIM_MELEE_ATTACK = 4, ANIM_STAND = 2,
		   ANIM_EVENT = 4, ANIM_DASH = 4, ANIM_SLIDING = 0, ANIM_MELEE_ATTACK2 = 3, };

public:
	CMonster();
	~CMonster();
	VOID Clear();
	VOID UpdateByValue( D3DXVECTOR3& a_vControl, FLOAT a_fAngle );
	VOID Update();
	VOID Render();

	HRESULT Create() {}
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, WCHAR* a_pFileName );
	VOID Realese();
	VOID Load( WCHAR* a_pFileName );

	// �ִϸ��̼� �����
	VOID InitAniAndState();
	VOID ChangeAnimation( INT a_iAniNum );
	
	// ���� �ִϸ��̼� ������ üũ ���� ( FALSE = ���� TRUE = �ȳ��� )
	BOOL Get_ChangingAnimation()
	{
		return m_bChangingAnimation;
	}

	// ���ݺ� �ִϸ��̼� ������ üũ ���� ( FALSE = ���� TRUE = �ȳ��� )
	BOOL Get_AnimationEndCheck()
	{
		return m_bAnimationEndCheck;
	}

	// Set
	VOID Set_MonsterNumber( INT iMonsterNumber )	{ m_iMonsterNumber = iMonsterNumber; }	VOID Set_iSelectedFrameNum( INT a_iAniNum )		{ m_iSelectedFrameNum = a_iAniNum; }
	INT	Get_iSelectedFrameNum()						{ return m_iSelectedFrameNum; }

	VOID Set_ClearTime()							{ m_fTime = 0.0f; }

	VOID Set_UpdateTime()
	{
		m_fTime += CFrequency::GetInstance()->getFrametime();
	}
	VOID Set_InterpolationTime( FLOAT a_fTime )		{ m_fInterpolationTime = a_fTime; }
	VOID Set_Angle( FLOAT a_fAngle )				{ m_fAngle = a_fAngle; }
	VOID Set_Pos( D3DXVECTOR3& a_vPos )				{ m_vControl = a_vPos; }
	VOID Set_Target( INT a_iTarget )				{ m_iTarget = a_iTarget; }
	VOID Set_TargetPos( INT a_iX, INT a_iZ )		{ m_iTargetPos[0] = a_iX; m_iTargetPos[1] = a_iZ; }
	VOID Set_TargetPos( D3DXVECTOR3 a_vPos )		{ m_vTargetPos = a_vPos; }
	VOID Set_TargetDistance( FLOAT a_fDistance )	{ m_fTargetDistance = a_fDistance; }
	VOID Set_Path( PathNode* a_pPath )				{ m_pPath = a_pPath; }
	
	// Dash
	VOID Set_DashData( D3DXVECTOR3 a_vStartPos, D3DXVECTOR3 a_vEndPos )
	{
		m_vDashStartPos = a_vStartPos;
		m_vDashEndPos	= a_vEndPos;
	}

	VOID Set_ChaseData();
	VOID Set_ChaseNextData();

	FLOAT			Get_Degree();
	D3DXVECTOR3		GetWorldPos( INT a_iX, INT a_iZ );


	// Get
	StateMachine<CMonster>* GetFSM() const			{ return m_pStateMachine; }
	INT Get_MonsterNumber()							{ return m_iMonsterNumber; }

	// Dash
	D3DXVECTOR3		Get_DashStartPos()				{ return m_vDashStartPos; }
	D3DXVECTOR3		Get_DashEndPos()				{ return m_vDashEndPos; }

	// Chase
	PathNode*		Get_ChaseNextPath()				{ return m_pChaseNextPath; }
	D3DXVECTOR3		Get_ChaseCurrentPos()			{ return m_vChaseCurrentPos; }
	D3DXVECTOR3		Get_ChasePreviousPos()			{ return m_vChasePreviousPos; }
	INT				Get_ChaseCurrentX()				{ return m_iChaseCurrentX; }
	INT				Get_ChaseCurrentZ()				{ return m_iChaseCurrentZ; }
	INT				Get_ChaseTotalPathCnt()			{ return m_iChaseTotalPathCnt; }
	FLOAT			Get_ChaseCurrentAngle()			{ return m_fChaseCurrentAngle; }
	FLOAT			Get_ChaseNextAngle()			{ return m_fChaseNextAngle; }
	FLOAT			Get_ChaseAngle0()				{ return m_fChaseAngle0; }
	FLOAT			Get_ChaseAngle1()				{ return m_fChaseAngle1; }

	FLOAT			Get_Time()						{ return m_fTime; }
	FLOAT			Get_InterpolationTime()			{ return m_fInterpolationTime; }
	D3DXVECTOR3&	Get_Pos()						{ return m_vControl; }
	INT				Get_Target()					{ return m_iTarget; }
//	INT* Get_TargetPos()							{ return m_iTargetPos; }
	D3DXVECTOR3&	Get_TargetPos()					{ return m_vTargetPos; }
//	INT				Get_iSelectedFrameNum()			{ return m_iSelectedFrameNum; }
	FLOAT			Get_TargetDistance()			{ return m_fTargetDistance; }
	FLOAT			Get_Angle()						{ return m_fAngle; }
	PathNode*		Get_Path()						{ return m_pPath; }

	CCharactor* Get_MonsterPart()
	{
		return m_pBox;
	}

	VOID CreateAttackBoundBox();
	VOID BreakCubeAll();
	VOID BreakNockdown();
	VOID EnableShadow( BOOL bEnable );

private:
	VOID AniInterpolation();

	VOID AnimationRotate( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime );
	VOID AnimationTrans( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime );
	VOID AnimationTotalRotate( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime );
	VOID AnimationTotalTrans( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime );

	VOID InterpolationTotalTrans( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime );
	VOID InterpolationTotalRotate( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime );
	VOID InterpolationTrans( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime );
	VOID InterpolationRotate( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime );


	//���� ����
	INT	  m_iMonsterNumber;				///<
	INT	  m_iCharEditorMax;				///< ���� �ڽ� �ִ� ����
	INT	  m_iSelectedCharEditor;		///< ���� �������� �ڽ�
	FLOAT m_fBoxSize;					///< �ڽ� ũ��
	FLOAT m_fBoxSizeDiv;				///< (�ڽ� ũ�� -1) / 2
	INT	  m_iFrameMax;					///< ������ �ִ� ����
	INT	  m_iSelectedFrameNum;			///< ���� �������� ������ �ѹ�
	CCharactor* m_pBox;					///< ĳ���� �ڽ�

	// AI ����
	StateMachine<CMonster>* m_pStateMachine;

	FLOAT		m_fTime;
	FLOAT		m_fInterpolationTime;

	// Dash
	D3DXVECTOR3 m_vDashStartPos;
	D3DXVECTOR3 m_vDashEndPos;

	// Chase
	PathNode*	m_pChaseNextPath;
	D3DXVECTOR3 m_vChaseCurrentPos;
	D3DXVECTOR3 m_vChasePreviousPos;
	INT			m_iChaseCurrentX;
	INT			m_iChaseCurrentZ;
	INT			m_iChaseTotalPathCnt;
	FLOAT		m_fChaseCurrentAngle;
	FLOAT		m_fChaseNextAngle;
	FLOAT		m_fChaseAngle0;
	FLOAT		m_fChaseAngle1;

	// Ÿ��
	INT			m_iTarget;
	INT			m_iTargetPos[2];
	D3DXVECTOR3 m_vTargetPos;
	FLOAT		m_fTargetDistance;

	// Path
	PathNode*	m_pPath;


	struct BoxData
	{
		D3DXVECTOR3 m_fCharEditorRotate;	///< �ڽ� ȸ����
		D3DXVECTOR3 m_fCharEditorTranslate;	///< �ڽ� �̵���

		BOOL		m_bAnimation;

		D3DXVECTOR3	m_vAniRotateStartValue;
		D3DXVECTOR3	m_vAniRotateEndValue;
		D3DXVECTOR3	m_vAniRotateSpeed;
		BOOL		m_bRotateXReplay;
		BOOL		m_bRotateYReplay;
		BOOL		m_bRotateZReplay;

		D3DXVECTOR3	m_vAniTransStartValue;
		D3DXVECTOR3	m_vAniTransEndValue;
		D3DXVECTOR3	m_vAniTransSpeed;
		BOOL		m_bTransXReplay;
		BOOL		m_bTransYReplay;
		BOOL		m_bTransZReplay;

		FLOAT		m_fTrans[3];
		BOOL		m_bTransReplay[3];
		FLOAT		m_fRotation[3];
		BOOL		m_bRotReplay[3];
		BOOL		m_bAniRotateEndCheck[3];
		
		BOOL		m_bAniTransEndCheck[3];
		D3DXVECTOR3 m_vAniTransSave;
		D3DXVECTOR3 m_vAniRotateSave;

		BoxData()
		{
			m_fCharEditorRotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_fCharEditorTranslate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			m_bAnimation = FALSE;

			m_vAniRotateStartValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vAniRotateEndValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vAniRotateSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_bRotateXReplay = FALSE;
			m_bRotateYReplay = FALSE;
			m_bRotateZReplay = FALSE;

			m_vAniTransStartValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vAniTransEndValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vAniTransSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_bTransXReplay = FALSE;
			m_bTransYReplay = FALSE;
			m_bTransZReplay = FALSE;

			ZeroMemory( m_fTrans, sizeof(m_fTrans) );
			ZeroMemory( m_fRotation, sizeof(m_fRotation) );
			ZeroMemory( m_bTransReplay, sizeof(m_bTransReplay) );
			ZeroMemory( m_bRotReplay, sizeof(m_bRotReplay) );
			ZeroMemory( m_bAniRotateEndCheck, sizeof(m_bAniRotateEndCheck) );
			ZeroMemory( m_bAniTransEndCheck, sizeof(m_bAniTransEndCheck) );
		}
	};

	struct Frame
	{

		BOOL		m_bAnimation;

		D3DXVECTOR3 m_fCharEditorRotate;	///< �ڽ� ȸ����
		D3DXVECTOR3 m_fCharEditorTranslate;	///< �ڽ� �̵���

		D3DXVECTOR3	m_vAniRotateStartValue;
		D3DXVECTOR3	m_vAniRotateEndValue;
		D3DXVECTOR3	m_vAniRotateSpeed;
		BOOL		m_bRotateXReplay;
		BOOL		m_bRotateYReplay;
		BOOL		m_bRotateZReplay;

		D3DXVECTOR3	m_vAniTransStartValue;
		D3DXVECTOR3	m_vAniTransEndValue;
		D3DXVECTOR3	m_vAniTransSpeed;
		BOOL		m_bTransXReplay;
		BOOL		m_bTransYReplay;
		BOOL		m_bTransZReplay;

		FLOAT		m_fTrans[3];
		BOOL		m_bTransReplay[3];
		FLOAT		m_fRotation[3];
		BOOL		m_bRotReplay[3];
		BOOL		m_bAniRotateEndCheck[3];
		
		BOOL		m_bAniTransEndCheck[3];
		D3DXVECTOR3 m_vAniTransSave;
		D3DXVECTOR3 m_vAniRotateSave;

		BoxData*	m_pBoxData;

		Frame() : m_pBoxData(NULL) 
		{
			m_bAnimation = FALSE;

			m_fCharEditorRotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_fCharEditorTranslate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			m_vAniRotateStartValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vAniRotateEndValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vAniRotateSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_bRotateXReplay = FALSE;
			m_bRotateYReplay = FALSE;
			m_bRotateZReplay = FALSE;

			m_vAniTransStartValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vAniTransEndValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_vAniTransSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_bTransXReplay = FALSE;
			m_bTransYReplay = FALSE;
			m_bTransZReplay = FALSE;

			ZeroMemory( m_fTrans, sizeof(m_fTrans) );
			ZeroMemory( m_fRotation, sizeof(m_fRotation) );
			ZeroMemory( m_bTransReplay, sizeof(m_bTransReplay) );
			ZeroMemory( m_bRotReplay, sizeof(m_bRotReplay) );
			ZeroMemory( m_bAniRotateEndCheck, sizeof(m_bAniRotateEndCheck) );
			ZeroMemory( m_bAniTransEndCheck, sizeof(m_bAniTransEndCheck) );
		}
	};

	Frame* m_pFrame;

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 ����̽�

	D3DXVECTOR3 m_vPreControl;			  ///< ���� ���� ��ġ
	D3DXVECTOR3 m_vControl;				  ///< ���� ��ġ
	D3DXVECTOR3 m_vColissionControl;      ///< ���� �浹�� ��ġ
	FLOAT		m_fAngle;				  ///< ���� ����
	D3DXVECTOR3 m_vFowardVector;		  /// ���� ���� �̵� ����
	D3DXVECTOR3 m_vSideStepVector;		  /// ���� �¿� �̵� ����

	D3DXMATRIXA16 m_matControl;           ///< ���� ���

	//CBoundBox m_pBoundBox;

	// ���� �ִϸ��̼ǿ� ������
	INT m_iChangeAnimationEndCheck;		  ///< ���� �ִϸ��̼� ������ üũ��
	INT m_iChangeAnimationEndCheck2;
	INT m_iNextFrame;					  ///< ���� �Ѿ ������ ��ȣ
	BOOL m_bChangingAnimation;			  ///< ���� �ִϸ��̼� ������ üũ
	FLOAT m_fMaxInterpolationLength;	  ///< ���� �ӵ� ���� �ִ� �Ÿ�

	FLOAT m_fLengthRotate;				  ///< ���� ȸ���� �ӽ�
	FLOAT m_fSpeedRotate;				  ///< ���� ȸ���� �ӽ�

	BOOL m_bAnimationEndCheck;			  ///< �ִϸ��̼� ������ üũ FALSE = ����

};

#endif