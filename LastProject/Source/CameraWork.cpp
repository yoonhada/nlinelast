#include "stdafx.h"
#include "CameraWork.h"

VOID CameraWork::Initialize()
{
	m_pdatCurrent		= NULL;

	//	30프레임 기준
	m_dFrameSpeed		= 33;	

	for( INT i=0 ; i<CWK_END ; i++ )
		CreateImage_Box( &m_aimgBoxs[ i ] );
}

VOID CameraWork::Release()
{
	DATAMAP::iterator itE;
	for( itE = m_mapData.begin(); itE != m_mapData.end() ; itE++ )
		delete itE->second;
	m_mapData.clear();
}

VOID CameraWork::AddData( DWORD _dID, LPPARAMETER _pParameter )
{
	LPDATA pData = new DATA;

	pData->avecPoint[ CWK_LOOKAT_BOX0 ] = _pParameter->avecLookAt[ 0 ];
	pData->avecPoint[ CWK_LOOKAT_BOX1 ] = _pParameter->avecLookAt[ 1 ];

	CreateInfo_Line2(	&pData->infLookAt, 100,
						_pParameter->avecLookAt[ 0 ],
						_pParameter->avecLookAt[ 1 ] );

	pData->avecPoint[ CWK_POSITION_BOX0 ] = _pParameter->avecPosition[ 0 ];
	pData->avecPoint[ CWK_POSITION_BOX1 ] = _pParameter->avecPosition[ 1 ];
	pData->avecPoint[ CWK_POSITION_BOX2 ] = _pParameter->avecPosition[ 2 ];
	pData->avecPoint[ CWK_POSITION_BOX3 ] = _pParameter->avecPosition[ 3 ];

	CreateInfo_BezierCurve( &pData->infPosition, 100, 
							_pParameter->avecPosition[ 0 ],
							_pParameter->avecPosition[ 1 ],
							_pParameter->avecPosition[ 2 ],
							_pParameter->avecPosition[ 3 ] );
	
	m_mapData.insert( std::make_pair( _dID, pData ) );

}

VOID CameraWork::SetWholeWorkingPeriod( DWORD _dID, DWORD _dType, DWORD _dTime )
{
	DATAMAP::iterator itE = m_mapData.find( _dID );

	if( m_mapData.end() == itE )
	{
		MessageBox( NULL, L"CameraWork::SetWholeWorkingPeriod() find ID failed...", NULL, MB_OK );
		return;
	}
	
	LPINFO	pInfo;

	switch( _dType )
	{
	case CWK_POSITION:
		pInfo = &(*itE).second->infPosition;
		break;
	case CWK_LOOKAT:
		pInfo = &(*itE).second->infLookAt;
		break;
	default:
		MessageBox( NULL, L"CameraWork::SetWholeWorkingPeriod() find type failed...", NULL, MB_OK );
		return;
	}

	FLOAT fTime					= static_cast<FLOAT>( _dTime );
	FLOAT fFrameSpeed			= static_cast<FLOAT>( m_dFrameSpeed );
	FLOAT fNumVertices			= static_cast<FLOAT>( pInfo->iNumVertices );
	
	FLOAT fIncInterporation		= fNumVertices / ( fTime / fFrameSpeed );
	
	if( fIncInterporation > 1.0f )
	{
		FLOAT fIncIndex = floor( fIncInterporation );

		pInfo->iIncIndex = static_cast<INT>( fIncIndex ) + 1;

		fIncInterporation = fIncInterporation - floor( fIncInterporation );
	}

	for( INT i=0 ; i<pInfo->iNumVertices ; i++ )
		pInfo->pfIncInterporation[ i ] = fIncInterporation;

}

