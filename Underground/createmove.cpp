#include "definitions.h"

class CTraceFilterNoPlayers2 : public CTraceFilter
{
public:

	CTraceFilterNoPlayers2(void)
	{
		m_maxClients = -1;
	}


	virtual bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask)
	{
		if (m_maxClients == -1)
		{
			m_maxClients = EngineSDK->GetMaxClients();
		}

		C_BasePlayer *Entity = (C_BasePlayer*)EntityListSDK->GetClientEntityFromHandle(pEntity->GetRefEHandle());

		return ((Entity->index < 1) || (Entity->index > m_maxClients));
	}


private:

	int m_maxClients;
};

void EdgeAntAim(CUserCmd* Cmd, C_BasePlayer* LocalPlayer, float Angle, bool CornersToo, bool ShouldSendOnCorners, bool ShouldSendNormal, bool*SendPacket)
{
	CTraceFilterNoPlayers2 filter;

	Vector localPosition = LocalPlayer->EyePosition();

	if (LocalPlayer->GetMoveType() != MOVETYPE_LADDER)
	{
		for (int y = 180; y >= 0; --y)
		{
			QAngle tmp(10.f, Cmd->viewangles.y, 360.f);
			tmp.y += y;
			NormalizeAngles(tmp);

			Vector forward;
			AngleVectors(tmp, &forward, 0, 0);

			float length = ((16 + 3) + ((16 + 3) * sin(DEG2RAD(10.f)))) + 7;
			forward *= length;

			Ray_t ray;
			trace_t traceData;
			ray.Init(localPosition, (localPosition + forward));
			EngineTraceSDK->TraceRay(ray, 0x200400B, (ITraceFilter*)&filter, &traceData);

			if (traceData.fraction != 1.f)
			{

				QAngle angles;
				VectorAngles(-traceData.plane.normal, angles);
				tmp.y = angles.y;
				NormalizeAngles(tmp);

				trace_t leftTrace, rightTrace;
				Vector left, right;
				AngleVectors(tmp + QAngle(0.f, 30.f, 0.f), &left, 0, 0);
				AngleVectors(tmp - QAngle(0.f, 30.f, 0.f), &right, 0, 0);

				left *= (length + (length * sin(DEG2RAD(30.f))));
				right *= (length + (length * sin(DEG2RAD(30.f))));

				ray.Init(localPosition, (localPosition + left));
				EngineTraceSDK->TraceRay(ray, 0x200400B, (ITraceFilter*)&filter, &leftTrace);

				ray.Init(localPosition, (localPosition + right));
				EngineTraceSDK->TraceRay(ray, 0x200400B, (ITraceFilter*)&filter, &rightTrace);

				bool Active = false;

				if ((leftTrace.fraction == 1.0f)
					&& (rightTrace.fraction != 1.0f))
				{
					Active = true;
					tmp.y -= 45.f;
				}
				else if ((leftTrace.fraction != 1.0f)
					&& (rightTrace.fraction == 1.0f))
				{
					Active = true;
					tmp.y += 45.f;
				}

				Cmd->viewangles.x = 180;

				if (!Active || CornersToo)
					Cmd->viewangles.y = tmp.y - Angle;
				else
					Cmd->viewangles.y = tmp.y;

				if (Active)
				{
					*SendPacket = ShouldSendOnCorners;
				}
				else
				{
					*SendPacket = ShouldSendNormal;
				}

			}
		}
	}
}

void ForceLowSpread(C_Weapon*weapon, CUserCmd *Cmd)
{

	int seed = 165;

	if (weapon->GetID() == WEAPON_M3)
		seed = 149;

	if (seed != -1)
	{
		for (;; ++Cmd->command_number)
		{
			Cmd->random_seed = (MD5_PseudoRandom(Cmd->command_number) & 0x7FFFFFFF);

			if ((Cmd->random_seed & 0xFF) == seed)
			{
				break;
			}
		}
	}
}

