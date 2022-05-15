#include "main.h"

HANDLE Thread;
float(*Shared_RandomFloat)(float, float);
int(*Shared_RandomInt)(int, int);
void(*Shared_RandomSeed)(int);

void MyMessage(const char* Message, ...)
{
	va_list list;
	char buffer[0xFF];
	va_start(list, Message);
	_vsnprintf_s(buffer, 0xFFF, Message, list);
	va_end(list);

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), 0, 0);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), "\n", strlen("\n"), 0, 0);
}

C_VMT *GetVMTInstancebyFindingInterfaces(char* szInterfaceName, char* interfaceHandle)
{
	char buffer[0xFF];

	do
	{
		Sleep(1);

	} while (!GetModuleHandleA(interfaceHandle));

	CreateInterfaceFn Interface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(interfaceHandle), "CreateInterface");

	int index = 0xFF;
	do
	{

		if (index <= 10)  { sprintf_s(buffer, "%s00%i", szInterfaceName, index); }
		if (index >= 10)  { sprintf_s(buffer, "%s0%i", szInterfaceName, index); }
		if (index >= 100) { sprintf_s(buffer, "%s%i", szInterfaceName, index); }

		if (Interface(buffer, 0))
		{
			break;
		}

		index--;

	} while (index >= 0);

	void* rendertmp = reinterpret_cast<void*>(Interface(buffer, 0));

	C_VMT *tmpvmt = new C_VMT(rendertmp);

	return tmpvmt;
}

void* GetSDKInstancebyFindingInterfaces(char* szInterfaceName, char* interfaceHandle)
{
	char buffer[0xFF];

	do
	{
		Sleep(1);

	} while (!GetModuleHandleA(interfaceHandle));

	CreateInterfaceFn Interface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(interfaceHandle), "CreateInterface");

	int index = 0xFF;
	do
	{

		if (index <= 10)  { sprintf_s(buffer, "%s00%i", szInterfaceName, index); }
		if (index >= 10)  { sprintf_s(buffer, "%s0%i", szInterfaceName, index); }
		if (index >= 100) { sprintf_s(buffer, "%s%i", szInterfaceName, index); }

		if (Interface(buffer, 0))
		{
			break;
		}

		index--;

	} while (index >= 0);

	void* rendertmp = reinterpret_cast<void*>(Interface(buffer, 0));

	return rendertmp;
}

