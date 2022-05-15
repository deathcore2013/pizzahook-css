#include "entity.h"

void C_Entity::ApplyAbsVelocityImpulse(const Vector &VelocityImpulse)
{
	typedef void (__thiscall*ApplyAbsVelocityImpulse_t)(void*thisptr,const Vector &VelocityImpulse);

	((ApplyAbsVelocityImpulse_t)(Offsets.ModuleHandle.ApplyAbsVelocityImpulse))(this,VelocityImpulse);
}

bool C_Entity::CanNextAttack()
{
	bool Result = false;

	if(GetNextAttack() <= Instances::MethodSDK.Globals->curtime)
		Result = true;

	return Result;
}

int Tick = 1;
void C_Entity::CanPrimaryAttack(bool ShouldBlockCmd,CUserCmd*Cmd,bool *Can)
{
	C_Weapon* Weapon = GetWeapon();

	if(!Weapon) return;

	if(!Weapon->IsFireWeapon(false)) return;

	//let it pass only for 1tick;
	if(CanNextAttack() && (Weapon->GetNextPrimaryAttack() <= Instances::MethodSDK.Globals->curtime))
	{
		if(Tick == 1)
		{
			*Can = true;
		}
		else
		{
			*Can = false;
		}
		Tick--;
	}
	else
	{
		Tick = 1;
		if(ShouldBlockCmd)
			Cmd->buttons &=~ IN_ATTACK;
		*Can = false;
	}
}

void C_Entity::CanSecondaryAttack(bool ShouldBlockCmd,CUserCmd*Cmd,bool *Can)
{
	C_Weapon* Weapon = GetWeapon();

	if(!Weapon) return;

	if(CanNextAttack() && (Weapon->GetNextSecondaryAttack() <= Instances::MethodSDK.Globals->curtime))
	{
		*Can = false;
	}
	else
	{
		if(ShouldBlockCmd)
			Cmd->buttons &=~ IN_ATTACK2;

		*Can = true;
	}
}

Vector C_Entity::GetAbsOrigin()
{
	typedef Vector &(__thiscall*GetAbsOrigin_t)(void*thisptr);

	return ((GetAbsOrigin_t)(FunctionOffset(Offsets->Player.GetAbsOrigin)))(this);
}

Vector C_Entity::GetAbsVelocity()
{
	return Offset(Offsets->Player.AbsVelocity,Vector);
}

int C_Entity::GetArmor()
{
	return Offset(Offsets->Player.Armor,int);
}

Vector C_Entity::GetBaseVelocity()
{
	return Offset(Offsets->Player.BaseVelocity,Vector);
}

ClientClass *C_Entity::GetClientClass()
{
	typedef ClientClass* (__thiscall*GetClientClass_t)(void*thisptr);

	return ((GetClientClass_t)(FunctionManualthisOffset(Offsets->Player.ClientClass,GetNetworkable())))(GetNetworkable());
}

CUserCmd *C_Entity::GetCurrentCommand()
{
	return Offset(Offsets->Player.CurrentCommand,CUserCmd*);
}

Vector C_Entity::GetEstimatedAbsVelocity()
{
	typedef void ( __thiscall *EstimateAbsVelocity_t )( void *thisptr, Vector &AbsVelocity );

	Vector AbsVelocity;

	((EstimateAbsVelocity_t)Offsets.ModuleHandle.EstimateAbsVelocity)( this, AbsVelocity );

	return AbsVelocity;	
}

QAngle C_Entity::GetEyeAngles()
{
	return Offset(Offsets->Player.EyeAngles,QAngle);
}

Vector C_Entity::GetEyePosition()
{
	return (GetViewOffset() + GetAbsOrigin());
}

float C_Entity::GetFallVelocity()
{
	return Offset(Offsets->Player.FallVelocity,float);
}

int C_Entity::GetFlags()
{
	return Offset(Offsets->Player.Flags,int);
}

C_Entity *C_Entity::GetGroundEntity()
{
	typedef C_Entity* (__thiscall*GetGroundEntity_t)(void*thisptr);

	return ((GetGroundEntity_t)Offsets.ModuleHandle.GetGroundEntity)(this);
}

int C_Entity::GetHealth()
{
	return Offset(Offsets->Player.Health,int);
}

int C_Entity::GetHitboxSet()
{
	return Offset(Offsets->Player.HitboxSet,int);
}

