#include "definitions.h"

typedef void(__thiscall*DrawPortalFn)(void*);
void __fastcall new_DrawPortals(void*thisptr, int edx)
{

	C_BasePlayer* LocalPlayer = (C_BasePlayer*)EntityListSDK->GetClientEntity(EngineSDK->GetLocalPlayer());

	for (int i = EntityListSDK->GetMaxEntities(); i >= 0; --i)
	{

		if (i == EngineSDK->GetLocalPlayer())
			continue;

		C_BaseCombatWeapon *weapon = (C_BaseCombatWeapon*)EntityListSDK->GetClientEntity(i);

		if (weapon == 0)
		{
			continue;
		}

		const char *Name = ModelInfoSDK->GetModelName(weapon->GetModel());

		if (!strstr(Name, "w_")) continue;

		if (OffsetManualthis(0x89C, int, weapon) == WEAPON_NOT_CARRIED)
		{
			Vector mins, maxs;

			weapon->GetRenderBounds(mins, maxs);

			RenderWireframeBox(weapon->GetAbsOrigin(), weapon->GetRenderAngles(), mins, maxs, Color(0, 0, 0, 255), false);
			RenderWireframeBox(weapon->GetAbsOrigin(), weapon->GetRenderAngles(), mins, maxs, Color(255, 255, 255, 255), true);
		}
	}
}
