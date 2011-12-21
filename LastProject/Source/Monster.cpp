#include "stdafx.h"
#include "Monster.h"

#include "Charactor.h"
#include "Weapon.h"

#include "Seek.h"
#include "Wait.h"

// 임시
#include "Qube.h"

CMonster::CMonster()
{
	Clear();
}

CMonster::~CMonster()
{
	Realese();

	// AI : 남아있는 Path를 제거한다.
	Astar::GetInstance()->removePath( m_pPath );
}

VOID CMonster::Clear()
{
	m_pFrame = NULL;
	m_iBreakPart = 0;
	m_iMonsterNumber = 0;
	m_iCharEditorMax = 7;
	m_iSelectedCharEditor=0;
	m_fBoxSize = 15.0f;
	m_fBoxSizeDiv = (m_fBoxSize) * 0.5f;
	m_iFrameMax = 30;
	m_iSelectedFrameNum = ANIM_STAND;
	D3DXMatrixIdentity( &m_matControl );
	m_vPreControl = D3DXVECTOR3(-250.0f, 0.0f, 650.0f);
	m_vControl = D3DXVECTOR3(-250.0f, 0.0f, 650.0f);
	m_vFowardVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vSideStepVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vColissionControl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_iChangeAnimationEndCheck = 0;
	m_iChangeAnimationEndCheck2 = 0;
	m_iNextFrame = 0;
	m_bChangingAnimation = FALSE;
	m_bAnimationEndCheck = FALSE;

	m_iBreakedCubeCnt = 0;
	m_iTotalBreakedCubeCnt = 0;
	m_bGroggyState = FALSE;
	
	m_fTime = 0.0f;
	m_fAttackTime = 0.0f;
	m_fSoundTime = 0.0f;
	m_fInterpolationTime = 0.0f;
	m_iTarget = -1;
	m_iTargetPos[0] = 0;
	m_iTargetPos[1] = 0;
	m_fTargetDistance = 0.0f;

	m_pPath = NULL;
	m_pBox = NULL;
	m_pStateMachine = NULL;
}

HRESULT CMonster::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, WCHAR* a_pFileName )
{
	m_pD3dDevice = a_pD3dDevice;

	m_pFrame = new Frame[m_iFrameMax];
	m_pBox = new CCharactor[m_iCharEditorMax];
	for( INT LoopBox=0; LoopBox<m_iCharEditorMax; ++LoopBox )
	{
		m_pBox[LoopBox].Create( m_pD3dDevice );
		//m_pBox[LoopBox].CreateWeapon( CWeapon::NONE );
		m_pBox[LoopBox].Set_MonsterPart( static_cast<CHAR>(LoopBox) );
	}

	//m_pBoundBox = new CBoundBox(&m_pBox[2]);
	//m_pBoundBox->Create();

	for( INT LoopFrame=0; LoopFrame<m_iFrameMax; ++LoopFrame )
	{
		m_pFrame[LoopFrame].m_pBoxData = new BoxData[m_iCharEditorMax];
	}

	//WCHAR* ptr = wcstok( a_pFileName, L"." );
	Load( a_pFileName );

	//for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	//{
	//	CTree::GetInstance()->GetMonsVector()->push_back( m_pBox[Loop].GetBoundBox() );
	//}

	// AI
	m_pStateMachine = new StateMachine<CMonster>( this );
	//m_pStateMachine->SetCurrentState( Seek::GetInstance() );

	m_pTestCube = new TestCube( a_pD3dDevice );

	return S_OK;
}

VOID CMonster::Realese()
{
	SAFE_DELETE_ARRAY(m_pBox);
	
	if( m_pFrame != NULL )
	{
		for( INT LoopFrame=0; LoopFrame<m_iFrameMax; ++LoopFrame )
		{
			SAFE_DELETE_ARRAY( m_pFrame[LoopFrame].m_pBoxData );
		}

		SAFE_DELETE_ARRAY( m_pFrame );
	}

	//SAFE_DELETE(m_pBoundBox);

	SAFE_DELETE( m_pStateMachine );
}