int C_Entity::GetIndex()
{
	typedef int (__thiscall*GetIndex_t)(void*thisptr);

	return ((GetIndex_t)(FunctionManualthisOffset(Offsets->Player.GetIndex,GetNetworkable())))(GetNetworkable());
}

const model_t *C_Entity::GetModel()
{
	typedef const model_t *(__thiscall*GetModel_t)(void*thisptr);

	return ((GetModel_t)FunctionManualthisOffset(Offsets->Player.GetModel,GetRenderable()))(GetRenderable());
}

BYTE C_Entity::GetMoveType()
{
	return Offset(Offsets->Player.MoveType,BYTE);
}

void *C_Entity::GetNetworkable()
{
	return POffset(Offsets->Player.Networkable,void*);
}

float C_Entity::GetNextAttack()
{
	return Offset(Offsets->Player.NextAttack,float);
}

int C_Entity::GetNextThinkTick(const char* something)
{
	typedef int (__thiscall*GetNextThinkTick_t)(void*thisptr,const char*something);

	return ((GetNextThinkTick_t)Offsets.ModuleHandle.GetNextThinkTick)(this,something);
}

QAngle C_Entity::GetPunchAngle()
{
	return Offset(Offsets->Player.PunchAngle,QAngle);
}

void *C_Entity::GetRenderable()
{
	return POffset(Offsets->Player.Renderable,void*);
}

color32 C_Entity::GetRenderColor()
{
	return Offset(Offsets->Player.RenderColor,color32);
}

float C_Entity::GetSimulationTime()
{
	return Offset(Offsets->Player.SimulationTime,float);
}

float C_Entity::GetStamina()
{
	return Offset(Offsets->Player.Stamina,float);
}

int C_Entity::GetTeamNum()
{
	return Offset(Offsets->Player.TeamNum,int);
}

int C_Entity::GetTickBase()
{
	return Offset(Offsets->Player.TickBase,int);
}

IClientVehicle *C_Entity::GetVehicle()
{
	typedef IClientVehicle* (__thiscall*GetVehicle_t)(void*thisptr);

	return ((GetVehicle_t)(FunctionOffset(Offsets->Player.GetVehicle)))(this);
}

Vector C_Entity::GetVelocity()
{
	return Offset(Offsets->Player.Velocity,Vector);
}

Vector C_Entity::GetViewOffset()
{
	return Offset(Offsets->Player.ViewOffset,Vector);
}

BYTE C_Entity::GetWaterLevel()
{
	return Offset(Offsets->Player.WaterLevel,BYTE);
}

C_Weapon *C_Entity::GetWeapon()
{
	return (C_Weapon*)(Instances::MethodSDK.EntityList->GetClientEntityFromHandle(Offset(Offsets->Player.ActiveWeapon,CBaseHandle)));
}

bool C_Entity::IsAlive()
{
	return (Offset(Offsets->Player.LifeState,BYTE) == LIFE_ALIVE);
}

bool C_Entity::IsDormant()
{
	typedef bool (__thiscall*IsDormant_t)(void*thisptr);

	return ((IsDormant_t)(FunctionManualthisOffset(Offsets->Player.IsDormant,GetNetworkable())))(GetNetworkable());
}

bool C_Entity::IsPlayer()
{
	typedef bool (__thiscall*IsPlayer_t)(void*thisptr);

	return ((IsPlayer_t)(FunctionOffset(Offsets->Player.IsPlayer)))(this);
}

void C_Entity::ItemPostFrame()
{
	typedef void (__thiscall*ItemPostFrame_t)(void*thisptr);

	((ItemPostFrame_t)(FunctionOffset(Offsets->Player.ItemPostFrame)))(this);
}

void C_Entity::ItemPreFrame()
{
	typedef void (__thiscall*ItemPreFrame_t)(void*thisptr);

	((ItemPreFrame_t)(FunctionOffset(Offsets->Player.ItemPreFrame)))(this);
}

bool C_Entity::PhysicsRunThink(E_ThinkMethods Method)
{
	typedef bool (__thiscall*PhysicsRunThink_t)(void*thisptr,E_ThinkMethods Method);

	return ((PhysicsRunThink_t)Offsets.ModuleHandle.PhysicsRunThink)(this,Method);
}

void C_Entity::PostThink(bool Server)
{
	typedef void (__thiscall*PostThink_t)(void*thisptr);

	if(Server)
	{
		PostThink_t PostThink = ((PostThink_t)Offsets.ModuleHandle.PostThink);
		PostThink(this);
	}
	else
		((PostThink_t)(FunctionOffset(Offsets->Player.PostThink)))(this);
}

