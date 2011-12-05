#include "stdafx.h"
#include "CameraWork.h"

VOID CameraWork::Initialize()
{
	m_dFrameSpeed		= 15;

	m_dColCamera			= 0xffffffff;
	m_dColBox				= 0xffff00ff;
	m_dColBoxCourse			= 0xffff00ff;
	m_dColPositionCourse	= 0xff0000ff;
	m_dColLookAtCourse		= 0xff00ff00;
	m_dCameraLink			= 0xff0f0f0f;
	m_dColPositionToLookAt	= 0xfff0f0f0;
}

VOID CameraWork::Release()
{
}

VOID CameraWork::InitCameraPosition()
{
	//	Create Course Info
	INT iNumVertices = m_datPosition.infCourse.iNumVertices;

	m_datPosition.infCourse.pVertex = new VERTEX[ iNumVertices ];
	
	for( INT i=0 ; i<iNumVertices ; i++ )
	{
		m_datPosition.infCourse.pVertex[ i ].vecPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_datPosition.infCourse.pVertex[ i ].dColor = m_dColPositionCourse;
	}

	m_datPosition.infCourse.pfIncInterporation = new FLOAT[ iNumVertices ];

	for( INT i=0 ; i<iNumVertices ; i++ )
		m_datPosition.infCourse.pfIncInterporation[ i ] = 0.1f;

	//	Create PositionCourse Image
	CreateImage_Course( &m_datPosition.imgCourse, &m_datPosition.infCourse );

	//	Create BoxCourse Info
	iNumVertices = 4;

	m_datPosition.infBoxLink.iNumVertices	= iNumVertices;
	m_datPosition.infBoxLink.pVertex		= new VERTEX[ iNumVertices ];
	
	for( INT i=0 ; i<iNumVertices ; i++ )
	{
		m_datPosition.infBoxLink.pVertex[ i ].vecPos	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_datPosition.infBoxLink.pVertex[ i ].dColor	= m_dColBoxCourse;
	}
	
	//	Create BoxCourse Image
	CreateImage_Course( &m_datPosition.imgBoxLink, &m_datPosition.infBoxLink );
	
	//	Create Box Image
	m_datPosition.iNumBox = 4;
	m_datPosition.pimgBox = new IMAGE[ m_datPosition.iNumBox ];
	
	CreateImage_Box( &m_datPosition.pimgBox[ 0 ], m_dColBox );
	CreateImage_Box( &m_datPosition.pimgBox[ 1 ], m_dColBox );
	CreateImage_Box( &m_datPosition.pimgBox[ 2 ], m_dColBox );
	CreateImage_Box( &m_datPosition.pimgBox[ 3 ], m_dColBox );
}

VOID CameraWork::InitCameraLookAt()
{
	//	Create Course Info
	INT iNumVertices = m_datLookAt.infCourse.iNumVertices;

	m_datLookAt.infCourse.pVertex = new VERTEX[ iNumVertices ];
	
	for( INT i=0 ; i<iNumVertices ; i++ )
	{
		m_datLookAt.infCourse.pVertex[ i ].vecPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_datLookAt.infCourse.pVertex[ i ].dColor = m_dColPositionCourse;
	}

	m_datLookAt.infCourse.pfIncInterporation = new FLOAT[ iNumVertices ];

	for( INT i=0 ; i<iNumVertices ; i++ )
		m_datLookAt.infCourse.pfIncInterporation[ i ] = 0.1f;

	//	Create PositionCourse Image
	CreateImage_Course( &m_datLookAt.imgCourse, &m_datLookAt.infCourse );

	//	Create BoxCourse Info
	iNumVertices = 2;

	m_datLookAt.infBoxLink.iNumVertices	= iNumVertices;
	m_datLookAt.infBoxLink.pVertex		= new VERTEX[ iNumVertices ];
	
	for( INT i=0 ; i<iNumVertices ; i++ )
	{
		m_datLookAt.infBoxLink.pVertex[ i ].vecPos	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_datLookAt.infBoxLink.pVertex[ i ].dColor	= m_dColBoxCourse;
	}
	
	//	Create BoxCourse Image
	CreateImage_Course( &m_datLookAt.imgBoxLink, &m_datLookAt.infBoxLink );
	
	//	Create Box Image
	m_datLookAt.iNumBox = 2;
	m_datLookAt.pimgBox = new IMAGE[ m_datLookAt.iNumBox ];
	
	CreateImage_Box( &m_datLookAt.pimgBox[ 0 ], m_dColBox );
	CreateImage_Box( &m_datLookAt.pimgBox[ 1 ], m_dColBox );
}

