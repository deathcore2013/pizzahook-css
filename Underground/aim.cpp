#include "bot.h"

CAimbot Aimbot;

CAimbot::CAimbot()
{
	Target = 0;
	m_vecMyEyePos.Init();

	m_vecDirection.Init();

	for (int i = Corners; i >= 0; i--)
		m_vecTargetPos[i].Init();
	for (int i = Corners; i >= 0; i--)
		m_vecCorners[i].Init();

	m_angAimAngle.Init();

	m_pEntities = 0;

	m_mMatrix = new matrix3x4_t[MAXSTUDIOBONES];

	Tick = new int[65];

	for(int i = 64; i >= 0; i--)
	{
		Tick[i] = 0;
	}

}

bool IsVisible(C_BasePlayer *TargetEntity, C_BasePlayer *LocalPlayer, const Vector &TargetPosition, const Vector &EyePos)
{
	trace_t Trace;
	Ray_t Ray;

	Ray.Init(EyePos, TargetPosition);

	DWORD TraceFilter[5];
	TraceFilterSkipTwoEntities(TraceFilter, LocalPlayer, TargetEntity, 0);

	EngineTraceSDK->TraceRay(Ray, 0x46004007, (ITraceFilter*)&TraceFilter, &Trace);

	return (Trace.fraction == 1.f);
}

static void GetMaterialParameters(int iMaterial, float &flPenetrationModifier, float &flDamageModifier)
{
	switch (iMaterial)
	{
	case CHAR_TEX_METAL:
		flPenetrationModifier = 0.5;  // If we hit metal, reduce the thickness of the brush we can't penetrate
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_DIRT:
		flPenetrationModifier = 0.5;
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_CONCRETE:
		flPenetrationModifier = 0.4;
		flDamageModifier = 0.25;
		break;
	case CHAR_TEX_GRATE:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.99;
		break;
	case CHAR_TEX_VENT:
		flPenetrationModifier = 0.5;
		flDamageModifier = 0.45;
		break;
	case CHAR_TEX_TILE:
		flPenetrationModifier = 0.65;
		flDamageModifier = 0.3;
		break;
	case CHAR_TEX_COMPUTER:
		flPenetrationModifier = 0.4;
		flDamageModifier = 0.45;
		break;
	case CHAR_TEX_WOOD:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.6;
		break;
	default:
		flPenetrationModifier = 1.0;
		flDamageModifier = 0.5;
		break;
	}

}

int GetPlayerModifiedDamage(const float &constdamage, bool isHeadshot, bool isFriendly, C_Weapon*Weapon, C_BasePlayer *targetEntity)
{
	float damage = constdamage;

	if (isFriendly)
	{
		damage *= .35f;
	}

	int armor = OffsetManualthis(Offsets->Player.Armor, int, targetEntity);
	if ((armor > 0) && (!isHeadshot || (isHeadshot)))
	{
		float weaponArmorRatio = (.5f * Weapon->GetWeaponInfo().ArmorRatio);

		float newdamage = (weaponArmorRatio * damage);
		float armordamage = ((damage - newdamage) * .5f);

		if (armordamage <= (float)armor)
		{
			armordamage = armordamage;
		}
		else
		{
			newdamage = (damage + ((float)armor * -2.f));
			armordamage = (int)armor;
		}

		damage = newdamage;
	}
	else
	{
		damage = damage;
	}

	return (int)damage;
}

// scales damage
float GetHitgroupModifiedDamage(float dmg, int hitgroup)
{
	static float hitgroupModifiers[] = { 1.f, 4.f, 1.f, 1.25f, 1.f, 1.f, .75f, .75f };
	return (dmg * hitgroupModifiers[hitgroup]);
}