VOID CameraWork::SetPartWorkingPeriod( DWORD _dID, DWORD _dType, DWORD _dTime, INT _iStartIndex, INT _iEndIndex )
{	//	느려지는것만 가능하다
	DATAMAP::iterator itE = m_mapData.find( _dID );

	if( m_mapData.end() == itE )
	{
		MessageBox( NULL, L"CameraWork::SetPartWorkingPeriod() find ID failed...", NULL, MB_OK );
		return;
	}
	
	LPINFO	pInfo;

	switch( _dType )
	{
	case CWK_POSITION:
		pInfo = &(*itE).second->infPosition;
		break;
	case CWK_LOOKAT:
		pInfo = &(*itE).second->infLookAt;
		break;
	default:
		MessageBox( NULL, L"CameraWork::SetPartWorkingPeriod() find type failed...", NULL, MB_OK );
		return;
	}
	
	//	StartIndex, EndIndex Error Check!
	//	StartIndex, EndIndex 크기 비교
	if( 0 > _iStartIndex || _iStartIndex > pInfo->iNumVertices )
	{
		MessageBox( NULL, L"CameraWork::SetPartWorkingPeriod(){ ... _iStartIndex failed... }", NULL, MB_OK );
		return;
	}
	if( 0 > _iEndIndex || _iEndIndex > pInfo->iNumVertices )
	{
		MessageBox( NULL, L"CameraWork::SetPartWorkingPeriod(){ ... _iEndIndex failed... }", NULL, MB_OK );
		return;
	}

	FLOAT fSumInterporation = 0.0f, fAverageInterporation = 0.0f;
	for( INT i=_iStartIndex ; i<_iEndIndex ; i++ )
		fSumInterporation += pInfo->pfIncInterporation[ i ];

	fAverageInterporation = fSumInterporation / static_cast<FLOAT>( _iEndIndex - _iStartIndex );

	FLOAT fFrameSpeed	= static_cast<FLOAT>( m_dFrameSpeed );
	FLOAT fNumVertices	= static_cast<FLOAT>( pInfo->iNumVertices );

	DWORD dTime = static_cast<DWORD>( fFrameSpeed * ( fNumVertices / fAverageInterporation ) );

	dTime += _dTime;
	FLOAT fTime = static_cast<FLOAT>( dTime );
	FLOAT fInterporation = fNumVertices / ( fTime / fFrameSpeed );

	for( INT i=_iStartIndex ; i<_iEndIndex ; i++ )
		pInfo->pfIncInterporation[ i ] = fInterporation;
}

VOID CameraWork::SelectData( DWORD _dID )
{
	DATAMAP::iterator itE = m_mapData.find( _dID );

	if( m_mapData.end() == itE )
	{
		MessageBox( NULL, L"CameraWork::SetPartWorkingPeriod() find ID failed...", NULL, MB_OK );
		return;
	}

	m_pdatCurrent = itE->second;

	//	Init Current Position
	//m_pdatCurrent->infLookAt.vecCurrentPosition		= m_pdatCurrent->avecPoint[ CWK_LOOKAT_BOX0 ];
	//m_pdatCurrent->infPosition.vecCurrentPosition	= m_pdatCurrent->avecPoint[ CWK_POSITION_BOX0 ];
}

BOOL CameraWork::UpdateInfo( LPINFO _pInfo, LPD3DXVECTOR3 _pvecPosition )
{
	if( _pInfo->iNextIndex >= _pInfo->iNumVertices )
	{
		////	Test 반복
		//_pInfo->iCurrentIndex	= 0;
		//_pInfo->iNextIndex		= _pInfo->iIncIndex;

		//_pInfo->vecCurrentPosition	=	_pInfo->pVertex[ 0 ].vecPos;
		////	End
		return FALSE;
	}

	_pInfo->dCurrentTime = timeGetTime();

	if( _pInfo->dCurrentTime > _pInfo->dBeginTime + m_dFrameSpeed )
	{
		_pInfo->dBeginTime = _pInfo->dCurrentTime;

		FLOAT fSumInterporation = 0.0f, fAverageInterporation = 0.0f;

		for( INT i=0 ; i<_pInfo->iIncIndex ; i++ )
			fSumInterporation += _pInfo->pfIncInterporation[ _pInfo->iCurrentIndex + i ];
		
		fAverageInterporation	= fSumInterporation / static_cast<FLOAT>( _pInfo->iIncIndex );

		_pInfo->fInterporation	+= fAverageInterporation;

		D3DXVec3Lerp(	_pvecPosition,
						&_pInfo->pVertex[ _pInfo->iCurrentIndex ].vecPos,
						&_pInfo->pVertex[ _pInfo->iNextIndex ].vecPos,
						_pInfo->fInterporation );

		if( _pInfo->fInterporation > 1.0f )
		{
			_pInfo->iCurrentIndex		+= _pInfo->iIncIndex;
			_pInfo->iNextIndex			+= _pInfo->iIncIndex;

			_pInfo->fInterporation		= _pInfo->fInterporation - floor( _pInfo->fInterporation );
		}
	}

	return TRUE;
}

BOOL CameraWork::Update()
{
	BOOL bPosition, bLookAt;

	bPosition	= UpdateInfo( &m_pdatCurrent->infLookAt, &m_vecCameraLookAt );
	bLookAt		= UpdateInfo( &m_pdatCurrent->infPosition, &m_vecCameraPosition );

	if( !bPosition & !bLookAt )
		return FALSE;

	return TRUE;
}

