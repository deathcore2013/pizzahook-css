#include "prediction.h"

typedef void(*SPSFn)(CUserCmd*Cmd);
typedef void(__thiscall*SetupMoveFn)(void*, C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move);

CMyPrediction MyPrediction;

CMyPrediction::CMyPrediction()
{
	Helper = 0;
	oldcurtime = 0;
	oldframetime = 0;
}

void CMyPrediction::RunLocalPrediction(CUserCmd*Cmd, C_BasePlayer*LocalPlayer)
{
	MoveData = *(CMoveData*)(CLIENT(0x515154));

	oldcurtime = Globals->curtime;
	oldframetime = Globals->frametime;

	Globals->curtime = *(int*)((DWORD)LocalPlayer + Offsets->Player.TickBase) * Globals->interval_per_tick;
	Globals->frametime = Globals->interval_per_tick;

	C_Weapon*weapon = (C_Weapon*)(EntityListSDK->GetClientEntityFromHandle(OffsetManualthis(Offsets->Player.ActiveWeapon, CBaseHandle, LocalPlayer)));
	{
		bool Update = true;

		if (weapon) 
		{

			if ((weapon->GetID() == WEAPON_GLOCK) || (weapon->GetID() == WEAPON_FAMAS))
			{
				if (weapon->SpecialMode(weapon->OffsetIndexForSpecialWeapons()))
				{
					if (weapon->GetBurstShotsRemaining(weapon->OffsetIndexForSpecialWeapons()) > 0)
						Update = false;
				}
			}

			if (Update)
			{
				weapon->UpdateAccuracyPenalty();
			}
		}
	}

	OldPlayer = (*(C_BasePlayer**)(CLIENT(0x539988)));
	OldCmd = (*(CUserCmd**)((DWORD)LocalPlayer + Offsets->Player.CurrentCommand));

	(*(C_BasePlayer**)(CLIENT(0x539988))) = LocalPlayer;
	(*(CUserCmd**)((DWORD)LocalPlayer + Offsets->Player.CurrentCommand)) = Cmd;
	((SPSFn)(CLIENT(0x678B0)))(Cmd);

	((SetupMoveFn)(CLIENT(0x1DE7A0)))(PredictionSDK, LocalPlayer, Cmd, Helper, &MoveData);
	GameMovementSDK->ProcessMovement(LocalPlayer, &MoveData);
	PredictionSDK->FinishMove(LocalPlayer, Cmd, &MoveData);

}

void C_BasePlayer::SetPunchAngle( const QAngle &angle )
{
	m_Local.m_vecPunchAngle = angle;
}

void CMyPrediction::FinishLocalPrediction(CUserCmd*Cmd, C_BasePlayer*LocalPlayer)
{
	(*(C_BasePlayer**)(CLIENT(0x539988))) = 0;
	(*(CUserCmd**)((DWORD)LocalPlayer + Offsets->Player.CurrentCommand)) = 0;
	((SPSFn)(CLIENT(0x678B0)))(0);

	Globals->curtime = oldcurtime;
	Globals->frametime = oldframetime;
	bool *SendPacket = (bool*)(*(DWORD*)GetEBP() - 0x1);
	if (*SendPacket == true)
	cmd = *Cmd;
}

void CMyPrediction::RunSimulation(CUserCmd*Cmd, C_BasePlayer*LocalPlayer)
{
	Globals->curtime = *(int*)((DWORD)LocalPlayer + Offsets->Player.TickBase) * Globals->interval_per_tick;
	Globals->frametime = Globals->interval_per_tick;

	typedef void(__thiscall*RunSimulationFn)(void*, int current_command, float curtime, CUserCmd *cmd, C_BasePlayer *localPlayer);
	((RunSimulationFn)CLIENT(0x17AD30))(PredictionSDK, Cmd->command_number, Globals->curtime, Cmd, LocalPlayer);
}

void CMyPrediction::RunEntsPrediction(CUserCmd*Cmd, C_BasePlayer*LocalPlayer)
{
	for (int i = 1; i < 64; i++)
	{
		C_BasePlayer*Player = (C_BasePlayer*)EntityListSDK->GetClientEntity(i);
		if (Player == 0) continue;
		if (Player == LocalPlayer) continue;
	}

}