VOID CMonster::Load( WCHAR* a_pFileName )
{
	FILE* pFile;
	WCHAR wchTemp[255];

	lstrcpy( wchTemp, a_pFileName );
	lstrcat( wchTemp, L".Acsav" );

	pFile = _wfopen( wchTemp, L"r" );

	if( NULL == pFile )
	{
		MessageBox(NULL, L"File Load Error", NULL, MB_OK);
		return;
	}

	fseek( pFile, 0L, SEEK_SET );

	WCHAR szTemp[255];
	//캐릭터 세이브 파일인지 체크
	fwscanf( pFile, L"%s", szTemp );
	if( 0 != lstrcmp( szTemp, L"BKHCharSave" ) )
	{
		MessageBox(NULL, L"Not Charactor Save File", NULL, MB_OK);
		return;
	}

	//박스 갯수
	fwscanf( pFile, L"%d", &m_iCharEditorMax);

	for(INT LoopFrame=0; LoopFrame<m_iFrameMax; ++LoopFrame)
	{
		//프레임 넘버
		fwscanf( pFile, L"%s", szTemp );

		//애니메이션 있나 없나 체크
		fwscanf( pFile, L"%d", &m_pFrame[LoopFrame].m_bAnimation );

		//이동 애니 x
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniTransStartValue.x,
			&m_pFrame[LoopFrame].m_vAniTransEndValue.x,
			&m_pFrame[LoopFrame].m_vAniTransSpeed.x,
			&m_pFrame[LoopFrame].m_bTransXReplay );

		//이동 애니 y
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniTransStartValue.y,
			&m_pFrame[LoopFrame].m_vAniTransEndValue.y,
			&m_pFrame[LoopFrame].m_vAniTransSpeed.y,
			&m_pFrame[LoopFrame].m_bTransYReplay );

		//이동 애니 z
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniTransStartValue.z,
			&m_pFrame[LoopFrame].m_vAniTransEndValue.z,
			&m_pFrame[LoopFrame].m_vAniTransSpeed.z,
			&m_pFrame[LoopFrame].m_bTransZReplay );

		//회전 애니 x
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniRotateStartValue.x,
			&m_pFrame[LoopFrame].m_vAniRotateEndValue.x,
			&m_pFrame[LoopFrame].m_vAniRotateSpeed.x,
			&m_pFrame[LoopFrame].m_bRotateXReplay );

		//각도를 라디안값으로 변경
		m_pFrame[LoopFrame].m_vAniRotateStartValue.x = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateStartValue.x );
		m_pFrame[LoopFrame].m_vAniRotateEndValue.x = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateEndValue.x );

		//회전 애니 y
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniRotateStartValue.y,
			&m_pFrame[LoopFrame].m_vAniRotateEndValue.y,
			&m_pFrame[LoopFrame].m_vAniRotateSpeed.y,
			&m_pFrame[LoopFrame].m_bRotateYReplay );

		//각도를 라디안값으로 변경
		m_pFrame[LoopFrame].m_vAniRotateStartValue.y = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateStartValue.y );
		m_pFrame[LoopFrame].m_vAniRotateEndValue.y = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateEndValue.y );

		//회전 애니 z
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniRotateStartValue.z,
			&m_pFrame[LoopFrame].m_vAniRotateEndValue.z,
			&m_pFrame[LoopFrame].m_vAniRotateSpeed.z,
			&m_pFrame[LoopFrame].m_bRotateZReplay );

		//각도를 라디안값으로 변경
		m_pFrame[LoopFrame].m_vAniRotateStartValue.z = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateStartValue.z );
		m_pFrame[LoopFrame].m_vAniRotateEndValue.z = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateEndValue.z );

		for(INT Loop=0; Loop<m_iCharEditorMax; ++Loop)
		{
			//박스 넘버
			fwscanf( pFile, L"%s", &szTemp );
			//박스 로테이션
			fwscanf( pFile, L"%f %f %f", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.z );

			//각도를 라디안값으로 변경
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.x = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.x );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.y = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.y );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.z = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.z );

			//박스 트랜스레이트
			fwscanf( pFile, L"%f %f %f", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorTranslate.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorTranslate.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorTranslate.z );

			//애니메이션 있나 없나 체크
			fwscanf( pFile, L"%d", &m_pFrame[LoopFrame].m_pBoxData[Loop].m_bAnimation );

			//이동 애니 x
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransStartValue.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransEndValue.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransSpeed.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bTransXReplay );

			//이동 애니 y
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransStartValue.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransEndValue.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransSpeed.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bTransYReplay );

			//이동 애니 z
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransStartValue.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransEndValue.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransSpeed.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bTransZReplay );

			//회전 애니 x
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateSpeed.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bRotateXReplay );

			//각도를 라디안값으로 변경
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.x = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.x );

			//회전 애니 y
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateSpeed.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bRotateYReplay );

			//각도를 라디안값으로 변경
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.y = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.y );

			//회전 애니 z
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateSpeed.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bRotateZReplay );

			//각도를 라디안값으로 변경
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.z = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.z );
		}
	}

	fclose(pFile);

	for(INT Loop=0; Loop<m_iCharEditorMax; ++Loop)
	{
		wsprintf(wchTemp, L"%s_%d.csav", a_pFileName, Loop);
		m_pBox[Loop].Load( wchTemp );
		m_iTotalBreakedCubeCnt += m_pBox[Loop].GetvCubeSize();
		CTree::GetInstance()->GetMonsVector()->push_back( m_pBox[Loop].GetBoundBox() );
	}
}

VOID CMonster::AnimationTotalTrans( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] <= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
			m_bAnimationEndCheck = TRUE;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] >= a_fStart && m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
					m_bAnimationEndCheck = TRUE;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				//m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] = a_fStart;
				m_bAnimationEndCheck = FALSE;
			}
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] >= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
			m_bAnimationEndCheck = TRUE;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] <= a_fStart && m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
					m_bAnimationEndCheck = TRUE;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				//m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] = a_fStart;
				m_bAnimationEndCheck = FALSE;
			}
		}
	}
	else
	{
		m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] = a_fStart;
	}

	switch(a_iXYZ)
	{
	case 0:
		Set_ControlTranslate( 0, m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] );
		break;
	case 1:
		Set_ControlTranslate( 1, m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] );
		break;
	case 2:
		Set_ControlTranslate( 2, m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] );
		break;
	}

}

VOID CMonster::AnimationTotalRotate( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] <= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
			m_bAnimationEndCheck = TRUE;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] >= a_fStart && m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
					m_bAnimationEndCheck = TRUE;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				//m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] = a_fStart;
				m_bAnimationEndCheck = FALSE;
			}
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] >= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
			m_bAnimationEndCheck = TRUE;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] <= a_fStart && m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
					m_bAnimationEndCheck = TRUE;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				//m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] = a_fStart;
				m_bAnimationEndCheck = FALSE;
			}
		}
	}
	//시작과 끝이 같으면 해당 방향으로 계속 회전
	else
	{
		if( 0 != a_fStart )
		{
			if( a_fStart > 0 )
			{
				m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
			}
			//ABSDEF( m_fRotation[a_iXYZ] ) > 6.2831853f ? m_fAngle = 0.0f : NULL;
		}
		// 0이면 초기화
		else
		{
			m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] = 0.0f;
		}
	}

	switch(a_iXYZ)
	{
	case 0:
		Set_ControlRotate( 0, m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		break;
	case 1:
		//Set_ControlRotate( 1, m_fAngle + m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		m_fAngle += m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ];
		break;
	case 2:
		Set_ControlRotate( 2, m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		break;
	}

	////CDebugConsole::GetInstance()->Messagef( L"Total X rotation : %f\n", m_pFrame[m_iSelectedFrameNum].m_fRotation[0] );
	////CDebugConsole::GetInstance()->Messagef( L"Total Y rotation : %f\n", m_pFrame[m_iSelectedFrameNum].m_fRotation[1] );
	////CDebugConsole::GetInstance()->Messagef( L"Total Z rotation : %f\n", m_pFrame[m_iSelectedFrameNum].m_fRotation[2] );

}

VOID CMonster::AnimationTrans( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] <= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
			m_bAnimationEndCheck = TRUE;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] >= a_fStart && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
					m_bAnimationEndCheck = TRUE;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				//m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] = a_fStart;
				m_bAnimationEndCheck = FALSE;
			}
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] >= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
			m_bAnimationEndCheck = TRUE;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] <= a_fStart && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
					m_bAnimationEndCheck = TRUE;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				//m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] = a_fStart;
				m_bAnimationEndCheck = FALSE;
			}
		}
	}
	else
	{
		m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] = a_fStart;
	}

	switch(a_iXYZ)
	{
	case 0:
		m_pBox[a_iLoopNum].Set_ControlTranslate( 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorTranslate.x + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] );
		break;
	case 1:
		m_pBox[a_iLoopNum].Set_ControlTranslate( 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorTranslate.y + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] );
		break;
	case 2:
		m_pBox[a_iLoopNum].Set_ControlTranslate( 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorTranslate.z + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] );
		break;
	}

}

