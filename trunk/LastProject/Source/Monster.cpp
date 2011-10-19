#include "stdafx.h"
#include "Monster.h"

#include "Charactor.h"

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
	m_fAngle = 0.0f;
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
		//CTree::GetInstance()->GetChaVector()->push_back( m_pBox[Loop].GetBoundBox() );
	}

	return S_OK;
}

VOID CMonster::Realese()
{
	if( m_pBox != NULL )
	{
		delete []m_pBox;
		m_pBox=NULL;
	}

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
				&m_pFrame[LoopFrame].m_vAniTransSpeed.z,
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
	}

	//MessageBox( GHWND, L"Monster Load OK", NULL, MB_OK );
}

VOID CMonster::AnimationTotalTrans( INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] <= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] >= a_fStart && m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] = a_fStart;
			}
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] >= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] <= a_fStart && m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_bTransReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_fTrans[a_iXYZ] = a_fStart;
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
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] >= a_fStart && m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] = a_fStart;
			}
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] >= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] <= a_fStart && m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_bRotReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] = a_fStart;
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
		}
	}

	switch(a_iXYZ)
	{
	case 0:
		Set_ControlRotate( 0, m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		break;
	case 1:
		Set_ControlRotate( 1, m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		break;
	case 2:
		Set_ControlRotate( 2, m_pFrame[m_iSelectedFrameNum].m_fRotation[a_iXYZ] );
		break;
	}

}

VOID CMonster::AnimationTrans( INT a_iLoopNum, INT a_iXYZ, FLOAT a_fStart, FLOAT a_fEnd, FLOAT a_fSpeed, BOOL a_bReplay, FLOAT a_fFrameTime )
{
	//시작이 끝 보다 작을때
	if( a_fStart < a_fEnd)
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] <= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] >= a_fStart && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] = a_fStart;
			}
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] >= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] <= a_fStart && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] += a_fSpeed * a_fFrameTime;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bTransReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fTrans[a_iXYZ] = a_fStart;
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
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] >= a_fStart && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] = a_fStart;
			}
		}
	}
	//시작이 끝보다 클때
	else if( a_fStart > a_fEnd )
	{
		if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] >= a_fEnd && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] == FALSE)
		{
			m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] -= a_fSpeed * a_fFrameTime;
		}
		else
		{
			if(a_bReplay == TRUE)
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] = TRUE;
				if(m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] <= a_fStart && m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] == TRUE)
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] += a_fSpeed * a_fFrameTime;
				}
				else
				{
					m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_bRotReplay[a_iXYZ] = FALSE;
				}
			}
			else
			{
				m_pFrame[m_iSelectedFrameNum].m_pBoxData[a_iLoopNum].m_fRotation[a_iXYZ] =  a_fStart;
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

VOID CMonster::UpdateByValue( D3DXVECTOR3& a_vControl, FLOAT a_fAngle )
{
	m_fAngle += a_fAngle;
	// 360도 넘으면 라디언 0으로 초기화
	if( m_fAngle > 6.2831853 || m_fAngle < -6.2831853 )
	{
		m_fAngle = 0.0f;
	}

	////CDebugConsole::GetInstance()->Messagef( L"Chara Angle : %f\n", m_fAngle );


	//D3DXVECTOR3 vControl = m_vControl;
	//전진 후진 처리
	if( a_vControl.z != 0 )
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle );
		m_vFowardVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vFowardVector, &m_vFowardVector);
		//m_vControl += (m_vFowardVector * a_vControl.z);
		m_vPreControl += (m_vFowardVector * a_vControl.z);
	}
	//좌우 처리
	if( a_vControl.x != 0)
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle + 1.5707963f );
		m_vSideStepVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vSideStepVector, &m_vSideStepVector);
		//m_vControl += (m_vSideStepVector * a_vControl.x);
		m_vPreControl += (m_vSideStepVector * a_vControl.x);
	}
	////CDebugConsole::GetInstance()->Messagef( L"Chara ControlX : %f\n", m_vControl.x );
	////CDebugConsole::GetInstance()->Messagef( L"Chara ControlZ : %f\n", m_vControl.z );

	BOOL bCol = FALSE;
	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{
		bCol = m_pBox[Loop].Collision( m_vPreControl );
		if( bCol == TRUE )
		{
			m_vControl = m_vPreControl;
			break;
		}
	}

	Set_ControlTranslate( 0, m_vControl.x );
	Set_ControlTranslate( 1, m_vControl.y );
	Set_ControlTranslate( 2, m_vControl.z );
	Set_ControlRotate( 1, m_fAngle );
	Calcul_MatWorld();
}

VOID CMonster::Update()
{
	/*Set_ControlTranslate( 0, m_vControl.x );
	Set_ControlTranslate( 1, m_vControl.y );
	Set_ControlTranslate( 2, m_vControl.z );
	Set_ControlRotate( 1, m_fAngle );
	Calcul_MatWorld();*/

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

	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{

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

		//m_pBox[Loop].Update();
		m_pBox[Loop].Calcul_MatWorld();
		m_pBox[Loop].UpdateMonsterMatrix( Get_MatWorld() );

		//if ( CInput::GetInstance()->Get_Lbutton() )
		//{
		m_pBox[Loop].TestBreakCube();
		//}

	}
}

VOID CMonster::Render()
{
	for( INT Loop=0; Loop<m_iCharEditorMax; ++Loop )
	{
		m_pBox[Loop].Render();
	}
}