void C_Entity::PreThink(bool Server)
{
	typedef void (__thiscall*PreThink_t)(void*thisptr);

	if(Server)
	{
		PreThink_t PreThink = ((PreThink_t)Offsets.ModuleHandle.PreThink);
		PreThink(this);
	}
	else
		((PreThink_t)(FunctionOffset(Offsets->Player.PreThink)))(this);
}

void C_Entity::SelectItem(const char* PStr, int SubType)
{
	typedef void (__thiscall*SelectItem_t)(void*thisptr,const char *PStr, int SubType);

	((SelectItem_t)(FunctionOffset(Offsets->Player.SelectItem)))(this,PStr,SubType);
}

void C_Entity::SetAbsVelocity(Vector AbsVelocity)
{
	Vector *PAbsVelocity = POffset(Offsets->Player.AbsVelocity,Vector);

	*PAbsVelocity = AbsVelocity;
}

void C_Entity::SetBaseVelocity(Vector BaseVelocity)
{
	Vector *PBaseVelocity= POffset(Offsets->Player.BaseVelocity,Vector);

	*PBaseVelocity = BaseVelocity;
}

void C_Entity::SetCurrentCommand(CUserCmd* Cmd)
{
	CUserCmd **PCmd = POffset(Offsets->Player.CurrentCommand,CUserCmd*);

	*PCmd = Cmd;
}

void C_Entity::SetEyeAngles(QAngle EyeAngles)
{
	QAngle *PEyeAngles = POffset(Offsets->Player.EyeAngles,QAngle);

	*PEyeAngles = EyeAngles;
}

void C_Entity::SetFallVelocity(float FallVelocity)
{
	float *PFallVelocity = POffset(Offsets->Player.FallVelocity,float);

	*PFallVelocity = FallVelocity;
}

void C_Entity::SetFlags(int Flags)
{
	int *PFlags = POffset(Offsets->Player.Flags,int);

	*PFlags = Flags;
}

void C_Entity::SetHitboxSet(int HitboxSet)
{
	int *PHitboxSet = POffset(Offsets->Player.HitboxSet,int);

	*PHitboxSet = HitboxSet;
}

void C_Entity::SetLocalViewAngles(const QAngle &ViewAngles)
{
	typedef void (__thiscall*SetLocalViewAngles_t)(void*thisptr,const QAngle& ViewAngles);

	((SetLocalViewAngles_t)(FunctionOffset(Offsets->Player.SetLocalViewAngles)))(this,ViewAngles);
}

void C_Entity::SetNextAttack(float NextAttack)
{
	float *PNextAttack = POffset(Offsets->Player.NextAttack,float);

	*PNextAttack = NextAttack;
}

void C_Entity::SetNextThinkTick(int NextThinkTick, const char* something)
{
	typedef void (__thiscall*SetNextThinkTick_t)(void*thisptr,int NextThinkTick,const char* something);

	((SetNextThinkTick_t)Offsets.ModuleHandle.SetNextThinkTick)(this,NextThinkTick,something);
}

void C_Entity::SetPredictionRandomSeed(CUserCmd *Cmd)
{
	typedef void (*SetPredictionRandomSeed_t)(CUserCmd*Cmd);

	((SetPredictionRandomSeed_t)Offsets.ModuleHandle.PredictionRandomSeed)(Cmd);
}

void C_Entity::SetPunchAngle(QAngle PunchAngle)
{
	QAngle *PPunchAngle = POffset(Offsets->Player.PunchAngle,QAngle);

	*PPunchAngle = PunchAngle;
}

void C_Entity::SetSimulationTime(float SimulationTime)
{
	float *PSimulationTime = POffset(Offsets->Player.SimulationTime,float);

	*PSimulationTime = SimulationTime;
}

void C_Entity::SetStamina(float Stamina)
{
	float *PStamina = POffset(Offsets->Player.Stamina,float);

	*PStamina = Stamina;
}

void C_Entity::SetTickBase(int TickBase)
{
	int *PTickBase = POffset(Offsets->Player.TickBase,int);

	*PTickBase = TickBase;
}

bool C_Entity::SetupBones(matrix3x4_t *PBoneToWorldOut, int NMaxBones, int BoneMask, float CurrentTime)
{
	typedef bool (__thiscall *SetupBones_t)(void*thisptr,matrix3x4_t *PBoneToWorldOut, int NMaxBones, int BoneMask, float CurrentTime);

	return ((SetupBones_t)(FunctionManualthisOffset(Offsets->Player.SetupBones,GetRenderable())))(GetRenderable(),PBoneToWorldOut,NMaxBones,BoneMask,CurrentTime);
}