VOID CMonster::AnimationRotate( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] <= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
			m_bAnimationEndCheck = TRUE;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] >= a_fStart && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
					m_bAnimationEndCheck = TRUE;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				//m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] = a_fStart;
				m_bAnimationEndCheck = FALSE;
			}
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] >= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
			m_bAnimationEndCheck = TRUE;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] <= a_fStart && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
					m_bAnimationEndCheck = TRUE;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				//m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] =  a_fStart;
				m_bAnimationEndCheck = FALSE;
			}
		}
	}
	//시작과 끝이 같으면 해당 방향으로 계속 회전
	else
	{
		if( 0 != a_fStart )
		{
			if( a_fStart > 0 )
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
			}
		}
		// 0이면 초기화
		else
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] = 0.0f;
		}
	}

	switch(a_iXYZ)
	{
	case 0:
		m_pBox[a_iLoopNum].Set_ControlRotate( 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorRotate.x + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] );
		break;
	case 1:
		m_pBox[a_iLoopNum].Set_ControlRotate( 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorRotate.y + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] );
		break;
	case 2:
		m_pBox[a_iLoopNum].Set_ControlRotate( 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorRotate.z + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] );
		break;
	}
}

VOID CMonster::InterpolationTotalTrans( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] <= a_fEnd )
		{
			// 애니메이션 안끝낫다
			m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			// 애니메이션 끝낫다
			m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] >= a_fEnd )
		{
			// 애니메이션 안끝낫다
			m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			// 애니메이션 끝낫다
			m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
		}
	}
	//시작과 끝이 같으면
	else
	{
		// 애니메이션 끝낫다
		m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
	}

	switch(a_iXYZ)
	{
	case 0:
		Set_ControlTranslate( 0, m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] );
		break;
	case 1:
		Set_ControlTranslate( 1, m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] );
		break;
	case 2:
		Set_ControlTranslate( 2, m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] );
		break;
	}
}

VOID CMonster::InterpolationTotalRotate( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] < a_fEnd )
		{
			// 애니메이션 안끝낫다
			m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			// 애니메이션 끝낫다
			m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] > a_fEnd )
		{
			// 애니메이션 안끝낫다
			m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			// 애니메이션 끝낫다
			m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
		}
	}
	//시작과 끝이 같으면
	else
	{
		// 애니메이션 끝낫다
		m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
	}

	switch(a_iXYZ)
	{
	case 0:
		Set_ControlRotate( 0, m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		break;
	case 1:
		//Set_ControlRotate( 1, m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		m_fAngle += m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ];
		break;
	case 2:
		Set_ControlRotate( 2, m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		break;
	}
	
}

VOID CMonster::InterpolationTrans( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//FLOAT fLength = ABSDEF( ABSDEF(a_fStart) - ABSDEF(a_fEnd) );
	//FLOAT fSpeed = m_fMaxInterpolationLength / fLength;

	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] <= a_fEnd )
		{
			// 애니메이션 안끝낫다
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			// 애니메이션 끝낫다
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] >= a_fEnd )
		{
			// 애니메이션 안끝낫다
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			// 애니메이션 끝낫다
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
		}
	}
	//시작과 끝이 같으면
	else
	{
		// 애니메이션 끝낫다
		m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
	}

	switch(a_iXYZ)
	{
	case 0:
		m_pBox[a_iLoopNum].Set_ControlTranslate( 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorTranslate.x + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] );
		break;
	case 1:
		m_pBox[a_iLoopNum].Set_ControlTranslate( 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorTranslate.y + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] );
		break;
	case 2:
		m_pBox[a_iLoopNum].Set_ControlTranslate( 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorTranslate.z + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] );
		break;
	}
}

VOID CMonster::InterpolationRotate( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//m_fLengthRotate = ABSDEF( ABSDEF(a_fStart) + ABSDEF(a_fEnd) );
	//m_fSpeedRotate = m_fMaxInterpolationLength / m_fLengthRotate;

	////CDebugConsole::GetInstance()->Messagef( L"fLen : %f / interLen : %f / fspeed : %f\n", fLength, m_fMaxInterpolationLength, fSpeed );

	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] <= a_fEnd )
		{
			// 애니메이션 안끝낫다
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			// 애니메이션 끝낫다
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] >= a_fEnd )
		{
			// 애니메이션 안끝낫다
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			// 애니메이션 끝낫다
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
		}
	}
	//시작과 끝이 같으면
	else
	{
		// 애니메이션 끝낫다
		m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
	}

	switch(a_iXYZ)
	{
	case 0:
		m_pBox[a_iLoopNum].Set_ControlRotate( 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorRotate.x + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] );
		break;
	case 1:
		m_pBox[a_iLoopNum].Set_ControlRotate( 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorRotate.y + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] );
		break;
	case 2:
		m_pBox[a_iLoopNum].Set_ControlRotate( 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fCharEditorRotate.z + m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] );
		break;
	}

	////CDebugConsole::GetInstance()->Messagef( L"a_iLoopNum : %d / Rotation XYZ : %d / EndCheck : %d\n", a_iLoopNum, a_iXYZ, m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] );
}

