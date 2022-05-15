#include "main.h"

#ifndef CVARS
#define CVARS
class C_ConVar
{
public:

	C_ConVar(const char* name,char* value,const char* info)
	{
		strcpy(m_cValue,value);
		m_cName = name;
		m_cInfos = info;
	};

	FORCEINLINE const char* GetChar( void ) { return m_cValue; }
	FORCEINLINE const char* GetName( void ) { return m_cName; }
	FORCEINLINE const char* GetInfos( void ) { return m_cInfos; }

	FORCEINLINE float GetFloat( void ) { return (float)atof(m_cValue); }
	FORCEINLINE int GetInt( void ) { return (int)atof(m_cValue); }
	FORCEINLINE bool GetBool( void ) { return GetInt() ? true : false; }

	FORCEINLINE void SetValue(const char* value)
	{
		strcpy(m_cValue,value);
	}

	char m_cValue[64];
	const char* m_cName;
	const char* m_cInfos;
};

class C_ConVars
{
public:

	C_ConVars(void);

	~C_ConVars( void )
	{
		vCvars.clear();
	}

	C_ConVar *autoedge;
	C_ConVar *showpathrecorded;
	C_ConVar *showstartrecorded;
	C_ConVar *showendrecorded;
	C_ConVar *showpatr;
	C_ConVar *strafebot;
	C_ConVar *rerecord;
	C_ConVar *autobhop;
	C_ConVar *show_beams;
	C_ConVar *knifebot;
	C_ConVar *setangleyinm;
	C_ConVar *triggerbot;
	C_ConVar *nospread;
	C_ConVar *visnospread;
	C_ConVar *correctangle_inm;
	C_ConVar *correctangle;
	C_ConVar *show_spectators_view;
	C_ConVar *show_infos_text;
	C_ConVar *angle_y_in_m;
	C_ConVar *angle_y;
	C_ConVar *fixmovement;
	C_ConVar *slowmotion;
	C_ConVar *chams_players;
	C_ConVar *chams_weapons;

	C_ConVar *chams_ct_behind_alpha;
	C_ConVar *chams_ct_behind_red;
	C_ConVar *chams_ct_behind_blue;
	C_ConVar *chams_ct_behind_green;

	C_ConVar *chams_t_behind_alpha;
	C_ConVar *chams_t_behind_red;
	C_ConVar *chams_t_behind_blue;
	C_ConVar *chams_t_behind_green;

	C_ConVar *chams_ct_front_alpha;
	C_ConVar *chams_ct_front_red;
	C_ConVar *chams_ct_front_blue;
	C_ConVar *chams_ct_front_green;

	C_ConVar *chams_t_front_alpha;
	C_ConVar *chams_t_front_red;
	C_ConVar *chams_t_front_blue;
	C_ConVar *chams_t_front_green;

	C_ConVar *chams_dead_behind_alpha;
	C_ConVar *chams_dead_behind_red;
	C_ConVar *chams_dead_behind_blue;
	C_ConVar *chams_dead_behind_green;

	C_ConVar *chams_dead_front_alpha;
	C_ConVar *chams_dead_front_red;
	C_ConVar *chams_dead_front_blue;
	C_ConVar *chams_dead_front_green;

	C_ConVar *chams_weapon_behind_alpha;
	C_ConVar *chams_weapon_behind_red;
	C_ConVar *chams_weapon_behind_blue;
	C_ConVar *chams_weapon_behind_green;

	C_ConVar *chams_weapon_front_alpha;
	C_ConVar *chams_weapon_front_red;
	C_ConVar *chams_weapon_front_blue;
	C_ConVar *chams_weapon_front_green;
	C_ConVar *autoedge_height;
	C_ConVar *strafebotmovement;
	C_ConVar *silenthalfsideways;
	C_ConVar *fakekey;
	C_ConVar *surf_style;
	C_ConVar *disconnect_msg;
	C_ConVar *slowmotion_time;
	C_ConVar *isuckplz;

	std::vector<C_ConVar*> vCvars;

	C_ConVar* GetCvarByName( const char*pszName ) 
	{
		for( int i = 0; i < vCvars.size(); ++i )
		{
			if( StringCompareI(vCvars[i]->m_cName, pszName) == true )
				return vCvars[i];
		}

		return 0;
	}

	FORCEINLINE C_ConVar* GetCvarByIndex( unsigned int nIndex )
	{
		return vCvars[nIndex];
	}

}extern ConVars;

#endif