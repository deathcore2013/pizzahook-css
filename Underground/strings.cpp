#include "strings.h"

void	CAlias::Initialize(const char* szName, const char* szContent)
{
	m_strName = szName;
	m_strContent = szContent;
}

void	CAlias::Execute(void)
{
	if (m_strContent.empty() == false)// pull the content through the interpreter
		InterpreterConsole.Line(m_strContent.c_str());
}

string&	CAlias::strGetContent(void)
{
	return m_strContent;
}

string& CAlias::strGetName(void)
{
	return m_strName;
}

CAliasManager m_pAliasManager;

CAlias*	CAliasManager::pGetAlias(const char*		szName)
{
	for (int i = 0; i < (int)m_vecAliases.size(); i++)
	{
		if (m_vecAliases[i].strGetName().compare(szName) == 0)
			return &m_vecAliases[i];

	}
	return 0;
}

void	CAliasManager::RegisterAlias(const char* pszName, string strContent)
{
	CAlias alias;
	alias.Initialize(pszName, strContent.c_str());
	m_vecAliases.push_back(alias);
}


cCBindManager m_pBindManager;

int cCBindManager::FindKeyBindByName(const char * szKeyName)
{
	if (m_vBindList.empty() == true) { return(-1); }

	for (int i = 0; i < (int)m_vBindList.size(); i++)
	{
		if (szKeyName == m_vBindList[i]->sKeyName)
		{
			return(i);
		}
	}
	return(-1);
}

int cCBindManager::FindKeyBindByCode(int iKeyCode)
{
	if (m_vBindList.empty() == true) { return(-1); }

	for (int i = 0; i < (int)m_vBindList.size(); i++)
	{
		if (iKeyCode == m_vBindList[i]->iKeyCode)
		{
			return(i);
		}
	}
	return(-1);
}

BOOL cCBindManager::AddBindByCode(int iKeyCode, const char * szCommand)
{
	if (iKeyCode >= 0 && iKeyCode < 256)
	{
		if (FindKeyBindByCode(iKeyCode) == -1)
		{
			keybind_p pBind = new keybind_s;
			if (pBind != NULL)
			{
				pBind->iExecute = 0;
				pBind->iKeyCode = iKeyCode;
				pBind->sCommand = szCommand;
				pBind->sKeyName = GetKeyNameByCode(iKeyCode);
				m_vBindList.push_back(pBind);
				return(TRUE);
			}
		}
	}
	return(FALSE);
}

BOOL cCBindManager::AddBindByName(const char * szKeyName, const char * szCommand)
{
	if (FindKeyBindByName(szKeyName) == -1)
	{
		keybind_p pBind = new keybind_s;
		if (pBind != NULL)
		{
			pBind->iExecute = 0;
			pBind->iKeyCode = GetKeyCodeByName(szKeyName);
			pBind->sCommand = szCommand;
			pBind->sKeyName = szKeyName;
			m_vBindList.push_back(pBind);
			return(TRUE);
		}
	}
	return(FALSE);
}

BOOL cCBindManager::RemoveBindByCode(int iKeyCode)
{
	int i = FindKeyBindByCode(iKeyCode);
	if (i != -1)
	{
		delete m_vBindList[i];
		m_vBindList[i] = NULL;
		m_vBindList.erase(m_vBindList.begin() + i);
		return(TRUE);
	}
	return(FALSE);
}

BOOL cCBindManager::RemoveBindByName(const char * szKeyName)
{
	int i = FindKeyBindByName(szKeyName);
	if (i != -1)
	{
		delete m_vBindList[i];
		m_vBindList[i] = NULL;
		m_vBindList.erase(m_vBindList.begin() + i);
		return(TRUE);
	}
	return(FALSE);
}

int cCBindManager::GetKeyCodeByName(const char * szKeyName)
{
	for (int i = 0; i < 256; i++)
	{
		if (szKeyName == m_vKeyNameList[i])
		{
			return(i);
		}
	}
	return(-1);
}

const char* cCBindManager::GetKeyNameByCode(int iKeyCode)
{
	if (iKeyCode >= 0 && iKeyCode < 256)
	{
		return(m_vKeyNameList[iKeyCode]());
	}
	return(NULL);
}

const char* cCBindManager::GetKeyCmdByName(const char * szKeyName)
{
	int i = FindKeyBindByName(szKeyName);
	if (i != -1)
	{
		return(m_vBindList[i]->sCommand());
	}
	return(NULL);
}

const char* cCBindManager::GetKeyCmdByCode(int iKeyCode)
{
	int i = FindKeyBindByCode(iKeyCode);
	if (i != -1)
	{
		return(m_vBindList[i]->sCommand());
	}
	return(NULL);
}

int cCBindManager::GetKeyCode(LPMSG lpMessage)
{
	int iKeyCode = 0;
	iKeyCode = ((lpMessage->lParam >> 16) & 0xFF);
	iKeyCode = (lpMessage->lParam & (1 << 24)) ? (iKeyCode + 100) : iKeyCode;
	iKeyCode = (iKeyCode > 255) ? 255 : iKeyCode;
	return(iKeyCode);
}

BOOL cCBindManager::KeyPress(int iKeyCode)
{
	if (iKeyCode == 0) { return(FALSE); }

	int iIndex = FindKeyBindByCode(iKeyCode);

	if (iIndex != -1)
	{
		char szCommand[1024] = { '\0' };
		if (GetKeyCmdByCode(iKeyCode) != NULL)
		{
			strncpy(szCommand, m_vBindList[iIndex]->sCommand(), 1024);

			if (m_vBindList[iIndex]->iExecute == 0 && szCommand[0] == '+')
			{
				InterpreterConsole.Execute(szCommand);
				m_vBindList[iIndex]->iExecute++;
			}
			else
				InterpreterConsole.Execute(szCommand);

			return(TRUE);
		}
	}
	return(FALSE);
}

