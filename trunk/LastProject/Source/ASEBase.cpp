#include "stdafx.h"
#include "ASEBase.h"


VOID ASEBase::Initialize()
{
}

VOID ASEBase::Release()
{
}

VOID ASEBase::StrTrim( LPWSTR _sLine )
{
	//	���پ� �о üũ�ϴ� ���̶� Line����~
	INT iLen	=	0;	//	���ڿ� ����
	INT	iCnt	=	0;	//	?

	iLen = lstrlen( _sLine );

	if( iLen < 1 )	//	����ִ� ���ڴ� ¥��~
		return;

	//	\r \n ����
	for( INT i=iLen-1 ; i>=0 ; i-- )
	{
		LPWSTR pstrTemp = _sLine + i;

		if( L'\n' == *pstrTemp || L'\r' == *pstrTemp )
			*( _sLine + i ) = L'\0';

		iCnt++;

		if( iCnt > 2 )
			break;
	}
	
	//	���� ���� ����
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

	//	���� ��������
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
VOID ASEBase::StrQuot( LPWSTR pDst, LPCWSTR pSrc )
{
	//	'\"' '\"' �߶󳻱�

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

VOID ASEBase::SetAseKey( LPASEKEY _pAseKey, DWORD _Type, LPWSTR _Str )
{
	lstrcpy( _pAseKey[ _Type ].sKey, _Str );
	_pAseKey[ _Type ].iKey = lstrlen( _pAseKey[ _Type ].sKey );

}