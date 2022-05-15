#include "interp.h"

CInterpreterConsole InterpreterConsole;

void Copylowerchar(string& strLine)
{
	int iLength = (int)strLine.length();

	for (int i = 0; i <= iLength; i++)
		strLine[i] = tolower(strLine[i]);
}

void CInterpreterConsole::Line(const char* pszLine)
{
	if (pszLine == 0)
		return;

	string strLine = pszLine;

	Copylowerchar(strLine);

	m_strCommandLine = strLine;

	// find end:
	char* pszFrom = const_cast<char*>(strLine.c_str());
	char* pszTo = pszFrom;

	while (*pszFrom == ' ' || *pszFrom == '\t')
	{
		++pszFrom;
		++pszTo;
	} // skip whitespaces

	while (*pszTo >= ' ' &&*pszTo != ';')
	{
		if (*pszTo == '\"')
		{
			do
			{
				++pszTo;
			} while (*pszTo &&*pszTo != '\"');
		}
		else
		{
			++pszTo;
		}
	}

	do{
		// comments...
		if (pszFrom[0] == '/' && pszFrom[1] == '/')
			return;

		// split up and exec
		if (pszFrom < pszTo)
		{
			char cOldChar = *pszTo;
			*pszTo = 0;
			Execute(pszFrom);
			*pszTo = cOldChar;
		}

		// advance
		if (*pszTo == 0)
			break;

		++pszTo;
		pszFrom = pszTo;

		while (*pszFrom == ' ' || *pszFrom == '\t')
		{
			++pszFrom;
			++pszTo;
		}  // skip ws

		while (*pszTo >= ' ' &&*pszTo != ';')
			++pszTo;              // find end
	} while (true);
}

void CInterpreterConsole::HandleHlCvarAction(ConVar* pCvar)
{
	if (m_vecWords.size() <= 1)
	{
		Console.Print("Cvar: %s - %f", pCvar->GetName(), pCvar->GetFloat());
	}
	else
	{
		pCvar->m_nFlags &= ~FCVAR_PROTECTED;
		pCvar->m_nFlags &= ~FCVAR_SPONLY;
		pCvar->m_nFlags &= ~FCVAR_CHEAT;
		pCvar->m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
		pCvar->SetValue(m_vecWords[1].c_str());
		Console.Print("Cvar: %s - %f", pCvar->GetName(), pCvar->GetFloat());
	}
}

void CInterpreterConsole::HandleCvarAction(C_ConVar* pCvar)
{
	if (m_vecWords.size() <= 1)
	{
		Console.Print("Cvar: %s - %s", pCvar->m_cName, pCvar->m_cValue);
	}
	else
	{
		pCvar->SetValue(m_vecWords[1].c_str());
		Console.Print("Cvar: %s - %s", pCvar->m_cName, pCvar->m_cValue);
	}
}

