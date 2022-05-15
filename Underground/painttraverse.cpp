#include "main.h"
#include <time.h>

char Roundsay[256];

class cGameEvent : public IGameEventListener2{
public:
	void FireGameEvent(IGameEvent* event);
	void RegisterSelf();
};

cGameEvent gEvent;

void cGameEvent::FireGameEvent(IGameEvent* event)
{

	const char* szEventName = event->GetName();
	if (!szEventName)
		return;

	if (strcmp(szEventName, "round_start") == 0 && UndergroundVars.roundsay == 1)
	
		sprintf(Roundsay, "upd4t3");
		EngineSDK->ClientCmd(Roundsay);
	


}



void cGameEvent::RegisterSelf()
{
	GameEventManagerSDK->AddListener(this, "player_connect", false);
	GameEventManagerSDK->AddListener(this, "round_start", false);
}

typedef void(__thiscall*PaintTraverseFn)(void*, int, bool, bool);
void __fastcall new_PaintTraverse(void*thisptr, int edx, int vguiPanel, bool forceRepaint, bool allowForce)
{
	Panel->GetMethod<PaintTraverseFn>(41)(thisptr, vguiPanel, forceRepaint, allowForce);

	C_BasePlayer* LocalPlayer = (C_BasePlayer*)EntityListSDK->GetClientEntity(EngineSDK->GetLocalPlayer());
	if (!LocalPlayer) return;

	{
		DWORD Add = ENGINE(0x2ED258);
		DWORD old;
		VirtualProtect((LPVOID)Add, 4, PAGE_READWRITE, &old);
		char test[] = "upd4t3";
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)Add, &test, sizeof(test), 0);
		VirtualProtect((LPVOID)Add, 4, old, &old);
	}

	const char* Name = PanelSDK->GetName(vguiPanel);
	if (strcmp(Name, "MatSystemTopPanel")) return;

	gEvent.RegisterSelf();

	int screenx, screeny;
	EngineSDK->GetScreenSize(screenx, screeny);
	int x = screenx / 2, y = screeny / 10;

	Menu.InitializeMenu();
	Menu.MenuDrawing();

	Draw.PrintText(false, 10, 10, Color(255, 255, 255, 255), "Private Cheat Leaked by deathcore2013");

	if (GetKeyState(0x45))
	{
		Draw.PrintText(false, 10, 30, Color(0, 255, 255, 255), "BodyAim Activated");
	}
	else if (!GetKeyState(0x45))
	{
		Draw.PrintText(false, 10, 30, Color(0, 255, 255, 255), "BodyAim Deactivated");
	}

	for (int i = Globals->maxClients; i >= 1; --i)
	{
		if (i == EngineSDK->GetLocalPlayer()) continue;

		C_BasePlayer *targetEntity = (C_BasePlayer*)EntityListSDK->GetClientEntity(i);

		if (targetEntity == 0) continue;

		if (targetEntity->IsAlive() && !targetEntity->IsDormant())
		{
			COLORREF GetTeamColor = RGB(255, 255, 255);

			Vector TargetPosition = targetEntity->GetAbsOrigin();
			Vector EyePosition = targetEntity->EyePosition();
			int PlayerFlags = targetEntity->GetFlags();
			Vector VecOrigin, AbsOrigin;

			player_info_t PlayerInfo;
			EngineSDK->GetPlayerInfo(targetEntity->index, &PlayerInfo);

			if (targetEntity->GetTeamNumber() == LocalPlayer->GetTeamNumber()) continue;

			if (targetEntity->GetTeamNumber() == 2) GetTeamColor = RGB(255, 125, 0);
			if (targetEntity->GetTeamNumber() == 3) GetTeamColor = RGB(0, 125, 255);


			if (Draw.WorldToScreen(EyePosition, VecOrigin))
			{
				if (Draw.WorldToScreen(TargetPosition, AbsOrigin))
				{
					float Width = 3.9;
					float Height = 8;


					int xOffset = (AbsOrigin.y - VecOrigin.y) / Width;
					int yOffset = (AbsOrigin.y - VecOrigin.y) / Height;

					int leftX = VecOrigin.x - xOffset;
					int rightX = VecOrigin.x + xOffset;
					int topY = VecOrigin.y - yOffset;
					int botY = AbsOrigin.y - VecOrigin.y + (yOffset * 1.9);
					int width = rightX - leftX;


					Draw.DrawOutlineRect(leftX + 1, topY + 1, width - 2, botY - 2, Color(0, 0, 0, 255));
					Draw.DrawOutlineRect(leftX - 1, topY - 1, width + 2, botY + 2, Color(0, 0, 0, 255));
					Draw.DrawOutlineRect(leftX, topY, width, botY, Color(GetRValue(GetTeamColor), GetGValue(GetTeamColor), GetBValue(GetTeamColor), 255));


					COLORREF GetHealthColor = RGB(15, 161, 0);
					if (targetEntity->GetHealth() <= 90) GetHealthColor = RGB(22, 223, 0);
					if (targetEntity->GetHealth() <= 80) GetHealthColor = RGB(74, 255, 54);
					if (targetEntity->GetHealth() <= 70) GetHealthColor = RGB(168, 255, 54);
					if (targetEntity->GetHealth() <= 60) GetHealthColor = RGB(216, 255, 54);
					if (targetEntity->GetHealth() <= 50) GetHealthColor = RGB(243, 255, 54);
					if (targetEntity->GetHealth() <= 40) GetHealthColor = RGB(236, 197, 0);
					if (targetEntity->GetHealth() <= 30) GetHealthColor = RGB(236, 165, 0);
					if (targetEntity->GetHealth() <= 20) GetHealthColor = RGB(208, 0, 0);
					if (targetEntity->GetHealth() > 100) GetHealthColor = RGB(255, 255, 255);
					float hpwidth;

					if (targetEntity->GetHealth() > 100)
					{
						hpwidth = 1;
					}
					else hpwidth = (targetEntity->GetHealth() / 100.f);


					Draw.DrawOutlineRect(leftX, topY + botY + 3, width, 4, Color(0, 0, 0, 255));
					Draw.FillRGBA(leftX + 1, topY + botY + 4, width * hpwidth - 2, 2, GetRValue(GetHealthColor), GetGValue(GetHealthColor), GetBValue(GetHealthColor), 255);

					C_Weapon* Weapon = (C_Weapon*)(EntityListSDK->GetClientEntityFromHandle(OffsetManualthis(Offsets->Player.ActiveWeapon, CBaseHandle, targetEntity)));

					if (Weapon)
					{
						int id = Weapon->GetID();
						const char *tmp = Weapon->GetWeaponAlias(id);
						char weaponName[0xFF];
						strcpy(weaponName, tmp);

						char *c = weaponName;

						while (*c)
						{
							*c = toupper(*c);
							++c;
						}
						if (UndergroundVars.name)
						{
							Draw.PrintText(false, rightX + 8, topY + 4, Color(255, 255, 255, 255), "%s", PlayerInfo.name);
						}
						if (UndergroundVars.weapon)
						{
							Draw.PrintText(false, rightX + 8, topY + 14, Color(255, 255, 255, 255), "%s", weaponName);
						}
					}

				}

			}
		}
	}
}
