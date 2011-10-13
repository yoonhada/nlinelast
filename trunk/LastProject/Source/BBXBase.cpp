#include "stdafx.h"
#include "BBXBase.h"

VOID BBXBase::Initialize()
{
}

VOID BBXBase::Release()
{
}

VOID BBXBase::StrTrim( LPWSTR _sLine )
{
	//	한 줄씩 읽어서 체크하는 것이라 Line으로~
	INT iLen	=	0;	//	문자열 길이
	INT iCnt	=	0;	//	?

	iLen = lstrlen( _sLine );

	if( iLen < 1 )	//	비어있는 문자는 짜져주세요
		return;

		//	\r \n 제거
	for( INT i=iLen-1 ; i>=0 ; i-- )
	{
		LPWSTR pstrTemp = _sLine + i;

		if( L'\n' == *pstrTemp || L'\r' == *pstrTemp )
			*( _sLine + i ) = L'\0';

		iCnt++;

		if( iCnt > 2 )
			break;
	}
	
	//	우측 공백 제거
	iLen	=	lstrlen( _sLine );
	iCnt	=	0;
	
	for( INT i=iLen-1 ; i>= 0 ; i-- )
	{
		LPWSTR pstrTemp = _sLine + i;

		if( ' ' == *pstrTemp || L'\t' == *pstrTemp )
			continue;

		*( _sLine + i + 1 ) = L'\0';
		iLen = i + 1 + 1;
		break;
	}

	//	좌측 공백제거
	TCHAR str[ 516 ] = { 0 };

	wcsncpy( str, _sLine, iLen );

	for( INT i=0 ; i<iLen ; i++ )
	{
		LPWSTR pstrTemp = str + i;

		if( L' ' == *pstrTemp || L'\t' == *pstrTemp )
			continue;
		
		iLen = i;
		break;
	}

	lstrcpy( _sLine, str + iLen );
}
VOID BBXBase::StrQuot( LPWSTR pDst, LPCWSTR pSrc )
{
	//	'\"' '\"' 잘라내기

	INT iLen	=	lstrlen( pSrc );
	INT nBgn	=	-1;
	INT nStrt	=	0;
	INT iRead	=	0;

	LPWSTR	str =	(LPWSTR)pSrc;

	while( 0 != *str )
	{
		if( '\"' == *str && 0 == nStrt )
			nStrt	=	1;
		else if( '\"' == *str && 1 == nStrt )
		{
			*( pDst + nBgn ) = 0;
			break;
		}

		if( nBgn >= 0 && 1 == nStrt )
			*( pDst + nBgn ) = *str;

		if( 1 == nStrt )
			nBgn++;

		str++;
	}
}

VOID BBXBase::SetBBXKey( LPBBXKEY _pBBXKey, DWORD _Type, LPWSTR _Str )
{
	lstrcpy( _pBBXKey[ _Type ].sKey, _Str );
	_pBBXKey[ _Type ].iKey = lstrlen( _pBBXKey[ _Type ].sKey );
}