void CInterpreterConsole::Execute(char*pszCommand, ...)
{

	m_vecWords.clear();
	string strCurrentWord;

	char *pszCommandtmp = pszCommand;

	while (true)
	{
		do
		{
			strCurrentWord += *pszCommandtmp;
			pszCommandtmp++;
		} while (*pszCommandtmp &&*pszCommandtmp != ';' &&*pszCommandtmp != '\t' &&*pszCommandtmp != ' ');

		m_vecWords.push_back(strCurrentWord);
		strCurrentWord.erase(strCurrentWord.begin(), strCurrentWord.end());

		while (*pszCommandtmp &&*pszCommandtmp != ';' && (*pszCommandtmp == '\t' || *pszCommandtmp == ' '))
			pszCommandtmp++;

		if (*pszCommandtmp == 0 && *pszCommandtmp != ';')
			break;
	}


	if (m_vecWords.size() == 0)
		return;

	C_ConVar*		pMyConVar = (C_ConVar*)ConVars.GetCvarByName(m_vecWords[0].c_str());
	ConVar*			pConVar = (ConVar*)CvarSDK->FindVar(m_vecWords[0].c_str());
	CAlias*			pAlias = (CAlias*)m_pAliasManager.pGetAlias(m_vecWords[0].c_str());

	if (pMyConVar)
	{
		HandleCvarAction(pMyConVar);
		return;

	}

	if (pConVar)
	{
		HandleHlCvarAction(pConVar);
		return;
	}

	if (pAlias)
	{
		pAlias->Execute();
		return;
	}

	if (m_vecWords[0].compare("help") == 0)
	{
		Console.Print("To show all convars type:   \"cvars\"   you can set them to every letters you want but its not recommanded on some cvars");
		Console.Print("bind example:   bind <key> \"<cvar>\" - bind f1 \"gamecmd say hello!\" ");
		Console.Print("unbind example:   unbind <key> - unbind f1 ");
		Console.Print("alias example:   +alias \"<cvar> 1;<cvar2> 1;...\" -alias \"<cvar> 0;<cvar2> 0;...\" ");
		Console.Print("To save type:   \"save\"   <namefile> ");
		Console.Print("To load type:   \"load\"   <namefile> ");
		Console.Print("Some cvars won't work on the console, since they're dependent on game's console.");
		Console.Print("Therefore you might wanna use \"gamecmd\" in front of the command that is going to be managed by the game's console.");
		Console.Print("Example : \"game_cmd say lol\". <say> beeing a game's console cmd, you just add gamecmd infront of the command.");
		Console.Print("\"record_demo\" to record movements");
		Console.Print("\"play_demo\" to play your movements saved (you can repeat the movements)");
		Console.Print("\"stop_demo\" to stop everything releated to recording/playing movements");
		Console.Print("\"auto_demo\" to autoplay your movements after recording");
		Console.Print("\"save_demo\" to save your movements recorded previously in your PC; usage: \"save_demo\" <namefile>");
		Console.Print("\"load_demo\" to load your movements recorded previously stored in your PC; usage: \"load_demo\" <namefile>");
		Console.Print("\"find_demo_start\" after demo recorded or loaded use this to find the exact start position");
		Console.Print("\"infos_demo_start\" after demo recorded or loaded use this to show the exact start position and angles");
	}
	else if (m_vecWords[0].compare("game_cmd") == 0)
	{
		EngineSDK->ClientCmd(pszCommand + 7);
	}
	else if (m_vecWords[0].compare("record_demo") == 0)
	{
		MovementRecorder.Situation = RECORDING;
		Console.Print("Recording movements...");
	}
	else if (m_vecWords[0].compare("play_demo") == 0)
	{
		MovementRecorder.Situation = PLAYING;
		Console.Print("Playing movements...");
	}
	else if (m_vecWords[0].compare("stop_demo") == 0)
	{
		MovementRecorder.Situation = NOTHING;
		Console.Print("Stopped every recording/playing movements...");
	}
	else if (m_vecWords[0].compare("save_demo") == 0)
	{
		if (m_vecWords.size() <= 1)
		{
			Console.Print("enter a name please");
			return;
		}

		Macro.CurrentName = pszGetWord(1);
		Macro.Save = true;
		Macro.Load = false;
		Console.Print("saved %s", pszGetWord(1));
	}
	else if (m_vecWords[0].compare("load_demo") == 0)
	{
		if (m_vecWords.size() <= 1)
		{
			Console.Print("enter a name please");
			return;
		}

		Macro.CurrentName = pszGetWord(1);
		Macro.Load = true;
		Macro.Save = false;
		Console.Print("loaded %s", pszGetWord(1));
	}
	else if (m_vecWords[0].compare("auto_demo") == 0)
	{
		MovementRecorder.AutoPlayer = !MovementRecorder.AutoPlayer;
		if (MovementRecorder.AutoPlayer)
			Console.Print("Autoplaying movements...");
		else
			Console.Print("Stop Autoplaying movements...");
	}
	else if (m_vecWords[0].compare("find_demo_start") == 0)
	{
		MovementRecorder.Situation = STARTPOS;
		Console.Print("Finding first position...");
	}
	else if (m_vecWords[0].compare("infos_demo_start") == 0)
	{
		Console.Print("pos: %f %f %f; angles: %f %f %f", MovementRecorder.DrawPath[0].x, MovementRecorder.DrawPath[0].y, MovementRecorder.DrawPath[0].z,
			MovementRecorder.Movements[0].viewangles.x, MovementRecorder.Movements[0].viewangles.y, MovementRecorder.Movements[0].viewangles.z);
		char pos[0xFF];
		sprintf(pos, "setpos %f %f %f; setang %f %f %f", MovementRecorder.DrawPath[0].x, MovementRecorder.DrawPath[0].y, MovementRecorder.DrawPath[0].z,
			MovementRecorder.Movements[0].viewangles.x, MovementRecorder.Movements[0].viewangles.y, MovementRecorder.Movements[0].viewangles.z);

		const size_t len = strlen(pos) + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
		memcpy(GlobalLock(hMem), pos, len);
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}
	else if (m_vecWords[0].compare("cvars") == 0)
	{
		for (int i = 0; i < ConVars.vCvars.size(); ++i)
		{
			Console.Print("Name: %s ->   Value: %s ->   Infos: %s", ConVars.vCvars[i]->GetName(), ConVars.vCvars[i]->GetChar(), ConVars.vCvars[i]->GetInfos());
		}

	}
	else if (m_vecWords[0].compare("save") == 0)
	{
		if (m_vecWords.size() <= 1)
			return;

		Save(szDirFileConfigDll(pszGetWord(1)).c_str());

		Console.Print("saved %s", pszGetWord(1));

	}
	else if (m_vecWords[0].compare("load") == 0)
	{
		if (m_vecWords.size() <= 1)
			return;

		string strFile = szDirFileConfigDll(pszGetWord(1));

		ifstream ifs(strFile.c_str());

		if (!ifs)
		{
			Console.Print("could'nt load (the file doesn't exist)");
			return;
		}

		char buf[0xFF];

		while (ifs)
		{
			ifs.getline(buf, 250, 10);

			if (!ifs)
				break;

			Line(buf);
		}

	}
	else if (m_vecWords[0].compare("alias") == 0)
	{
		string strContent = "";

		if (m_vecWords.size() < 3)
		{
			Console.Print("Error Alias");
			return;
		}

		if (bExtractAlias(strContent, (char*)m_strCommandLine.c_str()) && m_vecWords[1].empty() == false)
		{
			if (strContent.empty() == false)
				m_pAliasManager.RegisterAlias(m_vecWords[1].c_str(), strContent);
		}

		else if (m_vecWords[1].empty() == false)
		{
			m_pAliasManager.RegisterAlias(m_vecWords[1].c_str(), m_vecWords[2]);
		}

		Console.Print("alias : %s", m_vecWords[1].c_str());
	}
	else if (m_vecWords[0].compare("bind") == 0)
	{
		string strContent = "";

		if (m_vecWords.size() < 3)
		{
			Console.Print("Error Bind");
			return;
		}

		if (bExtractAlias(strContent, (char*)m_strCommandLine.c_str()) && m_vecWords[1].empty() == false)
		{
			if (strContent.empty() == false)
			{
				m_pBindManager.AddBindByName((char*)m_vecWords[1].c_str(), (char*)strContent.c_str());
			}
		}
		else if (m_vecWords[1].empty() == false)
		{
			m_pBindManager.AddBindByName((char*)m_vecWords[1].c_str(), (char*)m_vecWords[2].c_str());
		}

		Console.Print("binded : %s", m_vecWords[1].c_str());

	}
	else if (m_vecWords[0].compare("unbind") == 0)
	{
		m_pBindManager.RemoveBindByName((char*)m_vecWords[1].c_str());
		Console.Print("unbinded : %s", m_vecWords[1].c_str());

	}
	else
	{
		Console.Print("Invalid Command : %s", m_vecWords[0].c_str());

	}

}

