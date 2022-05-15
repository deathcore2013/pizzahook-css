//100eSRC Menu
#include <Windows.h>
#include "Menu.h"

UndergroundMenu Menu;

bool bMenu = false;

float submenu2, submenu3;
float menu1, menu2, menu3, menu4, menu5, menu6, menu7, menu8, menu9, menu10, menu11, menu12, menu13, menu14, posmenu;
struct a_menu{ char title[1000]; float* value; float min; float max; float step; };
int menuIndex = 0; int menuItems = 0; a_menu amenu[150];

int UndergroundMenu::AddMenuEntry(int n, char title[1000], float* value, float min, float max, float step)
{
	strcpy(amenu[n].title, title);
	amenu[n].value = value;
	amenu[n].min = min;
	amenu[n].max = max;
	amenu[n].step = step;
	return (n + 1);
}
float menu_x = int(UndergroundVars.xpos); //int(gCvars.xpos) or just gCvars.xpos
float menu_y = int(UndergroundVars.ypos); //int(gCvars.ypos) or just gCvars.ypos
float menu_w = int(UndergroundVars.weight); //int(gCvars.weight) or just gCvars.weight
float red = 1;
float green = 1;
float blue = 1;
float alpha = 200;


void UndergroundMenu::InitializeMenu()
{

	int i = 0;
	if (!menu1)
		i = AddMenuEntry(i, "[Aimbot]", &menu1, 0, 1, 1);
	if (menu1){
		i = AddMenuEntry(i, "[Aimbot]", &menu1, 0, 1, 1);
		i = AddMenuEntry(i, "Enable Aimbot", &UndergroundVars.enableaim, 0, 1, 1);
		i = AddMenuEntry(i, "Silent´Aim", &UndergroundVars.silentaim, 0, 1, 1);
	}

	if (!menu2)
		i = AddMenuEntry(i, "[Visuals & ESP]", &menu2, 0, 1, 1);
	if (menu2){
		i = AddMenuEntry(i, "[Visuals & ESP]", &menu2, 0, 1, 1);
		i = AddMenuEntry(i, "Name", &UndergroundVars.name, 0, 1, 1);
		i = AddMenuEntry(i, "Weapon", &UndergroundVars.weapon, 0, 1, 1);

	}

	if (!menu3)
		i = AddMenuEntry(i, "[Removals]", &menu3, 0, 1, 1);
	if (menu3){
		i = AddMenuEntry(i, "[Removals]", &menu3, 0, 1, 1);
		i = AddMenuEntry(i, "Perfect Accuracy", &UndergroundVars.perfectaccuracy, 0, 1, 1);
		i = AddMenuEntry(i, "Force Low Spread", &UndergroundVars.forcelowspread, 0, 1, 1);
	}
	if (!menu6)
		i = AddMenuEntry(i, "[AntiAim's]", &menu6, 0, 1, 1);
	if (menu6){
		i = AddMenuEntry(i, "[AntiAim's]", &menu6, 0, 1, 1);
		i = AddMenuEntry(i, "Static Anti-Aim", &UndergroundVars.antiaim, 0, 1, 1);
		i = AddMenuEntry(i, "Test AA", &UndergroundVars.antiaim3, 0, 1, 1);
		i = AddMenuEntry(i, "Spinbot - AA ", &UndergroundVars.antiaim2, 0, 1, 1);
	}
		if (!menu4)
			i = AddMenuEntry(i, "[Misc]", &menu4, 0, 1, 1);
			if (menu4){
			i = AddMenuEntry(i, "[Misc]", &menu4, 0, 1, 1);
			i = AddMenuEntry(i, "Roundsay", &UndergroundVars.roundsay, 0, 1, 1);
			i = AddMenuEntry(i, "Fakelag", &UndergroundVars.fakelag, 0, 16, 1);
			i = AddMenuEntry(i, "Fakelag IN_ATTACK", &UndergroundVars.fakelaginattack, 0, 16, 1);
			}

		if (!menu5)
			i = AddMenuEntry(i, "[Menu]", &menu5, 0, 1, 1);
		if (menu5){
			i = AddMenuEntry(i, "[Menu]", &menu5, 0, 1, 1);
			i = AddMenuEntry(i, "Red", &UndergroundVars.red, 0, 255, 1);
			i = AddMenuEntry(i, "Green", &UndergroundVars.green, 0, 255, 1);
			i = AddMenuEntry(i, "Blue", &UndergroundVars.blue, 0, 255, 1);
			i = AddMenuEntry(i, "Alpha", &UndergroundVars.alpha, 0, 255, 1);
			i = AddMenuEntry(i, "X Position", &UndergroundVars.xpos, 0, 5000/*2000*/, 1);
			i = AddMenuEntry(i, "Y Position", &UndergroundVars.ypos, 0, 5000/*2000*/, 1);
			i = AddMenuEntry(i, "Weight", &UndergroundVars.weight, 0, 5000/*2000*/, 1);
		}

		menuItems = i;

	}