BOOL cCBindManager::KeyRelease(int iKeyCode)
{
	if (iKeyCode == 0) { return(FALSE); }

	int iIndex = FindKeyBindByCode(iKeyCode);

	if (iIndex != -1)
	{
		char szCommand[1024] = { '\0' };
		if (GetKeyCmdByCode(iKeyCode) != NULL)
		{
			strncpy(szCommand, m_vBindList[iIndex]->sCommand(), 1024);

			if (m_vBindList[iIndex]->iExecute > 0 && szCommand[0] == '+')
			{
				szCommand[0] = '-';
				InterpreterConsole.Execute(szCommand);
				m_vBindList[iIndex]->iExecute--;
			}
			return(TRUE);
		}
	}
	return(FALSE);
}

void cCBindManager::HandleMessage(LPMSG lpMessage)
{
	int iMessage = lpMessage->message, iKeyCode = 0;
	BOOL bDown = FALSE;
	C_BasePlayer*LocalPlayer = (C_BasePlayer*)EntityListSDK->GetClientEntity(EngineSDK->GetLocalPlayer());

	if (iMessage >= WM_KEYFIRST && iMessage <= WM_KEYLAST)
	{
		iKeyCode = GetKeyCode(lpMessage);
		switch (iMessage)
		{
		case WM_KEYDOWN:	{if (KeyPress(iKeyCode) == TRUE){ lpMessage->message = WM_NULL; return; }}
		case WM_SYSKEYDOWN:	{if (KeyPress(iKeyCode) == TRUE){ lpMessage->message = WM_NULL; return; }}
		case WM_KEYUP:		{if (KeyRelease(iKeyCode) == TRUE){ lpMessage->message = WM_NULL; return; }}
		case WM_SYSKEYUP:	{if (KeyRelease(iKeyCode) == TRUE){ lpMessage->message = WM_NULL; return; }}
		}
	}
	else if (LocalPlayer)
	{
		if (iMessage >= WM_MOUSEFIRST && iMessage <= WM_MOUSELAST)
		{
			static bool wasprev = false;
			switch (iMessage)
			{
			case WM_LBUTTONDBLCLK:		{iKeyCode = 250; bDown = TRUE; break; }
			case WM_LBUTTONDOWN:		{iKeyCode = 250; bDown = TRUE; break; }
			case WM_RBUTTONDBLCLK:		{iKeyCode = 251; bDown = TRUE; break; }
			case WM_RBUTTONDOWN:		{iKeyCode = 251; bDown = TRUE; break; }
			case WM_MBUTTONDBLCLK:		{iKeyCode = 252; bDown = TRUE; break; }
			case WM_MBUTTONDOWN:		{iKeyCode = 252; bDown = TRUE; break; }
			case WM_LBUTTONUP:			{iKeyCode = 250; bDown = FALSE; break; }
			case WM_RBUTTONUP:			{iKeyCode = 251; bDown = FALSE; break; }
			case WM_MBUTTONUP:			{iKeyCode = 252; bDown = FALSE; break; }
			case WM_XBUTTONDOWN:
			{
				if (lpMessage->wParam > 0x1FFFF)
				{
					iKeyCode = 249;
					bDown = TRUE;
				}
				else if (lpMessage->wParam < 0x1FFFF)
				{
					iKeyCode = 248;
					bDown = TRUE;
				}

				break;
			}
			case WM_XBUTTONDBLCLK:
			{
				if (lpMessage->wParam > 0x1FFFF)
				{
					iKeyCode = 249;
					bDown = TRUE;
				}
				else if (lpMessage->wParam < 0x1FFFF)
				{
					iKeyCode = 248;
					bDown = TRUE;
				}
				break;
			}
			case WM_XBUTTONUP:
			{
				if (lpMessage->wParam > 0x1FFFF)
				{
					iKeyCode = 249;
					bDown = FALSE;
				}
				else if (lpMessage->wParam < 0x1FFFF)
				{
					iKeyCode = 248;
					bDown = FALSE;
				}
				break;
			}
			}
			if (iKeyCode != 0)
			{
				if (bDown != FALSE)		{ if (KeyPress(iKeyCode) == TRUE){ lpMessage->message = WM_NULL; return; } }
				else if (bDown != TRUE)	{ if (KeyRelease(iKeyCode) == TRUE){ lpMessage->message = WM_NULL; return; } }
			}
		}
		else if (iMessage == 0x020A || iMessage == (WM_MOUSELAST + 1))
		{
			if (KeyPress(iKeyCode) == TRUE)	{ lpMessage->message = WM_NULL; return; }
			if (((short)HIWORD(lpMessage->wParam)) > 0)	{ iKeyCode = 253; }
			else											{ iKeyCode = 254; }
		}
	}
}

