#include "main.h"

C_Offsets::C_Offsets ( void )
{
	//addrs

	ModuleHandle.CSWpnData = CLIENT(0x20D300);
	ModuleHandle.WeaponAlias = CLIENT(0x20E000);

	//player

	Player.AbsVelocity = Offset_BasePlayer("m_vecVelocity[0]") + 0xC;
	Player.ActiveWeapon = Offset_BaseCombatCharacter("m_hActiveWeapon");
	Player.Armor = Offset_CSPlayer("m_ArmorValue");
	Player.BaseVelocity = Offset_BasePlayer("m_vecBaseVelocity");
	Player.EyeAngles = Offset_CSPlayer("m_angEyeAngles[0]");
	Player.FallVelocity = Offset_BasePlayer("m_flFallVelocity");
	Player.Flags = Offset_BasePlayer("m_fFlags");
	Player.Health = Offset_BasePlayer("m_iHealth");
	Player.HitboxSet = Offset_BaseAnimating("m_nHitboxSet");
	Player.LifeState = Offset_BasePlayer("m_lifeState");
	Player.NextAttack = Offset_CSPlayer("m_flNextAttack");
	Player.PunchAngle = Offset_BasePlayer("m_vecPunchAngle");
	Player.RenderColor = Offset_BaseEntity("m_clrRender");
	Player.SimulationTime = Offset_BaseEntity("m_flSimulationTime");
	Player.OldSimulationTime = Offset_BaseEntity("m_flSimulationTime") - 0x4;
	Player.Stamina = Offset_CSPlayer("m_flStamina");
	Player.TeamNum = Offset_BasePlayer("m_iTeamNum");
	Player.TickBase = Offset_BasePlayer("m_nTickBase");
	Player.Velocity = Offset_BasePlayer("m_vecVelocity[0]");
	Player.ViewOffset = Offset_BasePlayer("m_vecViewOffset[0]");
	Player.ClientClass = 8;
	Player.CurrentCommand = 0x100C;
	Player.GetAbsOrigin = 0x24;
	Player.GetIndex = 0x24;
	Player.GetModel = 0x24;
	Player.IsDormant = 0x20;
	Player.SetupBones = 0x40;
	Player.WaterLevel = 0x178;
	Player.MoveType = 0x174;
	Player.Networkable = 8;
	Player.Renderable = 4;
	Player.InterpolationList = 0x4FA;

	//weapons

	Weapon.AccuracyPenalty = Offset_WeaponCSBase("m_fAccuracyPenalty");
	Weapon.BurstModeFamas = Offset_WeaponFamas("m_bBurstMode");
	Weapon.BurstModeGlock = Offset_WeaponGlock("m_bBurstMode");
	Weapon.Clip1 = Offset_BaseCombatWeapon("m_iClip1");
	Weapon.iBurstShotsRemainingGlock = Offset_WeaponGlock("m_iBurstShotsRemaining");
	Weapon.iBurstShotsRemainingFamas = Offset_WeaponFamas("m_iBurstShotsRemaining");
	Weapon.NextPrimaryAttack = Offset_BaseCombatWeapon("m_flNextPrimaryAttack");
	Weapon.NextSecondaryAttack = Offset_BaseCombatWeapon("m_flNextSecondaryAttack");
	Weapon.SilencerUsp = Offset_WeaponUSP("m_bSilencerOn");
	Weapon.SilencerM4 = Offset_WeaponM4A1("m_bSilencerOn");
	Weapon.NextBurstShootTimeFamas = Offset_WeaponFamas("m_iBurstShotsRemaining") + 0x4;
	Weapon.NextBurstShootTimeGlock = Offset_WeaponGlock("m_iBurstShotsRemaining") + 0x4;
	Weapon.Cone = 0x5CC;
	Weapon.Spread = 0x5D0;
	Weapon.UpdateAccuracyPenalty = 0x5D4;
	Weapon.ID = 0x5A0;
}