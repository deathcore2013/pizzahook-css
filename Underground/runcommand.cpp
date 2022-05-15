#include "definitions.h"

typedef void(__thiscall*RunCommandFn)(void*, C_BasePlayer*localplayer, CUserCmd*cmd, IMoveHelper*movehelper);
void __fastcall new_RunCommand(void*thisptr,int edx,C_BasePlayer*localplayer, CUserCmd*cmd, IMoveHelper*movehelper)
{
	Prediction->GetMethod<RunCommandFn>(17)(thisptr,localplayer, cmd, movehelper);
}