VOID CameraWork::InitPositionToLookAt()
{	//	Box랑 BoxLink만 있다
	
	//	Create BoxCourse Info
	INT iNumVertices = 2;

	m_datPositionToLookAt.infBoxLink.iNumVertices	= iNumVertices;
	m_datPositionToLookAt.infBoxLink.pVertex		= new VERTEX[ iNumVertices ];
	
	for( INT i=0 ; i<iNumVertices ; i++ )
	{
		m_datPositionToLookAt.infBoxLink.pVertex[ i ].vecPos	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_datPositionToLookAt.infBoxLink.pVertex[ i ].dColor	= m_dColBoxCourse;
	}
	
	//	Create BoxCourse Image
	CreateImage_Course( &m_datPositionToLookAt.imgBoxLink, &m_datPositionToLookAt.infBoxLink );
	
	//	Create Box Image
	m_datPositionToLookAt.iNumBox = 2;
	m_datPositionToLookAt.pimgBox = new IMAGE[ m_datPositionToLookAt.iNumBox ];
	
	CreateImage_Box( &m_datPositionToLookAt.pimgBox[ 0 ], m_dColBox );
	CreateImage_Box( &m_datPositionToLookAt.pimgBox[ 1 ], m_dColBox );
}

VOID CameraWork::SetLookAtCourse_Box2( D3DXVECTOR3& _vecPosition0, D3DXVECTOR3& _vecPosition1 )
{
	//	Set Vertex
	LPVERTEX pVertex = m_datLookAt.infCourse.pVertex;
	
	FLOAT fNumVertices = static_cast<FLOAT>( m_datLookAt.infCourse.iNumVertices );

	FLOAT fIncX = ( _vecPosition1.x - _vecPosition0.x ) / fNumVertices;
	FLOAT fIncY = ( _vecPosition1.y - _vecPosition0.y ) / fNumVertices;
	FLOAT fIncZ = ( _vecPosition1.z - _vecPosition0.z ) / fNumVertices;

	for( INT i=0 ; i<m_datLookAt.infCourse.iNumVertices ; i++ )
	{
		pVertex[ i ].vecPos.x = _vecPosition0.x + fIncX * static_cast<FLOAT>( i );
		pVertex[ i ].vecPos.y = _vecPosition0.y + fIncY * static_cast<FLOAT>( i );
		pVertex[ i ].vecPos.z = _vecPosition0.z + fIncZ * static_cast<FLOAT>( i );
	}

	CreateImage_Course( &m_datLookAt.imgCourse, &m_datLookAt.infCourse );

	//	Set LookAtBox Position
	m_datLookAt.pimgBox[ 0 ].vecTranslate = _vecPosition0;
	m_datLookAt.pimgBox[ 1 ].vecTranslate = _vecPosition1;

	//	Set CurrentPositoin
	m_datLookAt.vecCurrentPosition = _vecPosition0;
}
VOID CameraWork::Create()
{
	/*InitCamera();	
	InitBox();
	InitPositionCourse();
	InitLookAtCourse();
	InitPositionToLookAt();*/

	InitCameraPosition();
	InitCameraLookAt();
	InitPositionToLookAt();
}

BOOL CameraWork::UpdatePosition()
{
	if( m_datPosition.iNextIndex >= m_datPosition.infCourse.iNumVertices )
		return FALSE;


	static FLOAT fInterporation = 0.0f;

	m_datPosition.dCurrentTime = timeGetTime();

	if( m_datPosition.dCurrentTime > m_datPosition.dBeginTime + m_dFrameSpeed )
	{
		m_datPosition.dBeginTime = m_datPosition.dCurrentTime;

		FLOAT fSumInterporation = 0.0f, fAverageInterporation = 0.0f;

		for( INT i=0 ; i<m_datPosition.iIncIndex ; i++ )
			fSumInterporation += m_datPosition.infCourse.pfIncInterporation[ m_datPosition.iCurrentIndex + i ];
		
		fAverageInterporation = fSumInterporation / static_cast<FLOAT>( m_datPosition.iIncIndex );

		fInterporation += fAverageInterporation;

		D3DXVec3Lerp(	&m_vecCameraPosition,
						&m_datPosition.vecCurrentPosition,
						&m_datPosition.infCourse.pVertex[ m_datPosition.iNextIndex ].vecPos,
						fInterporation );

		if( fInterporation > 1.0f )
		{
			m_datPosition.vecCurrentPosition = m_vecCameraPosition;

			m_datPosition.iCurrentIndex += m_datPosition.iIncIndex;
			m_datPosition.iNextIndex	+= m_datPosition.iIncIndex;

			fInterporation	= fInterporation - floor( fInterporation );

			//if( m_datPosition.iNextIndex >= m_datPosition.infCourse.iNumVertices )
			//{	//	반복
			//	m_datPosition.iCurrentIndex = 0;
			//	m_datPosition.iNextIndex	= m_datPosition.iIncIndex;

			//	m_datPosition.vecCurrentPosition = m_datPosition.infCourse.pVertex[ 0 ].vecPos;
			//}
		}
	}

	return TRUE;
	
}