VOID CameraWork::DebugData( LPPARAMETER _pParameter )
{
	m_datDebug.avecPoint[ CWK_LOOKAT_BOX0 ] = _pParameter->avecLookAt[ 0 ];
	m_datDebug.avecPoint[ CWK_LOOKAT_BOX1 ] = _pParameter->avecLookAt[ 1 ];

	CreateInfo_Line2(	&m_datDebug.infLookAt, 100,
						_pParameter->avecLookAt[ 0 ],
						_pParameter->avecLookAt[ 1 ] );

	m_datDebug.avecPoint[ CWK_POSITION_BOX0 ] = _pParameter->avecPosition[ 0 ];
	m_datDebug.avecPoint[ CWK_POSITION_BOX1 ] = _pParameter->avecPosition[ 1 ];
	m_datDebug.avecPoint[ CWK_POSITION_BOX2 ] = _pParameter->avecPosition[ 2 ];
	m_datDebug.avecPoint[ CWK_POSITION_BOX3 ] = _pParameter->avecPosition[ 3 ];

	CreateInfo_BezierCurve( &m_datDebug.infPosition, 100, 
							_pParameter->avecPosition[ 0 ],
							_pParameter->avecPosition[ 1 ],
							_pParameter->avecPosition[ 2 ],
							_pParameter->avecPosition[ 3 ] );

	m_pdatCurrent = &m_datDebug;
	
}
VOID CameraWork::DebugUpdate()
{
	//	Position Box
	m_aimgBoxs[ CWK_POSITION_BOX0 ].vecTranslate	= m_pdatCurrent->avecPoint[ CWK_POSITION_BOX0 ];
	m_aimgBoxs[ CWK_POSITION_BOX1 ].vecTranslate	= m_pdatCurrent->avecPoint[ CWK_POSITION_BOX1 ];
	m_aimgBoxs[ CWK_POSITION_BOX2 ].vecTranslate	= m_pdatCurrent->avecPoint[ CWK_POSITION_BOX2 ];
	m_aimgBoxs[ CWK_POSITION_BOX3 ].vecTranslate	= m_pdatCurrent->avecPoint[ CWK_POSITION_BOX3 ];

	//	Position Box Link
	CreateInfo_Line4( &m_infBoxLink, 4, m_pdatCurrent->avecPoint[ CWK_POSITION_BOX0 ],
										m_pdatCurrent->avecPoint[ CWK_POSITION_BOX1 ],
										m_pdatCurrent->avecPoint[ CWK_POSITION_BOX2 ],
										m_pdatCurrent->avecPoint[ CWK_POSITION_BOX3 ] );
	
	CreateImage( &m_imgBoxLink, &m_infBoxLink );

	//	LookAt Box
	m_aimgBoxs[ CWK_LOOKAT_BOX0 ].vecTranslate		= m_pdatCurrent->avecPoint[ CWK_LOOKAT_BOX0 ];
	m_aimgBoxs[ CWK_LOOKAT_BOX1 ].vecTranslate		= m_pdatCurrent->avecPoint[ CWK_LOOKAT_BOX1 ];

	//	Position To LookAt Box
	m_aimgBoxs[ CWK_POSITIONTOLOOKAT_BOX0 ].vecTranslate	= m_vecCameraPosition;
	m_aimgBoxs[ CWK_POSITIONTOLOOKAT_BOX1 ].vecTranslate	= m_vecCameraLookAt;

	//	Position To LookAt Box Link
	CreateInfo_Line2( &m_infPositionToLookAtLink, 2, m_vecCameraPosition, m_vecCameraLookAt );

	CreateImage( &m_imgPositionToLookAtLink, &m_infPositionToLookAtLink );

	//	Position Course
	CreateImage( &m_imgPositionCourse, &m_pdatCurrent->infPosition );

	//	LookAt Course
	CreateImage( &m_imgLookAtCourse, &m_pdatCurrent->infLookAt );
}

VOID CameraWork::DebugRender()
{
	for( INT i=0 ; i<CWK_END ; i++ )
		RenderImage( &m_aimgBoxs[ i ] );

	RenderImage( &m_imgBoxLink );
	RenderImage( &m_imgPositionCourse );
	RenderImage( &m_imgLookAtCourse );
	RenderImage( &m_imgPositionToLookAtLink );

}