void UndergroundMenu::FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{
	SurfaceSDK->DrawSetColor(r, g, b, a);
	SurfaceSDK->DrawFilledRect(x, y, x + w, y + h);
}

void UndergroundMenu::TextA(vgui::HFont font, int x, int y, int r, int g, int b, int a, char *pszString)
{
	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(pszString) + 1) * sizeof(wchar_t)));

	mbstowcs(pszStringWide, pszString, (strlen(pszString) + 1) * sizeof(wchar_t));

	TextW(font, x, y, r, g, b, a, pszStringWide);

	free(pszStringWide);
}

void UndergroundMenu::TextW(vgui::HFont font, int x, int y, int r, int g, int b, int a, wchar_t *pszString)
{
	SurfaceSDK->DrawSetTextColor(r, g, b, a);
	SurfaceSDK->DrawSetTextFont(font);
	SurfaceSDK->DrawSetTextPos(x, y);
	SurfaceSDK->DrawPrintText(pszString, (int)wcslen(pszString)/*, FONT_DRAW_DEFAULT*//*Error with FONT_DRAW_DEFAULT*/); //wbuf, wcslen(wbuf) FONT_DRAW_DEFAULT
}

void UndergroundMenu::CenterText(vgui::HFont font, int x, int y, int r, int g, int b, int a, char *text, ...)
{
	if (text == NULL) return;

	va_list va_alist;

	char TotalBufferMaxLength[1024] = { 0 };

	memset(TotalBufferMaxLength, 0, sizeof(char)* 1024);

	va_start(va_alist, text);

	_vsnprintf(TotalBufferMaxLength + strlen(TotalBufferMaxLength),
		sizeof(TotalBufferMaxLength)-strlen(TotalBufferMaxLength),
		text, va_alist);

	va_end(va_alist);

	//	int CharLength = GMatSurface->DrawTextLen( font, TotalBufferMaxLength );

	int CharLength = strlen(TotalBufferMaxLength) * 5;

	int xx = x - (CharLength / 2);

	TextA(font, xx, y, r, g, b, a, TotalBufferMaxLength);
}

void UndergroundMenu::Border(int x, int y, int w, int h)
{
	FillRGBA(x - 1, y - 1, w + 2, 1, 0, 0, 0, 255); //top
	FillRGBA(x - 1, y, 1, h - 1, 0, 0, 0, 255);	//left
	FillRGBA(x + w, y, 1, h - 1, 0, 0, 0, 255);	//right
	FillRGBA(x - 1, y + h - 1, w + 2, 1, 0, 0, 0, 255); //bottom
}

void UndergroundMenu::DrawString(bool Centered, int x, int y, DWORD Colour, const char *fmt, ...){
	va_list va_alist;
	char buf[256];
	va_start(va_alist, fmt);
	_vsnprintf(buf, sizeof(buf), fmt, va_alist);
	va_end(va_alist);
	wchar_t wbuf[256];
	//g_pVGUILocalize->ConvertANSIToUnicode(buf, wbuf, 256);
	int iWidth, iHeight;
	SurfaceSDK->GetTextSize(Draw.ConsoleFont, wbuf, iWidth, iHeight);
	SurfaceSDK->DrawSetTextFont(Draw.ConsoleFont);
	SurfaceSDK->DrawSetTextColor(RED(Colour), GREEN(Colour), BLUE(Colour), ALPHA(Colour));
	SurfaceSDK->DrawSetTextPos(x - (Centered ? iWidth / 2 : 0), y);
	SurfaceSDK->DrawPrintText(wbuf, wcslen(wbuf));
}

void UndergroundMenu::DrawWindow(int x, int y, int w, int h, char* title)
{
	float CenterX = x + w - 6;
	float CenterY = y - 14;
	FillRGBA(x, y + 5, w, h, int(UndergroundVars.red), int(UndergroundVars.green), int(UndergroundVars.blue)/*255,0,0*/, int(UndergroundVars.alpha)/*150*/);
	Border(x, y + 5, w, h);

	//oben
	//DrawString(Fonts::m_hThird,false,15,10,COLORCODE(255,0,0,255),"Zylx - Hack");

	FillRGBA(x + 4, y, w - 8, 1, 0, 0, 0, 200);
	FillRGBA(x + 2, y + 1, 3, 1, 0, 0, 0, 200);
	FillRGBA(x + 5, y + 1, w - 10, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 5, y + 1, 3, 1, 0, 0, 0, 200);

	FillRGBA(x + 1, y + 2, 2, 1, 0, 0, 0, 200);
	FillRGBA(x + 3, y + 2, 2, 1, 0, 0, 0, 200);
	FillRGBA(x + 5, y + 2, w - 10, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 5, y + 2, 2, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 3, y + 2, 2, 1, 0, 0, 0, 200);

	FillRGBA(x + 1, y + 3, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + 2, y + 3, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + 3, y + 3, w - 6, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 3, y + 3, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 2, y + 3, 1, 1, 0, 0, 0, 200);

	FillRGBA(x, y + 4, 2, 1, 0, 0, 0, 200);
	FillRGBA(x + 2, y + 4, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + 3, y + 4, w - 6, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 3, y + 4, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 2, y + 4, 2, 1, 0, 0, 0, 200);


	// unten

	FillRGBA(x, y + h + 5, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + 1, y + h + 5, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + 2, y + h + 5, w - 4, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 2, y + h + 5, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 1, y + h + 5, 1, 1, 0, 0, 0, 200);

	FillRGBA(x, y + h + 6, 2, 1, 0, 0, 0, 200);
	FillRGBA(x + 2, y + h + 6, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + 3, y + h + 6, w - 6, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 3, y + h + 6, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 2, y + h + 6, 2, 1, 0, 0, 0, 200);

	FillRGBA(x + 1, y + h + 7, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + 2, y + h + 7, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + 3, y + h + 7, w - 6, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 3, y + h + 7, 1, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 2, y + h + 7, 1, 1, 0, 0, 0, 200);

	FillRGBA(x + 1, y + h + 8, 2, 1, 0, 0, 0, 200);
	FillRGBA(x + 3, y + h + 8, 2, 1, 0, 0, 0, 200);
	FillRGBA(x + 5, y + h + 8, w - 10, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 5, y + h + 8, 2, 1, 0, 0, 0, 200);
	FillRGBA(x + w - 3, y + h + 8, 2, 1, 0, 0, 0, 200);

}

