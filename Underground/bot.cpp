#include "bot.h"

bool IsBSPModel(void*thisptr)
{
	typedef bool(__thiscall*IsBSPModel_t)(void*thisptr);
	static IsBSPModel_t IsBSPModel = (IsBSPModel_t)((DWORD)GetModuleHandle("client.dll") + 0x66E90);
	return IsBSPModel(thisptr);
}

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IHandleEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef void(*UTIL_TraceLine_t)(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IHandleEntity *ignore, int collisionGroup, trace_t *ptr);
	static UTIL_TraceLine_t UTIL_TraceLine = (UTIL_TraceLine_t)((DWORD)GetModuleHandle("client.dll") + 0x67D70);
	UTIL_TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}

void UTIL_TraceHull(const Vector &vecAbsStart, const Vector &vecAbsEnd, const Vector &hullMin, const Vector &hullMax, unsigned int mask, const IHandleEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef void(*UTIL_TraceHull_t)(const Vector &vecAbsStart, const Vector &vecAbsEnd, const Vector &hullMin, const Vector &hullMax, unsigned int mask, const IHandleEntity *ignore, int collisionGroup, trace_t *ptr);
	static UTIL_TraceHull_t UTIL_TraceHull = (UTIL_TraceHull_t)((DWORD)GetModuleHandle("client.dll") + 0x6AAF0);
	UTIL_TraceHull(vecAbsStart, vecAbsEnd, hullMin, hullMax, mask, ignore, collisionGroup, ptr);
}

void FindHullIntersection(const Vector &vecSrc, trace_t &tr, const Vector &mins, const Vector &maxs, C_BaseEntity *pEntity)
{
	typedef void(*FindHullIntersection_t)(const Vector &vecSrc, trace_t &tr, const Vector &mins, const Vector &maxs, C_BaseEntity *pEntity);
	static FindHullIntersection_t FindHullIntersection = (FindHullIntersection_t)((DWORD)GetModuleHandle("client.dll") + 0x211C90);
	FindHullIntersection(vecSrc, tr, mins, maxs, pEntity);
}

bool IntersectRayWithOBB(const Ray_t &ray, const matrix3x4_t &matOBBToWorld, const Vector &vecOBBMins, const Vector &vecOBBMaxs, float flTolerance, CBaseTrace *pTrace)
{
	typedef bool(*IntersectRayWithOBB_t)(const Ray_t &ray, const matrix3x4_t &matOBBToWorld, const Vector &vecOBBMins, const Vector &vecOBBMaxs, float flTolerance, CBaseTrace *pTrace);
	static IntersectRayWithOBB_t IntersectRayWithOBB = (IntersectRayWithOBB_t)((DWORD)GetModuleHandle("client.dll") + 0xF5C30);
	return IntersectRayWithOBB(ray, matOBBToWorld, vecOBBMins, vecOBBMaxs, flTolerance, pTrace);
}

void TraceFilterSkipTwoEntities(DWORD* thisptr, const C_BasePlayer *PassEntity, const C_BasePlayer *PassEntity2, int CollisionGroup)
{
	typedef void(__thiscall *TraceFilterSkipTwoEntities_t)(void *thisptr, const C_BasePlayer *PassEntity, const C_BasePlayer *PassEntity2, int CollisionGroup);
	((TraceFilterSkipTwoEntities_t)CLIENT(0x19C830))(thisptr, PassEntity, PassEntity2, CollisionGroup);
}

void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, void *filter, trace_t *tr)
{
	typedef void(*UTIL_ClipTraceToPlayers_t)(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, void *filter, trace_t *tr);
	((UTIL_ClipTraceToPlayers_t)CLIENT(0x19CEE0))(vecAbsStart, vecAbsEnd, mask, filter, tr);
}