int CAimbot::GetTotalDamage(C_BasePlayer *LocalPlayer, C_Weapon* Weapon)
{
	trace_t traceData, wallTraceData;
	Ray_t ray;

	int currentPenetration = Weapon->GetWeaponInfo().Penetration;
	float currentPenetrationPower = Weapon->GetWeaponInfo().PenetrationPower;
	float currentDamage = (float)Weapon->GetWeaponInfo().Damage;
	float currentMaxRange = Weapon->GetWeaponInfo().MaxRange;

	Vector start = m_vecMyEyePos, wall;
	Vector end;


	float tracedDistance = 0.f;

	float multiplier = 24.f;


	C_BasePlayer *skipPlayer = 0;
	C_BasePlayer *tmp = 0;
	DWORD traceFilter[5]; // 20 or 0x14 bytes


	int totalDamage = 0;

	float wallThickness;

	bool isGrate;

	int material;

	float penetrationPowerModifier = 1.f;
	float damageModifier = .99f;

	Vector clip;
	static ConVar* mp_friendlyfire = CvarSDK->FindVar("mp_friendlyfire");
	float tmpDistance;


	while (true)
	{
		end = (start + (m_vecDirection * currentMaxRange));


		TraceFilterSkipTwoEntities(traceFilter, LocalPlayer, skipPlayer, 0);

		// initial trace
		ray.Init(start, end);
		EngineTraceSDK->TraceRay(ray, 0x4600400B, (ITraceFilter*)&traceFilter, &traceData);


		// shorten the trace
		clip = (end + (m_vecDirection * 40.0f));
		ClipTraceToPlayers(start, clip, 0x4600400B, &traceFilter, &traceData);


		tmp = (C_BasePlayer*)traceData.m_pEnt;

		// is the hit entity is a player?
		if (tmp
			&& (tmp->index > 0)
			&& (tmp->index <= 64))
		{
			// skip this player in the next trace
			skipPlayer = tmp;
		}
		else
		{
			skipPlayer = 0;
		}


		// did we hit something?
		if (traceData.fraction != 1.f)
		{
			// get the distance we've travelled so far
			tmpDistance = (tracedDistance + (currentMaxRange * traceData.fraction));

			// modify damage accordingly
			currentDamage *= pow(Weapon->GetWeaponInfo().RangeModifier, (tmpDistance * .002f));


			if (skipPlayer)
			{

				if ((mp_friendlyfire->GetBool()
					&& (skipPlayer->GetTeamNumber() == LocalPlayer->GetTeamNumber()))
					|| (skipPlayer->GetTeamNumber() != LocalPlayer->GetTeamNumber()))
				{
					totalDamage += GetPlayerModifiedDamage(GetHitgroupModifiedDamage(currentDamage, traceData.hitgroup), (traceData.hitgroup == 1), (skipPlayer->GetTeamNumber() == LocalPlayer->GetTeamNumber()), Weapon, skipPlayer);
				}
			}


			isGrate = (traceData.contents & CONTENTS_GRATE);


			material = (int)PhysicsSurfacePropsSDK->GetSurfaceData(traceData.surface.surfaceProps)->game.material;


			if (!isGrate)
			{
				GetMaterialParameters(material, penetrationPowerModifier, damageModifier);
			}


			// can we penetrate this distance?
			if (tmpDistance > Weapon->GetWeaponInfo().PenetrationRange)
			{
				currentPenetration = (currentPenetration <= 0) ? currentPenetration : 0;
			}


			// did we trace the max amount of surfaces?
			if ((currentPenetration < 0)
				|| ((currentPenetration == 0)
				&& !isGrate))
			{
				break;
			}

			// loop to the other side of the wall
			while (true)
			{
				wall = (traceData.endpos + (m_vecDirection * multiplier));

				// is this not solid?
				if (!(EngineTraceSDK->GetPointContents((traceData.endpos + (m_vecDirection * multiplier)), 0) & 0x200400B))
				{
					break;
				}


				multiplier += 24.f;

				if (multiplier > 128.f)
				{
					multiplier = -1.f; // too thick
					break;
				}
			}


			if (multiplier == -1.f)
			{
				break;
			}


			// trace back into the wall
			ray.Init(wall, traceData.endpos);
			EngineTraceSDK->TraceRay(ray, 0x4600400B, 0, &wallTraceData);


			// did we hit an entity, and it is not the same entity as we traced initially?
			if (wallTraceData.m_pEnt && (wallTraceData.m_pEnt != traceData.m_pEnt))
			{
				UTIL_TraceLine(wall, traceData.endpos, 0x4600400B, wallTraceData.m_pEnt, 0, &wallTraceData);
			}


			wallThickness = (wallTraceData.endpos - traceData.endpos).Length();


			// is it the same materials, and is it wood or metal?
			if ((material == (int)PhysicsSurfacePropsSDK->GetSurfaceData(wallTraceData.surface.surfaceProps)->game.material)
				&& ((material == 'W') || (material == 'M')))
			{
				penetrationPowerModifier += penetrationPowerModifier;
			}


			// is the wall too thick for us to penetrate?
			if (wallThickness > (currentPenetrationPower * penetrationPowerModifier))
			{
				break;
			}


			// we used some of our penetration power to penetrate the  essaye
			currentPenetrationPower -= (wallThickness / penetrationPowerModifier);


			start = wallTraceData.endpos;


			currentDamage *= damageModifier;

			if (currentDamage == 0.f)
			{
				break;
			}


			// add in the walls thickness to our traced distance
			tracedDistance = (tmpDistance + wallThickness);


			// modify the weapons currentMaxRange accordingly
			currentMaxRange = ((currentMaxRange - tracedDistance) * .5f);


			--currentPenetration;
		}
		else
		{
			break;
		}
	}


	if (totalDamage == 0)
	{
		totalDamage = -1;
	}

	return totalDamage;
}