void cCBindManager::Init(void)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		m_vKeyNameList[i] = '\0';
		m_vShiftKeyNameList[i] = '\0';
	}

	m_vKeyNameList[2] = "1";
	m_vKeyNameList[3] = "2";
	m_vKeyNameList[4] = "3";
	m_vKeyNameList[5] = "4";
	m_vKeyNameList[6] = "5";
	m_vKeyNameList[7] = "6";
	m_vKeyNameList[8] = "7";
	m_vKeyNameList[9] = "8";
	m_vKeyNameList[10] = "9";
	m_vKeyNameList[11] = "0";
	m_vKeyNameList[12] = "-";
	m_vKeyNameList[13] = "=";
	m_vKeyNameList[14] = "backspace";
	m_vKeyNameList[15] = "tab";
	m_vKeyNameList[16] = "q";
	m_vKeyNameList[17] = "w";
	m_vKeyNameList[18] = "e";
	m_vKeyNameList[19] = "r";
	m_vKeyNameList[20] = "t";
	m_vKeyNameList[21] = "y";
	m_vKeyNameList[22] = "u";
	m_vKeyNameList[23] = "i";
	m_vKeyNameList[24] = "o";
	m_vKeyNameList[25] = "p";
	m_vKeyNameList[26] = "[";
	m_vKeyNameList[27] = "]";
	m_vKeyNameList[28] = "enter";
	m_vKeyNameList[29] = "ctrl";
	m_vKeyNameList[30] = "a";
	m_vKeyNameList[31] = "s";
	m_vKeyNameList[32] = "d";
	m_vKeyNameList[33] = "f";
	m_vKeyNameList[34] = "g";
	m_vKeyNameList[35] = "h";
	m_vKeyNameList[36] = "j";
	m_vKeyNameList[37] = "k";
	m_vKeyNameList[38] = "l";
	m_vKeyNameList[39] = ";";
	m_vKeyNameList[40] = "'";
	m_vKeyNameList[41] = "`";
	m_vKeyNameList[42] = "shift";
	m_vKeyNameList[43] = "\\";
	m_vKeyNameList[44] = "z";
	m_vKeyNameList[45] = "x";
	m_vKeyNameList[46] = "c";
	m_vKeyNameList[47] = "v";
	m_vKeyNameList[48] = "b";
	m_vKeyNameList[49] = "n";
	m_vKeyNameList[50] = "m";
	m_vKeyNameList[51] = ",";
	m_vKeyNameList[52] = ".";
	m_vKeyNameList[53] = "/";
	m_vKeyNameList[54] = "rshift";
	m_vKeyNameList[55] = "*";
	m_vKeyNameList[56] = "alt";
	m_vKeyNameList[57] = "space";
	m_vKeyNameList[58] = "capslock";
	m_vKeyNameList[59] = "f1";
	m_vKeyNameList[60] = "f2";
	m_vKeyNameList[61] = "f3";
	m_vKeyNameList[62] = "f4";
	m_vKeyNameList[63] = "f5";
	m_vKeyNameList[64] = "f6";
	m_vKeyNameList[65] = "f7";
	m_vKeyNameList[66] = "f8";
	m_vKeyNameList[67] = "f9";
	m_vKeyNameList[68] = "f10";
	m_vKeyNameList[69] = "pause";
	m_vKeyNameList[71] = "kp_home";
	m_vKeyNameList[72] = "kp_uparrow";
	m_vKeyNameList[73] = "kp_pgup";
	m_vKeyNameList[74] = "kp_minus";
	m_vKeyNameList[75] = "kp_leftarrow";
	m_vKeyNameList[76] = "kp_5";
	m_vKeyNameList[77] = "kp_rightarrow";
	m_vKeyNameList[78] = "kp_plus";
	m_vKeyNameList[79] = "kp_end";
	m_vKeyNameList[80] = "kp_downarrow";
	m_vKeyNameList[81] = "kp_pgdn";
	m_vKeyNameList[82] = "kp_ins";
	m_vKeyNameList[83] = "kp_del";
	m_vKeyNameList[87] = "f11";
	m_vKeyNameList[88] = "f12";
	m_vKeyNameList[153] = "kp_slash";
	m_vKeyNameList[171] = "home";
	m_vKeyNameList[172] = "uparrow";
	m_vKeyNameList[173] = "pgup";
	m_vKeyNameList[174] = "minus";
	m_vKeyNameList[175] = "leftarrow";
	m_vKeyNameList[176] = "kp2_5";
	m_vKeyNameList[177] = "rightarrow";
	m_vKeyNameList[178] = "plus";
	m_vKeyNameList[179] = "end";
	m_vKeyNameList[180] = "downarrow";
	m_vKeyNameList[181] = "pgdn";
	m_vKeyNameList[182] = "ins";
	m_vKeyNameList[183] = "del";
	m_vKeyNameList[248] = "mouse4";
	m_vKeyNameList[249] = "mouse5";
	m_vKeyNameList[250] = "mouse1";
	m_vKeyNameList[251] = "mouse2";
	m_vKeyNameList[252] = "mouse3";
	m_vKeyNameList[253] = "mwheelup";
	m_vKeyNameList[254] = "mwheeldown";

	for (i = 0; i < 256; i++)
	{
		m_vShiftKeyNameList[i] = '\0';
		if (m_vKeyNameList[i].Length() == 1 || m_vKeyNameList[i][1] == '\0')
		{
			m_vShiftKeyNameList[i] = toupper(m_vKeyNameList[i][0]);
		}
	}
	m_vShiftKeyNameList[2] = '!';
	m_vShiftKeyNameList[3] = '@';
	m_vShiftKeyNameList[4] = '#';
	m_vShiftKeyNameList[5] = '$';
	m_vShiftKeyNameList[6] = '%';
	m_vShiftKeyNameList[7] = '^';
	m_vShiftKeyNameList[8] = '&';
	m_vShiftKeyNameList[9] = '*';
	m_vShiftKeyNameList[10] = '(';
	m_vShiftKeyNameList[11] = ')';
	m_vShiftKeyNameList[12] = '_';
	m_vShiftKeyNameList[13] = '+';
	m_vShiftKeyNameList[26] = '{';
	m_vShiftKeyNameList[27] = '}';
	m_vShiftKeyNameList[39] = ':';
	m_vShiftKeyNameList[40] = '"';
	m_vShiftKeyNameList[41] = '~';
	m_vShiftKeyNameList[43] = '|';
	m_vShiftKeyNameList[51] = '<';
	m_vShiftKeyNameList[52] = '>';
	m_vShiftKeyNameList[53] = '?';
}

cCBindManager::cCBindManager(void)
{

}