void GetInterfaces(void)// do u 
{


	Client = GetVMTInstancebyFindingInterfaces("VClient", "client");
	EntityList = GetVMTInstancebyFindingInterfaces("VClientEntityList", "client");
	Prediction = GetVMTInstancebyFindingInterfaces("VClientPrediction", "client");
	GameMovement = GetVMTInstancebyFindingInterfaces("GameMovement", "client");
	Engine = GetVMTInstancebyFindingInterfaces("VEngineClient", "engine");
	Cvar = GetVMTInstancebyFindingInterfaces("VEngineCvar", "vstdlib");
	ModelInfo = GetVMTInstancebyFindingInterfaces("VModelInfoClient", "engine");
	EngineTrace = GetVMTInstancebyFindingInterfaces("EngineTraceClient", "engine");
	PhysicsSurfaceProps = GetVMTInstancebyFindingInterfaces("VPhysicsSurfaceProps", "vphysics");
	Panel = GetVMTInstancebyFindingInterfaces("VGUI_Panel", "vgui2");
	Surface = GetVMTInstancebyFindingInterfaces("VGUI_Surface", "vguimatsurface");
	RenderView = GetVMTInstancebyFindingInterfaces("VEngineRenderView", "engine");
	MaterialSystem = GetVMTInstancebyFindingInterfaces("VMaterialSystem", "materialsystem");
	ModelRender = GetVMTInstancebyFindingInterfaces("VEngineModel", "engine");
	Effects = GetVMTInstancebyFindingInterfaces("VEngineEffects", "engine");
	GameEventManager = GetVMTInstancebyFindingInterfaces("GAMEEVENTSMANAGER", "engine");
	Input = new C_VMT(**(void***)(Client->GetMethod<DWORD>(15) + 0x2));

	ClientMode = new C_VMT(*(void**)(CLIENT(0x576004)));

	Globals = *(CGlobalVars**)(CLIENT(0x50B2B0));



	ClientSDK = (IBaseClientDLL*)GetSDKInstancebyFindingInterfaces("VClient", "client");
	EntityListSDK = (IClientEntityList*)GetSDKInstancebyFindingInterfaces("VClientEntityList", "client");
	PredictionSDK = (CPrediction*)GetSDKInstancebyFindingInterfaces("VClientPrediction", "client");
	GameMovementSDK = (CGameMovement*)GetSDKInstancebyFindingInterfaces("GameMovement", "client");
	EngineSDK = (IVEngineClient*)GetSDKInstancebyFindingInterfaces("VEngineClient", "engine");
	CvarSDK = (ICvar*)GetSDKInstancebyFindingInterfaces("VEngineCvar", "vstdlib");
	ModelInfoSDK = (IVModelInfoClient*)GetSDKInstancebyFindingInterfaces("VModelInfoClient", "engine");
	EngineTraceSDK = (IEngineTrace*)GetSDKInstancebyFindingInterfaces("EngineTraceClient", "engine");
	PhysicsSurfacePropsSDK = (IPhysicsSurfaceProps*)GetSDKInstancebyFindingInterfaces("VPhysicsSurfaceProps", "vphysics");
	PanelSDK = (vgui::IPanel*)GetSDKInstancebyFindingInterfaces("VGUI_Panel", "vgui2");
	SurfaceSDK = (vgui::ISurface*)GetSDKInstancebyFindingInterfaces("VGUI_Surface", "vguimatsurface");
	RenderViewSDK = (IVRenderView*)GetSDKInstancebyFindingInterfaces("VEngineRenderView", "engine");
	MaterialSystemSDK = (IMaterialSystem*)GetSDKInstancebyFindingInterfaces("VMaterialSystem", "materialsystem");
	ModelRenderSDK = (IVModelRender*)GetSDKInstancebyFindingInterfaces("VEngineModel", "engine");
	EffectsSDK = (IVEfx*)GetSDKInstancebyFindingInterfaces("VEngineEffects", "engine");
	GameEventManagerSDK = (IGameEventManager2*)GetSDKInstancebyFindingInterfaces("GAMEEVENTSMANAGER", "engine");
	DebugOverlay = (IVDebugOverlay*)GetSDKInstancebyFindingInterfaces("VDebugOverlay", "engine");

	PDWORD Client = (PDWORD)*(PDWORD)(ClientSDK);
	InputSDK = (CInput*)*(PDWORD)*(PDWORD)(Client[15] + 0x2);


}
typedef void(__thiscall*FireBullett)(
	void*,
	Vector vecSrc,	// shooting postion
	const QAngle &shootAngles,  //shooting angle
	float vecSpread, // spread vector
	int iPenetration, // how many obstacles can be penetrated
	int iBulletType, // ammo type
	int iDamage, // base damage
	float flRangeModifier, // damage range modifier
	CBaseEntity *pevAttacker, // shooter
	bool bDoEffects,
	float x,
	float y
	); FireBullett FireBulletC;
FireBullett FireBulletS;

void __fastcall new_FireBulletS(
	void *thisptr,
	int edx,
	Vector vecSrc,	// shooting postion
	const QAngle &shootAngles,  //shooting angle
	float vecSpread, // spread vector
	int iPenetration, // how many obstacles can be penetrated
	int iBulletType, // ammo type
	int iDamage, // base damage
	float flRangeModifier, // damage range modifier
	CBaseEntity *pevAttacker, // shooter
	bool bDoEffects,
	float x,
	float y
	)
{
	x = RandomFloat(0.5f, -0.5f);
	y = RandomFloat(0.5f, -0.5f);
	FireBulletS(thisptr, vecSrc, shootAngles, vecSpread, iPenetration, iBulletType, iDamage, flRangeModifier, pevAttacker, bDoEffects, x, y);
}

