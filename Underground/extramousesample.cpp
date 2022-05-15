#include "definitions.h"

typedef void(__thiscall*ExtraMouseSampleFn)(void*, float, bool);
void __fastcall new_ExtraMouseSample(void*thisptr,int edx,float frametime, bool active)
{
	Client->GetMethod<ExtraMouseSampleFn>(22)(thisptr,frametime, active);
}