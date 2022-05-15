#include <Windows.h>
#include "main.h"
#include "vars.h"

//===================================================================================
CControlVariables UndergroundVars;
//===================================================================================
void CControlVariables::Initialize(void)
{
	
}

CControlVariables::CControlVariables()
{
	//Add stuff here to let it be default.

	UndergroundVars.red = 0;
	UndergroundVars.green = 255;
	UndergroundVars.blue = 0;
	UndergroundVars.alpha = 255;
	UndergroundVars.xpos = 100;
	UndergroundVars.ypos = 300;
	UndergroundVars.weight = 175;
	UndergroundVars.fakelaginattack = 16;
	UndergroundVars.fakelag = 16;
	UndergroundVars.roundsay = 1;
	UndergroundVars.antiaim3 = 1;
	UndergroundVars.forcelowspread = 1;
	UndergroundVars.perfectaccuracy = 1;
	UndergroundVars.name = 1;
	UndergroundVars.silentaim = 1;
	UndergroundVars.enableaim = 1;
}
