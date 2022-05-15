#ifndef C_WEAPON
#define C_WEAPON

#include "main.h"


struct S_WeaponInfo
{
	int	ID,
		Penetration,
		Damage,
		BulletsPerShots,
		BulletType;

	float MaxRange,
		RangeModifier,
		PenetrationRange,
		PenetrationPower,
		ArmorRatio,
		CycleTime;
	
	char *Alias;
};

enum 
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM, 
	BULLET_PLAYER_45ACP,
	BULLET_PLAYER_338MAG,
	BULLET_PLAYER_762MM,
	BULLET_PLAYER_556MM,
	BULLET_PLAYER_50AE,
	BULLET_PLAYER_57MM,
	BULLET_PLAYER_357SIG,
};

enum E_OffsetFG
{
	OffsetGlock = 0,
	OffsetFamas = 1,
	OffsetM4A1 = 2,
	OffsetUSP = 3
};

class C_Weapon
{
	public:

	void UpdateAccuracyPenalty();

	float GetSpread();

	float GetCone();

	float GetNextPrimaryAttack();

	float GetNextSecondaryAttack();

	float GetAccuracyPenalty();

	int GetID();

	int GetClip1();

	bool IsFireWeapon(bool IncludeKnife);

	void SetAccuracyPenalty(float AccuracyPenalty);

	void SetNextPrimaryAttack(float NextPrimaryAttack);

	void SetNextSecondaryAttack(float NextSecondaryAttack);
	
	void SetNextBurstShootTime(int Index,float NextBurstShootTime);

	void SetClip1(int Clip1);

	int OffsetIndexForSpecialWeapons();

	float GetNextBurstShootTime(int Index);

	bool SpecialMode(int Index);

	int GetBurstShotsRemaining(int Index);

	const char *GetWeaponAlias(int ID);

	S_WeaponInfo GetWeaponInfo();

	bool IsPistols();

	bool IsSnipers();

	bool IsRifles();

	bool IsShotGuns();

	bool IsSMGs();
};


#endif