void CAimbot::GetHitbox(int hitbox, mstudiohitboxset_t* HitboxSet, float scalecenter = 0.0f)
{
	mstudiobbox_t *Box = HitboxSet->pHitbox(hitbox);

	m_vecCorners[0] = (Box->bbmax + Box->bbmin) * 0.5f;
	m_vecCorners[1] = Box->bbmin + m_vecCorners[0] * scalecenter;

	m_vecCorners[2].x = Box->bbmin.x + m_vecCorners[0].x * scalecenter;
	m_vecCorners[2].y = Box->bbmin.y + m_vecCorners[0].y * scalecenter;
	m_vecCorners[2].z = Box->bbmax.z - m_vecCorners[0].z * scalecenter;

	m_vecCorners[3].x = Box->bbmin.x + m_vecCorners[0].x * scalecenter;
	m_vecCorners[3].y = Box->bbmax.y - m_vecCorners[0].y * scalecenter;
	m_vecCorners[3].z = Box->bbmax.z - m_vecCorners[0].z * scalecenter;

	m_vecCorners[4].x = Box->bbmax.x - m_vecCorners[0].x * scalecenter;
	m_vecCorners[4].y = Box->bbmax.y - m_vecCorners[0].y * scalecenter;
	m_vecCorners[4].z = Box->bbmin.z + m_vecCorners[0].z * scalecenter;

	m_vecCorners[5].x = Box->bbmax.x - m_vecCorners[0].x * scalecenter;
	m_vecCorners[5].y = Box->bbmin.y + m_vecCorners[0].y * scalecenter;
	m_vecCorners[5].z = Box->bbmin.z + m_vecCorners[0].z * scalecenter;

	m_vecCorners[6].x = Box->bbmax.x - m_vecCorners[0].x * scalecenter;
	m_vecCorners[6].y = Box->bbmin.y + m_vecCorners[0].y * scalecenter;
	m_vecCorners[6].z = Box->bbmax.z - m_vecCorners[0].z * scalecenter;

	m_vecCorners[7].x = Box->bbmin.x + m_vecCorners[0].x * scalecenter;
	m_vecCorners[7].y = Box->bbmax.y - m_vecCorners[0].y * scalecenter;
	m_vecCorners[7].z = Box->bbmin.z + m_vecCorners[0].z * scalecenter;

	m_vecCorners[8] = Box->bbmax - m_vecCorners[0] * scalecenter;

	VectorTransform(m_vecCorners[0], m_mMatrix[Box->bone], m_vecTargetPos[0]);
	VectorTransform(m_vecCorners[1], m_mMatrix[Box->bone], m_vecTargetPos[1]);
	VectorTransform(m_vecCorners[2], m_mMatrix[Box->bone], m_vecTargetPos[2]);
	VectorTransform(m_vecCorners[3], m_mMatrix[Box->bone], m_vecTargetPos[3]);
	VectorTransform(m_vecCorners[4], m_mMatrix[Box->bone], m_vecTargetPos[4]);
	VectorTransform(m_vecCorners[5], m_mMatrix[Box->bone], m_vecTargetPos[5]);
	VectorTransform(m_vecCorners[6], m_mMatrix[Box->bone], m_vecTargetPos[6]);
	VectorTransform(m_vecCorners[7], m_mMatrix[Box->bone], m_vecTargetPos[7]);
	VectorTransform(m_vecCorners[8], m_mMatrix[Box->bone], m_vecTargetPos[8]);
}

