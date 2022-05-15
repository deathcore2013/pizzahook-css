#include "weapon.h"

void C_Weapon::UpdateAccuracyPenalty()
{
	typedef void (__thiscall*UpdateAccuracyPenalty_t)(void*thisptr);

	((UpdateAccuracyPenalty_t)(FunctionOffset(Offsets->Weapon.UpdateAccuracyPenalty)))(this);
}

float C_Weapon::GetSpread()
{
	typedef float (__thiscall*GetSpread_t)(void*thisptr);

	return ((GetSpread_t)(FunctionOffset(Offsets->Weapon.Spread)))(this);
}

float C_Weapon::GetCone()
{
	typedef float (__thiscall*GetCone_t)(void*thisptr);

	return ((GetCone_t)(FunctionOffset(Offsets->Weapon.Cone)))(this);
}

int C_Weapon::GetID()
{
	typedef int (__thiscall*GetID_t)(void*thisptr);

	return ((GetID_t)(FunctionOffset(Offsets->Weapon.ID)))(this);
}

bool C_Weapon::IsFireWeapon(bool IncludeKnife)
{
	if (!this) return false;

	bool Result = true;
	int ID = GetID();

	if(!IncludeKnife)
	{
		if(    ( ID == 0 )
			|| ( ID == WEAPON_KNIFE )
			|| ( ID == WEAPON_FLASHBANG )
			|| ( ID == WEAPON_HEGRENADE )
			|| ( ID == WEAPON_SMOKEGRENADE )
			|| ( ID == WEAPON_C4 ) )
			Result = false;
	}
	else
	{
		if(    ( ID == 0 )
			|| ( ID == WEAPON_FLASHBANG )
			|| ( ID == WEAPON_HEGRENADE )
			|| ( ID == WEAPON_SMOKEGRENADE )
			|| ( ID == WEAPON_C4 ) )
			Result = false;
	}

	return Result;
}

bool C_Weapon::IsPistols()
{
	bool Result = false;
	int ID = GetID();

	if(  (ID == WEAPON_GLOCK) 
		||(ID == WEAPON_DEAGLE) 
		||(ID == WEAPON_USP) 
		||(ID == WEAPON_FIVESEVEN) 
		||(ID == WEAPON_P228) 
		||(ID == WEAPON_ELITE ) )

		Result = true;

	return Result;
}

bool C_Weapon::IsSnipers()
{
	bool Result = false;
	int ID = GetID();

	if(  (ID == WEAPON_AWP) 
		||(ID == WEAPON_SCOUT) 
		||(ID == WEAPON_SG550) 
		||(ID == WEAPON_G3SG1 ))

		Result = true;

	return Result;
}

bool C_Weapon::IsRifles()
{
	bool Result = false;
	int ID = GetID();

	if((ID == WEAPON_AK47) 
		||(ID == WEAPON_M4A1) 
		||(ID == WEAPON_GALIL) 
		||(ID == WEAPON_FAMAS) 
		||(ID == WEAPON_AUG) 
		||(ID == WEAPON_SG552) 
		||(ID == WEAPON_M249))

		Result = true;

	return Result;
}

bool C_Weapon::IsShotGuns()
{
	bool Result = false;
	int ID = GetID();

	if((ID == WEAPON_M3) 
		||(ID == WEAPON_XM1014))

		Result = true;

	return Result;
}

bool C_Weapon::IsSMGs()
{
	bool Result = false;
	int ID = GetID();

	if( (ID == WEAPON_UMP45) 
		||(ID == WEAPON_TMP) 
		||(ID == WEAPON_MAC10) 
		||(ID == WEAPON_MP5) 
		||(ID == WEAPON_P90 ) )

		Result = true;

	return Result;
}

int C_Weapon::GetClip1()
{
	return Offset(Offsets->Weapon.Clip1,int);
}

float C_Weapon::GetAccuracyPenalty()
{
	return Offset(Offsets->Weapon.AccuracyPenalty,float);
}

float C_Weapon::GetNextPrimaryAttack()
{
	return Offset(Offsets->Weapon.NextPrimaryAttack,float);
}

float C_Weapon::GetNextSecondaryAttack()
{
	return Offset(Offsets->Weapon.NextSecondaryAttack,float);
}

void C_Weapon::SetClip1(int Clip1)
{
	int *PClip1 = POffset(Offsets->Weapon.Clip1,int);

	*PClip1 = Clip1;

}

void C_Weapon::SetAccuracyPenalty(float AccuracyPenalty)
{
	float *PAccuracyPenalty = POffset(Offsets->Weapon.AccuracyPenalty,float);

	*PAccuracyPenalty = AccuracyPenalty;
}