bool CanFire(C_BasePlayer*LocalPlayer, C_Weapon*weapon)
{

	bool canFire = ((Globals->curtime >= weapon->GetNextPrimaryAttack()) && (Globals->curtime >= LocalPlayer->m_flNextAttack));

	int id = weapon->GetID();
	int offsetIndex = weapon->OffsetIndexForSpecialWeapons();


	if ((id == WEAPON_GLOCK) || (id == WEAPON_FAMAS))
	{
		if (weapon->SpecialMode(weapon->OffsetIndexForSpecialWeapons()) && (weapon->GetBurstShotsRemaining(weapon->OffsetIndexForSpecialWeapons()) > 0))
		{
			canFire = (canFire || (Globals->curtime >= weapon->GetNextBurstShootTime(offsetIndex)));
		}
	}

	return canFire;

}

void NoSpread(CUserCmd*Cmd, C_BasePlayer*LocalPlayer, C_Weapon*weapon, QAngle OldAngle)
{
	if (!weapon) return;

	const float constInaccuracy = weapon->GetCone();
	const float constSpread = weapon->GetSpread();

	Vector direction, right, up;
	QAngle oldangle = OldAngle;
	AngleVectors(oldangle, &direction, &right, &up);

	RandomSeed((Cmd->random_seed & 0xFF) + 1);

	float pi_1 = RandomFloat(0.f, 6.2831855f);
	float inaccuracy = RandomFloat(0.f, constInaccuracy);
	Vector2D leftSpread((cosf(pi_1) * inaccuracy), (sinf(pi_1) * inaccuracy));

	float pi_2 = RandomFloat(0.f, 6.2831855f);
	float spread = RandomFloat(0.f, constSpread);
	Vector2D rightSpread = Vector2D((cosf(pi_2) * spread), (sinf(pi_2) * spread));
	Vector2D totalSpread = (leftSpread + rightSpread);
	totalSpread = -totalSpread;

	Vector shot = (direction + (right * totalSpread.x) + (up * totalSpread.y));
	VectorNormalizeFast(shot);

	Vector lolz[3];

	lolz[2][0] = 1.0f;
	lolz[2][1] = -totalSpread[0];
	lolz[2][2] = totalSpread[1];

	VectorNormalizeFast(lolz[2]);

	lolz[0][0] = 0.0f;
	lolz[0][1] = -totalSpread[0];
	lolz[0][2] = (1.0f / totalSpread[1]) + (1.0f / lolz[2][2]) + totalSpread[1];

	if (totalSpread[0] > 0.0f && totalSpread[1] < 0.0f)
	{
		if (lolz[0][1] < 0.0f)
			lolz[0][1] = -lolz[0][1];
	}
	else if (totalSpread[0] < 0.0f && totalSpread[1] < 0.0f)
	{
		if (lolz[0][1] > 0.0f)
			lolz[0][1] = -lolz[0][1];
	}

	if (lolz[0][2] < 0.0f)
		lolz[0][2] = -lolz[0][2];

	VectorNormalizeFast(lolz[0]);

	QAngle test;
	VectorAngles(lolz[2], lolz[0], test);

	QAngle angles;
	VectorAngles(shot, up, angles);
	angles.z += test.z;

	angles -= oldangle;
	NormalizeAngles(angles);

	Cmd->viewangles.x = (180.0f - oldangle.x) - LocalPlayer->GetPunchAngle().x * 2.0f + angles.x;
	Cmd->viewangles.y = (180.0f + oldangle.y) - LocalPlayer->GetPunchAngle().y * 2.0f - angles.y;
	Cmd->viewangles.z = oldangle.z - LocalPlayer->GetPunchAngle().z * 2.0f + angles.z;

	NormalizeAngles(Cmd->viewangles);

}
QAngle DirectionAngle;
static void RunAimPlayerBehindWall(CUserCmd* Cmd, C_BasePlayer* LocalPlayer)
{
	Vector EyePos = LocalPlayer->GetAbsOrigin() + LocalPlayer->GetViewOffset();

	int tmpHealth = INT_MAX;

	bool TmpVisible = false;

	for (int i = 64; i >= 1; --i)
	{
		if (i == EngineSDK->GetLocalPlayer())
		{
			continue;
		}

		C_BasePlayer *TargetPlayer = (C_BasePlayer*)EntityListSDK->GetClientEntity(i);

		if (TargetPlayer == 0)
			continue;

		if ((TargetPlayer->IsAlive() == false)
			|| (TargetPlayer->GetTeamNumber() == LocalPlayer->GetTeamNumber())
			|| (TargetPlayer->IsDormant())
			|| (TargetPlayer->GetRenderColor().a < 255.f))
		{
			continue;
		}

		int Health = TargetPlayer->GetHealth();

		if ((Health <= 0) || (Health >= 500))
		{
			continue;
		}

		if (tmpHealth < Health) continue;

		Vector FinalTargetPosition = TargetPlayer->GetAbsOrigin() + TargetPlayer->GetViewOffset();

		tmpHealth = Health;

		Vector FinalTargetDirection = FinalTargetPosition - EyePos;

		VectorNormalizeFast(FinalTargetDirection);

		VectorAngles(FinalTargetDirection, DirectionAngle);

		NormalizeAngles(DirectionAngle);

		Cmd->viewangles = DirectionAngle;

	}
}
//Anti-Aim
void AntiAim(CUserCmd*Cmd, C_BasePlayer*LocalPlayer, bool**SendPacket, int Power = 15)
{
	RunAimPlayerBehindWall(Cmd, LocalPlayer);

	Vector AbsVelocity = LocalPlayer->m_vecAbsVelocity;
	bool Check;
	if (LocalPlayer->GetTeamNumber() == 3)
	{
		Check = !((DirectionAngle.y >= 0.0f) && (DirectionAngle.y <= 180.0f));
	}
	else
	{
		Check = (DirectionAngle.y > 0.0f) && (DirectionAngle.y < 180.0f);
	}

	if (Check)
	{
		AbsVelocity.x = -AbsVelocity.x;
	}

	if ((Cmd->buttons & IN_DUCK) && (LocalPlayer->m_vecAbsVelocity.Length() < 400.f))
	{
		Cmd->viewangles.x = 180;

		static int Ticks = 2;
		if (Ticks <= 0)
		{
			Cmd->viewangles.y = 180;
			Cmd->buttons &= ~IN_DUCK;
			**SendPacket = false;
			Ticks = 2;
		}
		else
		{
			**SendPacket = true;
			Cmd->viewangles.y -= 90;
			EdgeAntAim(Cmd, LocalPlayer, 0.0f, false, true, true, *SendPacket);
			Ticks--;
		}
	}
	else
	{
		Cmd->viewangles.x = 180.087936f;
		static int Ticks = Power;

		if (Ticks <= 0)
		{
			Cmd->viewangles.y -= 90;
			**SendPacket = true;
			Ticks = Power;
		}
		else if ((Ticks >= 1) && (Ticks <= 4))
		{
			Cmd->viewangles.y -= 90;
			**SendPacket = true;
			Ticks--;
		}
		else
		{
			Cmd->viewangles.y -= 90;
			**SendPacket = false;
			Ticks--;
		}

		if (LocalPlayer->m_vecAbsVelocity.Length() < 300.f)
			EdgeAntAim(Cmd, LocalPlayer, 0.0f, true, true, true, *SendPacket);

		if (LocalPlayer->m_vecAbsVelocity.Length() < 400.f)
			EdgeAntAim(Cmd, LocalPlayer, 0.0f, true, true, true, *SendPacket);
		static bool crash4 = true;
		static bool crash5 = false;
		static bool crash3 = (RandomInt(0, 3)) != (1 || 3);

		if (crash4)

			Cmd->viewangles.x = (crash3) ? 180.087936f : 180.087936f;

		if (crash5)
		{
			Cmd->viewangles.y = (crash3) ? (90) : (90);
			crash5 = false;
		}
		else
		{
			Cmd->viewangles.y = 90;
			crash5 = true;
		}

		crash4 = false;
		{
			Cmd->viewangles.x = 180.087936f;
			Cmd->viewangles.y = 90 - AbsVelocity.Length() < 300;
			crash4 = true;
		}

	}
}
void AntiAim2(CUserCmd*Cmd, C_BasePlayer*LocalPlayer, bool**SendPacket, int Power = 15)
{
	RunAimPlayerBehindWall(Cmd, LocalPlayer);

	Vector AbsVelocity = LocalPlayer->m_vecAbsVelocity;
	bool Check;
	if (LocalPlayer->GetTeamNumber() == 3)
	{
		Check = !((DirectionAngle.y >= 0.0f) && (DirectionAngle.y <= 180.0f));
	}
	else
	{
		Check = (DirectionAngle.y > 0.0f) && (DirectionAngle.y < 180.0f);
	}

	if (Check)
	{
		AbsVelocity.x = -AbsVelocity.x;
	}

	if ((Cmd->buttons & IN_DUCK) && (LocalPlayer->m_vecAbsVelocity.Length() < 300.f))
	{
		Cmd->viewangles.x = 180.0172653f;

		static int Ticks = 2;
		if (Ticks <= 0)
		{

			Cmd->viewangles.y -= 180;
			Cmd->buttons &= ~IN_DUCK;
			**SendPacket = false;
			Ticks = 2;
			//Cmd->viewangles.x = 180.00f;

			//Cmd->viewangles.x = 180.087936f;

			Cmd->viewangles.x = 180.071235f;
		}
		else
		{
			**SendPacket = true;
			Cmd->viewangles.y -= 90;
			EdgeAntAim(Cmd, LocalPlayer, 0.0f, false, true, true, *SendPacket);
			Ticks--;
		}
	}
	else
	{
		//Cmd->viewangles.x = 180.00f; //normal down

		//Cmd->viewangles.x = 180.087936f; //fakedown

		Cmd->viewangles.x = 180.071234f;//it was in the code rifk, nah I never used that pitc then look here better?



		//from there 
		EdgeAntAim(Cmd, LocalPlayer, 180.0f, false, true, false, *SendPacket);

		Cmd->viewangles.y -= 180.0f;

		static int Ticks = Power;

		if (Ticks <= 0)
		{
			Cmd->viewangles.y -= 180.0f;
			**SendPacket = true;
			Ticks = Power;
			EdgeAntAim(Cmd, LocalPlayer, 180.0f, false, true, false, *SendPacket);

		}
		else if ((Ticks >= 1) && (Ticks <= 4))
		{
			static bool switche = true;

			if (switche)
			{
				Cmd->viewangles.y -= 180.0f;
				switche = false;
				EdgeAntAim(Cmd, LocalPlayer, 180.0f, false, true, false, *SendPacket);

			}
			else
			{
				Cmd->viewangles.y -= 360.0f;
				switche = false;
				EdgeAntAim(Cmd, LocalPlayer, 180.0f, false, true, false, *SendPacket);

			}
			**SendPacket = true;
			Ticks--;
			EdgeAntAim(Cmd, LocalPlayer, 180.0f, false, true, false, *SendPacket);
		}
		else
		{
			Cmd->viewangles.y -= 180.0f;
			**SendPacket = false;
			Ticks--;
			EdgeAntAim(Cmd, LocalPlayer, 180.0f, false, true, false, *SendPacket);
		}
		//to there this is pure non sense rofl it works fine lookwt

	}
}
void AntiAim3(CUserCmd*Cmd, C_BasePlayer*LocalPlayer, bool**SendPacket, int Power = 12)
{
	RunAimPlayerBehindWall(Cmd, LocalPlayer);

	Vector AbsVelocity = LocalPlayer->m_vecAbsVelocity;
	bool Check;
	if (LocalPlayer->GetTeamNumber() == 3)
	{
		Check = !((DirectionAngle.y >= 0.0f) && (DirectionAngle.y <= 90.0f));
	}
	else
	{
		Check = (DirectionAngle.y > 0.0f) && (DirectionAngle.y < 70.0f);
	}

	if (Check)
	{
		AbsVelocity.x = -AbsVelocity.x;
	}

	if ((Cmd->buttons & IN_DUCK) && (LocalPlayer->m_vecAbsVelocity.Length() < 600.f))
	{
		static bool crash1 = true;
		static bool crash2 = false;
		static bool crash3 = (RandomInt(0, 10)) != (1 || 3);

		if (crash1)
		{
			Cmd->viewangles.x = (crash3) ? 180.087936f : 270.0f;

			if (crash2)
			{
				Cmd->viewangles.y = (crash3) ? (180.087936) : (270.0);
				crash2 = false;
			}
			else
			{
				Cmd->viewangles.y = 270.0;
				crash2 = true;
			}

			crash1 = false;
		}
		else
		{
			Cmd->viewangles.x = 180.087936f;
			Cmd->viewangles.y = 270;
			crash1 = true;
		}

		static int Ticks = 2;
		if (Ticks <= 0)
		{
			static bool crash1 = true;
			static bool crash2 = false;
			static bool crash3 = (RandomInt(0, 10)) != (1 || 3);

			if (crash1)
			{
				Cmd->viewangles.x = (crash3) ? 180.087936f : 270.0;
				{
					Cmd->viewangles.x = 180.087936f;
					Cmd->viewangles.y = 270;
					crash1 = true;
				}
				if (crash2)
				{
					Cmd->viewangles.y = (crash3) ? (180.087936) : (270);
					crash2 = false;
					{
						Cmd->viewangles.x = 180.087936f;
						Cmd->viewangles.y = 270;
						crash1 = false;
					}
				}
				else
				{
					Cmd->viewangles.y = 180;
					crash2 = true;
				}

				crash1 = false;
			}
			else
			{
				Cmd->viewangles.x = 180.087936f;
				Cmd->viewangles.y = 270;
				crash1 = true;
			}
			Cmd->buttons &= ~IN_DUCK;
			**SendPacket = false;
			Ticks = 2;
		}
		else
		{
			**SendPacket = true;
			Cmd->viewangles.y -= 155;
			//EdgeAntAim(Cmd, LocalPlayer, 0.0f, false, true, true, *SendPacket);
			Ticks--;
		}
	}
	else
	{
		//Cmd->viewangles.x = 180.00f; //normal down

		Cmd->viewangles.x = 180.087936f; //fakedown

		//Cmd->viewangles.x = 80.0f; //test
		static int Ticks = Power;

		if (Ticks <= 0)
		{
			Cmd->viewangles.y -= 155.0f;
			**SendPacket = true;
			Ticks = Power;
		}
		else if ((Ticks >= 1) && (Ticks <= 4))
		{
			Cmd->viewangles.y -= 155.0f;
			**SendPacket = true;
			Ticks--;
		}
		else
		{
			Cmd->viewangles.y -= 360.0f;
			**SendPacket = false;
			Ticks--;
		}

		if (LocalPlayer->m_vecAbsVelocity.Length() < 300.f)
			EdgeAntAim(Cmd, LocalPlayer, 0.0f, true, true, true, *SendPacket);
	}
}


