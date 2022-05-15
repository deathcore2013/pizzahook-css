#include "main.h"

#ifndef UTILS_H
#define UTILS_H

typedef unsigned int uint;
typedef int int_ptr;
extern void MyMessage(const char* Message,...);
class Vector;
class Color;
struct matrix3x4_t;
class QAngle;

extern FORCEINLINE DWORD_PTR				GetEBP ( void );
extern FORCEINLINE DWORD_PTR				GetESP ( void );
extern FORCEINLINE DWORD_PTR				GetESI ( void );
extern FORCEINLINE DWORD_PTR				GetEDI ( void );

//////////////////////////////////////////////////////////////////////////////////

FORCEINLINE void**& GetVTable( void* inst )
{
	return *reinterpret_cast<void***>( (size_t)inst );
}

FORCEINLINE const void** GetVTable( const void* inst )
{
	return *reinterpret_cast<const void***>( (size_t)inst );
}

template< typename T > FORCEINLINE T GetMethod( const void* pInstant, size_t nIndex )
{
	return reinterpret_cast< T >( GetVTable( pInstant )[ nIndex ] );
}

template< typename T > FORCEINLINE T make_ptr(void* ptr, int_ptr offset) 
{
	return reinterpret_cast<T>((size_t)ptr + offset);
}

FORCEINLINE void**& getvtable(void* inst, int_ptr offset = 0) 
{
	return *reinterpret_cast<void***>( (size_t)inst + offset );
}

// Find the number of vfuncs in a vtable
uint CountFuncs(void** pVMT);
uint CountFuncs(void* begin, void* end, void** pVMT);

// Find the index for a vfunc, result is negative if not found
int FindFunc(void** pVMT, void* pFunc, uint vfuncs = 0);

void lowercase(char *str);

bool StringCompareI( const char*pszOrgString, const char*pszCompString );

std::string szDirFile (const char* pszName );
extern char *m_pszDllPath;

std::string szDirFileConfig(const char* pszName);

std::string szDirFileDemos(const char* pszName);

std::string szDirFileConfigDll(const char* pszName);

std::string szDirFileDemosDll(const char* pszName);

#define FunctionOffset(offset) *(DWORD*)(*(DWORD*)this + offset)
#define PFunctionOffset(offset) (DWORD*)(*(DWORD*)this + offset)
#define FunctionManualthisOffset(offset,thiss) *(DWORD*)(*(DWORD*)thiss + offset)
#define PFunctionManualthisOffset(offset,thiss) (DWORD*)(*(DWORD*)thiss + offset)
#define Offset(offset,type) *(type*)((DWORD)this + offset) 
#define POffset(offset,type) (type*)((DWORD)this + offset)
#define OffsetManualthis(offset,type,thiss) *(type*)((DWORD)thiss + offset)
#define POffsetManualthis(offset,type,thiss) (type*)((DWORD)thiss + offset) 

#define CLIENT(off) ((DWORD)GetModuleHandle("client")+off)
#define ENGINE(off) ((DWORD)GetModuleHandle("engine")+off)
#define SERVER(off) ((DWORD)GetModuleHandle("server")+off)

#define Offset_BaseEntity(name) NetworkedVariableManager->GetOffset("DT_BaseEntity",name)
#define Offset_BasePlayer(name) NetworkedVariableManager->GetOffset("DT_BasePlayer",name)
#define Offset_CSPlayer(name) NetworkedVariableManager->GetOffset("DT_CSPlayer",name)
#define Offset_BaseCombatCharacter(name) NetworkedVariableManager->GetOffset("DT_BaseCombatCharacter",name)
#define Offset_BaseAnimating(name) NetworkedVariableManager->GetOffset("DT_BaseAnimating",name)
#define Offset_BaseCombatWeapon(name) NetworkedVariableManager->GetOffset("DT_BaseCombatWeapon",name)
#define Offset_WeaponCSBase(name) NetworkedVariableManager->GetOffset("DT_WeaponCSBase",name)
#define Offset_WeaponFamas(name) NetworkedVariableManager->GetOffset("DT_WeaponFamas",name)
#define Offset_WeaponGlock(name) NetworkedVariableManager->GetOffset("DT_WeaponGlock",name)
#define Offset_WeaponM4A1(name) NetworkedVariableManager->GetOffset("DT_WeaponM4A1",name)
#define Offset_WeaponUSP(name) NetworkedVariableManager->GetOffset("DT_WeaponUSP",name)

/////////////////////////////////////////////////////////

class C_Offsets
{


public:

	C_Offsets( void );

	struct S_ModuleHandle
	{
		DWORD WeaponAlias,
			CSWpnData;

	}ModuleHandle;

