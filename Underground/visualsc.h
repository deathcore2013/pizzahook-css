#include "main.h"

#ifndef VISUALCONSOLE
#define VISUALCONSOLE
using namespace std;

class C_ConsoleVisuals
{
public:

	void DrawConsole();

	bool peekMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	bool active;
	string cmdLine;
	int lineIndex;
	int curColorTag;
	int scrollCount;
	vector<string>conLines;
	vector<string>prevLines;
	void Print(const char*,...);
	int	m_iNameCount; 
	void ShowCvarsforCmdline( std::string strCmdLine, int x, int y ,int h , int w );

}extern Console;

#endif