

#ifndef CLIENT_DLL
#define CLIENT_DLL

#include "SDK/public/cdll_int.h"
#include "SDK/public/IClientEntityList.h"
#include "SDK/public/IClientEntity.h"
#include "SDK/public/view_shared.h"
#include "SDK/game/client/cliententitylist.h"
#include "SDK/game/client/cbase.h"
#include "SDK/game/client/c_baseanimating.h"
#include "SDK/game/client/c_basecombatweapon.h"
#include "SDK/game/client/c_baseplayer.h"
#include "SDK/game/client/iclientvehicle.h"
#include "SDK/game/client/tempent.h"
#include "SDK/game/client/input.h"
#include "SDK/public/icvar.h"
#include "SDK/public/inetchannel.h"
#include "SDK/public/engine/ivdebugoverlay.h"
#include "SDK/public/engine/ivmodelinfo.h"
#include "SDK/public/engine/IEngineTrace.h"
#include "SDK/public/inetchannelinfo.h"
#include "SDK/public/globalvars_base.h"
#include "SDK/public/vphysics_interface.h"
#include "SDK/game/shared/usermessages.h"
#include "SDK/game/shared/basecombatweapon_shared.h"
#include "SDK/game/shared/takedamageinfo.h"
#include "SDK/game/shared/in_buttons.h"
#include "SDK/public/engine/ivdebugoverlay.h"
#include "SDK/public/vgui/IPanel.h"
#include "SDK/public/vgui/ISurface.h"
#include "SDK/public/vgui/IInput.h"
#include "SDK/public/inputsystem/iinputsystem.h"
#include "SDK/public/igameresources.h"
#include "SDK/public/iprediction.h"
#include "SDK/public/inetmessage.h"
#include "SDK/game/client/prediction.h"
#include "SDK/game/shared/igamemovement.h"
#include "SDK/public/igameresources.h"
#include "SDK/public/mathlib/compressed_vector.h"
#include "SDK/game/shared/gamemovement.h"
#include "SDK/public/materialsystem/imaterialsystem.h"
#include "SDK/public/engine/ivmodelrender.h"
#include "SDK/public/ivrenderview.h"
#include "SDK/public/engine/ienginesound.h"
#include "SDK/public/iefx.h"
#include "SDK/public/dlight.h"
#include "SDK/public/igameevents.h"
#include "checksum_md5.h"
#include <tier1/tier1.h>
#include <tier2/tier2.h>
#include <tier3/tier3.h>
#include <interface.h>

#pragma comment ( lib, "SDK/lib/public/vstdlib.lib" )
#pragma comment ( lib, "SDK/lib/public/mathlib.lib" )
#pragma comment ( lib, "SDK/lib/public/tier0.lib" )
#pragma comment ( lib, "SDK/lib/public/tier1.lib" )
#pragma comment ( lib, "SDK/lib/public/tier2.lib" )
#pragma comment ( lib, "SDK/lib/public/tier3.lib" )

typedef void* (*CreateInterfaceFn)( const char*, int* );
class Vector;
#undef CreateFont

#define CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_BLOODYFLESH	'B'
#define CHAR_TEX_CLIP			'I'
#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_PLASTIC		'L'

#define IN_ATTACK				( 1 << 0 )
#define IN_JUMP					( 1 << 1 )
#define IN_DUCK					( 1 << 2 )

#define	FL_ONGROUND				( 1 << 0 )
#define FL_DUCKING				( 1 << 1 )

