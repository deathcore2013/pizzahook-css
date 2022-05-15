#ifndef BIND_MANAGER_H
#define BIND_MANAGER_H

#include "main.h"

using namespace std;

void TrimSpace		( const char * szBufferIn, char * szBufferOut );
void TrimQuote		( const char * szBufferIn, char * szBufferOut );
void TrimBrackets	( const char * szBufferIn, char * szBufferOut );

class CString
{
public:
	CString& InsertChar		( int iPosition, char szCharacter );
	CString& RemoveChar		( int iPosition );
	CString& LeftCopy		( int iLength );
	CString& RightCopy		( int iLength );
	CString& MiddleCopy		( int iStart, int iLength );

	void AllocateString		( int iLength, BOOL bKeepString );
	void CopyString			( const char * szString, int iLength );
	void ConcatenateString	( const char * szString, int iLength );
	int Length				( void ) const;
	char * String			( void ) const;
	int Compare				( const char * szString ) const;
	void Erase				( void );
	bool Empty				( void );

	char * operator ( )		( void );
	const char operator [ ]	( int iIndex ) const;
	char& operator [ ]		( int iIndex );

	CString& operator -=	( const int iLength );
	CString& operator +=	( const CString& String );
	CString& operator +=	( const char * szString );
	CString& operator +=	( char szCharacter );

	CString& operator =		( const CString& String );
	CString& operator =		( const char * szString );
	CString& operator =		( char szCharacter );


	CString					( const CString& String );
	CString					( const char * szString );
	CString					( char szCharacter );
	CString					( void );
	~CString				( void );
	
private:
	char *					m_szString;
	int						m_iLength;
	int						m_iMemorySize;
};

class CStringList
{
public:
	int Find					( const char * szString );
	BOOL Add					( const char * szString, BOOL bCheckDup = FALSE );
	BOOL Remove					( const char * szString );
	void IteratorStart			( void );
	BOOL IteratorValid			( void );
	void IteratorNext			( void );
	const char * Iterator		( void );
	bool Empty					( void );
	int Size					( void );
	void Clear					( void );
	const char * operator []	( int iIndex );
	~CStringList				( void );

private:
	std::vector< char * >	m_vStringList;
	char *				m_szIterator;
	int					m_iIndex;
};

enum eTokenizeMode
{
	QUOTE			= (0 << 1), // ignore token in quotes
	INQUOTE			= (1 << 2), // ignore token out quotes
	BRACKET			= (3 << 4), // ignore token in brackets
	INBRACKET		= (4 << 5), // ignore token out brackets
	QUOTEBRACKET	= (5 << 6), // ignore token in quotes and brackets
	INBRACKETQUOTE	= (6 << 7), // ignore token in quotes if in brackets
	INQUOTEBRACKET	= (7 << 8), // ignore token in brackets if in quotes
	UNKNOWN_MODE	= (8 << 9), // None
};

class CTokenize
{
public:
	int TokenizeBuffer			( const char * szBuffer, char * szToken );
	void SetTokenizeMode		( eTokenizeMode TokenizeMode );

	bool Empty					( void );
	int Size					( void );
	void Clear					( void );
	const char * operator []	( int iIndex );

	void Init					( const char * szBuffer, char * szToken );
	CTokenize					( void );
	CTokenize					( const char * szBuffer, char * szToken );
	~CTokenize					( void );


private:
	eTokenizeMode		m_TokenizeMode;
	CStringList			m_vTokenList;
};

BOOL operator ==	( const CString& String, const char * szString );
BOOL operator !=	( const CString& String, const char * szString );
BOOL operator ==	( const char * szString, const CString& String );
BOOL operator !=	( const char * szString, const CString& String );
CString operator +	( const CString& String, char szCharacter );
CString operator +	( char szCharacter, const CString& String );
CString operator +	( const CString& String, const char * szString );
CString operator +	( const char * szString, const CString& String );
CString operator +	( const CString& StringA, const CString& StringB );

struct keybind_s
{
	int		iExecute;
	int		iKeyCode;
	CString sKeyName;
	CString sCommand;
	keybind_s( void )
	{
		iExecute = 0;
		iKeyCode = 0;
		sKeyName = '\0';
		sCommand = '\0';
	}
	~keybind_s( void )
	{
		iExecute = 0;
		iKeyCode = 0;
		sKeyName = '\0';
		sCommand = '\0';
	}
};
typedef struct keybind_s keybind_t, *keybind_p;
typedef std::vector< keybind_p >::iterator BindIter_t;

class cCBindManager
{
public:
	int FindKeyBindByName					( const char * szKeyName );
	int FindKeyBindByCode					( int iKeyCode );
	BOOL AddBindByCode						( int iKeyCode, const char * szCommand );
	BOOL AddBindByName						( const char * szKeyName, const char * szCommand );
	BOOL RemoveBindByCode					( int iKeyCode );
	BOOL RemoveBindByName					( const char * szKeyName );
	int GetKeyCodeByName					( const char * szKeyName );
	const char * GetKeyNameByCode			( int iKeyCode );
	const char * GetKeyCmdByName			( const char * szKeyName );
	const char * GetKeyCmdByCode			( int iKeyCode );

	int GetKeyCode							( LPMSG lpMessage );
	BOOL KeyPress							( int iKeyCode );
	BOOL KeyRelease							( int iKeyCode );
	void HandleMessage						( LPMSG lpMessage );

	void Init								( void );
	cCBindManager							( void );
	~cCBindManager							( void );

public:
	std::vector< keybind_p >		m_vBindList;
	CString							m_vKeyNameList[256];
	CString							m_vShiftKeyNameList[256];
}extern m_pBindManager;

class CAlias
{
public:
	void	Initialize(const char* szName, const char* szContent);
	void	Execute(void);
	string&	strGetContent(void);
	string& strGetName(void);

private:
	string	m_strName;
	string	m_strContent;
};

class CAliasManager
{
public:
	CAlias*	pGetAlias(const char*		szName);
	void	RegisterAlias(const char* pszName, string strContent);
	vector<CAlias>	m_vecAliases;
}extern m_pAliasManager;

#endif //  BIND_MANAGER_H