void C_Entity::SetVelocity(Vector Velocity)
{
	Vector *PVelocity = POffset(Offsets->Player.Velocity,Vector);

	*PVelocity = Velocity;
}

void C_Entity::SetViewOffset(Vector ViewOffset)
{
	Vector *PViewOffset = POffset(Offsets->Player.ViewOffset,Vector);

	*PViewOffset = ViewOffset;
}

void C_Entity::Think()
{
	typedef void (__thiscall*Think_t)(void*thisptr);

	((Think_t)(FunctionOffset(Offsets->Player.Think)))(this);
}

bool C_Entity::UsingStandardWeaponsInVehicle()
{
	typedef bool (__thiscall*UsingStandardWeaponsInVehicle_t)(void *thisptr);

	return ((UsingStandardWeaponsInVehicle_t)Offsets.ModuleHandle.UsingStandardWeaponsInVehicle)(this);
}

void C_Entity::PredictWeapon()
{
	C_Weapon* Weapon = GetWeapon();

	if(!Weapon) return;

	Weapon->UpdateAccuracyPenalty();
}

void C_Entity::UpdateButtonState(int Buttons)
{
	typedef void (__thiscall*UpdateButtonState_t)(void*thisptr,int Buttons);

	((UpdateButtonState_t)Offsets.ModuleHandle.UpdateButtonState)(this,Buttons);
}

void C_Entity::SetRenderColor(color32 RenderColor)
{
	color32*PRenderColor = POffset(Offsets->Player.RenderColor,color32);

	*PRenderColor = RenderColor;
}

bool C_Entity::CanShoot()
{
	bool Result = false;

	C_Weapon* Weapon = GetWeapon();

	if(Weapon)
	{
		int WeaponID = Weapon->GetID();

		int Index = Weapon->OffsetIndexForSpecialWeapons();

		if((WeaponID == WEAPON_GLOCK) || (WeaponID == WEAPON_FAMAS))
		{ 
			if(Weapon->SpecialMode(Index))
			{
				if(Weapon->GetBurstShotsRemaining(Index) > 0)
					Result = ( (Weapon->GetNextPrimaryAttack() <= Instances::MethodSDK.Globals->curtime) || (Instances::MethodSDK.Globals->curtime > Weapon->GetNextBurstShootTime(Index)));
			}
		}
	}

	return Result;
}

void C_Entity::RemoveFromInterpolationList()
{
	typedef void(__thiscall* RemoveFromInterpolationList_t)(void*thisptr);

	((RemoveFromInterpolationList_t)Offsets.ModuleHandle.RemoveFromInterpolationList)(this);
}

void C_Entity::AddFromInterpolationList()
{
	typedef void(__thiscall* AddFromInterpolationList_t)(void*thisptr);

	((AddFromInterpolationList_t)Offsets.ModuleHandle.AddFromInterpolationList)(this);
}

void C_Entity::MoveToLastReceivedPosition(bool Force)
{
	typedef void(__thiscall* MoveToLastReceivedPosition_t)(void*thisptr,bool Force);

	((MoveToLastReceivedPosition_t)Offsets.ModuleHandle.MoveToLastReceivedPosition)(this,Force);
}

unsigned short C_Entity::GetInterpolationList()
{
	return Offset(Offsets->Player.InterpolationList,unsigned short);
}

void C_Entity::SetInterpolationList(unsigned short InterpolationList)
{
	unsigned short* PInterpolationList = POffset(Offsets->Player.InterpolationList,unsigned short);

	*PInterpolationList = InterpolationList;
}

void C_Entity::SetLocalOrigin( const Vector& Origin )
{
	typedef void (__thiscall *SetLocalOrigin_t)(void*thisptr, const Vector& Origin);

	((SetLocalOrigin_t)Offsets.ModuleHandle.SetLocalOrigin)(this,Origin);
}

bool C_Entity::HitboxToWorldTransforms(matrix3x4_t *pHitboxToWorld[MAXSTUDIOBONES] )
{
	typedef bool(__thiscall*HitboxToWorldTransforms_t)(void*thisptr, matrix3x4_t *pHitboxToWorld[MAXSTUDIOBONES] );

	return ((HitboxToWorldTransforms_t)Offsets.ModuleHandle.HitboxToWorldTransforms)(this,pHitboxToWorld);
}