VOID CMonster::UpdateByValue( D3DXVECTOR3& a_vControl, FLOAT a_fAngle )
{
	m_fAngle += a_fAngle;
	// 360도 넘으면 라디언 0으로 초기화
	ABSDEF( m_fAngle ) > 6.2831853f ? m_fAngle = 0.0f : NULL;

	m_vColissionControl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//m_vControl;
	// 전진 후진 처리
	if( a_vControl.z != 0 )
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle );
		m_vFowardVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vFowardVector, &m_vFowardVector);
		m_vColissionControl = (m_vFowardVector * a_vControl.z);	
	}
	// 좌우 처리
	if( a_vControl.x != 0)
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle + 1.5707963f );
		m_vSideStepVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vSideStepVector, &m_vSideStepVector);
		m_vColissionControl += (m_vSideStepVector * a_vControl.x);
	}
	////CDebugConsole::GetInstance()->Messagef( L"CharCollision Pos: %f %f %f\n", m_vColissionControl.x, m_vColissionControl.y, m_vColissionControl.z );

	//BOOL bCol = FALSE;
	//for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	//{
	//	//m_pBox[Loop].GetBoundBox()->SetAngle( m_pBox[Loop].Get_CharaAngle() );
	//	if( m_pBox[Loop].Collision( m_vColissionControl ) == TRUE )
	//	{
	//		bCol = TRUE;
	//		////CDebugConsole::GetInstance()->Messagef( L"Loop: %d bCol: %d\n ", Loop, bCol );
	//		//m_vColissionControl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//		
	//		////CDebugConsole::GetInstance()->Messagef( L"Char Pos: %f %f %f\n", m_pBox[Loop].Get_CharaPos().x, m_pBox[Loop].Get_CharaPos().y, m_pBox[Loop].Get_CharaPos().z );
	//		break;
	//	}
	//}
	
	INT nCout, Loop = 0;
	BOOL bCollision = FALSE;
	D3DXMATRIXA16 mat = Get_MatWorld();
	for( Loop = 0; Loop < m_iCharEditorMax; ++Loop )
	{
		if ( m_pBox[Loop].CollisionAtk( ) )
		{
			nCout = m_pBox[Loop].BreakQube( mat );
			if ( nCout )
			{
				CGameEvent::GetInstance()->Set_PlayerIndex( CObjectManage::GetInstance()->Get_CharTable( CObjectManage::GetInstance()->Get_ClientNumber() ) );
				CGameEvent::GetInstance()->Set_MonsterIndex( m_iMonsterNumber );
				m_iBreakedCubeCnt += nCout;

				bCollision = TRUE;
			}
		}
	}

	// 몬스터 피격됨
	if( bCollision )
	{
		CSound::GetInstance()->PlayEffect( CSound::EFFECT_HIT );

		// 판다
		if( m_iMonsterNumber == 0 || m_iMonsterNumber == 1 )
		{
			if( FastRand2() < 0.25f )
			{
				CSound::GetInstance()->PlayEffect( CSound::EFFECT_PANDA_DAMAGED1 );
			}
		}
		// 삐에로
		else if( m_iMonsterNumber == 2 )
		{
			if( FastRand2() < 0.25f )
			{
				CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_DAMAGED1 + rand() % 4 );
			}
		}
	}

	CObjectManage::GetInstance()->Send_NetworkSendDestroyData( TRUE, m_iMonsterNumber );
/*
	CDebugInterface::GetInstance()->AddMessageFloat( "MonsterAngle", m_fAngle );

	static FLOAT fTemp = 11.0f;
	CDebugInterface::GetInstance()->AddMessageFloat( "TempAngle", fTemp );
*/
	m_vPreControl = m_vControl;
	m_vControl += m_vColissionControl;
/*
	CDebugInterface::GetInstance()->AddMessageVector( "MonsterPos", m_vControl );

	static D3DXVECTOR3 Temp(10.0f,10.0f,10.f);
	CDebugInterface::GetInstance()->AddMessageVector( "TempPos", Temp );
*/
	Set_ControlTranslate( 0, m_vControl.x );
	Set_ControlTranslate( 1, m_vControl.y );
	Set_ControlTranslate( 2, m_vControl.z );
	Set_ControlRotate( 1, m_fAngle );
	Calcul_MatWorld();

	if ( m_iBreakedCubeCnt > ( m_iTotalBreakedCubeCnt * 5 ) >> 3 )
	{				
		CGameEvent::GetInstance()->SetDie( m_iMonsterNumber );
	}
}

