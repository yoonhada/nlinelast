#include "stdafx.h"
#include "Monster.h"

#include "Charactor.h"
#include "Weapon.h"

#include "Seek.h"

#define _PHY_COLL_PRT

CMonster::CMonster()
{
	Clear();
}

CMonster::~CMonster()
{
	Realese();
}

VOID CMonster::Clear()
{
	m_iCharEditorMax = 7;
	m_iSelectedCharEditor=0;
	m_fBoxSize = 15.0f;
	m_fBoxSizeDiv = (m_fBoxSize) * 0.5f;
	m_iFrameMax = 10;
	m_iSelectedFrameNum = 0;
	D3DXMatrixIdentity( &m_matControl );
	m_vPreControl = D3DXVECTOR3(300.0f, 0.0f, 100.0f);
	m_vControl = D3DXVECTOR3(300.0f, 0.0f, 100.0f);
	m_vFowardVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vSideStepVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vColissionControl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_iChangeAnimationEndCheck = 0;
	m_iChangeAnimationEndCheck2 = 0;
	m_iNextFrame = 0;
	m_bChangingAnimation = FALSE;

	m_iTarget = -1;
	m_fTargetDistance = 0.0f;
}

HRESULT CMonster::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, WCHAR* a_pFileName )
{
	m_pD3dDevice = a_pD3dDevice;
	m_pMatrices = CMatrices::GetInstance();

	m_pFrame = new Frame[m_iFrameMax];
	m_pBox = new CCharactor[m_iCharEditorMax];
	for( INT LoopBox=0; LoopBox<m_iCharEditorMax; ++LoopBox )
	{
		m_pBox[LoopBox].Create( m_pD3dDevice, m_pMatrices );
		m_pBox[LoopBox].CreateWeapon( CWeapon::NONE );
	}

	//m_pBoundBox = new CBoundBox(&m_pBox[2]);
	//m_pBoundBox->Create();

	for( INT LoopFrame=0; LoopFrame<m_iFrameMax; ++LoopFrame )
	{
		m_pFrame[LoopFrame].m_pBoxData = new BoxData[m_iCharEditorMax];
	}

	//WCHAR* ptr = wcstok( a_pFileName, L"." );
	Load( a_pFileName );

	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{
		CTree::GetInstance()->GetChaVector()->push_back( m_pBox[Loop].GetBoundBox() );
	}

	// AI
	m_pStateMachine = new StateMachine<CMonster>( this );
	m_pStateMachine->SetCurrentState( Seek::GetInstance() );

	return S_OK;
}

