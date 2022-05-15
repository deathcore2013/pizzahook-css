#include "bot.h"

CKnifebot Knifebot;

CKnifebot::CKnifebot()
{
	Target = 0;
	MyEyePos.Init();
}

bool BotKnife(bool bStab, CUserCmd*Cmd, C_BasePlayer*localPlayer, C_BasePlayer*targetEntity, Vector vEyePos) 
{
	float fRange = bStab ? 31 : 47; // knife range

	const Vector head_hull_mins = *(Vector*)CLIENT(0x515FCC);
	const Vector head_hull_maxs = *(Vector*)CLIENT(0x515FD8);
	const Vector mins(-16, -16, 0);
	const Vector maxs(16, 16, 45);

	Vector vForward; AngleVectors(Cmd->viewangles, &vForward);
	Vector vecEnd = vEyePos + vForward * fRange;

	trace_t tr;
	UTIL_TraceLine(vEyePos, vecEnd, MASK_SOLID, (IHandleEntity*)localPlayer, COLLISION_GROUP_NONE, &tr);

	if (tr.fraction >= 1.0)
	{
		UTIL_TraceHull(vEyePos, vecEnd, head_hull_mins, head_hull_maxs, MASK_SOLID, (IHandleEntity*)localPlayer, COLLISION_GROUP_NONE, &tr);
		if (tr.fraction < 1.0)
		{
			if (!tr.m_pEnt || IsBSPModel(tr.m_pEnt))
				FindHullIntersection(vEyePos, tr, mins, maxs, (C_BaseEntity*)localPlayer);
			vecEnd = tr.endpos;
		}
	}

	if ((tr.fraction < 1.0) && (tr.m_pEnt->index == targetEntity->index))
	{
		Knifebot.Target = targetEntity->index;

		return true;
	}
	else
		return false;
}


void CKnifebot::RunBot(CUserCmd*Cmd, C_BasePlayer*LocalPlayer, C_Weapon*weapon)
{
	if (!weapon) return;

	MyEyePos = (LocalPlayer->GetAbsOrigin() + LocalPlayer->GetViewOffset());

	if (weapon->GetID() == WEAPON_KNIFE)
	{
		for (int i = Globals->maxClients; i >= 1; --i)
		{
			C_BasePlayer *targetEntity = (C_BasePlayer*)EntityListSDK->GetClientEntity(i);

			if (targetEntity == 0 || targetEntity->IsAlive() == false || (*(int*)((DWORD)targetEntity + Offsets->Player.TeamNum) == *(int*)((DWORD)LocalPlayer + Offsets->Player.TeamNum)))
			{
				continue;
			}

			int health = *(int*)((DWORD)targetEntity + Offsets->Player.Health);
			if ((health <= 0) || (health >= 500))
			{
				continue;
			}

			bool bFirstSwing = (weapon->GetNextPrimaryAttack() + 0.4) < Globals->curtime;

			if ((health <= 20) && bFirstSwing)
			{
				if (BotKnife(false, Cmd, LocalPlayer, targetEntity, MyEyePos))
					Cmd->buttons |= IN_ATTACK;
			}
			else if ((health <= 15) && !bFirstSwing)
			{
				if (BotKnife(false, Cmd, LocalPlayer, targetEntity, MyEyePos))
					Cmd->buttons |= IN_ATTACK;
			}
			else
			{
				if (BotKnife(true, Cmd, LocalPlayer, targetEntity, MyEyePos))
					Cmd->buttons |= IN_ATTACK2;
			}
		}
	}
}