void C_Weapon::SetNextPrimaryAttack(float NextPrimaryAttack)
{
	float *PNextPrimaryAttack = POffset(Offsets->Weapon.NextPrimaryAttack,float);

	*PNextPrimaryAttack = NextPrimaryAttack;
}

void C_Weapon::SetNextSecondaryAttack(float NextSecondaryAttack)
{
	float *PNextSecondaryAttack = POffset(Offsets->Weapon.NextSecondaryAttack,float);

	*PNextSecondaryAttack = NextSecondaryAttack;
}

void C_Weapon::SetNextBurstShootTime(int Index,float NextBurstShootTime)
{
	DWORD DOffset[2];
	DOffset[0] = Offsets->Weapon.NextBurstShootTimeGlock;
	DOffset[1] = Offsets->Weapon.NextBurstShootTimeFamas;
	float *PNextBurstShootTime = POffset(DOffset[Index],float);
	*PNextBurstShootTime = NextBurstShootTime;
}

const char *C_Weapon::GetWeaponAlias(int ID)
{
	typedef const char* (*WeaponIDToAlias_t)(int ID);
	WeaponIDToAlias_t WeaponIDToAlias = ((WeaponIDToAlias_t)Offsets->ModuleHandle.WeaponAlias);
	return WeaponIDToAlias(ID);
}

DWORD GetCSWpnData(C_Weapon *Weapon)
{
	typedef DWORD (__thiscall*GetCSWpnData_t)(void *thisptr);
	GetCSWpnData_t GetCSWpnData = ((GetCSWpnData_t)Offsets->ModuleHandle.CSWpnData);
	return GetCSWpnData(Weapon);
}

//save fps plz
void GetWeaponBulletType(C_Weapon* Weapon, int &iBulletType)
{
	switch(Weapon->GetID())
	{
	case 0:
		iBulletType = 0;
		break;
	case WEAPON_P228:
		iBulletType = BULLET_PLAYER_357SIG;
		break;
	case WEAPON_GLOCK:
		iBulletType = BULLET_PLAYER_9MM;
		break;
	case WEAPON_SCOUT:
		iBulletType = BULLET_PLAYER_762MM;
		break;
	case WEAPON_MAC10:
		iBulletType = BULLET_PLAYER_45ACP;
		break;
	case WEAPON_AUG:
		iBulletType = BULLET_PLAYER_762MM;
		break;
	case WEAPON_ELITE:
		iBulletType = BULLET_PLAYER_9MM;
		break;
	case WEAPON_FIVESEVEN:
		iBulletType = BULLET_PLAYER_57MM;
		break;
	case WEAPON_UMP45:
		iBulletType = BULLET_PLAYER_45ACP;
		break;
	case WEAPON_SG550:
		iBulletType = BULLET_PLAYER_556MM;
		break;
	case WEAPON_GALIL:
		iBulletType = BULLET_PLAYER_556MM;
		break;
	case WEAPON_FAMAS:
		iBulletType = BULLET_PLAYER_556MM;
		break;
	case WEAPON_USP:
		iBulletType = BULLET_PLAYER_45ACP;
		break;
	case WEAPON_AWP:
		iBulletType = BULLET_PLAYER_338MAG;
		break;
	case WEAPON_MP5:
		iBulletType = BULLET_PLAYER_9MM;
		break;
	case WEAPON_M249:
		iBulletType = BULLET_PLAYER_556MM;
		break;
	case WEAPON_M4A1:
		iBulletType = BULLET_PLAYER_556MM;
		break;
	case WEAPON_TMP:
		iBulletType = BULLET_PLAYER_9MM;
		break;
	case WEAPON_G3SG1:
		iBulletType = BULLET_PLAYER_762MM;
		break;
	case WEAPON_DEAGLE:
		iBulletType = BULLET_PLAYER_50AE;
		break;
	case WEAPON_SG552:
		iBulletType = BULLET_PLAYER_556MM;
		break;
	case WEAPON_AK47:
		iBulletType = BULLET_PLAYER_762MM;
		break;
	case WEAPON_P90:
		iBulletType = BULLET_PLAYER_57MM;
		break;
	default:
		iBulletType = 0;
		break;
	}
}