bool CInterpreterConsole::bExtractAlias(string& strContent, char* szLine)
{
	char* pszFirstToken = strchr(szLine, '"');

	if (pszFirstToken == 0)
		return false;

	pszFirstToken++;// skip the first separator


	strContent.erase(strContent.begin(), strContent.end());

	for (; *pszFirstToken; pszFirstToken++)
	{
		if (*pszFirstToken == '"')
			return true;
		else
			strContent += *pszFirstToken;
	}

	return true; // token isn't terminated but let's try to compensate the bug in the scripts
}

char* CInterpreterConsole::pszGetWord(int Index)
{
	if (Index < (int)m_vecWords.size())
	{
		return(char*)m_vecWords[Index].c_str();
	}

	return 0;
}


void CInterpreterConsole::Save(const char* pszFileName)
{
	if (pszFileName != NULL)
	{
		static char szLine[3][0xFF][0xFF];
		static char buffer[0xFF][0xFF];

		static ofstream oLogFile;

		oLogFile.open(pszFileName, ios::out);

		if (oLogFile && oLogFile.is_open())
		{

			for (int i = 0; i < ConVars.vCvars.size(); i++)
			{
				sprintf(szLine[0][i], "%s %s \n", ConVars.vCvars[i]->GetName(), ConVars.vCvars[i]->GetChar());
			}

			for (int i = 0; i < m_pBindManager.m_vBindList.size(); i++)
			{
				sprintf(szLine[1][i], "bind %s \"%s\"\n", m_pBindManager.m_vBindList[i]->sKeyName(), m_pBindManager.m_vBindList[i]->sCommand());
			}

			for (int i = 0; i < m_pAliasManager.m_vecAliases.size(); i++)
			{
				sprintf(szLine[2][i], "alias %s \"%s\"\n", m_pAliasManager.m_vecAliases[i].strGetName().c_str(), m_pAliasManager.m_vecAliases[i].strGetContent().c_str());
			}

			for (int i = 0; i < 0xFF; i++)
			{
				strcpy(buffer[i], szLine[0][i]);
				strcat(buffer[i], szLine[1][i]);
				strcat(buffer[i], szLine[2][i]);

				oLogFile.write(buffer[i], (streamsize)strlen(buffer[i]));
			}

			oLogFile.close();
		}
	}

}