enum WeaponId_t
{
	WEAPON_CSBASE = 0,
	WEAPON_P228,
	WEAPON_GLOCK,
	WEAPON_SCOUT,
	WEAPON_HEGRENADE,
	WEAPON_XM1014,
	WEAPON_C4,
	WEAPON_MAC10,
	WEAPON_AUG,
	WEAPON_SMOKEGRENADE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_UMP45,
	WEAPON_SG550,
	WEAPON_GALIL,
	WEAPON_FAMAS,
	WEAPON_USP,
	WEAPON_AWP,
	WEAPON_MP5,
	WEAPON_M249,
	WEAPON_M3,
	WEAPON_M4A1,
	WEAPON_TMP,
	WEAPON_G3SG1,
	WEAPON_FLASHBANG,
	WEAPON_DEAGLE,
	WEAPON_SG552,
	WEAPON_AK47,
	WEAPON_KNIFE,
	WEAPON_P90,
	WEAPON_SHIELD,
	WEAPON_KEVLAR,
	WEAPON_ASSAULTSUIT,
	WEAPON_NIGHTVISION
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC			= 0x001,
	FONTFLAG_UNDERLINE		= 0x002,
	FONTFLAG_STRIKEOUT		= 0x004,
	FONTFLAG_SYMBOL			= 0x008,
	FONTFLAG_ANTIALIAS		= 0x010,
	FONTFLAG_GAUSSIANBLUR	= 0x020,
	FONTFLAG_ROTARY			= 0x040,
	FONTFLAG_DROPSHADOW		= 0x080,
	FONTFLAG_ADDITIVE		= 0x100,
	FONTFLAG_OUTLINE		= 0x200,
	FONTFLAG_CUSTOM			= 0x400,		// custom generated font - never fall back to asian compatibility mode
};

extern IBaseClientDLL *ClientSDK;
extern IClientEntityList *EntityListSDK;
extern CPrediction *PredictionSDK;
extern CGameMovement *GameMovementSDK;
extern IVEngineClient *EngineSDK;
extern CInput *InputSDK;
extern ICvar *CvarSDK;
extern IVModelInfoClient *ModelInfoSDK;
extern IEngineTrace *EngineTraceSDK;
extern IPhysicsSurfaceProps *PhysicsSurfacePropsSDK;
extern vgui::IPanel *PanelSDK;
extern vgui::ISurface *SurfaceSDK;
extern IVRenderView *RenderViewSDK;
extern IMaterialSystem *MaterialSystemSDK;
extern IVModelRender *ModelRenderSDK;
extern IGameEventManager2 *GameEventManagerSDK;
extern IVEfx *EffectsSDK;
extern CGlobalVars *Globals;
extern IVDebugOverlay *DebugOverlay;

extern float (*Shared_RandomFloat)(float,float);
extern int (*Shared_RandomInt)(int,int);
extern void (*Shared_RandomSeed)(int);

#define DO_ONCE(yey) static bool init = false; if(!init) { yey init = true; }

class CBeamSeg
{
public:
	Vector		m_vPos;
	Vector		m_vColor;
	float		m_flTexCoord;	// Y texture coordinate
	float		m_flWidth;
	float		m_flAlpha;
};

class CBeamSegDraw
{
public:
	void Start(int nSegs, IMaterial *pMaterial);
	void NextSeg(CBeamSeg *pSeg);
	void End();
private:
	void SpecifySeg(Vector &vNormal);
	void ComputeNormal(Vector &vStartPos, Vector &vNextPos, Vector *pNormal);
private:
	CMeshBuilder	m_Mesh;
	Vector			m_vNormalLast;
	int				m_nTotalSegs;
	int				m_nSegsDrawn;
	CBeamSeg		m_Seg;
};


class CTraceFilterNoPlayers : public CTraceFilter
{
public:

	CTraceFilterNoPlayers(void)
	{
		m_maxClients = -1;
	}


	virtual bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask)
	{
		if (m_maxClients == -1)
		{
			m_maxClients = EngineSDK->GetMaxClients();
		}

		C_BasePlayer *Entity = (C_BasePlayer*)EntityListSDK->GetClientEntityFromHandle(pEntity->GetRefEHandle());

		return ((Entity->index < 1) || (Entity->index > m_maxClients));
	}


private:

	int m_maxClients;
};

class cCModelHack
{
public:
	cCModelHack();
	void SetTextSolid(void);
	void ClearMapTextures(void);
	void SetTexturesTransparent(void);
	std::vector<IMaterial*> m_vec_Map_Textures;
	std::vector<IMaterial*> m_vec_Objects_Textures;
	std::vector<IMaterial*> m_vec_players_materials;

}extern ModelHack;

#endif 