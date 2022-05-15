//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//
#include <Windows.h>
#include "main.h"
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef CMENU_H
#define CMENU_H
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class UndergroundMenu
{
public:

	int   AddMenuEntry(int n, char title[1000], float* value, float min, float max, float step);
	void  InitializeMenu();
	void  MenuDrawing();
	void  KeyMenuEvent();
	void DrawWindow(int x, int y, int w, int h, char* title);
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	void DrawString(bool Centered, int x, int y, DWORD Colour, const char *fmt, ...);
	void Border(int x, int y, int w, int h);
	void TextA(vgui::HFont font, int x, int y, int r, int g, int b, int a, char *pszString);
	void TextW(vgui::HFont font, int x, int y, int r, int g, int b, int a, wchar_t *pszString);
	void CenterText(vgui::HFont font, int x, int y, int r, int g, int b, int a, char *text, ...);
	void DrawStringMenu(bool Centered, int x, int y, DWORD dwColor, const char *pszText, ...);



};
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
extern UndergroundMenu Menu;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#endif