VOID CMonster::ChangeAnimation( INT a_iAniNum )
{
	m_bChangingAnimation = TRUE;
	m_iNextFrame = a_iAniNum;

	D3DXVECTOR3 vTransLength(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vRotateLength(0.0f, 0.0f, 0.0f);

	m_pFrame[m_iSelectedFrameNum].m_vAniTransSave.x = m_pFrame[m_iSelectedFrameNum].m_fTrans[0];
	m_pFrame[m_iSelectedFrameNum].m_vAniTransSave.y = m_pFrame[m_iSelectedFrameNum].m_fTrans[1];
	m_pFrame[m_iSelectedFrameNum].m_vAniTransSave.z = m_pFrame[m_iSelectedFrameNum].m_fTrans[2];

	m_pFrame[m_iSelectedFrameNum].m_vAniRotateSave.x = m_pFrame[m_iSelectedFrameNum].m_fRotation[0];
	m_pFrame[m_iSelectedFrameNum].m_vAniRotateSave.y = m_fAngle;// + m_pFrame[m_iSelectedFrameNum].m_fRotation[1];
	m_pFrame[m_iSelectedFrameNum].m_vAniRotateSave.z = m_pFrame[m_iSelectedFrameNum].m_fRotation[2];

	//for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	//{
	//	m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.x = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[0];
	//	m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.y = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[1];
	//	m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.z = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[2];

	//	m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.x = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[0];
	//	m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.y = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[1];
	//	m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.z = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[2];
	//	
	//	vTransLength.x = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.x ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.x - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.x ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.x );
	//	vTransLength.y = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.y ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.y - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.y ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.y );
	//	vTransLength.z = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.z ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.z - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.z ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.z );

	//	vRotateLength.x = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.x ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.x - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.x ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x );
	//	vRotateLength.y = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.y ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.y - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.y ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y );
	//	vRotateLength.z = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.z ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.z - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.z ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z );

	//	vTransLength.x = ABSDEF(vTransLength.x);
	//	vTransLength.y = ABSDEF(vTransLength.y);
	//	vTransLength.z = ABSDEF(vTransLength.z);

	//	vRotateLength.x = ABSDEF(vRotateLength.x);
	//	vRotateLength.y = ABSDEF(vRotateLength.y);
	//	vRotateLength.z = ABSDEF(vRotateLength.z);
	//	
	//	if( m_fMaxInterpolationLength < vTransLength.x )
	//	{
	//		m_fMaxInterpolationLength = vTransLength.x;
	//	}

	//	if( m_fMaxInterpolationLength < vTransLength.y )
	//	{
	//		m_fMaxInterpolationLength = vTransLength.y;
	//	}

	//	if( m_fMaxInterpolationLength < vTransLength.z )
	//	{
	//		m_fMaxInterpolationLength = vTransLength.z;
	//	}

	//	if( m_fMaxInterpolationLength < vRotateLength.x )
	//	{
	//		m_fMaxInterpolationLength = vRotateLength.x;
	//		//if( m_fMaxInterpolationLength > 10.0f ) //CDebugConsole::GetInstance()->Messagef( L" RotLenX : %f \n ", m_fMaxInterpolationLength );
	//	}

	//	if( m_fMaxInterpolationLength < vRotateLength.y )
	//	{
	//		m_fMaxInterpolationLength = vRotateLength.y;
	//		//if( m_fMaxInterpolationLength > 10.0f ) //CDebugConsole::GetInstance()->Messagef( L" RotLenY : %f \n ", m_fMaxInterpolationLength );
	//	}

	//	if( m_fMaxInterpolationLength < vRotateLength.z )
	//	{
	//		m_fMaxInterpolationLength = vRotateLength.z;
	//		//if( m_fMaxInterpolationLength > 10.0f ) //CDebugConsole::GetInstance()->Messagef( L" RotLenZ : %f \n ", m_fMaxInterpolationLength );
	//	}
	//}

	
}

VOID CMonster::AniInterpolation()
{

	////CDebugConsole::GetInstance()->Messagef( L"InterLength : %f \n", m_fMaxInterpolationLength );

	m_iChangeAnimationEndCheck = 0;
	m_iChangeAnimationEndCheck2 = 0;
	FLOAT Speed = 6.0f;
	if( m_pFrame[m_iSelectedFrameNum].m_bAnimation == TRUE )
	{

		InterpolationTotalTrans( 0, m_pFrame[m_iSelectedFrameNum].m_vAniTransSave.x, 
			m_pFrame[m_iNextFrame].m_vAniTransStartValue.x, 
			Speed, 
			FALSE, 
			CFrequency::GetInstance()->getFrametime() );

		InterpolationTotalTrans( 1, m_pFrame[m_iSelectedFrameNum].m_vAniTransSave.y, 
			m_pFrame[m_iNextFrame].m_vAniTransStartValue.y, 
			Speed, 
			FALSE, 
			CFrequency::GetInstance()->getFrametime() );

		InterpolationTotalTrans( 2, m_pFrame[m_iSelectedFrameNum].m_vAniTransSave.z, 
			m_pFrame[m_iNextFrame].m_vAniTransStartValue.z, 
			Speed, 
			FALSE, 
			CFrequency::GetInstance()->getFrametime() );

		InterpolationTotalRotate( 0, m_pFrame[m_iSelectedFrameNum].m_vAniRotateSave.x, 
			m_pFrame[m_iNextFrame].m_vAniRotateStartValue.x, 
			Speed, 
			FALSE, 
			CFrequency::GetInstance()->getFrametime() );

		InterpolationTotalRotate( 1, m_pFrame[m_iSelectedFrameNum].m_vAniRotateSave.y, 
			m_pFrame[m_iNextFrame].m_vAniRotateStartValue.y, 
			Speed, 
			FALSE, 
			CFrequency::GetInstance()->getFrametime() );

		InterpolationTotalRotate( 2, m_pFrame[m_iSelectedFrameNum].m_vAniRotateSave.z, 
			m_pFrame[m_iNextFrame].m_vAniRotateStartValue.z, 
			Speed, 
			FALSE, 
			CFrequency::GetInstance()->getFrametime() );
	}

	m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[0];
	m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[1];
	m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[2];

	m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[0];
	m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[1];
	m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[2];

	m_iChangeAnimationEndCheck2 += m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[0];
	m_iChangeAnimationEndCheck2 += m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[1];
	m_iChangeAnimationEndCheck2 += m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[2];

	m_iChangeAnimationEndCheck2 += m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[0];
	m_iChangeAnimationEndCheck2 += m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[1];
	m_iChangeAnimationEndCheck2 += m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[2];

	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{
		m_pBox[Loop].Set_ControlTranslate( 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.x );
		m_pBox[Loop].Set_ControlTranslate( 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.y );
		m_pBox[Loop].Set_ControlTranslate( 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.z );
		m_pBox[Loop].Set_ControlRotate( 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.x );
		m_pBox[Loop].Set_ControlRotate( 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.y );
		m_pBox[Loop].Set_ControlRotate( 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.z );

		m_pBox[Loop].UpdateMonsterPos( m_vPreControl + m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate, 
			m_vControl + m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate, m_fAngle );

		if( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bAnimation == TRUE )
		{
			InterpolationTrans( Loop, 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.x, 
				( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.x - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.x ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.x, 
				Speed, 
				FALSE, 
				CFrequency::GetInstance()->getFrametime() );

			InterpolationTrans( Loop, 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.y, 
				( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.y - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.y ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.y, 
				Speed, 
				FALSE, 
				CFrequency::GetInstance()->getFrametime() );

			InterpolationTrans( Loop, 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.z, 
				( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.z - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.z ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.z, 
				Speed,
				FALSE, 
				CFrequency::GetInstance()->getFrametime() );

			InterpolationRotate( Loop, 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.x, 
				( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.x - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.x ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x, 
				Speed,
				FALSE, 
				CFrequency::GetInstance()->getFrametime() );

			InterpolationRotate( Loop, 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.y, 
				( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.y - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.y ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y, 
				Speed,
				FALSE, 
				CFrequency::GetInstance()->getFrametime() );

			InterpolationRotate( Loop, 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.z, 
				( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.z - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.z ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z, 
				Speed,
				FALSE, 
				CFrequency::GetInstance()->getFrametime() );
		}

		//m_pBox[Loop].Update();
		m_pBox[Loop].Calcul_MatWorld();
		m_pBox[Loop].UpdateMonsterMatrix( Get_MatWorld() );

		m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bAniRotateEndCheck[0];
		m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bAniRotateEndCheck[1];
		m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bAniRotateEndCheck[2];

		m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bAniTransEndCheck[0];
		m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bAniTransEndCheck[1];
		m_iChangeAnimationEndCheck += m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bAniTransEndCheck[2];
	}

	if( m_iChangeAnimationEndCheck == 0 )
	{
		m_iSelectedFrameNum = m_iNextFrame;
		m_bChangingAnimation = FALSE;

		m_pFrame[m_iSelectedFrameNum].m_fTrans[0] = m_pFrame[m_iSelectedFrameNum].m_vAniTransStartValue.x;
		m_pFrame[m_iSelectedFrameNum].m_fTrans[1] = m_pFrame[m_iSelectedFrameNum].m_vAniTransStartValue.y;
		m_pFrame[m_iSelectedFrameNum].m_fTrans[2] = m_pFrame[m_iSelectedFrameNum].m_vAniTransStartValue.z;

		m_pFrame[m_iSelectedFrameNum].m_fRotation[0] = m_pFrame[m_iSelectedFrameNum].m_vAniRotateStartValue.x;
		m_pFrame[m_iSelectedFrameNum].m_fRotation[1] = m_pFrame[m_iSelectedFrameNum].m_vAniRotateStartValue.y;
		m_pFrame[m_iSelectedFrameNum].m_fRotation[2] = m_pFrame[m_iSelectedFrameNum].m_vAniRotateStartValue.z;

		for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[0] = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransStartValue.x;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[1] = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransStartValue.y;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[2] = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransStartValue.z;

			m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[0] = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateStartValue.x;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[1] = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateStartValue.y;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[2] = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateStartValue.z;
		}
	}
}

VOID CMonster::CreateAttackBoundBox()
{
	// 애니메이션이 끝났고 보간 애니메이션중이 아니라면
	//if( m_bAnimationEndCheck == FALSE && m_iChangeAnimationEndCheck == FALSE )
	//{
		//프레임에 해당 하는 바운드 박스 생성
		switch( m_iSelectedFrameNum )
		{
		case ANIM_MELEE_ATTACK:			// 양손
		case ANIM_MELEE_ATTACK2:		// 오른손
		case ANIM_MELEE_ATTACK3:		// 왼쪽
		case ANIM_COMBO_ATTACK3:
		case ANIM_SLIDING:				// 슬라이딩

		case ANIM_MELEE_ATTACK4:		// 발차기
		case ANIM_SPIN_ATTACK:			// 회전		
		case ANIM_COMBO_ATTACK1:		// 연속기
		case ANIM_COMBO_ATTACK2:
		case ANIM_SENPUKYAKU:
			{
				// QubeATKEffect( Get_Pos() );
				// 몬스터 공격 바운드 박스.
				CBoundBox * m_pBBx = new CBoundBox;
				D3DXVECTOR3 vec( 0.0f, 0.0f, -25.0f );
				D3DXVec3TransformCoord( &vec, &vec, &Get_MatWorld() );
				vec.y = 0.1f;
				QubeATKEffect( vec );
				m_pBBx->SetPosition( vec );
				//m_pBBx->SetAngle( 0.0f, Get_Angle(), 0.0f );

				m_pBBx->SetSize( 0, -25.0f );
				m_pBBx->SetSize( 1, -25.0f );
				m_pBBx->SetSize( 2, -25.0f );

				m_pBBx->SetSize( 3,  25.0f );
				m_pBBx->SetSize( 4,  25.0f );
				m_pBBx->SetSize( 5,  25.0f );

				m_pBBx->SetDirection( D3DXVECTOR3( 0.0f, -1.3f, -1.0f ) );

				CTree::GetInstance()->GetMonsAtkVector()->push_back( m_pBBx );
			}
			break;

		case ANIM_ROCKETPUNCH:
			{
				// 몬스터 공격 바운드 박스.
				CBoundBox * m_pBBx = new CBoundBox;
				D3DXVECTOR3 vec( 0.0f, 0.0f, -70.0f );
				vec += Get_Pos();
				D3DXVec3TransformCoord( &vec, &vec, &Get_MatWorld() );
				
				m_pBBx->SetPosition( vec );
				//m_pBBx->SetAngle( 0.0f, Get_Angle(), 0.0f );

				m_pBBx->SetSize( 0, -25.0f );
				m_pBBx->SetSize( 1, -25.0f );
				m_pBBx->SetSize( 2, -25.0f );

				m_pBBx->SetSize( 3, 25.0f );
				m_pBBx->SetSize( 4, 25.0f );
				m_pBBx->SetSize( 5, 25.0f );

				m_pBBx->SetDirection( D3DXVECTOR3( 0.0f, -1.3f, -1.0f ) );

				CTree::GetInstance()->GetMonsAtkVector()->push_back( m_pBBx );
			}
			break;
		}
//	}
}

VOID CMonster::Update()
{
	//CDebugConsole::GetInstance()->Messagef( L"ChangingAnimation : %d\n", m_bChangingAnimation );
	//CDebugConsole::GetInstance()->Messagef( L"AnimationEndCheck : %d\n", m_iChangeAnimationEndCheck );
	//CDebugConsole::GetInstance()->Messagef( L"AnimationEndCheck2 : %d\n", m_iChangeAnimationEndCheck2 );
	//CDebugConsole::GetInstance()->Messagef( L"SelectedFrame : %d\n", m_iSelectedFrameNum );
	//CDebugConsole::GetInstance()->Messagef( L"bAnimationEnd : %d\n", m_bAnimationEndCheck );

	if( m_bChangingAnimation == FALSE )
	{
		if( m_pFrame[m_iSelectedFrameNum].m_bAnimation == TRUE )
		{
			AnimationTotalTrans( 0, m_pFrame[m_iSelectedFrameNum].m_vAniTransStartValue.x, 
				m_pFrame[m_iSelectedFrameNum].m_vAniTransEndValue.x, 
				m_pFrame[m_iSelectedFrameNum].m_vAniTransSpeed.x, 
				m_pFrame[m_iSelectedFrameNum].m_bTransXReplay, 
				CFrequency::GetInstance()->getFrametime() );
			AnimationTotalTrans( 1, m_pFrame[m_iSelectedFrameNum].m_vAniTransStartValue.y, 
				m_pFrame[m_iSelectedFrameNum].m_vAniTransEndValue.y, 
				m_pFrame[m_iSelectedFrameNum].m_vAniTransSpeed.y, 
				m_pFrame[m_iSelectedFrameNum].m_bTransYReplay, 
				CFrequency::GetInstance()->getFrametime() );
			AnimationTotalTrans( 2, m_pFrame[m_iSelectedFrameNum].m_vAniTransStartValue.z, 
				m_pFrame[m_iSelectedFrameNum].m_vAniTransEndValue.z, 
				m_pFrame[m_iSelectedFrameNum].m_vAniTransSpeed.z, 
				m_pFrame[m_iSelectedFrameNum].m_bTransZReplay, 
				CFrequency::GetInstance()->getFrametime() );

			AnimationTotalRotate( 0, m_pFrame[m_iSelectedFrameNum].m_vAniRotateStartValue.x, 
				m_pFrame[m_iSelectedFrameNum].m_vAniRotateEndValue.x, 
				m_pFrame[m_iSelectedFrameNum].m_vAniRotateSpeed.x, 
				m_pFrame[m_iSelectedFrameNum].m_bRotateXReplay, 
				CFrequency::GetInstance()->getFrametime() );

			AnimationTotalRotate( 1, m_pFrame[m_iSelectedFrameNum].m_vAniRotateStartValue.y, 
				m_pFrame[m_iSelectedFrameNum].m_vAniRotateEndValue.y, 
				m_pFrame[m_iSelectedFrameNum].m_vAniRotateSpeed.y, 
				m_pFrame[m_iSelectedFrameNum].m_bRotateYReplay, 
				CFrequency::GetInstance()->getFrametime() );

			AnimationTotalRotate( 2, m_pFrame[m_iSelectedFrameNum].m_vAniRotateStartValue.z, 
				m_pFrame[m_iSelectedFrameNum].m_vAniRotateEndValue.z, 
				m_pFrame[m_iSelectedFrameNum].m_vAniRotateSpeed.z, 
				m_pFrame[m_iSelectedFrameNum].m_bRotateZReplay, 
				CFrequency::GetInstance()->getFrametime() );
		}

		//Set_ControlTranslate( 0, m_pFrame[m_iSelectedFrameNum].m_fCharEditorTranslate.x );
		//Set_ControlTranslate( 1, m_pFrame[m_iSelectedFrameNum].m_fCharEditorTranslate.y );
		//Set_ControlTranslate( 2, m_pFrame[m_iSelectedFrameNum].m_fCharEditorTranslate.z );
		//Set_ControlRotate( 0, m_pFrame[m_iSelectedFrameNum].m_fCharEditorRotate.x );
		//Set_ControlRotate( 1, m_pFrame[m_iSelectedFrameNum].m_fCharEditorRotate.y );
		//Set_ControlRotate( 2, m_pFrame[m_iSelectedFrameNum].m_fCharEditorRotate.z );

		for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
		{
			m_pBox[Loop].UpdateMonsterPos( m_vPreControl + m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate, 
				m_vControl + m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate, m_fAngle );

			m_pBox[Loop].Set_ControlTranslate( 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.x );
			m_pBox[Loop].Set_ControlTranslate( 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.y );
			m_pBox[Loop].Set_ControlTranslate( 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.z );
			m_pBox[Loop].Set_ControlRotate( 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.x );
			m_pBox[Loop].Set_ControlRotate( 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.y );
			m_pBox[Loop].Set_ControlRotate( 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.z );

			if( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bAnimation == TRUE )
			{
				AnimationTrans( Loop, 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransStartValue.x, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransEndValue.x, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSpeed.x, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bTransXReplay, 
					CFrequency::GetInstance()->getFrametime() );

				AnimationTrans( Loop, 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransStartValue.y, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransEndValue.y, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSpeed.y, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bTransYReplay, 
					CFrequency::GetInstance()->getFrametime() );

				AnimationTrans( Loop, 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransStartValue.z, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransEndValue.z, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSpeed.z, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bTransZReplay, 
					CFrequency::GetInstance()->getFrametime() );

				AnimationRotate( Loop, 0, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateStartValue.x, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateEndValue.x, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSpeed.x, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bRotateXReplay, 
					CFrequency::GetInstance()->getFrametime() );

				AnimationRotate( Loop, 1, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateStartValue.y, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateEndValue.y, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSpeed.y, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bRotateYReplay, 
					CFrequency::GetInstance()->getFrametime() );

				AnimationRotate( Loop, 2, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateStartValue.z, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateEndValue.z, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSpeed.z, 
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_bRotateZReplay, 
					CFrequency::GetInstance()->getFrametime() );
			}

			////CDebugConsole::GetInstance()->Messagef( L"Loop : %d / Start : %f \n", Loop, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateStartValue.x );
			////CDebugConsole::GetInstance()->Messagef( L"Loop : %d / End : %f \n", Loop, m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateStartValue.x );

			//m_pBox[Loop].Update();
			m_pBox[Loop].Calcul_MatWorld();
			m_pBox[Loop].UpdateMonsterMatrix( Get_MatWorld() );
		}
	}
	else
	{
		AniInterpolation();
	}

	// AI
	m_pStateMachine->Update();


	UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f );
}

VOID CMonster::Render()
{
	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{
		m_pBox[Loop].Render();
	}

	// 임시
	m_pTestCube->Render();
}

VOID CMonster::BreakCubeAll()
{
	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{
		m_iBreakedCubeCnt += m_pBox[Loop].GetvCubeSize();
		m_pBox[Loop].BreakCubeAll();
	}
}

VOID CMonster::BreakNockdown()
{
	m_iBreakedCubeCnt += 
		m_pBox[0].GetvCubeSize() + m_pBox[1].GetvCubeSize() + m_pBox[2].GetvCubeSize() + 
		m_pBox[3].GetvCubeSize() + m_pBox[4].GetvCubeSize() + m_pBox[5].GetvCubeSize() + m_pBox[6].GetvCubeSize();
	m_pBox[0].BreakNockdown();
	m_pBox[1].BreakNockdown();
	m_pBox[2].BreakNockdown();
	m_pBox[3].BreakNockdown();
	m_pBox[4].BreakNockdown();
	m_pBox[5].BreakNockdown();
	m_pBox[6].BreakNockdown();
}

BOOL CMonster::BreakNockdown(BOOL IsLive)
{
	BOOL bRet = FALSE;
	m_pBox[m_iBreakPart].BreakNockdown();

	switch ( m_iBreakPart )
	{
	case 0:					// 머리
		m_iBreakPart = 4;
		break;
	case 1:					// 몸
		m_iBreakPart = 6;
		bRet = TRUE;
		break;
	case 4:					// 팔
		m_iBreakPart = 5;
		break;
	case 5:					// 팔
		m_iBreakPart = 1;
		break;
	default:
		break;
	}

	return bRet;
}

VOID CMonster::RepairCube()
{
	for( INT Loop = 0; Loop < m_iCharEditorMax; ++Loop )
	{
		m_pBox[Loop].RepairCube();
	}
}

VOID CMonster::QubeATKEffect( const D3DXVECTOR3 &a_vPos )
{
	m_pBox[0].QubeATKEffect( a_vPos );
}

VOID CMonster::EnableShadow( BOOL bEnable )
{
	for( INT Loop = 0; Loop < m_iCharEditorMax; ++Loop )
	{
		if (Loop == 2 || Loop == 3 )
		{
			m_pBox[Loop].EnableShadow( bEnable );
		}	
		else
		{
			m_pBox[Loop].EnableShadow( FALSE );
		}
	}
}


VOID CMonster::InitAniAndState()
{
	//ChangeAnimation( CMonster::ANIM_STAND );
	m_pStateMachine->SetCurrentState( NULL );
}


VOID CMonster::Set_ChaseData()
{
	m_vChaseCurrentPos = Get_Pos();
	m_vChasePreviousPos = m_vChaseCurrentPos;
	m_iChaseCurrentX = INT(  ( m_vChaseCurrentPos.x + 510.0f ) / 10.0f );
	m_iChaseCurrentZ = INT( -( m_vChaseCurrentPos.z - 950.0f ) / 10.0f );
	m_pChaseNextPath = Get_Path();
	m_iChaseTotalPathCnt = m_pChaseNextPath->remainedNode;

	m_fChaseCurrentAngle = D3DXToDegree( Get_Angle() );
	m_fChaseNextAngle = Get_Degree();	

	if( m_fChaseCurrentAngle == 225.0f && m_fChaseNextAngle == -90.0f )
	{
		m_fChaseAngle0 = 225.0f;
		m_fChaseAngle1 = 270.0f;
	}
	else if( m_fChaseCurrentAngle == -90.0f && m_fChaseNextAngle == 225.0f )
	{
		m_fChaseAngle0 = 270.0f;
		m_fChaseAngle1 = 225.0f;
	}
	else
	{
		m_fChaseAngle0 = m_fChaseCurrentAngle;
		m_fChaseAngle1 = m_fChaseNextAngle;
	}
}


VOID CMonster::Set_ChaseNextData()
{
	// 현재 위치를 이전 위치에
	m_vChasePreviousPos = m_vChaseCurrentPos;

	// 각도 오차값이 있으므로 최종 각도값을 넣어준다.
	Set_Angle( D3DXToRadian( m_fChaseNextAngle ) );

	m_iChaseCurrentX = m_pChaseNextPath->x;
	m_iChaseCurrentZ = m_pChaseNextPath->y;

	// 도착했으면 Seek 상태로, 최대 이동 타일수는 10
	m_pChaseNextPath = m_pChaseNextPath->next;
	if( m_pChaseNextPath->remainedNode <= 0 || m_iChaseTotalPathCnt - m_pChaseNextPath->remainedNode == 10 )
	{
		Set_ClearSoundTime();

		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			m_pStateMachine->ChangeState( Seek::GetInstance() );
		}
		else
		{
			// Path 제거
			Astar::GetInstance()->removePath( Get_Path() );
			Set_Path( NULL );
			m_pStateMachine->ChangeState( NULL );
		}
	}
	else
	{
		m_vChaseCurrentPos = Get_Pos();

		m_fChaseCurrentAngle = m_fChaseNextAngle;
		m_fChaseNextAngle = Get_Degree();

		if( m_fChaseCurrentAngle == 225.0f && m_fChaseNextAngle == -90.0f )
		{
			m_fChaseAngle0 = 225.0f;
			m_fChaseAngle1 = 270.0f;
		}
		else if( m_fChaseCurrentAngle == -90.0f && m_fChaseNextAngle == 225.0f )
		{
			m_fChaseAngle0 = 270.0f;
			m_fChaseAngle1 = 225.0f;
		}
		else
		{
			m_fChaseAngle0 = m_fChaseCurrentAngle;
			m_fChaseAngle1 = m_fChaseNextAngle;
		}
	}
}


FLOAT CMonster::Get_Degree()
{
	static FLOAT fDegree[3][3] = {
		{ 135.0f, 180.0f, 225.0f },
		{  90.0f,   0.0f, -90.0f },
		{  45.0f,   0.0f, -45.0f }
	};

	INT iDirX = m_pChaseNextPath->x - m_iChaseCurrentX + 1;
	INT iDirZ = m_pChaseNextPath->y - m_iChaseCurrentZ + 1;

	return fDegree[iDirZ][iDirX];
}


D3DXVECTOR3 CMonster::GetWorldPos( INT a_iX, INT a_iZ )
{
	D3DXVECTOR3 pos;
/*
	pos.x = m_pMapInfo->vecStart.x + m_pMapInfo->fTileSize * a_iX + m_pMapInfo->fTileSize / 2;
	pos.y = 0.0f;
	pos.z = m_pMapInfo->vecEnd.z - m_pMapInfo->fTileSize * a_iZ - m_pMapInfo->fTileSize / 2;
*/
	pos.x = -510.0f + 10.0f * a_iX + 10.0f / 2;
	pos.y = 0.0f;
	pos.z = 950.0f - 10.0f * a_iZ - 10.0f / 2;

	return pos;
}