void GetBulletTypeParameters( int iBulletType, float &fPenetrationPower,float &flPenetrationDistance )
{
	switch(iBulletType)
	{
	case BULLET_PLAYER_50AE :
		fPenetrationPower = 30;
		flPenetrationDistance = 1000.0;
		break;

	case BULLET_PLAYER_762MM :
		fPenetrationPower = 39;
		flPenetrationDistance = 5000.0;
		break;

	case BULLET_PLAYER_556MM :
		fPenetrationPower = 35;
		flPenetrationDistance = 4000.0;
		break;
	case BULLET_PLAYER_338MAG :
		fPenetrationPower = 45;
		flPenetrationDistance = 8000.0;
		break;

	case BULLET_PLAYER_9MM :
		fPenetrationPower = 21;
		flPenetrationDistance = 800.0;
		break;

	case BULLET_PLAYER_45ACP :
		fPenetrationPower = 15;
		flPenetrationDistance = 500.0;
		break;

	case BULLET_PLAYER_357SIG :
		fPenetrationPower = 25;
		flPenetrationDistance = 800.0;
		break;

	case BULLET_PLAYER_57MM :
		fPenetrationPower = 30;
		flPenetrationDistance = 2000.0;
		break;

	default :
		fPenetrationPower = 0;
		flPenetrationDistance = 0.0;
		break;

	}
}

S_WeaponInfo C_Weapon::GetWeaponInfo()
{
	S_WeaponInfo TempWeaponInfo;
	DWORD WeaponData = GetCSWpnData(this);

	if(WeaponData)
	{
		TempWeaponInfo.Penetration = *(int*)( WeaponData + 0x884 );
		TempWeaponInfo.Damage = *(int*)( WeaponData + 0x888 );
		TempWeaponInfo.MaxRange = *(float*)( WeaponData + 0x88C );
		TempWeaponInfo.RangeModifier = *(float*)( WeaponData + 0x890 );
		TempWeaponInfo.BulletsPerShots = *(int*)( WeaponData + 0x894 );
		TempWeaponInfo.CycleTime = *(float*)( WeaponData + 0x880 );
		TempWeaponInfo.ArmorRatio = *(float*)( WeaponData + 0x70C );
	}

	GetWeaponBulletType(this,TempWeaponInfo.BulletType);
	TempWeaponInfo.ID = this->GetID();
	TempWeaponInfo.Alias = const_cast<char*>(GetWeaponAlias( TempWeaponInfo.ID ));

	int Index = OffsetIndexForSpecialWeapons();

	if (TempWeaponInfo.ID == WEAPON_M4A1)
	{
		if(SpecialMode(Index))
		{
			TempWeaponInfo.RangeModifier = .95f;
		}
	}


	if (TempWeaponInfo.ID == WEAPON_GLOCK)
	{
		if(SpecialMode(Index))
		{
			if(GetBurstShotsRemaining(Index) > 0)
			{
				TempWeaponInfo.Damage = 18;
				TempWeaponInfo.RangeModifier = .9f;
			}
		}
	}


	if (TempWeaponInfo.ID == WEAPON_USP)
	{
		if(SpecialMode(Index))
		{
			TempWeaponInfo.Damage = 30;
		}
	}

	GetBulletTypeParameters(TempWeaponInfo.BulletType, TempWeaponInfo.PenetrationPower, TempWeaponInfo.PenetrationRange);

	return TempWeaponInfo;
}

float C_Weapon::GetNextBurstShootTime(int Index)
{
	DWORD DOffset[2];
	DOffset[0] = Offsets->Weapon.NextBurstShootTimeGlock;
	DOffset[1] = Offsets->Weapon.NextBurstShootTimeFamas;

	return Offset(DOffset[Index],float);
}

bool C_Weapon::SpecialMode(int Index)
{
	DWORD DOffset[4];
	DOffset[OffsetGlock] = Offsets->Weapon.BurstModeGlock;
	DOffset[OffsetFamas] = Offsets->Weapon.BurstModeFamas;
	DOffset[OffsetM4A1] = Offsets->Weapon.SilencerM4;
	DOffset[OffsetUSP] = Offsets->Weapon.SilencerUsp;

	return Offset(DOffset[Index],bool);
}

int C_Weapon::GetBurstShotsRemaining(int Index)
{
	DWORD Offset[2];
	Offset[0] = Offsets->Weapon.iBurstShotsRemainingGlock;
	Offset[1] = Offsets->Weapon.iBurstShotsRemainingFamas;

	return Offset(Offset[Index],int);
}

int C_Weapon::OffsetIndexForSpecialWeapons()
{
	int WeaponID = GetID();

	if ( WeaponID == WEAPON_GLOCK )
	{
		return OffsetGlock;
	}
	else if ( WeaponID == WEAPON_FAMAS )
	{
		return OffsetFamas;
	}
	else if ( WeaponID == WEAPON_M4A1 )
	{
		return OffsetM4A1;
	}
	else if ( WeaponID == WEAPON_USP )
	{
		return OffsetUSP;
	}

	return -1;
}