cCBindManager::~cCBindManager(void)
{
	int i;
	for (i = 0; i < (int)m_vBindList.size(); i++)
	{
		delete m_vBindList[i];
		m_vBindList[i] = NULL;
	}
	m_vBindList.clear();
	for (i = 0; i < 256; i++)
	{
		m_vKeyNameList[i] = '\0';
		m_vShiftKeyNameList[i] = '\0';
	}
}


void TrimSpace(const char * szBufferIn, char * szBufferOut)
{
	int z = 0;
	bool bInQuote = false;
	bool bInBracket = false;
	for (int i = 0; i < (int)strlen(szBufferIn); i++)
	{
		if (szBufferIn[i] == '(' &&  bInBracket == false)	{ bInBracket = true; }
		if (szBufferIn[i] == ')' &&  bInBracket == true)	{ bInBracket = false; }
		if (szBufferIn[i] == '\"')							{ bInQuote = ((bInQuote == true) ? false : true); }
		if (bInQuote == false && bInBracket == false)
		{
			if (szBufferIn[i] != ' ' && szBufferIn[i] != '\t')
			{
				szBufferOut[z] = szBufferIn[i];
				z++;
			}
		}
		else
		{
			szBufferOut[z] = szBufferIn[i];
			z++;
		}
	}
	szBufferOut[z] = '\0';
}

void TrimQuote(const char * szBufferIn, char * szBufferOut)
{
	int z = 0;
	for (int i = 0; i < (int)strlen(szBufferIn); i++)
	{
		if (szBufferIn[i] != '\"' && szBufferIn[i] != '\'')
		{
			szBufferOut[z] = szBufferIn[i];
			z++;
		}
	}
	szBufferOut[z] = '\0';
}

void TrimBrackets(const char * szBufferIn, char * szBufferOut)
{
	int z = 0;
	for (int i = 0; i < (int)strlen(szBufferIn); i++)
	{
		if (szBufferIn[i] != '(' && szBufferIn[i] != ')')
		{
			szBufferOut[z] = szBufferIn[i];
			z++;
		}
	}
	szBufferOut[z] = '\0';
}

BOOL operator == (const CString& String, const char * szString)
{
	return(String.Compare(szString) == 0);
}

BOOL operator != (const CString& String, const char * szString)
{
	return(String.Compare(szString) != 0);
}

BOOL operator == (const char * szString, const CString& String)
{
	return(strcmp(szString, String.String()) == 0);
}

BOOL operator != (const char * szString, const CString& String)
{
	return(strcmp(szString, String.String()) != 0);
}

CString operator + (const CString& String, char szCharacter)
{
	CString Result(String);
	Result += szCharacter;

	return(Result);
}

CString operator + (char szCharacter, const CString& String)
{
	CString Result(szCharacter);
	Result += String;

	return(Result);
}

CString operator + (const CString& String, const char * szString)
{
	CString Result(String);
	Result += szString;

	return(Result);
}

CString operator + (const char * szString, const CString& String)
{
	CString Result(szString);

	Result += String;

	return(Result);
}

CString operator + (const CString& StringA, const CString& StringB)
{
	CString Result(StringA);
	Result += StringB;

	return(Result);
}

CString& CString::InsertChar(int iPosition, char szCharacter)
{
	int y = 0, x = 0;
	char * szBuffer = new char[(m_iMemorySize + 2)];
	if (szBuffer != NULL)
	{
		if (m_iMemorySize >= iPosition)
		{
			for (y = 0; y < iPosition; y++)				{ szBuffer[y] = m_szString[y]; }
			szBuffer[iPosition] = szCharacter;				y = (iPosition + 1);
			for (x = iPosition; x < m_iLength; x++, y++)	{ szBuffer[y] = m_szString[x]; }
			szBuffer[y] = 0;
			*this = szBuffer;
		}
		delete[] szBuffer;
		szBuffer = NULL;
	}
	return(*this);
}

CString& CString::RemoveChar(int iPosition)
{
	int y = 0, x = 0;
	char * szBuffer = new char[m_iMemorySize];
	if (szBuffer != NULL)
	{
		if (m_iMemorySize >= iPosition)
		{
			for (y = 0; y < iPosition; y++)				{ szBuffer[y] = m_szString[y]; }
			y = (iPosition + 1);
			for (x = iPosition; x < m_iLength; x++, y++)	{ szBuffer[x] = m_szString[y]; }
			szBuffer[y] = 0;
			*this = szBuffer;
		}
		delete[] szBuffer;
		szBuffer = NULL;
	}
	return(*this);
}

CString& CString::LeftCopy(int iLength)
{
	int z = 0;
	char * szBuffer = new char[(m_iMemorySize + 2)];
	if (szBuffer != NULL)
	{
		if (m_iMemorySize >= iLength)
		{
			for (int i = 0; i < iLength; i++)
			{
				szBuffer[z] = m_szString[i];
				z++;
			}
			szBuffer[z] = '\0';
			*this = szBuffer;
		}
		delete[] szBuffer;
		szBuffer = NULL;
	}

	return(*this);
}

CString& CString::RightCopy(int iLength)
{
	char * szBuffer = new char[(m_iMemorySize + 2)];
	if (szBuffer != NULL)
	{
		if (m_iMemorySize >= iLength)
		{
			strncpy(szBuffer, m_szString, 1024);
			*this = szBuffer[(strlen(szBuffer) - iLength)];
		}
		delete[] szBuffer;
		szBuffer = NULL;
	}
	return(*this);
}

CString& CString::MiddleCopy(int iStart, int iLength)
{
	int z = 0;
	char * szBuffer = new char[(m_iMemorySize + 2)];

	if (szBuffer != NULL)
	{
		if (m_iMemorySize >= iLength)
		{
			for (int i = iStart; i < (iStart + iLength); i++)
			{
				szBuffer[z] = m_szString[i];
				z++;
			}
			szBuffer[z] = '\0';
			*this = szBuffer;
		}
		delete[] szBuffer;
		szBuffer = NULL;
	}
	return(*this);
}