VOID CMonster::Realese()
{
	SAFE_DELETE_ARRAY(m_pBox);
	
	if( m_pFrame != NULL )
	{
		for( INT LoopFrame=0; LoopFrame<m_iFrameMax; ++LoopFrame )
		{
			delete []m_pFrame[LoopFrame].m_pBoxData;
			m_pFrame[LoopFrame].m_pBoxData=NULL;
		}
		delete []m_pFrame;
		m_pFrame=NULL;
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
	//ĳ���� ���̺� �������� üũ
	fwscanf( pFile, L"%s", szTemp );
	if( 0 != lstrcmp( szTemp, L"BKHCharSave" ) )
	{
		MessageBox(NULL, L"Not Charactor Save File", NULL, MB_OK);
		return;
	}

	//�ڽ� ����
	fwscanf( pFile, L"%d", &m_iCharEditorMax);

	for(INT LoopFrame=0; LoopFrame<m_iFrameMax; ++LoopFrame)
	{
		//������ �ѹ�
		fwscanf( pFile, L"%s", szTemp );

		//�ִϸ��̼� �ֳ� ���� üũ
		fwscanf( pFile, L"%d", &m_pFrame[LoopFrame].m_bAnimation );

		//�̵� �ִ� x
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniTransStartValue.x,
			&m_pFrame[LoopFrame].m_vAniTransEndValue.x,
			&m_pFrame[LoopFrame].m_vAniTransSpeed.x,
			&m_pFrame[LoopFrame].m_bTransXReplay );

		//�̵� �ִ� y
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniTransStartValue.y,
			&m_pFrame[LoopFrame].m_vAniTransEndValue.y,
			&m_pFrame[LoopFrame].m_vAniTransSpeed.y,
			&m_pFrame[LoopFrame].m_bTransYReplay );

		//�̵� �ִ� z
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniTransStartValue.z,
			&m_pFrame[LoopFrame].m_vAniTransEndValue.z,
			&m_pFrame[LoopFrame].m_vAniTransSpeed.z,
			&m_pFrame[LoopFrame].m_bTransZReplay );

		//ȸ�� �ִ� x
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniRotateStartValue.x,
			&m_pFrame[LoopFrame].m_vAniRotateEndValue.x,
			&m_pFrame[LoopFrame].m_vAniRotateSpeed.x,
			&m_pFrame[LoopFrame].m_bRotateXReplay );

		//������ ���Ȱ����� ����
		m_pFrame[LoopFrame].m_vAniRotateStartValue.x = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateStartValue.x );
		m_pFrame[LoopFrame].m_vAniRotateEndValue.x = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateEndValue.x );

		//ȸ�� �ִ� y
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniRotateStartValue.y,
			&m_pFrame[LoopFrame].m_vAniRotateEndValue.y,
			&m_pFrame[LoopFrame].m_vAniRotateSpeed.y,
			&m_pFrame[LoopFrame].m_bRotateYReplay );

		//������ ���Ȱ����� ����
		m_pFrame[LoopFrame].m_vAniRotateStartValue.y = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateStartValue.y );
		m_pFrame[LoopFrame].m_vAniRotateEndValue.y = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateEndValue.y );

		//ȸ�� �ִ� z
		fwscanf( pFile, L"%f %f %f %d", 
			&m_pFrame[LoopFrame].m_vAniRotateStartValue.z,
			&m_pFrame[LoopFrame].m_vAniRotateEndValue.z,
			&m_pFrame[LoopFrame].m_vAniRotateSpeed.z,
			&m_pFrame[LoopFrame].m_bRotateZReplay );

		//������ ���Ȱ����� ����
		m_pFrame[LoopFrame].m_vAniRotateStartValue.z = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateStartValue.z );
		m_pFrame[LoopFrame].m_vAniRotateEndValue.z = D3DXToRadian( m_pFrame[LoopFrame].m_vAniRotateEndValue.z );

		for(INT Loop=0; Loop<m_iCharEditorMax; ++Loop)
		{
			//�ڽ� �ѹ�
			fwscanf( pFile, L"%s", &szTemp );
			//�ڽ� �����̼�
			fwscanf( pFile, L"%f %f %f", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.z );

			//������ ���Ȱ����� ����
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.x = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.x );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.y = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.y );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.z = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorRotate.z );

			//�ڽ� Ʈ��������Ʈ
			fwscanf( pFile, L"%f %f %f", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorTranslate.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorTranslate.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_fCharEditorTranslate.z );

			//�ִϸ��̼� �ֳ� ���� üũ
			fwscanf( pFile, L"%d", &m_pFrame[LoopFrame].m_pBoxData[Loop].m_bAnimation );

			//�̵� �ִ� x
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransStartValue.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransEndValue.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransSpeed.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bTransXReplay );

			//�̵� �ִ� y
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransStartValue.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransEndValue.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransSpeed.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bTransYReplay );

			//�̵� �ִ� z
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransStartValue.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniTransEndValue.z,
				&m_pFrame[LoopFrame].m_vAniTransSpeed.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bTransZReplay );

			//ȸ�� �ִ� x
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateSpeed.x,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bRotateXReplay );

			//������ ���Ȱ����� ����
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.x = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.x );

			//ȸ�� �ִ� y
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateSpeed.y,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bRotateYReplay );

			//������ ���Ȱ����� ����
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.y = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.y );

			//ȸ�� �ִ� z
			fwscanf( pFile, L"%f %f %f %d", 
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateSpeed.z,
				&m_pFrame[LoopFrame].m_pBoxData[Loop].m_bRotateZReplay );

			//������ ���Ȱ����� ����
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z );
			m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.z = D3DXToRadian( m_pFrame[LoopFrame].m_pBoxData[Loop].m_vAniRotateEndValue.z );
		}
	}

	fclose(pFile);

	for(INT Loop=0; Loop<m_iCharEditorMax; ++Loop)
	{
		wsprintf(wchTemp, L"%s_%d.csav", a_pFileName, Loop);
		m_pBox[Loop].Load( wchTemp );

		CTree::GetInstance()->GetChaVector()->push_back( m_pBox[Loop].GetBoundBox() );
	}

	//MessageBox( GHWND, L"Monster Load OK", NULL, MB_OK );
}

