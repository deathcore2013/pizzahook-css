#include "visualsc.h"

C_ConsoleVisuals Console;
void DrawText(const string &str, int x, int y)
{
	const char *line = str.c_str();
	char buf[256], *bufpos;
	for (;;){
		bufpos = buf;
		for (;;){
			*bufpos = *line;
			if (!*line || *line == '^')
				break;
			++line;
			++bufpos;
		}
		bufpos[0] = 0;
		bufpos[1] = 0;
		Draw.PrintText(false, x + 8, y, Color(255, 255, 255, 255), buf);
		x += Draw.GetWidth(buf);
		if (*line == '^'){
			break;
			if (!*++line)
				break;
		}
		else
			break;
	}
}

bool bCvarCompare(std::string strCmdLine, const char* szCvarName)
{
	int iLen1 = (int)strCmdLine.length();
	int iLen2 = (int)strlen(szCvarName);

	if (iLen2 < iLen1)
		return false;

	for (int i = 0; i < iLen1 && i < iLen2; i++)
	{
		char cSign = tolower(strCmdLine[i]);

		if (szCvarName[i] != cSign)
			return false;
	}

	return true;
}

void C_ConsoleVisuals::ShowCvarsforCmdline(std::string strCmdLine, int x, int y, int w, int h)
{
	m_iNameCount = 0;

	if (strCmdLine.length() > 1)
	{
		for (int i = 0; i < ConVars.vCvars.size(); i++)
		{
			C_ConVar* pCVar = ConVars.GetCvarByIndex(i);

			if (bCvarCompare(strCmdLine, pCVar->GetName()))
			{
				m_iNameCount++;  // recount
				int topy = (y -= 18) + 18;
				Draw.FillRGBA(x, topy, w, h, Color(0, 0, 0, 200));
				Draw.DrawOutlineRect(x - 1, topy - 1, w + 1, h + 2, Color(255, 200, 15, 150));
				Draw.PrintText(false, x + 8, topy + 4, Color(255, 255, 255, 255), "Name: %s ->   Value: %s ->   Infos: %s", ConVars.vCvars[i]->GetName(), ConVars.vCvars[i]->GetChar(), ConVars.vCvars[i]->GetInfos());
			}
		}
	}
}

void C_ConsoleVisuals::DrawConsole()
{
	int screenx, screeny;
	EngineSDK->GetScreenSize(screenx, screeny);
	int x = screenx / 4, y = screeny / 4 - 20, w = screenx / 2, h = screeny / 2;

	Draw.FillRGBA(x, y, w, h, Color(0, 0, 0, 200));
	Draw.FillRGBA(x, (screeny - y) - 39, w, 20 - 1, Color(0, 0, 0, 200));
	Draw.DrawOutlineRect(x - 1, y - 1, w + 1, h + 2, Color(0, 255, 255, 150));
	Draw.DrawOutlineRect(x - 1, (screeny - y) - 40, w + 1, 21, Color(0, 255, 255, 150));

	Draw.PrintText(false, x + 8, (screeny - y) - 20 - 10 - 5, Color(255, 255, 255, 255), cmdLine.c_str());

	ShowCvarsforCmdline(cmdLine, x, y - 20, w, 18);

	int fHeight = 12;
	int yOffset = h - (fHeight);
	int yPos = y + yOffset;

	if (!conLines.size())
		return;

	for (int i = conLines.size() - 1 - scrollCount; i > 0; i--)
	{
		string &strLine = conLines[i];
		DrawText(strLine, x, yPos -= fHeight);
		if (yPos - fHeight <= y)
			break;
	}
}

float xPos, yPos;

bool C_ConsoleVisuals::peekMessage(UINT uMsg, WPARAM wParam, LPARAM lParam){
	if (!active)
		return false;

	switch (uMsg){
	case WM_MOUSEWHEEL:
		if ((short)HIWORD(wParam) > 0){
			if (scrollCount < (int)conLines.size())
				scrollCount++;
			return true;
		}
		else{
			if (scrollCount > 0)
				scrollCount--;
			return true;
		}
		return true;
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		switch (wParam){
		case VK_ESCAPE:
		case VK_OEM_2:
			return true;
			break;
		case VK_OEM_3:
			active = false;
			return true;
			break;
		case VK_RETURN:

			char buf[0xFF];
			sprintf(buf, "-> %s", cmdLine.c_str());
			scrollCount = 0;
			lineIndex = prevLines.size() + 1;
			conLines.push_back(buf);
			prevLines.push_back(cmdLine);
			InterpreterConsole.Line(cmdLine.c_str());
			cmdLine.clear();

			return true;
			break;
		case VK_UP:
			if (lineIndex > 0)
				lineIndex--;
			if (lineIndex >= 0 && (unsigned int)lineIndex < prevLines.size())
				cmdLine = prevLines[lineIndex];
			return true;
			break;
		case VK_DOWN:
			lineIndex++;
			if (lineIndex < prevLines.size()){
				if (lineIndex >= 0 && (unsigned int)lineIndex < prevLines.size())
					cmdLine = prevLines[lineIndex];
			}
			else
				cmdLine = "";
			return true;
			break;
		case VK_BACK:
			if (cmdLine.length() > 0)
				cmdLine.erase(cmdLine.length() - 1, cmdLine.length());
			return true;
			break;
		default:
			BYTE btKeyboardState[256];
			char cPressedKey[2] = { 0 };
			if (GetKeyboardState(btKeyboardState)){
				if (ToAscii(wParam, 0, btKeyboardState, (LPWORD)cPressedKey, 0)){
					cmdLine += cPressedKey[0];
					return true;
				}
			}
			break;
		}
		break;
	}
	return false;
}

void C_ConsoleVisuals::Print(const char *szText, ...)
{
	char buf[0xFF];
	vsprintf(buf, szText, (char*)&szText + _INTSIZEOF(szText));
	conLines.push_back(buf);
}

