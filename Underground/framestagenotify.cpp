#include "definitions.h"

typedef void(__thiscall*FrameStageNotifyFn)(void*, ClientFrameStage_t curStage);

void __fastcall new_FrameStageNotify(void*thisptr,int edx,ClientFrameStage_t curStage)
{
	Client->GetMethod<FrameStageNotifyFn>(36)(thisptr,curStage);
}