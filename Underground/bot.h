#include "main.h"

#define Corners 9

class C_Weapon;

#ifndef UTILBOT
#define UTILBOT

	bool IsBSPModel(void*thisptr);
	void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IHandleEntity *ignore, int collisionGroup, trace_t *ptr);
	void UTIL_TraceHull(const Vector &vecAbsStart, const Vector &vecAbsEnd, const Vector &hullMin, const Vector &hullMax, unsigned int mask, const IHandleEntity *ignore, int collisionGroup, trace_t *ptr);
	void FindHullIntersection(const Vector &vecSrc, trace_t &tr, const Vector &mins, const Vector &maxs, C_BaseEntity *pEntity);
	bool IntersectRayWithOBB(const Ray_t &ray, const matrix3x4_t &matOBBToWorld, const Vector &vecOBBMins, const Vector &vecOBBMaxs, float flTolerance, CBaseTrace *pTrace);
	void TraceFilterSkipTwoEntities(DWORD* thisptr, const C_BasePlayer *PassEntity, const C_BasePlayer *PassEntity2, int CollisionGroup);
	void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, void *filter, trace_t *tr);

	class CAimbot
	{

	public:
		CAimbot();

		void RunBot(CUserCmd*Cmd, C_BasePlayer*LocalPlayer, C_Weapon*weapon);

		void GetHitbox(int hitbox, mstudiohitboxset_t* HitboxSet,float scale);
		int GetTotalDamage(C_BasePlayer *LocalPlayer, C_Weapon* Weapon);
		int Target;
		Vector m_vecTargetPos[Corners], m_vecCorners[Corners];
		Vector m_vecDirection;
		Vector m_vecMyEyePos;
		QAngle m_angAimAngle;
		C_BasePlayer* m_pEntities;
		matrix3x4_t *m_mMatrix;
		int *Tick;

	}extern Aimbot;

	class CKnifebot
	{

	public:
		CKnifebot();

		void RunBot(CUserCmd*Cmd, C_BasePlayer*LocalPlayer, C_Weapon*weapon);

		int Target;
		Vector MyEyePos;

	}extern Knifebot;


	
typedef void(__thiscall*AddVar_t)(void*, void *data, IInterpolatedVar *watcher, int type, bool bSetup );
extern AddVar_t AddVar;

#endif