	struct S_Player
	{
		int Renderable,
			Networkable,
			ClientClass;
		DWORD CurrentCommand,
			MoveType,
			WaterLevel,
			IsDormant,
			GetIndex,
			GetModel,
			SetupBones,
			GetAbsOrigin,
			ActiveWeapon,
			PunchAngle,
			LifeState,
			TickBase,
			Flags,
			NextAttack,
			HitboxSet,
			EyeAngles,
			ViewOffset,
			TeamNum,
			Health,
			RenderColor,
			Armor,
			Velocity,
			AbsVelocity,
			BaseVelocity,
			FallVelocity,
			SimulationTime,
			OldSimulationTime,
			Stamina,
			InterpolationList;

	}Player;

	struct S_Weapon
	{
		DWORD ID,
			Cone,
			Spread,
			UpdateAccuracyPenalty,
			NextPrimaryAttack,
			NextSecondaryAttack,
			Clip1,
			AccuracyPenalty,
			iBurstShotsRemainingGlock,
			iBurstShotsRemainingFamas,
			SilencerM4,SilencerUsp,
			BurstModeFamas,BurstModeGlock,
			NextBurstShootTimeFamas,NextBurstShootTimeGlock;

	}Weapon;

} extern *Offsets;


/////////////////////////////////////// VISUALS PART


class CDraw
{
public:
	void Line( int x, int y, int x2, int y2, int w, Color dwColor ); 
	void FillRGBA( int x, int y, int w, int h, int r, int g, int b, int a ); 
	void FillRGBA( int x, int y, int w, int h, Color dwColor ); 
	void Outline( int x, int y, int w, int h ); 
	void OutlineBox( int x, int y, int w, int h, int r, int g, int b, int a ); 
	void DrawBox( int x, int y, Color dwColor, int box_width, int radius ); 
	void DrawRect( int x, int y, int w, int h, Color dwColor ); 
	void DrawOutlineRect( int x, int y, int w, int h, Color dwColor ); 
	void tintAreaDegradeUp(int x,int y,int w,int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int variation); 
	void tintAreaDegradeLeft(int x,int y,int w,int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int variation); 
	void tintAreaDegradeDown(int x,int y,int w,int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int variation); 
	void tintAreaDegradeRight(int x,int y,int w,int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int variation); 
	void OutlineArea(int x, int y, int w, int h, int r, int g, int b, int a); 
	void DrawLine(int startX, int startY, int endX, int endY, int r, int g, int b, int a); 
	void RenderBox(Vector const& vMin, Vector const& vMax, matrix3x4_t const& xToWorld );
	bool WorldToScreen(const Vector &Origin,Vector &Screen);
	void PrintText(bool center,int x,int y,Color dwColor,const char *input,...);
	int GetWidth(const char *input);
	int GetHeight(const char *input);

	DWORD ConsoleFont;
	DWORD TextWorld;


}extern Draw; 

class CMeshBuilder;
class IMaterial;

void DrawScreenSpaceRectangle(IMaterial *pMaterial,
	int nDestX, int nDestY, int nWidth, int nHeight,	// Rect to draw into in screen space
	float flSrcTextureX0, float flSrcTextureY0,		// which texel you want to appear at destx/y
	float flSrcTextureX1, float flSrcTextureY1,		// which texel you want to appear at destx+width-1, desty+height-1
	int nSrcTextureWidth, int nSrcTextureHeight,		// needed for fixup
	void *pClientRenderable,							// Used to pass to the bind proxies
	int nXDice, int nYDice);// Amount to tessellate the 

void RenderQuad(IMaterial *pMaterial, float x, float y, float w, float h,
	float z, float s0, float t0, float s1, float t1, const Color& clr);

void RenderWireframeSweptBox(const Vector &vStart, const Vector &vEnd, const QAngle &angles, const Vector &vMins, const Vector &vMaxs, Color c, bool bZBuffer);
static void DrawExtrusionFace(const Vector& start, const Vector& end,
	Vector* pts, int idx1, int idx2, Color c, CMeshBuilder& meshBuilder);
static void DrawAxes(const Vector& origin, Vector* pts, int idx, Color c, CMeshBuilder& meshBuilder);
void RenderTriangle(const Vector& p1, const Vector& p2, const Vector& p3, Color c, bool bZBuffer);
void RenderLine(const Vector& v1, const Vector& v2, Color c, bool bZBuffer);
void RenderAxes(const matrix3x4_t &transform, float flScale, bool bZBuffer);
void RenderAxes(const Vector &vOrigin, float flScale, bool bZBuffer);
void RenderBox(const Vector& vOrigin, const QAngle& angles, const Vector& vMins, const Vector& vMaxs, Color c, bool bZBuffer, bool bInsideOut);
void RenderWireframeBox(const Vector &vOrigin, const QAngle& angles, const Vector &vMins, const Vector &vMaxs, Color c, bool bZBuffer);
void RenderSphere(const Vector &vCenter, float flRadius, int nTheta, int nPhi, Color c, bool bZBuffer);
void RenderWireframeSphere(const Vector &vCenter, float flRadius, int nTheta, int nPhi, Color c, bool bZBuffer);

extern float xPos, yPos;


class C_BasePlayer;
float GroundDistance(C_BasePlayer*LocalPlayer);

void VectorAngles2(const Vector& forward, QAngle &angles);

extern bool Yey;
#endif