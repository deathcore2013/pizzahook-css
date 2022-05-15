#include "main.h"

#ifndef TERP
#define TERP

class C_ConVar;
class CInterpreterConsole
{
public:
	void Line(const char*);
	void Execute( char*yourmother,...);
	std::vector <std::string> m_vecWords;
	string m_strCommandLine;
	bool bExtractAlias	( string& strContent, char* szLine );
	void HandleHlCvarAction( ConVar* pCvar );
	char* pszGetWord( int Index );
	void Save(const char*);
	void HandleCvarAction( C_ConVar* pCvar );

}extern InterpreterConsole;

#endif