void __fastcall new_FireBulletC(
	void *thisptr,
	int edx,
	Vector vecSrc,	// shooting postion
	const QAngle &shootAngles,  //shooting angle
	float vecSpread, // spread vector
	int iPenetration, // how many obstacles can be penetrated
	int iBulletType, // ammo type
	int iDamage, // base damage
	float flRangeModifier, // damage range modifier
	CBaseEntity *pevAttacker, // shooter
	bool bDoEffects,
	float x,
	float y
	)
{

	FireBulletC(thisptr, vecSrc, shootAngles, vecSpread, iPenetration, iBulletType, iDamage, flRangeModifier, pevAttacker, bDoEffects, x, y);
}

void Player_EyeAnglesX(const CRecvProxyData *pData, void *pStruct, void *pOut)//aaa?
{
	static bool LookedDownLastTick[64];

	float pitch = pData->m_Value.m_Float;

	DWORD Dpitch = *reinterpret_cast< DWORD const* >(&pitch);

	if ((Dpitch > 0x43341683)
		&& (Dpitch <= 0x43870000))
	{
		pitch = 270.f;
	}
	else if ((Dpitch >= 0x42b40000)
		&& (Dpitch <= 0x43341683))
	{
		pitch = 90.f;
	}

	bool tmp = LookedDownLastTick[((C_BaseEntity*)(pStruct))->index];
	LookedDownLastTick[((C_BaseEntity*)(pStruct))->index] = (pitch == 90);

	if (pitch == -90)
	{
		if (tmp)
		{
			pitch = 90;
		}
	}


	*(float*)(pOut) = pitch;

}

void Hooks(void)
{

	Client->HookMethod(&new_CreateMoveClient, 21);
	ClientMode->HookMethod(&new_CreateMoveClientMode, 21);
	Panel->HookMethod(&new_PaintTraverse, 41);
	Input->HookMethod(&new_WriteUsercmdDeltaToBuffer, 5);
	Prediction->HookMethod(&new_RunCommand, 17);
	Prediction->HookMethod(&new_InPrediction, 14);
	Input->HookMethod(&new_GetUserCmd, 8);
	Engine->HookMethod(&new_DrawPortals, 75);
	ModelRender->HookMethod(&new_DrawModelExecute, 19);
	MaterialSystem->HookMethod(&new_FindMaterial, 71);

	FireBulletC = (FireBullett)DetourFunction((PBYTE)CLIENT(0x1DBE70), (PBYTE)new_FireBulletC);
    FireBulletS = (FireBullett)DetourFunction((PBYTE)SERVER(0x27E910), (PBYTE)new_FireBulletS);
}

void MainUndergroundHack(HMODULE hinstDLL)
{
	GetInterfaces();
	Hooks();
	NetworkedVariableManager = new CNetworkedVariableManager();

	NetworkedVariableManager->HookProp("DT_CSPlayer", "m_angEyeAngles[0]", Player_EyeAnglesX);

	Offsets = new C_Offsets();
	Draw.ConsoleFont = SurfaceSDK->CreateFont();
	SurfaceSDK->SetFontGlyphSet(Draw.ConsoleFont, "Tahoma", 12, FW_BOLD, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_DROPSHADOW);
	Shared_RandomFloat = (float(__cdecl*)(float, float))GetProcAddress(GetModuleHandle("vstdlib"), "RandomFloat");
	Shared_RandomInt = (int(__cdecl*)(int, int))GetProcAddress(GetModuleHandle("vstdlib"), "RandomInt");
	Shared_RandomSeed = (void(__cdecl*)(int))GetProcAddress(GetModuleHandle("vstdlib"), "RandomSeed");

	bool one = true;
	WriteProcessMemory(GetCurrentProcess(),(LPVOID)CLIENT(0x957E4),&one,1,0);
}


char g_DllPath[MAX_PATH];
bool __stdcall DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	AllocConsole();
	SetConsoleTitle("Kiro Base - CSS");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY
		| FOREGROUND_RED
		| COMMON_LVB_REVERSE_VIDEO);

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);

		GetModuleFileName(hinstDLL, g_DllPath, sizeof(g_DllPath));

		int	 iLen = (int)strlen(g_DllPath);
		char* pos = g_DllPath + iLen;
		while (pos >= g_DllPath &&*pos != '\\')
			--pos;
		*(char*)(pos + 1) = 0;

		m_pszDllPath = g_DllPath;

		Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainUndergroundHack, 0, 0, 0);
	}

	return true;
}