void CString::AllocateString(int iLength, BOOL bKeepString)
{
	char * szBuffer = new char[iLength + 1];
	m_iMemorySize = iLength + 1;
	szBuffer[iLength] = 0;

	if (m_szString != NULL)
	{
		if (bKeepString != FALSE)
		{
			if (iLength < m_iLength) { m_iLength = iLength; }
			memcpy(szBuffer, m_szString, m_iLength);
			szBuffer[m_iLength] = 0;
		}
		delete[] m_szString;
	}

	m_szString = szBuffer;

	if (bKeepString != TRUE)
	{
		m_iLength = 0;
		m_szString[0] = 0;
	}
}

void CString::CopyString(const char * szString, int iLength)
{
	if (m_iMemorySize < (iLength + 1))
	{
		AllocateString(iLength, FALSE);
	}
	m_iLength = iLength;
	memcpy(m_szString, szString, m_iLength);
	m_szString[m_iLength] = '\0';
}

void CString::ConcatenateString(const char * szString, int iLength)
{
	if (m_iMemorySize < ((m_iLength + iLength) + 1))
	{
		AllocateString((m_iLength + iLength), TRUE);
	}

	memcpy(&m_szString[m_iLength], szString, iLength);
	m_iLength = (m_iLength + iLength);
	m_szString[m_iLength] = 0;
}

int CString::Length(void) const
{
	return(m_iLength);
}

char * CString::String(void) const
{
	return(m_szString);
}

int CString::Compare(const char * szString) const
{
	return(strcmp(m_szString, szString));
}

void CString::Erase(void)
{
	if (m_szString != NULL)
	{
		delete[] m_szString;
		m_szString = NULL;
	}
	m_iLength = 0;
	m_iMemorySize = 0;
}

bool CString::Empty(void)
{
	if (m_iMemorySize > 0)
	{
		return((m_szString[0] == '\0') ? TRUE : FALSE);
	}
	return((m_szString == NULL) ? TRUE : FALSE);
}

char * CString::operator ( ) (void)
{
	return(String());
}

const char CString::operator [ ] (int iIndex) const
{
	if (iIndex >= 0 && iIndex < m_iMemorySize)
	{
		return(m_szString[iIndex]);
	}
	return('\0');
}

char& CString::operator [ ] (int iIndex)
{
	static char szChar = '\0';
	if (iIndex >= 0 && iIndex < m_iMemorySize)
	{
		return(m_szString[iIndex]);
	}
	return(szChar);
}

CString& CString::operator -= (const int iLength)
{
	LeftCopy(m_iLength - iLength);
	m_iLength = strlen(m_szString);
	return(*this);
}

CString& CString::operator += (const CString& String)
{
	ConcatenateString(String.String(), String.Length());
	m_iLength = strlen(m_szString);
	return(*this);
}

CString& CString::operator += (const char * szString)
{
	ConcatenateString(szString, strlen(szString));
	m_iLength = strlen(m_szString);
	return(*this);
}

CString& CString::operator += (char szCharacter)
{
	ConcatenateString(&szCharacter, 1);
	m_iLength = strlen(m_szString);
	return(*this);
}

CString& CString::operator = (const CString& String)
{
	Erase();
	CopyString(String.String(), String.Length());
	m_iLength = strlen(m_szString);
	return(*this);
}
CString& CString::operator = (const char * szString)
{
	Erase();
	CopyString(szString, strlen(szString));
	m_iLength = strlen(m_szString);
	return(*this);
}
CString& CString::operator = (char szCharacter)
{
	Erase();
	CopyString(&szCharacter, 1);
	m_iLength = strlen(m_szString);
	return(*this);
}

CString::CString(const CString& String)
{
	CopyString(String.String(), String.Length());
	m_iLength = strlen(m_szString);
}

CString::CString(const char * szString)
{
	CopyString(szString, strlen(szString));
	m_iLength = strlen(m_szString);
}

CString::CString(char szCharacter)
{
	CopyString(&szCharacter, 1);
	m_iLength = strlen(m_szString);
}

CString::CString(void)
{
	m_szString = NULL;
	m_iLength = 0;
	m_iMemorySize = 0;
}

CString::~CString(void)
{
	if (m_szString != NULL)
	{
		delete[] m_szString;
		m_szString = NULL;
	}
	m_iLength = 0;
	m_iMemorySize = 0;
}

int CStringList::Find(const char * szString)
{
	if (m_vStringList.empty() == true) { return(-1); }

	if (szString != NULL)
	{
		for (int i = 0; i < (int)m_vStringList.size(); i++)
		{
			if (strcmp(szString, m_vStringList[i]) == 0)
			{
				return(i);
			}
		}
	}
	return(-1);
}

BOOL CStringList::Add(const char * szString, BOOL bCheckDup)
{
	if (szString != NULL)
	{
		if (bCheckDup != FALSE && Find(szString) != -1) { return(FALSE); }
		int iIndex = (int)m_vStringList.size();
		char * pString = new char[(strlen(szString) + 1)];
		if (pString != NULL)
		{
			m_vStringList.push_back(pString);
			strcpy(m_vStringList[iIndex], szString);
			return(TRUE);
		}
	}
	return(FALSE);
}

BOOL CStringList::Remove(const char * szString)
{
	if (szString != NULL)
	{
		int iIndex = Find(szString);
		if (iIndex != -1)
		{
			delete[] m_vStringList[iIndex];
			m_vStringList[iIndex] = NULL;
			m_vStringList.erase(m_vStringList.begin() + iIndex);
			return(TRUE);
		}
	}
	return(FALSE);
}

