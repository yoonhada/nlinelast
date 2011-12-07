#ifndef _Monster_H_
#define _Monster_H_

#include "StateMachine.h"

class CCharactor;

class CMonster : public CObjectSRT
{
public:
	// 애니메이션 번호
	enum { ANIM_MOVE = 0, ANIM_SPIN_ATTACK = 1, ANIM_MELEE_ATTACK = 2, ANIM_STAND = 3,
		   ANIM_EVENT = 4, ANIM_DASH = 5, ANIM_SLIDING = 6, ANIM_MELEE_ATTACK2 = 7, };

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

	// 애니메이션 변경용
	VOID InitAniAndState();
	VOID ChangeAnimation( INT a_iAniNum );
	
	// 보간 애니메이션 끝낫나 체크 얻어옴 ( FALSE = 끝남 TRUE = 안끝남 )
	BOOL Get_ChangingAnimation()
	{
		return m_bChangingAnimation;
	}

	// 무반복 애니메이션 끝낫나 체크 얻어옴 ( FALSE = 끝남 TRUE = 안끝남 )
	BOOL Get_AnimationEndCheck()
	{
		return m_bAnimationEndCheck;
	}

	// Set
	VOID Set_MonsterNumber( INT iMonsterNumber )	{ m_iMonsterNumber = iMonsterNumber; }
	VOID Set_Angle( FLOAT a_fAngle )				{ m_fAngle = a_fAngle; }
	VOID Set_Pos( D3DXVECTOR3& a_vPos )				{ m_vControl = a_vPos; }
	VOID Set_Target( INT a_iTarget )				{ m_iTarget = a_iTarget; }
	VOID Set_TargetPos( INT a_iX, INT a_iZ )		{ m_iTargetPos[0] = a_iX; m_iTargetPos[1] = a_iZ; }
	VOID Set_TargetPos( D3DXVECTOR3 a_vPos )		{ m_vTargetPos = a_vPos; }
	VOID Set_TargetUnitVector( D3DXVECTOR3 a_vUnit ) { m_vTargetUnitVector = a_vUnit; }
	VOID Set_TargetDistance( FLOAT a_fDistance )	{ m_fTargetDistance = a_fDistance; }
	VOID Set_iSelectedFrameNum( INT a_iAniNum )		{ m_iSelectedFrameNum = a_iAniNum; }
	VOID Set_Path( PathNode* a_pPath )				{ m_pPath = a_pPath; }

	// Get
	INT Get_MonsterNumber()					{ return m_iMonsterNumber; }
	StateMachine<CMonster>* GetFSM() const	{ return m_pStateMachine; }
	D3DXVECTOR3& Get_Pos()					{ return m_vControl; }
	INT Get_Target()						{ return m_iTarget; }
//	INT* Get_TargetPos()					{ return m_iTargetPos; }
	D3DXVECTOR3& Get_TargetPos()			{ return m_vTargetPos; }
	D3DXVECTOR3& Get_TargetUnitVector()		{ return m_vTargetUnitVector; }
	INT Get_iSelectedFrameNum()				{ return m_iSelectedFrameNum; }
	FLOAT Get_TargetDistance()				{ return m_fTargetDistance; }
	FLOAT Get_Angle()						{ return m_fAngle; }
	PathNode* Get_Path()					{ return m_pPath; }

	CCharactor* Get_MonsterPart()
	{
		return m_pBox;
	}

	VOID CreateAttackBoundBox();
	VOID BreakCubeAll();
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


	//몬스터 관리
	INT	  m_iMonsterNumber;				///<
	INT	  m_iCharEditorMax;				///< 몬스터 박스 최대 갯수
	INT	  m_iSelectedCharEditor;		///< 현재 선택중인 박스
	FLOAT m_fBoxSize;					///< 박스 크기
	FLOAT m_fBoxSizeDiv;				///< (박스 크기 -1) / 2
	INT	  m_iFrameMax;					///< 프레임 최대 갯수
	INT	  m_iSelectedFrameNum;			///< 현재 선택중인 프레임 넘버
	CCharactor* m_pBox;					///< 캐릭터 박스

	// AI 관리
	StateMachine<CMonster>* m_pStateMachine;

	// 타겟
	INT			m_iTarget;
	INT			m_iTargetPos[2];
	D3DXVECTOR3 m_vTargetPos;
	D3DXVECTOR3 m_vTargetUnitVector;
	FLOAT		m_fTargetDistance;

	// Path
	PathNode*	m_pPath;


	struct BoxData
	{
		D3DXVECTOR3 m_fCharEditorRotate;	///< 박스 회전값
		D3DXVECTOR3 m_fCharEditorTranslate;	///< 박스 이동값

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

		D3DXVECTOR3 m_fCharEditorRotate;	///< 박스 회전값
		D3DXVECTOR3 m_fCharEditorTranslate;	///< 박스 이동값

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

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스

	D3DXVECTOR3 m_vPreControl;			  ///< 몬스터 이전 위치
	D3DXVECTOR3 m_vControl;				  ///< 몬스터 위치
	D3DXVECTOR3 m_vColissionControl;      ///< 몬스터 충돌용 위치
	FLOAT		m_fAngle;				  ///< 몬스터 각도
	D3DXVECTOR3 m_vFowardVector;		  /// 몬스터 전진 이동 벡터
	D3DXVECTOR3 m_vSideStepVector;		  /// 몬스터 좌우 이동 벡터

	D3DXMATRIXA16 m_matControl;           ///< 조작 행렬

	//CBoundBox m_pBoundBox;

	// 보간 애니메이션용 변수들
	INT m_iChangeAnimationEndCheck;		  ///< 보간 애니메이션 끝낫나 체크용
	INT m_iChangeAnimationEndCheck2;
	INT m_iNextFrame;					  ///< 다음 넘어갈 프레임 번호
	BOOL m_bChangingAnimation;			  ///< 보간 애니메이션 끝낫나 체크
	FLOAT m_fMaxInterpolationLength;	  ///< 보간 속도 계산용 최대 거리

	FLOAT m_fLengthRotate;				  ///< 보간 회전용 임시
	FLOAT m_fSpeedRotate;				  ///< 보간 회전용 임시

	BOOL m_bAnimationEndCheck;			  ///< 애니메이션 끝낫나 체크 FALSE = 끝남

};

#endif