//VOID BBXBase::SetVertex( LPVERTEX _pVertex, FLOAT _x, FLOAT _y, FLOAT _z, D3DCOLOR _Color )
VOID BBXBase::SetVertex( LPDATA _pData )
{
	D3DXVECTOR3	vPivot = _pData->Info.vPivot;

	_pData->aVertex[ 0 ].pos.x = vPivot.x + _pData->Info.fMinusSize[ 0 ];
	_pData->aVertex[ 0 ].pos.y = vPivot.y + _pData->Info.fPlusSize[ 1 ];
	_pData->aVertex[ 0 ].pos.z = vPivot.z + _pData->Info.fPlusSize[ 2 ];
	_pData->aVertex[ 0 ].color = _pData->Info.Color;
		
	_pData->aVertex[ 1 ].pos.x = vPivot.x + _pData->Info.fPlusSize[ 0 ];
	_pData->aVertex[ 1 ].pos.y = vPivot.y + _pData->Info.fPlusSize[ 1 ];
	_pData->aVertex[ 1 ].pos.z = vPivot.z + _pData->Info.fPlusSize[ 2 ];
	_pData->aVertex[ 1 ].color = _pData->Info.Color;
		
	_pData->aVertex[ 2 ].pos.x = vPivot.x + _pData->Info.fPlusSize[ 0 ];
	_pData->aVertex[ 2 ].pos.y = vPivot.y + _pData->Info.fPlusSize[ 1 ];
	_pData->aVertex[ 2 ].pos.z = vPivot.z + _pData->Info.fMinusSize[ 2 ];
	_pData->aVertex[ 2 ].color = _pData->Info.Color;
		
	_pData->aVertex[ 3 ].pos.x = vPivot.x + _pData->Info.fMinusSize[ 0 ];
	_pData->aVertex[ 3 ].pos.y = vPivot.y + _pData->Info.fPlusSize[ 1 ];
	_pData->aVertex[ 3 ].pos.z = vPivot.z + _pData->Info.fMinusSize[ 2 ],
	_pData->aVertex[ 3 ].color = _pData->Info.Color;
		
	_pData->aVertex[ 4 ].pos.x = vPivot.x + _pData->Info.fMinusSize[ 0 ];
	_pData->aVertex[ 4 ].pos.y = vPivot.y + _pData->Info.fMinusSize[ 1 ];
	_pData->aVertex[ 4 ].pos.z = vPivot.z + _pData->Info.fPlusSize[ 2 ];
	_pData->aVertex[ 4 ].color = _pData->Info.Color;

	_pData->aVertex[ 5 ].pos.x = vPivot.x + _pData->Info.fPlusSize[ 0 ];
	_pData->aVertex[ 5 ].pos.y = vPivot.y + _pData->Info.fMinusSize[ 1 ];
	_pData->aVertex[ 5 ].pos.z = vPivot.z + _pData->Info.fPlusSize[ 2 ];
	_pData->aVertex[ 5 ].color = _pData->Info.Color;
		
	_pData->aVertex[ 6 ].pos.x = vPivot.x + _pData->Info.fPlusSize[ 0 ];
	_pData->aVertex[ 6 ].pos.y = vPivot.y + _pData->Info.fMinusSize[ 1 ];
	_pData->aVertex[ 6 ].pos.z = vPivot.z + _pData->Info.fMinusSize[ 2 ];
	_pData->aVertex[ 6 ].color = _pData->Info.Color;
		
	_pData->aVertex[ 7 ].pos.x = vPivot.x + _pData->Info.fMinusSize[ 0 ];
	_pData->aVertex[ 7 ].pos.y = vPivot.y + _pData->Info.fMinusSize[ 1 ];
	_pData->aVertex[ 7 ].pos.z = vPivot.z + _pData->Info.fMinusSize[ 2 ];
	_pData->aVertex[ 7 ].color = _pData->Info.Color;
}
VOID BBXBase::SetIndex( LPINDEX _pIndex )
{
	_pIndex[ 0 ]._0 = 0;	_pIndex[ 0 ]._1 = 1;	//	0 -> 1
	_pIndex[ 1 ]._0 = 1;	_pIndex[ 1 ]._1 = 2;	//	1 -> 2
	_pIndex[ 2 ]._0 = 2;	_pIndex[ 2 ]._1 = 3;	//	2 -> 3
	_pIndex[ 3 ]._0 = 3;	_pIndex[ 3 ]._1 = 0;	//	3 -> 0

	_pIndex[ 4 ]._0 = 0;	_pIndex[ 4 ]._1 = 4;	//	0 -> 4
	_pIndex[ 5 ]._0 = 1;	_pIndex[ 5 ]._1 = 5;	//	1 -> 5
	_pIndex[ 6 ]._0 = 2;	_pIndex[ 6 ]._1 = 6;	//	2 -> 6
	_pIndex[ 7 ]._0 = 3;	_pIndex[ 7 ]._1 = 7;	//	3 -> 7

	_pIndex[ 8 ]._0 = 4;	_pIndex[ 8 ]._1 = 5;	//	4 -> 5
	_pIndex[ 9 ]._0 = 5;	_pIndex[ 9 ]._1 = 6;	//	5 -> 6
	_pIndex[ 10 ]._0 = 6;	_pIndex[ 10 ]._1 = 7;	//	6 -> 7
	_pIndex[ 11 ]._0 = 7;	_pIndex[ 11 ]._1 = 4;	//	7 -> 4

}