BOOL CameraWork::UpdateLookAt()
{
	if( m_datLookAt.iNextIndex >= m_datLookAt.infCourse.iNumVertices )
		return FALSE;
	
	static FLOAT fInterporation = 0.0f;

	m_datLookAt.dCurrentTime = timeGetTime();

	if( m_datLookAt.dCurrentTime > m_datLookAt.dBeginTime + m_dFrameSpeed )
	{
		m_datLookAt.dBeginTime = m_datLookAt.dCurrentTime;

		FLOAT fSumInterporation = 0.0f, fAverageInterporation = 0.0f;

		for( INT i=0 ; i<m_datLookAt.iIncIndex ; i++ )
			fSumInterporation += m_datLookAt.infCourse.pfIncInterporation[ m_datLookAt.iCurrentIndex + i ];
		
		fAverageInterporation = fSumInterporation / static_cast<FLOAT>( m_datLookAt.iIncIndex );

		fInterporation += fAverageInterporation;

		D3DXVec3Lerp(	&m_vecCameraLookAt,
						&m_datLookAt.vecCurrentPosition,
						&m_datLookAt.infCourse.pVertex[ m_datLookAt.iNextIndex ].vecPos,
						fInterporation );

		/*Console::GetInstance().Messagef( L"CameraLookAt : %f %f %f\n", m_vecCameraLookAt.x, m_vecCameraLookAt.y, m_vecCameraLookAt.z );
		Console::GetInstance().Messagef( L"CurrentPosition : %f %f %f\n", m_datLookAt.vecCurrentPosition.x, m_datLookAt.vecCurrentPosition.y, m_datLookAt.vecCurrentPosition.z );
		Console::GetInstance().Messagef( L"NextPosition : %f %f %f\n", m_datLookAt.infCourse.pVertex[ m_datPosition.iNextIndex ].vecPos.x, m_datLookAt.infCourse.pVertex[ m_datPosition.iNextIndex ].vecPos.y, m_datLookAt.infCourse.pVertex[ m_datPosition.iNextIndex ].vecPos.z );*/

		if( fInterporation > 1.0f )
		{
			m_datLookAt.vecCurrentPosition = m_vecCameraLookAt;

			m_datLookAt.iCurrentIndex	+= m_datLookAt.iIncIndex;
			m_datLookAt.iNextIndex		+= m_datLookAt.iIncIndex;

			fInterporation	= fInterporation - floor( fInterporation );

			//if( m_datLookAt.iNextIndex >= m_datLookAt.infCourse.iNumVertices )
			//{	//	반복
			//	m_datLookAt.iCurrentIndex	= 0;
			//	m_datLookAt.iNextIndex		= m_datLookAt.iIncIndex;

			//	m_datLookAt.vecCurrentPosition = m_datLookAt.infCourse.pVertex[ 0 ].vecPos;
			//}
		}
	}

	return TRUE;
}

BOOL CameraWork::Update()
{
	BOOL bPosition, bLookAt;
	
	bPosition	= UpdatePosition();
	bLookAt		= UpdateLookAt();

	/*m_Data.infPositionToLookAt.pVertex[ 0 ].vecPos = m_vecCameraPosition;
	m_Data.infPositionToLookAt.pVertex[ 1 ].vecPos = m_vecCameraLookAt;
	CreateImage_Course( &m_Data.imgPositionToLookAt, &m_Data.infPositionToLookAt );*/

	m_datPositionToLookAt.pimgBox[ 0 ].vecTranslate = m_vecCameraPosition;
	m_datPositionToLookAt.pimgBox[ 1 ].vecTranslate = m_vecCameraLookAt;

	m_datPositionToLookAt.infBoxLink.pVertex[ 0 ].vecPos = m_vecCameraPosition;
	m_datPositionToLookAt.infBoxLink.pVertex[ 1 ].vecPos = m_vecCameraLookAt;
	CreateImage_Course( &m_datPositionToLookAt.imgBoxLink, &m_datPositionToLookAt.infBoxLink );

	if( bPosition == FALSE && bLookAt == FALSE )
		return FALSE;

	return TRUE;
}