void CAimbot::RunBot(CUserCmd*Cmd, C_BasePlayer*LocalPlayer, C_Weapon*weapon)
{
	m_vecMyEyePos = LocalPlayer->GetAbsOrigin() + LocalPlayer->GetViewOffset();

	int Health = INT_MAX;

	int Choose[] = { 12, 11, 5, 0, 1, 9, 10, 13, 14, 16, 17, 18, 8, 7, 6, 4, 3, 2, 15 };

	if (GetKeyState(0x45))
	{
		Choose[0] = 0;
		Choose[3] = 12;
	}

	if (!weapon) return;

	if (!weapon->IsFireWeapon(false)) return;

	for (int i = 64; i >= 1; i--)
	{
		if (i == EngineSDK->GetLocalPlayer())
		{
			continue;
		}

		m_pEntities = (C_BasePlayer*)EntityListSDK->GetClientEntity(i);
		if (m_pEntities == 0)
			continue;

		player_info_t infos;

		EngineSDK->GetPlayerInfo(m_pEntities->index,&infos);
		if (!strcmp("", infos.guid))
			continue;

		if(!m_pEntities->IsAlive()

			|| m_pEntities->GetHealth() <= 0

			|| LocalPlayer->GetTeamNumber() == m_pEntities->GetTeamNumber()

			|| Health < m_pEntities->GetHealth() ) continue;

		m_pEntities->SetupBones(m_mMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, m_pEntities->m_flSimulationTime);

		const model_t* Model = m_pEntities->GetModel();

		studiohdr_t* StudioHdr = ModelInfoSDK->GetStudiomodel(Model);

		mstudiohitboxset_t* HitboxSet = StudioHdr->pHitboxSet(m_pEntities->m_nHitboxSet);


		static int wait = 0;

		if (weapon->GetID() == WEAPON_AWP)
		{
			if ((wait--) <= 0 && weapon->GetClip1() < 10)
			{
				EngineSDK->ExecuteClientCmd("say !awpdeaglehelm");
				wait = 15;
			}
		}

		if (weapon->GetID() == WEAPON_SCOUT)
		{
			if ((wait--) <= 0 && weapon->GetClip1() < 10)
			{
				EngineSDK->ExecuteClientCmd("say !scoutdeaglehelm");
				wait = 15;
			}
		}


		if (weapon->GetID() == WEAPON_AWP)
		{
			GetHitbox(10, HitboxSet, Globals->interval_per_tick);
			m_vecDirection = m_vecTargetPos[0] - m_vecMyEyePos;
			VectorNormalizeFast(m_vecDirection);

			if (IsVisible(m_pEntities,LocalPlayer,m_vecTargetPos[0],m_vecMyEyePos))
			{
				Health = m_pEntities->GetHealth();

				Target = i;

				VectorAngles(m_vecDirection, Cmd->viewangles);
				NormalizeAngles(Cmd->viewangles);

				if (weapon->GetClip1() == 0)
				{
					Cmd->buttons &= ~IN_ATTACK;
					Cmd->buttons |= IN_RELOAD;
				}
				else
				{
					Cmd->buttons &= ~IN_RELOAD;
					Cmd->buttons |= IN_ATTACK;
				}
			}
			if (UndergroundVars.silentaim)
			{

			}
			else
			{
				EngineSDK->SetViewAngles(Cmd->viewangles);
			}

		}
		else
		{
			for (int m_iHitbox = 12; m_iHitbox >= 0; m_iHitbox--)
			{
				GetHitbox(Choose[m_iHitbox], HitboxSet,Globals->interval_per_tick);

				for (int m_iCorners = Corners - 1; m_iCorners >= 1; m_iCorners--)
				{
					m_vecDirection = m_vecTargetPos[m_iCorners] - m_vecMyEyePos;
					VectorNormalizeFast(m_vecDirection);

					if (IsVisible(m_pEntities,LocalPlayer,m_vecTargetPos[m_iCorners],m_vecMyEyePos))
					{
						Health = m_pEntities->GetHealth();

						Target = i;

						VectorAngles(m_vecDirection, Cmd->viewangles);
						NormalizeAngles(Cmd->viewangles);

						if (weapon->GetClip1() == 0)
						{
							Cmd->buttons &= ~IN_ATTACK;
							Cmd->buttons |= IN_RELOAD;
						}
						else
						{
							Cmd->buttons &= ~IN_RELOAD;
							Cmd->buttons |= IN_ATTACK;
						}
					}
					if (UndergroundVars.silentaim)
					{

					}
					else
					{
						EngineSDK->SetViewAngles(Cmd->viewangles);
					}
				}
			}

			for (int m_iHitbox = 12; m_iHitbox >= 0; m_iHitbox--)
			{
				GetHitbox(Choose[m_iHitbox], HitboxSet, Globals->interval_per_tick);
				m_vecDirection = m_vecTargetPos[0] - m_vecMyEyePos;
				VectorNormalizeFast(m_vecDirection);

				if (GetTotalDamage(LocalPlayer, weapon) != -1)
				{
					Health = m_pEntities->GetHealth();

					Target = i;

					VectorAngles(m_vecDirection, Cmd->viewangles);
					NormalizeAngles(Cmd->viewangles);

					if (weapon->GetClip1() == 0)
					{
						Cmd->buttons &= ~IN_ATTACK;
						Cmd->buttons |= IN_RELOAD;
					}
					else
					{
						Cmd->buttons &= ~IN_RELOAD;
						Cmd->buttons |= IN_ATTACK;
					}
				}
				if (UndergroundVars.silentaim)
				{

				}
				else
				{
					EngineSDK->SetViewAngles(Cmd->viewangles);
				}
			}
		}
	}
}