void CStringList::IteratorStart(void)
{
	if (m_vStringList.empty() != true)
	{
		m_iIndex = 0;
		m_szIterator = m_vStringList[m_iIndex];
	}
}

BOOL CStringList::IteratorValid(void)
{
	if (m_vStringList.empty() != true)
	{
		if ((m_iIndex + 1) < (int)m_vStringList.size())
		{
			return(TRUE);
		}
	}
	return(FALSE);
}

void CStringList::IteratorNext(void)
{
	if (m_vStringList.empty() != true)
	{
		m_iIndex++;
		m_szIterator = m_vStringList[m_iIndex];
	}
}

const char * CStringList::Iterator(void)
{
	return(m_szIterator);
}

bool CStringList::Empty(void)
{
	return(m_vStringList.empty());
}

int CStringList::Size(void)
{
	return((int)m_vStringList.size());
}

void CStringList::Clear(void)
{
	if (m_vStringList.empty() == true) { return; }

	for (int i = 0; i < (int)m_vStringList.size(); i++)
	{
		delete[]  m_vStringList[i];
		m_vStringList[i] = NULL;
	}
	m_vStringList.clear();
}

const char * CStringList::operator [] (int iIndex)
{
	if (m_vStringList.empty() == true) { return(NULL); }

	if (iIndex >= 0 && iIndex < (int)m_vStringList.size())
	{
		return(m_vStringList[iIndex]);
	}
	return(NULL);
}

CStringList::~CStringList(void)
{
	for (int i = 0; i < (int)m_vStringList.size(); i++)
	{
		delete[] m_vStringList[i];
		m_vStringList[i] = NULL;
	}
	m_vStringList.clear();
	m_szIterator = NULL;
	m_iIndex = 0;
}