VOID CameraWork::Render()
{
	//	Render Position
	RenderImage( &m_datPosition.imgCourse );

	for( INT i=0 ; i<m_datPosition.iNumBox ; i++ )
		RenderImage( &m_datPosition.pimgBox[ i ] );

	RenderImage( &m_datPosition.imgBoxLink );
	
	//	Render LookAt
	RenderImage( &m_datLookAt.imgCourse );

	for( INT i=0 ; i<m_datLookAt.iNumBox ; i++ )
		RenderImage( &m_datLookAt.pimgBox[ i ] );

	RenderImage( &m_datLookAt.imgBoxLink );

	//	Render PositionToLookAt
	for( INT i=0 ; i<m_datPositionToLookAt.iNumBox ; i++ )
		RenderImage( &m_datPositionToLookAt.pimgBox[ i ] );

	RenderImage( &m_datPositionToLookAt.imgBoxLink );

}


VOID CameraWork::SetPositionCourse(	D3DXVECTOR3& _vecPoint0, 
									D3DXVECTOR3& _vecPoint1, 
									D3DXVECTOR3& _vecPoint2, 
									D3DXVECTOR3& _vecPoint3 )
{
	//	Set Vertex
	LPVERTEX pVertex = m_datPosition.infCourse.pVertex;

	FLOAT fIncT = 1.0f / static_cast<FLOAT>( m_datPosition.infCourse.iNumVertices );

	INT i=0;
	for( FLOAT t=0.0f ; t<1.0f ; t+=fIncT )
	{
		pVertex[ i ].vecPos.x = _vecPoint0.x * ( 1.0f - t ) * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint1.x * 3.0f * t * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint2.x * 3.0f * t * t * ( 1.0f - t ) +
								_vecPoint3.x * t * t * t;
		
		pVertex[ i ].vecPos.y = _vecPoint0.y * ( 1.0f - t ) * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint1.y * 3.0f * t * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint2.y * 3.0f * t * t * ( 1.0f - t ) +
								_vecPoint3.y * t * t * t;

		pVertex[ i ].vecPos.z = _vecPoint0.z * ( 1.0f - t ) * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint1.z * 3.0f * t * ( 1.0f - t ) * ( 1.0f - t ) + 
								_vecPoint2.z * 3.0f * t * t * ( 1.0f - t ) +
								_vecPoint3.z * t * t * t;
		
		pVertex[ i ].dColor	= 0xff0000ff;

		i++;
		if( i >= m_datPosition.infCourse.iNumVertices )
			break;
	}

	CreateImage_Course( &m_datPosition.imgCourse, &m_datPosition.infCourse );
}

VOID CameraWork::SetPosition_Box4( D3DXVECTOR3& _vecPosition0, D3DXVECTOR3& _vecPosition1, D3DXVECTOR3& _vecPosition2, D3DXVECTOR3& _vecPosition3 )
{
	//	Set Box Position
	m_datPosition.pimgBox[ 0 ].vecTranslate = _vecPosition0;
	m_datPosition.pimgBox[ 1 ].vecTranslate = _vecPosition1;
	m_datPosition.pimgBox[ 2 ].vecTranslate = _vecPosition2;
	m_datPosition.pimgBox[ 3 ].vecTranslate = _vecPosition3;

	m_datPosition.infBoxLink.pVertex[ 0 ].vecPos = _vecPosition0;
	m_datPosition.infBoxLink.pVertex[ 1 ].vecPos = _vecPosition1;
	m_datPosition.infBoxLink.pVertex[ 2 ].vecPos = _vecPosition2;
	m_datPosition.infBoxLink.pVertex[ 3 ].vecPos = _vecPosition3;

	CreateImage_Course( &m_datPosition.imgBoxLink, &m_datPosition.infBoxLink );

	//	Set PositionCourse Position
	SetPositionCourse( _vecPosition0, _vecPosition1, _vecPosition2, _vecPosition3 );

	//	Set CurrentPositoin
	m_datPosition.vecCurrentPosition = _vecPosition0;
}

