#include "cvars.h"

C_ConVars ConVars;

#define Setup_ConVar(convar,name,value,infos) convar = new C_ConVar(name,value,infos); vCvars.push_back(convar);

#define Setup_ConVarChams(convar,name) convar = new C_ConVar(name,"0","chams options"); vCvars.push_back(convar);

C_ConVars::C_ConVars()
{
	Setup_ConVar(showpathrecorded, "showpathrecorded", "1", "show your path when you recorded");
	Setup_ConVar(showstartrecorded, "showstartrecorded", "1", "show your start pos when you recorded");
	Setup_ConVar(showendrecorded, "showendrecorded", "1", "show your end pos when you recorded");
	Setup_ConVar(showpatr, "showpatr", "0", "show your position in the tick you want when you recorded");
	Setup_ConVar(strafebot, "strafebot", "1", "simple strafe bot");
	Setup_ConVar(autobhop, "autobhop", "1", "simple autobhop");
	Setup_ConVar(rerecord, "rerecord", "0", "re editing demos at the tick you want (tick set by this convar),then at the end of re editing the convar will be reset at 0")
	Setup_ConVar(show_beams, "show_beams", "0", "show the beams on recorded positions 1) only start and end pos 2) all");
	Setup_ConVar(setangleyinm, "setangleyinm", "0", "show your angles client side and server, if disabled only server side (you can control your mouse)");
	Setup_ConVar(knifebot, "knifebot", "1", "perfect knifebot (only client side)");
	Setup_ConVar(triggerbot, "triggerbot", "6", "triggerbot lol");
	Setup_ConVar(visnospread, "visnospread", "1", "visual no spread");
	Setup_ConVar(nospread, "nospread", "1", "no spread 2 is seed 1 is normal");

	Setup_ConVarChams(chams_players, "chams_players");
	Setup_ConVarChams(chams_weapons, "chams_weapons");

	Setup_ConVarChams(chams_ct_behind_alpha, "chams_ct_behind_alpha");
	Setup_ConVarChams(chams_ct_behind_red, "chams_ct_behind_red");
	Setup_ConVarChams(chams_ct_behind_blue, "chams_ct_behind_blue");
	Setup_ConVarChams(chams_ct_behind_green, "chams_ct_behind_green");

	Setup_ConVarChams(chams_ct_front_alpha, "chams_ct_front_alpha");
	Setup_ConVarChams(chams_ct_front_red, "chams_ct_front_red");
	Setup_ConVarChams(chams_ct_front_blue, "chams_ct_front_blue");
	Setup_ConVarChams(chams_ct_front_green, "chams_ct_front_green");

	Setup_ConVarChams(chams_t_behind_alpha, "chams_t_behind_alpha");
	Setup_ConVarChams(chams_t_behind_red, "chams_t_behind_red");
	Setup_ConVarChams(chams_t_behind_blue, "chams_t_behind_blue");
	Setup_ConVarChams(chams_t_behind_green, "chams_t_behind_green");

	Setup_ConVarChams(chams_t_front_alpha, "chams_t_front_alpha");
	Setup_ConVarChams(chams_t_front_red, "chams_t_front_red");
	Setup_ConVarChams(chams_t_front_blue, "chams_t_front_blue");
	Setup_ConVarChams(chams_t_front_green, "chams_t_front_green");

	Setup_ConVarChams(chams_dead_behind_alpha, "chams_dead_behind_alpha");
	Setup_ConVarChams(chams_dead_behind_red, "chams_dead_behind_red");
	Setup_ConVarChams(chams_dead_behind_blue, "chams_dead_behind_blue");
	Setup_ConVarChams(chams_dead_behind_green, "chams_dead_behind_green");

	Setup_ConVarChams(chams_dead_front_alpha, "chams_dead_front_alpha");
	Setup_ConVarChams(chams_dead_front_red, "chams_dead_front_red");
	Setup_ConVarChams(chams_dead_front_blue, "chams_dead_front_blue");
	Setup_ConVarChams(chams_dead_front_green, "chams_dead_front_green");

	Setup_ConVarChams(chams_weapon_behind_alpha, "chams_weapon_behind_alpha");
	Setup_ConVarChams(chams_weapon_behind_red, "chams_weapon_behind_red");
	Setup_ConVarChams(chams_weapon_behind_blue, "chams_weapon_behind_blue");
	Setup_ConVarChams(chams_weapon_behind_green, "chams_weapon_behind_green");

	Setup_ConVarChams(chams_weapon_front_alpha, "chams_weapon_front_alpha");
	Setup_ConVarChams(chams_weapon_front_red, "chams_weapon_front_red");
	Setup_ConVarChams(chams_weapon_front_blue, "chams_weapon_front_blue");
	Setup_ConVarChams(chams_weapon_front_green, "chams_weapon_front_green");

	Setup_ConVar(autoedge, "autoedge", "0", "jump when you're about to fall to the next ground");
	Setup_ConVar(autoedge_height, "autoedge_height", "2.0", "at wich distance you want to jump from the ground")
	Setup_ConVar(strafebotmovement, "strafebotmovement", "400", "strafebot - movement, regulates your \"sync\" , max is 400 (you can put higher value but it wont make differences)");
	Setup_ConVar(correctangle_inm, "correctangle_inm", "0.95", "interpolate your angles recorded when you're playing back the demo (the spectators won't see this) value going to > 0 and < 1");
	//Setup_ConVar(correctangle, "correctangle", "0.95", "interpolate your angles recorded when you're playing back the demo on a server (the spectators will see this) value going to > 0 and < 1");
	Setup_ConVar(show_spectators_view, "show_spectators_view", "0", "show spectators view in a camera")
	Setup_ConVar(show_infos_text, "show_infos_text", "0", "show infos in a text (like hour,demo playtime ect..")
	Setup_ConVar(angle_y_in_m, "angleyinm", "0", "hange your y angle when you repeat your movement (180 backwards,90 sideways..)");
	Setup_ConVar(angle_y, "angley", "0", "change your y angle while playing (180 backwards,90 sideways..)");
	Setup_ConVar(silenthalfsideways, "silenthalfsideways","1", "gain speed like u were in half sideways (45 : -45)");
	Setup_ConVar(surf_style, "surf_style", "0", "surf style is just setting your angle x in 89 when you're at backwards or bhsw");
	Setup_ConVar(disconnect_msg, "disconnect_msg", "bo$$", "set your own disconnect msg");
	Setup_ConVar(fixmovement, "fixmovement","1", "disable this while ure playing on ksf and use the right keys instead ill found a right solution later.");
	Setup_ConVar(slowmotion, "slowmotion", "1", "slow motion");
	Setup_ConVar(slowmotion_time, "slowmotion_time", "0.001", "slow motion time");
	Setup_ConVar(isuckplz, "isuckplz", "0", "this is for noobs, dont use it if ure good enough.");
}
