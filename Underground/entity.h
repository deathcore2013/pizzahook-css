#ifndef C_ENTITY
#define C_ENTITY

#include "main.h"

enum E_ThinkMethods
{
	THINK_FIRE_ALL_FUNCTIONS,
	THINK_FIRE_BASE_ONLY,
	THINK_FIRE_ALL_BUT_BASE,
};

class C_Weapon;

class C_Entity
{
	public:

	CUserCmd *GetCurrentCommand();

	void SetCurrentCommand(CUserCmd*Cmd);

	BYTE GetMoveType();

	BYTE GetWaterLevel();

	void *GetRenderable();

	void *GetNetworkable();

	ClientClass *GetClientClass();

	bool IsDormant();

	int GetIndex();

	const model_t* GetModel();

	bool SetupBones(matrix3x4_t *PBoneToWorldOut, int NMaxBones, int BoneMask, float CurrentTime);

	Vector GetAbsOrigin();

	Vector GetEstimatedAbsVelocity();

	C_Weapon *GetWeapon();

	void SetPredictionRandomSeed(CUserCmd*Cmd);

	bool PhysicsRunThink(E_ThinkMethods Method);

	int GetNextThinkTick(const char *something = 0);

	void SetNextThinkTick(int NextThinkTick ,const char *something = 0);

	void ItemPreFrame();

	void ItemPostFrame();

	void PreThink(bool Server);

	void Think();

	void PostThink(bool Server);

	void SetLocalViewAngles(const QAngle &ViewAngles);

	C_Entity* GetGroundEntity();

	void SelectItem(const char* PStr, int SubType);

	void ApplyAbsVelocityImpulse(const Vector &VelocityImpulse);

	IClientVehicle* GetVehicle();

	bool UsingStandardWeaponsInVehicle();

	bool IsPlayer();

	QAngle GetPunchAngle();

	void SetPunchAngle(QAngle PunchAngle);

	bool IsAlive();

	int GetTickBase();

	void SetTickBase(int TickBase);

	int GetFlags();

	void SetFlags(int Flags);

	float GetNextAttack();

	void SetNextAttack(float NextAttack);

	bool CanNextAttack();

	int GetHitboxSet();

	void SetHitboxSet(int HitboxSet);

	QAngle GetEyeAngles();

	void SetEyeAngles(QAngle EyeAngles);

	Vector GetViewOffset();

	void SetViewOffset(Vector ViewOffset);

	int GetTeamNum();

	int GetHealth();

	int GetArmor();

	color32 GetRenderColor();

	Vector GetVelocity();

	void SetVelocity(Vector Velocity);

	Vector GetAbsVelocity();

	void SetAbsVelocity(Vector AbsVelocity);

	Vector GetBaseVelocity();

	void SetBaseVelocity(Vector BaseVelocity);

	float GetFallVelocity();

	void SetFallVelocity(float FallVelocity);

	float GetSimulationTime();

	void SetSimulationTime(float SimulationTime);

	float GetStamina();

	void SetStamina(float Stamina);

	Vector GetEyePosition();

	void PredictWeapon();

	void UpdateButtonState(int Buttons);

	void SetRenderColor(color32 RenderColor);

	void CanPrimaryAttack(bool ShouldBlockCmd,CUserCmd*Cmd,bool *Can);

	void CanSecondaryAttack(bool ShouldBlockCmd,CUserCmd*Cmd,bool *Can);

	bool CanShoot();

	void RemoveFromInterpolationList();

	void AddFromInterpolationList();

	void MoveToLastReceivedPosition(bool Force);

	unsigned short GetInterpolationList();

	void SetInterpolationList(unsigned short InterpolationList);

	void SetLocalOrigin( const Vector& Origin );

	bool HitboxToWorldTransforms(matrix3x4_t *pHitboxToWorld[MAXSTUDIOBONES]);

};


#endif