int CTokenize::TokenizeBuffer(const char * szBuffer, char * szToken)
{
	char szBuf[1024] = { '\0' };
	bool bInQuote = false;
	bool bInBracket = false;
	int i = 0, p = 0, z = 0, iPosition = 0;
	if (szToken != NULL)
	{
		int iTokenLength = (int)strlen(szToken);
		Clear();
		if (szBuffer != NULL)
		{
			switch (m_TokenizeMode)
			{
			case QUOTE:
			{
				for (i = 0; i < (int)strlen(szBuffer); i++)
				{
					if (szBuffer[i] == '(' &&  bInBracket == false)
					{
						bInBracket = true;
					}
					if (szBuffer[i] == ')' &&  bInBracket == true)
					{
						bInBracket = false;
					}
					if (szBuffer[i] == '\"')
					{
						bInQuote = ((bInQuote == true) ? false : true);
					}
					if (szBuffer[i] == szToken[0] && bInQuote != true)
					{
						if (iTokenLength > 1)
						{
							bool bToken = true;
							int q = 0;
							if ((i + iTokenLength) < (int)strlen(szBuffer))
							{
								for (q = 0; q < iTokenLength; q++)
								{
									if (szBuffer[(i + q)] != szToken[q]) { bToken = false; }
								}
								if (bToken != false)
								{
									szBuf[0] = '\0';
									for (z = 0, p = iPosition; p < i; p++)
									{
										szBuf[z] = szBuffer[p];
										z++;
									}
									szBuf[z] = '\0';
									m_vTokenList.Add(szBuf);
									iPosition = (i + iTokenLength);
								}
							}
						}
						else
						{
							szBuf[0] = '\0';
							for (z = 0, p = iPosition; p < i; p++)
							{
								szBuf[z] = szBuffer[p];
								z++;
							}
							szBuf[z] = '\0';
							m_vTokenList.Add(szBuf);
							iPosition = (i + 1);
						}
					}
				}
				for (z = 0, p = iPosition; p < (int)strlen(szBuffer); p++)
				{
					szBuf[z] = szBuffer[p];
					z++;
				}
				szBuf[z] = '\0';
				m_vTokenList.Add(szBuf);
				return(m_vTokenList.Size());
			} break;
			case INQUOTE:
			{
				for (i = 0; i < (int)strlen(szBuffer); i++)
				{
					if (szBuffer[i] == '(' &&  bInBracket == false)
					{
						bInBracket = true;
					}
					if (szBuffer[i] == ')' &&  bInBracket == true)
					{
						bInBracket = false;
					}
					if (szBuffer[i] == '\"')
					{
						bInQuote = ((bInQuote == true) ? false : true);
					}
					if (szBuffer[i] == szToken[0] && bInQuote != false)
					{
						if (iTokenLength > 1)
						{
							bool bToken = true;
							int q = 0;
							if ((i + iTokenLength) < (int)strlen(szBuffer))
							{
								for (q = 0; q < iTokenLength; q++)
								{
									if (szBuffer[(i + q)] != szToken[q]) { bToken = false; }
								}
								if (bToken != false)
								{
									szBuf[0] = '\0';
									for (z = 0, p = iPosition; p < i; p++)
									{
										szBuf[z] = szBuffer[p];
										z++;
									}
									szBuf[z] = '\0';
									m_vTokenList.Add(szBuf);
									iPosition = (i + iTokenLength);
								}
							}
						}
						else
						{
							szBuf[0] = '\0';
							for (z = 0, p = iPosition; p < i; p++)
							{
								szBuf[z] = szBuffer[p];
								z++;
							}
							szBuf[z] = '\0';
							m_vTokenList.Add(szBuf);
							iPosition = (i + 1);
						}
					}
				}
				for (z = 0, p = iPosition; p < (int)strlen(szBuffer); p++)
				{
					szBuf[z] = szBuffer[p];
					z++;
				}
				szBuf[z] = '\0';
				m_vTokenList.Add(szBuf);
				return(m_vTokenList.Size());
			} break;
			case BRACKET:
			{
				for (i = 0; i < (int)strlen(szBuffer); i++)
				{
					if (szBuffer[i] == '(' &&  bInBracket == false)
					{
						bInBracket = true;
					}
					if (szBuffer[i] == ')' &&  bInBracket == true)
					{
						bInBracket = false;
					}
					if (szBuffer[i] == '\"')
					{
						bInQuote = ((bInQuote == true) ? false : true);
					}
					if (szBuffer[i] == szToken[0] && bInBracket != true)
					{
						if (iTokenLength > 1)
						{
							bool bToken = true;
							int q = 0;
							if ((i + iTokenLength) < (int)strlen(szBuffer))
							{
								for (q = 0; q < iTokenLength; q++)
								{
									if (szBuffer[(i + q)] != szToken[q]) { bToken = false; }
								}
								if (bToken != false)
								{
									szBuf[0] = '\0';
									for (z = 0, p = iPosition; p < i; p++)
									{
										szBuf[z] = szBuffer[p];
										z++;
									}
									szBuf[z] = '\0';
									m_vTokenList.Add(szBuf);
									iPosition = (i + iTokenLength);
								}
							}
						}
						else
						{
							szBuf[0] = '\0';
							for (z = 0, p = iPosition; p < i; p++)
							{
								szBuf[z] = szBuffer[p];
								z++;
							}
							szBuf[z] = '\0';
							m_vTokenList.Add(szBuf);
							iPosition = (i + 1);
						}
					}
				}
				for (z = 0, p = iPosition; p < (int)strlen(szBuffer); p++)
				{
					szBuf[z] = szBuffer[p];
					z++;
				}
				szBuf[z] = '\0';
				m_vTokenList.Add(szBuf);
				return(m_vTokenList.Size());
			} break;
			case INBRACKET:
			{
				for (i = 0; i < (int)strlen(szBuffer); i++)
				{
					if (szBuffer[i] == '(' &&  bInBracket == false)
					{
						bInBracket = true;
					}
					if (szBuffer[i] == ')' &&  bInBracket == true)
					{
						bInBracket = false;
					}
					if (szBuffer[i] == '\"')
					{
						bInQuote = ((bInQuote == true) ? false : true);
					}
					if (szBuffer[i] == szToken[0] && bInBracket != false)
					{
						if (iTokenLength > 1)
						{
							bool bToken = true;
							int q = 0;
							if ((i + iTokenLength) < (int)strlen(szBuffer))
							{
								for (q = 0; q < iTokenLength; q++)
								{
									if (szBuffer[(i + q)] != szToken[q]) { bToken = false; }
								}
								if (bToken != false)
								{
									szBuf[0] = '\0';
									for (z = 0, p = iPosition; p < i; p++)
									{
										szBuf[z] = szBuffer[p];
										z++;
									}
									szBuf[z] = '\0';
									m_vTokenList.Add(szBuf);
									iPosition = (i + iTokenLength);
								}
							}
						}
						else
						{
							szBuf[0] = '\0';
							for (z = 0, p = iPosition; p < i; p++)
							{
								szBuf[z] = szBuffer[p];
								z++;
							}
							szBuf[z] = '\0';
							m_vTokenList.Add(szBuf);
							iPosition = (i + 1);
						}
					}
				}
				for (z = 0, p = iPosition; p < (int)strlen(szBuffer); p++)
				{
					szBuf[z] = szBuffer[p];
					z++;
				}
				szBuf[z] = '\0';
				m_vTokenList.Add(szBuf);
				return(m_vTokenList.Size());
			} break;
			case QUOTEBRACKET:
			{
				for (i = 0; i < (int)strlen(szBuffer); i++)
				{
					if (szBuffer[i] == '(' &&  bInBracket == false)
					{
						bInBracket = true;
					}
					if (szBuffer[i] == ')' &&  bInBracket == true)
					{
						bInBracket = false;
					}
					if (szBuffer[i] == '\"')
					{
						bInQuote = ((bInQuote == true) ? false : true);
					}
					if (szBuffer[i] == szToken[0] && bInBracket != true && bInQuote != true)
					{
						if (iTokenLength > 2)
						{
							bool bToken = true;
							int q = 0;
							if ((i + iTokenLength) < (int)strlen(szBuffer))
							{
								for (q = 0; q < iTokenLength; q++)
								{
									if (szBuffer[(i + q)] != szToken[q]) { bToken = false; }
								}
								if (bToken != false)
								{
									szBuf[0] = '\0';
									for (z = 0, p = iPosition; p < i; p++)
									{
										szBuf[z] = szBuffer[p];
										z++;
									}
									szBuf[z] = '\0';
									m_vTokenList.Add(szBuf);
									iPosition = (i + iTokenLength);
								}
							}
						}
						else
						{
							szBuf[0] = '\0';
							for (z = 0, p = iPosition; p < i; p++)
							{
								szBuf[z] = szBuffer[p];
								z++;
							}
							szBuf[z] = '\0';
							m_vTokenList.Add(szBuf);
							iPosition = (i + 1);
						}
					}
				}
				for (z = 0, p = iPosition; p < (int)strlen(szBuffer); p++)
				{
					szBuf[z] = szBuffer[p];
					z++;
				}
				szBuf[z] = '\0';
				m_vTokenList.Add(szBuf);
				return(m_vTokenList.Size());
			} break;
			case INBRACKETQUOTE:
			{
				for (i = 0; i < (int)strlen(szBuffer); i++)
				{
					if (szBuffer[i] == '(' &&  bInBracket == false)
					{
						bInBracket = true;
					}
					if (szBuffer[i] == ')' &&  bInBracket == true)
					{
						bInBracket = false;
					}
					if (szBuffer[i] == '\"')
					{
						bInQuote = ((bInQuote == true) ? false : true);
					}
					if (szBuffer[i] == szToken[0] && bInBracket != false && bInQuote != true)
					{
						if (iTokenLength > 1)
						{
							bool bToken = true;
							int q = 0;
							if ((i + iTokenLength) < (int)strlen(szBuffer))
							{
								for (q = 0; q < iTokenLength; q++)
								{
									if (szBuffer[(i + q)] != szToken[q]) { bToken = false; }
								}
								if (bToken != false)
								{
									szBuf[0] = '\0';
									for (z = 0, p = iPosition; p < i; p++)
									{
										szBuf[z] = szBuffer[p];
										z++;
									}
									szBuf[z] = '\0';
									m_vTokenList.Add(szBuf);
									iPosition = (i + iTokenLength);
								}
							}
						}
						else
						{
							szBuf[0] = '\0';
							for (z = 0, p = iPosition; p < i; p++)
							{
								szBuf[z] = szBuffer[p];
								z++;
							}
							szBuf[z] = '\0';
							m_vTokenList.Add(szBuf);
							iPosition = (i + 1);
						}
					}
				}
				for (z = 0, p = iPosition; p < (int)strlen(szBuffer); p++)
				{
					szBuf[z] = szBuffer[p];
					z++;
				}
				szBuf[z] = '\0';
				m_vTokenList.Add(szBuf);
				return(m_vTokenList.Size());
			} break;
			case INQUOTEBRACKET:
			{
				for (i = 0; i < (int)strlen(szBuffer); i++)
				{
					if (szBuffer[i] == '(' &&  bInBracket == false)
					{
						bInBracket = true;
					}
					if (szBuffer[i] == ')' &&  bInBracket == true)
					{
						bInBracket = false;
					}
					if (szBuffer[i] == '\"')
					{
						bInQuote = ((bInQuote == true) ? false : true);
					}
					if (szBuffer[i] == szToken[0] && bInQuote != false && bInBracket != true)
					{
						if (iTokenLength > 1)
						{
							bool bToken = true;
							int q = 0;
							if ((i + iTokenLength) < (int)strlen(szBuffer))
							{
								for (q = 0; q < iTokenLength; q++)
								{
									if (szBuffer[(i + q)] != szToken[q]) { bToken = false; }
								}
								if (bToken != false)
								{
									szBuf[0] = '\0';
									for (z = 0, p = iPosition; p < i; p++)
									{
										szBuf[z] = szBuffer[p];
										z++;
									}
									szBuf[z] = '\0';
									m_vTokenList.Add(szBuf);
									iPosition = (i + iTokenLength);
								}
							}
						}
						else
						{
							szBuf[0] = '\0';
							for (z = 0, p = iPosition; p < i; p++)
							{
								szBuf[z] = szBuffer[p];
								z++;
							}
							szBuf[z] = '\0';
							m_vTokenList.Add(szBuf);
							iPosition = (i + 1);
						}
					}
				}
				for (z = 0, p = iPosition; p < (int)strlen(szBuffer); p++)
				{
					szBuf[z] = szBuffer[p];
					z++;
				}
				szBuf[z] = '\0';
				m_vTokenList.Add(szBuf);
				return(m_vTokenList.Size());
			} break;
			case UNKNOWN_MODE:
			{
				for (i = 0; i < (int)strlen(szBuffer); i++)
				{
					if (szBuffer[i] == szToken[0])
					{
						if (iTokenLength > 1)
						{
							bool bToken = true;
							int q = 0;
							if ((i + iTokenLength) < (int)strlen(szBuffer))
							{
								for (q = 0; q < iTokenLength; q++)
								{
									if (szBuffer[(i + q)] != szToken[q]) { bToken = false; }
								}
								if (bToken != false)
								{
									szBuf[0] = '\0';
									for (z = 0, p = iPosition; p < i; p++)
									{
										szBuf[z] = szBuffer[p];
										z++;
									}
									szBuf[z] = '\0';
									m_vTokenList.Add(szBuf);
									iPosition = (i + iTokenLength);
								}
							}
						}
						else
						{
							szBuf[0] = '\0';
							for (z = 0, p = iPosition; p < i; p++)
							{
								szBuf[z] = szBuffer[p];
								z++;
							}
							szBuf[z] = '\0';
							m_vTokenList.Add(szBuf);
							iPosition = (i + 1);
						}
					}
				}
				for (z = 0, p = iPosition; p < (int)strlen(szBuffer); p++)
				{
					szBuf[z] = szBuffer[p];
					z++;
				}
				szBuf[z] = '\0';
				m_vTokenList.Add(szBuf);
				return(m_vTokenList.Size());
			} break;
			}
		}
	}
	return(0);
}

void CTokenize::SetTokenizeMode(eTokenizeMode TokenizeMode)
{
	m_TokenizeMode = TokenizeMode;
}

bool CTokenize::Empty(void)
{
	return(m_vTokenList.Empty());
}

int CTokenize::Size(void)
{
	return((int)m_vTokenList.Size());
}

void CTokenize::Clear(void)
{
	m_vTokenList.Clear();
}

const char * CTokenize::operator [] (int iIndex)
{
	if (iIndex >= 0 && iIndex < (int)m_vTokenList.Size())
	{
		return(m_vTokenList[iIndex]);
	}
	return(NULL);
}

void CTokenize::Init(const char * szBuffer, char * szToken)
{
	TokenizeBuffer(szBuffer, szToken);
}

CTokenize::CTokenize(void)
{
	m_TokenizeMode = UNKNOWN_MODE;
}

CTokenize::CTokenize(const char * szBuffer, char * szToken)
{
	Init(szBuffer, szToken);
}

CTokenize::~CTokenize(void)
{
	m_vTokenList.Clear();
}