VOID CameraWork::SetWorkingPeriod( DWORD _dTime )
{
	FLOAT fTime					= static_cast<FLOAT>( _dTime );
	FLOAT fFrameSpeed			= static_cast<FLOAT>( m_dFrameSpeed );
	FLOAT fNumVertices_Position	= static_cast<FLOAT>( m_datPosition.infCourse.iNumVertices );
	FLOAT fNumVertices_LookAt	= static_cast<FLOAT>( m_datLookAt.infCourse.iNumVertices );

	FLOAT fIncInterporation_Position	= fNumVertices_Position / ( fTime / fFrameSpeed );
	FLOAT fIncInterporation_LookAt		= fNumVertices_LookAt / ( fTime / fFrameSpeed );
	
	//Console::GetInstance().Messagef( L"fNumVertices_Position / ( fTime / fFrameSpeed ) = %f\n", fIncInterporation_Position );
	//Console::GetInstance().Messagef( L"fNumVertices_LookAt / ( fTime / fFrameSpeed ) = %f\n", fIncInterporation_LookAt );

	if( fIncInterporation_Position > 1.0f )
	{
		FLOAT fIncIndex = floor( fIncInterporation_Position );

		m_datPosition.iIncIndex = static_cast<INT>( fIncIndex ) + 1;

		fIncInterporation_Position = fIncInterporation_Position - floor( fIncInterporation_Position );
	}
	if( fIncInterporation_LookAt > 1.0f )
	{
		FLOAT fIncIndex = floor( fIncInterporation_LookAt );

		m_datLookAt.iIncIndex = static_cast<INT>( fIncIndex ) + 1;

		fIncInterporation_LookAt = fIncInterporation_LookAt - floor( fIncInterporation_LookAt );
	}

	//Console::GetInstance().Messagef( L"m_iIncIndex = %d\n", m_iIncIndex );
	//Console::GetInstance().Messagef( L"fIncInterporation = %f\n", fIncInterporation );

	for( INT i=0 ; i<m_datPosition.infCourse.iNumVertices ; i++ )
		m_datPosition.infCourse.pfIncInterporation[ i ] = fIncInterporation_Position;

	for( INT i=0 ; i<m_datLookAt.infCourse.iNumVertices ; i++ )
		m_datLookAt.infCourse.pfIncInterporation[ i ] = fIncInterporation_LookAt;


}

//VOID CameraWork::AddWorkingPeriod( INT _iStartIndex, INT _iEndIndex, DWORD _dTime )
//{	//	느려지는 것만 가능하다
//	//	StartIndex, EndIndex Error Check!
//	//	StartIndex, EndIndex 크기 비교
//	if( 0 > _iStartIndex || _iStartIndex > m_Data.infCourse.iNumVertices )
//	{
//		MessageBox( NULL, L"CameraWork::AddWorkingPeriod(){ ... _iStartIndex failed... }", NULL, MB_OK );
//		return;
//	}
//	if( 0 > _iEndIndex || _iEndIndex > m_Data.infCourse.iNumVertices )
//	{
//		MessageBox( NULL, L"CameraWork::AddWorkingPeriod(){ ... _iStartIndex failed... }", NULL, MB_OK );
//		return;
//	}
//
//	FLOAT fSumInterporation = 0.0f, fAverageInterporation = 0.0f;
//	for( INT i=_iStartIndex ; i<_iEndIndex ; i++ )
//		fSumInterporation += m_Data.infCourse.pIncInterporation[ i ];
//
//	fAverageInterporation = fSumInterporation / static_cast<FLOAT>( _iEndIndex - _iStartIndex );
//
//	FLOAT fFrameSpeed	= static_cast<FLOAT>( m_dFrameSpeed );
//	FLOAT fNumVertices	= static_cast<FLOAT>( m_Data.infCourse.iNumVertices );
//
//	DWORD dTime = static_cast<DWORD>( fFrameSpeed * ( fNumVertices / fAverageInterporation ) );
//
//	m_dWorkingPeriod += _dTime;
//
//	dTime += _dTime;
//	FLOAT fTime = static_cast<FLOAT>( dTime );
//	FLOAT fInterporation = fNumVertices / ( fTime / fFrameSpeed );
//
//	for( INT i=_iStartIndex ; i<_iEndIndex ; i++ )
//		m_Data.infCourse.pIncInterporation[ i ] = fInterporation;
//
//}