void UndergroundMenu::DrawStringMenu(bool Centered, int x, int y, DWORD dwColor, const char *pszText, ...){
	if (pszText == NULL)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);
	int iWidth, iHeight;
	SurfaceSDK->GetTextSize(Draw.ConsoleFont, szString, iWidth, iHeight);
	SurfaceSDK->DrawSetTextFont(Draw.ConsoleFont);
	SurfaceSDK->DrawSetTextPos(x - (Centered ? iWidth / 2 : 0), y);
	SurfaceSDK->DrawSetTextColor(Color(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor)));
	SurfaceSDK->DrawPrintText(szString, wcslen(szString));
}

void UndergroundMenu::MenuDrawing()
{

	if (GetAsyncKeyState(VK_INSERT) & 1) bMenu = !bMenu;

	if (!bMenu)
		return;



	KeyMenuEvent();
	int i = menuItems;

	DrawWindow(int(UndergroundVars.xpos) - 10, int(UndergroundVars.ypos) - 8, int(UndergroundVars.weight), 10 + (14 * i) + 5, "");

	for (int i = 0; i<menuItems; i++)
	if (i != menuIndex)
	{
		TextA(Draw.ConsoleFont, int(UndergroundVars.xpos), int(UndergroundVars.ypos) + 2 + (14 * i) + 2, 255, 255, 255, 255, amenu[i].title);
		if (amenu[i].value[0] == 0)
			CenterText(Draw.ConsoleFont, int(UndergroundVars.xpos) + 58, int(UndergroundVars.ypos) + 2 + (14 * i) + 2, 255, 0, 0, 255, "                                                                                                                                                                     %2.2f", amenu[i].value[0]);
		else
			CenterText(Draw.ConsoleFont, int(UndergroundVars.xpos) + 58, int(UndergroundVars.ypos) + 2 + (14 * i) + 2, 0, 255, 0, 255, "                                                                                                                                                                     %2.2f", amenu[i].value[0]);
	}

	else

	{
		TextA(Draw.ConsoleFont, int(UndergroundVars.xpos), int(UndergroundVars.ypos) + 2 + (14 * i) + 2, 255, 255, 255, 255, amenu[i].title);
		if (amenu[i].value[0] == 0)
			CenterText(Draw.ConsoleFont, int(UndergroundVars.xpos) + 58, int(UndergroundVars.ypos) + 2 + (14 * i) + 2, 255, 0, 0, 255, "                                                                                                                                                                   > %2.2f", amenu[i].value[0]);
		else
			CenterText(Draw.ConsoleFont, int(UndergroundVars.xpos) + 58, int(UndergroundVars.ypos) + 2 + (14 * i) + 2, 0, 255, 0, 255, "                                                                                                                                                                   > %2.2f", amenu[i].value[0]);
	}
}

void UndergroundMenu::KeyMenuEvent()
{
	if (GetAsyncKeyState(VK_UP) & 1)
	{
		if (menuIndex>0) menuIndex--;
		else menuIndex = menuItems - 1;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 1)
	{
		if (menuIndex<menuItems - 1) menuIndex++;
		else menuIndex = 0;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 1)
	{
		if (amenu[menuIndex].value)
		{
			amenu[menuIndex].value[0] -= amenu[menuIndex].step;
			if (amenu[menuIndex].value[0] < amenu[menuIndex].min)
				amenu[menuIndex].value[0] = amenu[menuIndex].max;
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 1)
	{
		if (amenu[menuIndex].value)
		{
			amenu[menuIndex].value[0] += amenu[menuIndex].step;
			if (amenu[menuIndex].value[0] > amenu[menuIndex].max)
				amenu[menuIndex].value[0] = amenu[menuIndex].min;
		}
	}
}