VOID CMonster::AnimationTotalTrans( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//������ �� ���� ������
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
	//������ ������ Ŭ��
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
	//������ �� ���� ������
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
	//������ ������ Ŭ��
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
	//���۰� ���� ������ �ش� �������� ��� ȸ��
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
		// 0�̸� �ʱ�ȭ
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
	//������ �� ���� ������
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
	//������ ������ Ŭ��
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
	//������ �� ���� ������
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
	//������ ������ Ŭ��
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
	//���۰� ���� ������ �ش� �������� ��� ȸ��
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
		// 0�̸� �ʱ�ȭ
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
	//������ �� ���� ������
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] <= a_fEnd )
		{
			// �ִϸ��̼� �ȳ�����
			m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			// �ִϸ��̼� ������
			m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
		}
	}
	//������ ������ Ŭ��
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] >= a_fEnd )
		{
			// �ִϸ��̼� �ȳ�����
			m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			// �ִϸ��̼� ������
			m_pFrame[m_iSelectedFrameNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
		}
	}
	//���۰� ���� ������
	else
	{
		// �ִϸ��̼� ������
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
	//������ �� ���� ������
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] < a_fEnd )
		{
			// �ִϸ��̼� �ȳ�����
			m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			// �ִϸ��̼� ������
			m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
		}
	}
	//������ ������ Ŭ��
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] > a_fEnd )
		{
			// �ִϸ��̼� �ȳ�����
			m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			// �ִϸ��̼� ������
			m_pFrame[m_iSelectedFrameNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
		}
	}
	//���۰� ���� ������
	else
	{
		// �ִϸ��̼� ������
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

	//������ �� ���� ������
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] <= a_fEnd )
		{
			// �ִϸ��̼� �ȳ�����
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			// �ִϸ��̼� ������
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
		}
	}
	//������ ������ Ŭ��
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] >= a_fEnd )
		{
			// �ִϸ��̼� �ȳ�����
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			// �ִϸ��̼� ������
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniTransEndCheck[a_iXYZ] = FALSE;
		}
	}
	//���۰� ���� ������
	else
	{
		// �ִϸ��̼� ������
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

	//������ �� ���� ������
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] <= a_fEnd )
		{
			// �ִϸ��̼� �ȳ�����
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			// �ִϸ��̼� ������
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
		}
	}
	//������ ������ Ŭ��
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] >= a_fEnd )
		{
			// �ִϸ��̼� �ȳ�����
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = TRUE;
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			// �ִϸ��̼� ������
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bAniRotateEndCheck[a_iXYZ] = FALSE;
		}
	}
	//���۰� ���� ������
	else
	{
		// �ִϸ��̼� ������
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
	// 360�� ������ ���� 0���� �ʱ�ȭ
	ABSDEF( m_fAngle ) > 6.2831853f ? m_fAngle = 0.0f : NULL;

	m_vColissionControl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//m_vControl;
	// ���� ���� ó��
	if( a_vControl.z != 0 )
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle );
		m_vFowardVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vFowardVector, &m_vFowardVector);
		m_vColissionControl = (m_vFowardVector * a_vControl.z);	
	}
	// �¿� ó��
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
	
	INT Loop;
	D3DXMATRIXA16 mat = Get_MatWorld();
	D3DXMATRIXA16 matR;
	D3DXMatrixInverse(&matR, NULL, &mat);
	for( Loop = 0; Loop < m_iCharEditorMax; ++Loop )
	{
		if ( m_pBox[Loop].CollisionAtk( matR ) )
		{
			m_pBox[Loop].BreakQube( mat );
		}
	}
	CTree::GetInstance()->GetAtkVector()->clear();

	CDebugInterface::GetInstance()->AddMessageFloat( "MonsterAngle", m_fAngle );

	static FLOAT fTemp = 11.0f;
	CDebugInterface::GetInstance()->AddMessageFloat( "TempAngle", fTemp );

	m_vPreControl = m_vControl;
	m_vControl += m_vColissionControl;

	CDebugInterface::GetInstance()->AddMessageVector( "MonsterPos", m_vControl );

	static D3DXVECTOR3 Temp(10.0f,10.0f,10.f);
	CDebugInterface::GetInstance()->AddMessageVector( "TempPos", Temp );

	Set_ControlTranslate( 0, m_vControl.x );
	Set_ControlTranslate( 1, m_vControl.y );
	Set_ControlTranslate( 2, m_vControl.z );
	Set_ControlRotate( 1, m_fAngle );
	Calcul_MatWorld();
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

	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{
		m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.x = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[0];
		m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.y = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[1];
		m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.z = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fTrans[2];

		m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.x = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[0];
		m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.y = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[1];
		m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.z = m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fRotation[2];
		
		vTransLength.x = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.x ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.x - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.x ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.x );
		vTransLength.y = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.y ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.y - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.y ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.y );
		vTransLength.z = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniTransSave.z ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorTranslate.z - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorTranslate.z ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniTransStartValue.z );

		vRotateLength.x = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.x ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.x - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.x ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.x );
		vRotateLength.y = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.y ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.y - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.y ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.y );
		vRotateLength.z = ABSDEF( m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_vAniRotateSave.z ) - ABSDEF( ( m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_fCharEditorRotate.z - m_pFrame[m_iSelectedFrameNum].m_pBoxData[Loop].m_fCharEditorRotate.z ) + m_pFrame[m_iNextFrame].m_pBoxData[Loop].m_vAniRotateStartValue.z );

		vTransLength.x = ABSDEF(vTransLength.x);
		vTransLength.y = ABSDEF(vTransLength.y);
		vTransLength.z = ABSDEF(vTransLength.z);

		vRotateLength.x = ABSDEF(vRotateLength.x);
		vRotateLength.y = ABSDEF(vRotateLength.y);
		vRotateLength.z = ABSDEF(vRotateLength.z);
		
		if( m_fMaxInterpolationLength < vTransLength.x )
		{
			m_fMaxInterpolationLength = vTransLength.x;
		}

		if( m_fMaxInterpolationLength < vTransLength.y )
		{
			m_fMaxInterpolationLength = vTransLength.y;
		}

		if( m_fMaxInterpolationLength < vTransLength.z )
		{
			m_fMaxInterpolationLength = vTransLength.z;
		}

		if( m_fMaxInterpolationLength < vRotateLength.x )
		{
			m_fMaxInterpolationLength = vRotateLength.x;
			//if( m_fMaxInterpolationLength > 10.0f ) //CDebugConsole::GetInstance()->Messagef( L" RotLenX : %f \n ", m_fMaxInterpolationLength );
		}

		if( m_fMaxInterpolationLength < vRotateLength.y )
		{
			m_fMaxInterpolationLength = vRotateLength.y;
			//if( m_fMaxInterpolationLength > 10.0f ) //CDebugConsole::GetInstance()->Messagef( L" RotLenY : %f \n ", m_fMaxInterpolationLength );
		}

		if( m_fMaxInterpolationLength < vRotateLength.z )
		{
			m_fMaxInterpolationLength = vRotateLength.z;
			//if( m_fMaxInterpolationLength > 10.0f ) //CDebugConsole::GetInstance()->Messagef( L" RotLenZ : %f \n ", m_fMaxInterpolationLength );
		}
	}

	
}

VOID CMonster::AniInterpolation()
{

	////CDebugConsole::GetInstance()->Messagef( L"InterLength : %f \n", m_fMaxInterpolationLength );

	m_iChangeAnimationEndCheck = 0;
	m_iChangeAnimationEndCheck2 = 0;
	FLOAT Speed = 3.0f;
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
		
		////CDebugConsole::GetInstance()->Messagef( L"AniEndCheck : %d\n", m_iChangeAnimationEndCheck );
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

//#ifndef _DEBUG
//	static INT TempLoop = 0;
//	if ( CInput::GetInstance()->Get_Lbutton() )
//	{
//		m_pBox[TempLoop].TestBreakCubeAll();
//		if(TempLoop < 7) ++TempLoop;
//	}
//#endif


	// AI
	//m_pStateMachine->Update();

}

VOID CMonster::Render()
{
	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{
		m_pBox[Loop].Render();
	}
}