inline IHandleEntity* CBaseHandle::Get() const
{
	return	(*(CBaseEntityList*)CLIENT(0x5495A0)).LookupEntity(*this);
}

void TriggerBot(C_BasePlayer *LocalPlayer, CUserCmd *Cmd)
{
	C_Weapon*weapon = (C_Weapon*)(EntityListSDK->GetClientEntityFromHandle(OffsetManualthis(Offsets->Player.ActiveWeapon, CBaseHandle, LocalPlayer)));

	if (weapon)
	{
		Ray_t Ray;
		Vector Forward, Right, Up;
		QAngle View;

		EngineSDK->GetViewAngles(View);

		AngleVectors(View, &Forward, &Right, &Up);

		Vector Start = (LocalPlayer->GetAbsOrigin() + LocalPlayer->GetViewOffset());
		Vector End = Start + Forward * 8192.f;

		Ray.Init(Start, End);

		if (weapon->IsFireWeapon(false))
		{
			for (int i = Globals->maxClients; i >= 1; --i)
			{
				if (i == EngineSDK->GetLocalPlayer())
				{
					continue;
				}

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

				matrix3x4_t Matrix[MAXSTUDIOBONES];
				if (!targetEntity->SetupBones(Matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
					continue;

				const model_t* Model = targetEntity->GetModel();

				if (Model)
				{
					studiohdr_t* StudioHdr = ModelInfoSDK->GetStudiomodel(Model);
					if (StudioHdr)
					{
						mstudiohitboxset_t* HitboxSet = StudioHdr->pHitboxSet(targetEntity->m_nHitboxSet);
						if (HitboxSet)
						{
							for (int hitscans = HitboxSet->numhitboxes - 1; hitscans >= 0; hitscans--)
							{
								mstudiobbox_t* Box = HitboxSet->pHitbox(hitscans);
								if (Box)
								{
									CBaseTrace BoxTrace;
									if (IntersectRayWithOBB(Ray, Matrix[Box->bone], Box->bbmin * .985f, Box->bbmax * .985f, 0, &BoxTrace))
									{
										Cmd->buttons |= IN_ATTACK;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

typedef void(__thiscall*CreateMoveFN2)(void*thisptr, int tick, float seq, bool active);
void __fastcall new_CreateMoveClient(void*thisptr, int edx, int seq, float tick, bool active)
{
	Client->GetMethod<CreateMoveFN2>(21)(thisptr, seq, tick, active);

	CUserCmd* Cmd = &InputSDK->m_pCommands[seq % 90];
	C_BasePlayer* LocalPlayer = (C_BasePlayer*)EntityListSDK->GetClientEntity(EngineSDK->GetLocalPlayer());
	C_Weapon*weapon = (C_Weapon*)(EntityListSDK->GetClientEntityFromHandle(OffsetManualthis(Offsets->Player.ActiveWeapon, CBaseHandle, LocalPlayer)));

	bool OnGround = LocalPlayer->m_hGroundEntity != 0;

	bool MustActiveBhop = false;
	if ((LocalPlayer->GetWaterLevel() >= 2) || (LocalPlayer->GetMoveType() == MOVETYPE_LADDER) || OnGround)
	{
		MustActiveBhop = true;
	}

	if (Cmd->buttons & IN_JUMP)
	{
		if (MustActiveBhop)
		{
			Cmd->buttons |= IN_JUMP;
		}
		else
		{
			Cmd->buttons &= ~IN_JUMP;
		}
	}

	MyPrediction.RunLocalPrediction(Cmd, LocalPlayer);
	QAngle OldAngle = Cmd->viewangles, OldAngle2 = Cmd->viewangles;

	bool *SendPacket = (bool*)(*(DWORD*)GetEBP() - 0x1);
	*SendPacket = true;

	if (UndergroundVars.enableaim)
	{
		Aimbot.RunBot(Cmd, LocalPlayer, weapon);
	}

	QAngle OldAngleA = Cmd->viewangles;

	if (weapon->IsFireWeapon(false))
	{
		if (Cmd->buttons & IN_ATTACK)
		{
			if (CanFire(LocalPlayer, weapon))
			{
				if (UndergroundVars.forcelowspread)
				{
					ForceLowSpread(weapon, Cmd);
				}
				if (UndergroundVars.perfectaccuracy)
				{
					NoSpread(Cmd, LocalPlayer, weapon, OldAngleA);
				}
						if (weapon->GetID() != WEAPON_AWP)
							*SendPacket = false;
					
			}
			else
			{
				if (LocalPlayer->GetMoveType() != MOVETYPE_LADDER && UndergroundVars.antiaim)
					AntiAim(Cmd, LocalPlayer, &SendPacket, UndergroundVars.fakelaginattack);
				Cmd->buttons &= ~IN_ATTACK;
				if (LocalPlayer->GetMoveType() != MOVETYPE_LADDER && UndergroundVars.antiaim2)
					AntiAim2(Cmd, LocalPlayer, &SendPacket, UndergroundVars.fakelaginattack);
				Cmd->buttons &= ~IN_ATTACK;
				if (LocalPlayer->GetMoveType() != MOVETYPE_LADDER && UndergroundVars.antiaim3)
					AntiAim3(Cmd, LocalPlayer, &SendPacket, UndergroundVars.fakelaginattack);
				Cmd->buttons &= ~IN_ATTACK;
			}
		}
		else
		{
			if (LocalPlayer->GetMoveType() != MOVETYPE_LADDER && UndergroundVars.antiaim)
				AntiAim(Cmd, LocalPlayer, &SendPacket, UndergroundVars.fakelag);
			if (LocalPlayer->GetMoveType() != MOVETYPE_LADDER && UndergroundVars.antiaim2)
				AntiAim2(Cmd, LocalPlayer, &SendPacket, UndergroundVars.fakelag);
			if (LocalPlayer->GetMoveType() != MOVETYPE_LADDER && UndergroundVars.antiaim3)
				AntiAim3(Cmd, LocalPlayer, &SendPacket, UndergroundVars.fakelag);
		}
	}

	Knifebot.RunBot(Cmd, LocalPlayer, weapon);

	float Speed = sqrt((LocalPlayer->m_vecVelocity[0] * LocalPlayer->m_vecVelocity[0]) + (LocalPlayer->m_vecVelocity[1] * LocalPlayer->m_vecVelocity[1]));

	float Tickrate;

	if (Globals->interval_per_tick * 100 > 1) Tickrate = 1.1;
	else Tickrate = 1;

	float Value = (8.15 - Tickrate) - (Speed / 340);

	if (Speed > 160 && Speed < 750)
	{
		Value = (4.6 - Tickrate) - (Speed / 340);
	}
	if (Speed > 750)
	{
		Value = (3 - Tickrate) - (Speed / 1000);
	}
	if (Value <= 0.275)
	{
		Value = 0.275;
	}


	if (!OnGround)
	{
		static float OldY;
		float Difference = AngleNormalize(OldAngle2.y - OldY);

		if (abs(Difference) < Value)
		{
			static bool Flip = false;
			if (Flip)
			{
				OldAngle.y -= Value;
				Cmd->sidemove = -400;
				Flip = false;
			}
			else
			{
				OldAngle.y += Value;
				Cmd->sidemove = 400;
				Flip = true;
			}
		}
		else
		{
			if (Difference > 0.0f)
			{
				Cmd->sidemove = -400;
			}
			if (Difference < 0.0f)
			{
				Cmd->sidemove = 400;
			}
		}

		OldY = OldAngle2.y;
	}

	static bool _FastRun;
	if (OnGround)
	{
		if ((Cmd->buttons&IN_FORWARD && Cmd->buttons&IN_MOVELEFT) || (Cmd->buttons&IN_BACK && Cmd->buttons&IN_MOVERIGHT))
		{
			if (_FastRun)
			{
				_FastRun = false;
				Cmd->sidemove -= 204;
				Cmd->forwardmove -= 204;
			}
			else
			{
				_FastRun = true;
				Cmd->sidemove += 204;
				Cmd->forwardmove += 204;
			}
		}
		else if ((Cmd->buttons&IN_FORWARD && Cmd->buttons&IN_MOVERIGHT) || (Cmd->buttons&IN_BACK && Cmd->buttons&IN_MOVELEFT))
		{
			if (_FastRun)
			{
				_FastRun = false;
				Cmd->sidemove -= 204;
				Cmd->forwardmove += 204;
			}
			else
			{
				_FastRun = true;
				Cmd->sidemove += 204;
				Cmd->forwardmove -= 204;
			}
		}
		else if (Cmd->buttons&IN_FORWARD || Cmd->buttons&IN_BACK)
		{
			if (_FastRun)
			{
				_FastRun = false;
				Cmd->sidemove -= 204;
			}
			else
			{
				_FastRun = true;
				Cmd->sidemove += 204;
			}
		}
		else if (Cmd->buttons&IN_MOVELEFT || Cmd->buttons&IN_MOVERIGHT)
		{
			if (_FastRun)
			{
				_FastRun = false;
				Cmd->forwardmove -= 204;
			}
			else
			{
				_FastRun = true;
				Cmd->forwardmove += 204;
			}
		}
	}
	Vector Direction(Cmd->forwardmove, Cmd->sidemove, 0.0f);
	float Velocity = Direction.Length();
	VectorNormalize(Direction);
	QAngle Angles;
	VectorAngles2(Direction, Angles);
	float delta = (Cmd->viewangles.y - OldAngle.y);
	float yaw = DEG2RAD(delta + Angles.y);
	if (LocalPlayer->GetMoveType() != MOVETYPE_LADDER)
	{
		if ((Cmd->viewangles.x >= 90.0f) || (Cmd->viewangles.x <= -90.0f))
			Cmd->forwardmove = (-cos(yaw) * Velocity);
		else
			Cmd->forwardmove = (cos(yaw) * Velocity);

		Cmd->sidemove = (sin(yaw) * Velocity);
	}
	
	NormalizeAngles(Cmd->viewangles);

	MyPrediction.FinishLocalPrediction(Cmd, LocalPlayer);
}

bool __fastcall new_CreateMoveClientMode(void*thisptr, int edx, float tick, CUserCmd*Cmd)
{

	CvarSDK->FindVar("cl_updaterate")->SetValue(66 / Globals->interval_per_tick);
	CvarSDK->FindVar("cl_interp")->SetValue(0.937f);
	CvarSDK->FindVar("cl_interp_ratio")->SetValue(1.0f);
	CvarSDK->FindVar("cl_interpolate")->SetValue("0.937");
	CvarSDK->FindVar("sv_cheats")->SetValue(1.0f);
	CvarSDK->FindVar("cl_cmdrate")->SetValue(66.0f);


	for (int i = 64; i >= 1; i--)
	{
		if (i == EngineSDK->GetLocalPlayer())
		{
			continue;
		}

		C_BasePlayer *m_pEntities = (C_BasePlayer*)EntityListSDK->GetClientEntity(i);
		if (m_pEntities == 0)
			continue;

		m_pEntities->m_iv_vecOrigin.SetMaxCount(1);
		m_pEntities->m_iv_vecOrigin.SetInterpolationAmount(Globals->interval_per_tick);
		for (int i = 0; i <= 1;i++)
		m_pEntities->m_iv_vecOrigin.SetLooping(true, i);
		if(m_pEntities->m_flSimulationTime > m_pEntities->m_flOldSimulationTime) continue;

		typedef void(__thiscall* RemoveFromInterpolationList_t)(void*thisptr);
		((RemoveFromInterpolationList_t)((DWORD)GetModuleHandleA("client")+0x973A0))(m_pEntities);
	}

	return true;
}